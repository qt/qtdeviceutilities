#include <OMXAL/OpenMAXAL.h>
#include <OMXAL/OpenMAXAL_Android.h>

#include <SurfaceTexture.h>
#include <gui/SurfaceTextureClient.h>

#include "omxplayer.h"

#include <QtGui>

#include <cassert>

// number of required interfaces for the MediaPlayer creation
#define NB_MAXAL_INTERFACES 3 // XAAndroidBufferQueueItf, XAStreamInformationItf and XAPlayItf

// number of buffers in our buffer queue, an arbitrary number
#define NB_BUFFERS 8

// we're streaming MPEG-2 transport stream data, operate on transport stream block size
#define MPEG2_TS_PACKET_SIZE 188

// number of MPEG-2 transport stream blocks per buffer, an arbitrary number
#define PACKETS_PER_BUFFER 10

// determines how much memory we're dedicating to memory caching
#define BUFFER_SIZE (PACKETS_PER_BUFFER*MPEG2_TS_PACKET_SIZE)

using namespace android;

// constant to identify a buffer context which is the end of the stream to decode
static const int kEosBufferCntxt = 1980; // a magic value we can compare against

OmxPlayer::~OmxPlayer()
{
}

struct OmxContext : public OmxPlayer {
    // engine interfaces
    XAObjectItf engineObject;
    XAEngineItf engineEngine;

    // output mix interfaces
    XAObjectItf outputMixObject;

    // streaming media player interfaces
    XAObjectItf             playerObj;
    XAPlayItf               playerPlayItf;
    XAAndroidBufferQueueItf playerBQItf;
    XAStreamInformationItf  playerStreamInfoItf;
    XAVolumeItf             playerVolItf;

    // where we cache in memory the data to play
    // note this memory is re-used by the buffer queue callback
    char dataCache[BUFFER_SIZE * NB_BUFFERS];

    // handle of the file to play
    FILE *file;

    // has the app reached the end of the file
    bool reachedEof;

    // For mutual exclusion between callback thread and application thread(s).
    // The mutex protects reachedEof, discontinuity,
    // The condition is signalled when a discontinuity is acknowledged.

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    sp<SurfaceTexture> surfaceTexture;
    sp<SurfaceTexture> surfaceTextureClient;

    int currentFrame;
    GLuint textureId;

    // for render, useful for debugging
    QOpenGLShaderProgram *program;

    int vertexLocation;
    int texCoordLocation;
    int textureLocation;

    OmxContext()
        : engineObject(0)
        , engineEngine(0)
        , outputMixObject(0)
        , playerObj(0)
        , playerPlayItf(0)
        , playerBQItf(0)
        , playerStreamInfoItf(0)
        , playerVolItf(0)
        , file(0)
        , reachedEof(false)
        , mutex(PTHREAD_MUTEX_INITIALIZER)
        , cond(PTHREAD_COND_INITIALIZER)
        , currentFrame(-1)
        , textureId(0)
        , program(0)
        , vertexLocation(0)
        , texCoordLocation(0)
        , textureLocation(0)
    {
    }

    ~OmxContext();

    void updateTexture()
    {
        if (surfaceTexture.get())
            surfaceTexture->updateTexImage();
    }

    bool hasFrame()
    {
        return currentFrame >= 0;
    }

    void setPaused(bool paused);

    bool initialize(const QByteArray &filename);
    bool enqueueInitialBuffers(bool discontinuity);

    XAresult AndroidBufferQueueCallback(
        XAAndroidBufferQueueItf caller,
        void *pCallbackContext,
        void *pBufferContext,
        void *pBufferData,
        XAuint32 dataSize,
        XAuint32 dataUsed,
        const XAAndroidBufferItem *pItems,
        XAuint32 itemsLength);

    void StreamChangeCallback(XAStreamInformationItf caller,
        XAuint32 eventId,
        XAuint32 streamIndex,
        void * pEventData,
        void * pContext );

    void render(QOpenGLContext *context, QWindow *surface);
};

static XAresult AndroidBufferQueueCallback(
        XAAndroidBufferQueueItf caller,
        void *pCallbackContext,        /* input */
        void *pBufferContext,          /* input */
        void *pBufferData,             /* input */
        XAuint32 dataSize,             /* input */
        XAuint32 dataUsed,             /* input */
        const XAAndroidBufferItem *pItems,/* input */
        XAuint32 itemsLength           /* input */)
{
    return static_cast<OmxContext *>(pCallbackContext)->AndroidBufferQueueCallback(
        caller, pCallbackContext, pBufferContext, pBufferData, dataSize, dataUsed, pItems, itemsLength);
}

// AndroidBufferQueueItf callback to supply MPEG-2 TS packets to the media player
XAresult OmxContext::AndroidBufferQueueCallback(
        XAAndroidBufferQueueItf caller,
        void *pCallbackContext,        /* input */
        void *pBufferContext,          /* input */
        void *pBufferData,             /* input */
        XAuint32 dataSize,             /* input */
        XAuint32 dataUsed,             /* input */
        const XAAndroidBufferItem *pItems,/* input */
        XAuint32 itemsLength           /* input */)
{
    XAresult res;
    int ok;

    // note there is never any contention on this mutex unless a discontinuity request is active
    ok = pthread_mutex_lock(&mutex);
    assert(0 == ok);

#if 0
    // was a discontinuity requested?
    if (discontinuity) {
        // Note: can't rewind after EOS, which we send when reaching EOF
        // (don't send EOS if you plan to play more content through the same player)
        if (!reachedEof) {
            // clear the buffer queue
            res = (*playerBQItf)->Clear(playerBQItf);
            assert(XA_RESULT_SUCCESS == res);
            // rewind the data source so we are guaranteed to be at an appropriate point
            rewind(file);
            // Enqueue the initial buffers, with a discontinuity indicator on first buffer
            (void) enqueueInitialBuffers(JNI_TRUE);
        }
        // acknowledge the discontinuity request
        discontinuity = JNI_FALSE;
        ok = pthread_cond_signal(&cond);
        assert(0 == ok);
        goto exit;
    }
#endif

    if ((pBufferData == NULL) && (pBufferContext != NULL)) {
        const int processedCommand = *(int *)pBufferContext;
        if (kEosBufferCntxt == processedCommand) {
            qDebug("EOS was processed\n");
            // our buffer with the EOS message has been consumed
            assert(0 == dataSize);
            goto exit;
        }
    }

    // pBufferData is a pointer to a buffer that we previously Enqueued
    assert((dataSize > 0) && ((dataSize % MPEG2_TS_PACKET_SIZE) == 0));
    assert(dataCache <= (char *) pBufferData && (char *) pBufferData <
            &dataCache[BUFFER_SIZE * NB_BUFFERS]);
    assert(0 == (((char *) pBufferData - dataCache) % BUFFER_SIZE));

    // don't bother trying to read more data once we've hit EOF
    if (reachedEof) {
        goto exit;
    }

    size_t nbRead;
    // note we do call fread from multiple threads, but never concurrently
    size_t bytesRead;
    bytesRead = fread(pBufferData, 1, BUFFER_SIZE, file);
    if (bytesRead > 0) {
        if ((bytesRead % MPEG2_TS_PACKET_SIZE) != 0) {
            qDebug("Dropping last packet because it is not whole");
        }
        size_t packetsRead = bytesRead / MPEG2_TS_PACKET_SIZE;
        size_t bufferSize = packetsRead * MPEG2_TS_PACKET_SIZE;
        res = (*caller)->Enqueue(caller, NULL /*pBufferContext*/,
                pBufferData /*pData*/,
                bufferSize /*dataLength*/,
                NULL /*pMsg*/,
                0 /*msgLength*/);
        assert(XA_RESULT_SUCCESS == res);
    } else {
        // EOF or I/O error, signal EOS
        XAAndroidBufferItem msgEos[1];
        msgEos[0].itemKey = XA_ANDROID_ITEMKEY_EOS;
        msgEos[0].itemSize = 0;
        // EOS message has no parameters, so the total size of the message is the size of the key
        //   plus the size if itemSize, both XAuint32
        res = (*caller)->Enqueue(caller, (void *)&kEosBufferCntxt /*pBufferContext*/,
                NULL /*pData*/, 0 /*dataLength*/,
                msgEos /*pMsg*/,
                sizeof(XAuint32)*2 /*msgLength*/);
        assert(XA_RESULT_SUCCESS == res);
        reachedEof = true;
    }

exit:
    ok = pthread_mutex_unlock(&mutex);
    assert(0 == ok);
    return XA_RESULT_SUCCESS;
}

// callback invoked whenever there is new or changed stream information
static void StreamChangeCallback(XAStreamInformationItf caller,
        XAuint32 eventId,
        XAuint32 streamIndex,
        void * pEventData,
        void * pContext )
{
    static_cast<OmxContext *>(pContext)->StreamChangeCallback(
        caller, eventId, streamIndex, pEventData, pContext);
}

// callback invoked whenever there is new or changed stream information
void OmxContext::StreamChangeCallback(XAStreamInformationItf caller,
        XAuint32 eventId,
        XAuint32 streamIndex,
        void * pEventData,
        void * pContext )
{
    qDebug("StreamChangeCallback called for stream %u", streamIndex);

    switch (eventId) {
      case XA_STREAMCBEVENT_PROPERTYCHANGE: {
        /** From spec 1.0.1:
            "This event indicates that stream property change has occurred.
            The streamIndex parameter identifies the stream with the property change.
            The pEventData parameter for this event is not used and shall be ignored."
         */

        XAresult res;
        XAuint32 domain;
        res = (*caller)->QueryStreamType(caller, streamIndex, &domain);
        assert(XA_RESULT_SUCCESS == res);
        switch (domain) {
          case XA_DOMAINTYPE_VIDEO: {
            XAVideoStreamInformation videoInfo;
            res = (*caller)->QueryStreamInformation(caller, streamIndex, &videoInfo);
            assert(XA_RESULT_SUCCESS == res);
            qDebug("Found video size %u x %u, codec ID=%u, frameRate=%u, bitRate=%u, duration=%u ms",
                        videoInfo.width, videoInfo.height, videoInfo.codecId, videoInfo.frameRate,
                        videoInfo.bitRate, videoInfo.duration);
            emit videoSize(videoInfo.width, videoInfo.height);
          } break;
          default:
            fprintf(stderr, "Unexpected domain %u\n", domain);
            break;
        }
      } break;
      default:
        fprintf(stderr, "Unexpected stream event ID %u\n", eventId);
        break;
    }
}

// Enqueue the initial buffers, and optionally signal a discontinuity in the first buffer
bool OmxContext::enqueueInitialBuffers(bool discontinuity)
{

    /* Fill our cache.
     * We want to read whole packets (integral multiples of MPEG2_TS_PACKET_SIZE).
     * fread returns units of "elements" not bytes, so we ask for 1-byte elements
     * and then check that the number of elements is a multiple of the packet size.
     */
    size_t bytesRead;
    bytesRead = fread(dataCache, 1, BUFFER_SIZE * NB_BUFFERS, file);
    if (bytesRead <= 0) {
        // could be premature EOF or I/O error
        return false;
    }
    if ((bytesRead % MPEG2_TS_PACKET_SIZE) != 0) {
        qDebug("Dropping last packet because it is not whole");
    }
    size_t packetsRead = bytesRead / MPEG2_TS_PACKET_SIZE;
    qDebug("Initially queueing %u packets", packetsRead);

    /* Enqueue the content of our cache before starting to play,
       we don't want to starve the player */
    size_t i;
    for (i = 0; i < NB_BUFFERS && packetsRead > 0; i++) {
        // compute size of this buffer
        size_t packetsThisBuffer = packetsRead;
        if (packetsThisBuffer > PACKETS_PER_BUFFER) {
            packetsThisBuffer = PACKETS_PER_BUFFER;
        }
        size_t bufferSize = packetsThisBuffer * MPEG2_TS_PACKET_SIZE;
        XAresult res;
        if (discontinuity) {
            // signal discontinuity
            XAAndroidBufferItem items[1];
            items[0].itemKey = XA_ANDROID_ITEMKEY_DISCONTINUITY;
            items[0].itemSize = 0;
            // DISCONTINUITY message has no parameters,
            //   so the total size of the message is the size of the key
            //   plus the size if itemSize, both XAuint32
            res = (*playerBQItf)->Enqueue(playerBQItf, NULL /*pBufferContext*/,
                    dataCache + i*BUFFER_SIZE, bufferSize, items /*pMsg*/,
                    sizeof(XAuint32)*2 /*msgLength*/);
            //discontinuity = false;
        } else {
            res = (*playerBQItf)->Enqueue(playerBQItf, NULL /*pBufferContext*/,
                    dataCache + i*BUFFER_SIZE, bufferSize, NULL, 0);
        }
        assert(XA_RESULT_SUCCESS == res);
        packetsRead -= packetsThisBuffer;
    }

    return true;
}

class FrameCallback : public SurfaceTexture::FrameAvailableListener
{
public:
    FrameCallback(OmxContext *ctx)
        : context(ctx)
    {
    }

    void onFrameAvailable()
    {
        emit context->frameAvailable();
        ++context->currentFrame;
    }

private:
    OmxContext *context;
};

OmxPlayer *OmxPlayer::create()
{
    return new OmxContext;
}

bool OmxContext::initialize(const QByteArray &filename)
{
    file = fopen(filename.data(), "rb");
    if (!file) {
        qWarning("Failed to open %s", filename.data());
        return false;
    }

    XAresult res;

    qDebug() << "Creating engine object";

    // create engine
    res = xaCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(XA_RESULT_SUCCESS == res);

    qDebug() << "Creating engine object succeeded?" << (res == XA_RESULT_SUCCESS);

    // realize the engine
    res = (*engineObject)->Realize(engineObject, XA_BOOLEAN_FALSE);
    assert(XA_RESULT_SUCCESS == res);

    qDebug() << "Realizing engine object succeeded?" << (res == XA_RESULT_SUCCESS);

    // get the engine interface, which is needed in order to create other objects
    res = (*engineObject)->GetInterface(engineObject, XA_IID_ENGINE, &engineEngine);
    assert(XA_RESULT_SUCCESS == res);

    qDebug() << "Getting engine interface succeeded?" << (res == XA_RESULT_SUCCESS);

    // create output mix
    res = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 0, NULL, NULL);
    assert(XA_RESULT_SUCCESS == res);

    qDebug() << "Creating output mix succeeded?" << (res == XA_RESULT_SUCCESS);

    // realize the output mix
    res = (*outputMixObject)->Realize(outputMixObject, XA_BOOLEAN_FALSE);
    assert(XA_RESULT_SUCCESS == res);

    qDebug() << "Realizing output mix succeeded?" << (res == XA_RESULT_SUCCESS);

    // configure data source
    XADataLocator_AndroidBufferQueue loc_abq = { XA_DATALOCATOR_ANDROIDBUFFERQUEUE, NB_BUFFERS };
    XADataFormat_MIME format_mime = {
        XA_DATAFORMAT_MIME, XA_ANDROID_MIME_MP2TS, XA_CONTAINERTYPE_MPEG_TS };
    XADataSource dataSrc = {&loc_abq, &format_mime};

    // configure audio sink
    XADataLocator_OutputMix loc_outmix = { XA_DATALOCATOR_OUTPUTMIX, outputMixObject };
    XADataSink audioSnk = { &loc_outmix, 0 };

    qDebug() << "Creating SurfaceTexture";

    glGenTextures(1, &textureId);
    surfaceTexture = new android::SurfaceTexture(textureId);

    qDebug() << "Creating SurfaceTextureClient";

    android::sp<android::SurfaceTextureClient> surfaceTextureClient = new android::SurfaceTextureClient(surfaceTexture);

    sp<SurfaceTexture::FrameAvailableListener> listener = new FrameCallback(this);

    surfaceTexture->setFrameAvailableListener(listener);

    // configure image video sink
    XADataLocator_NativeDisplay loc_nd = {
        XA_DATALOCATOR_NATIVEDISPLAY,        // locatorType
        // the video sink must be an ANativeWindow created from a Surface or SurfaceTexture
        static_cast<ANativeWindow *>(surfaceTextureClient.get()),              // hWindow
        // must be 0
        0                                 // hDisplay
    };
    XADataSink imageVideoSink = {&loc_nd, 0};

    // declare interfaces to use
    XAboolean     required[NB_MAXAL_INTERFACES]
        = {XA_BOOLEAN_TRUE, XA_BOOLEAN_TRUE,           XA_BOOLEAN_TRUE};
    XAInterfaceID iidArray[NB_MAXAL_INTERFACES]
        = {XA_IID_PLAY,     XA_IID_ANDROIDBUFFERQUEUESOURCE,
            XA_IID_STREAMINFORMATION};


    qDebug() << "Creating media player for engine" << engineEngine;

    // create media player
    res = (*engineEngine)->CreateMediaPlayer(engineEngine, &playerObj, &dataSrc,
            0, &audioSnk, &imageVideoSink, 0, 0,
            NB_MAXAL_INTERFACES /*XAuint32 numInterfaces*/,
            iidArray /*const XAInterfaceID *pInterfaceIds*/,
            required /*const XAboolean *pInterfaceRequired*/);

    qDebug() << "CreateMediaPlayer succeeded?" << (res == XA_RESULT_SUCCESS);

    // realize the player
    res = (*playerObj)->Realize(playerObj, XA_BOOLEAN_FALSE);

    qDebug() << "Realize media player succeeded?" << (res == XA_RESULT_SUCCESS);

    // get the play interface
    res = (*playerObj)->GetInterface(playerObj, XA_IID_PLAY, &playerPlayItf);

    qDebug() << "Get play interface succeeded?" << (res == XA_RESULT_SUCCESS);

    // get the stream information interface (for video size)
    res = (*playerObj)->GetInterface(playerObj, XA_IID_STREAMINFORMATION, &playerStreamInfoItf);

    qDebug() << "Get stream information interface succeeded?" << (res == XA_RESULT_SUCCESS);

    // get the volume interface
    res = (*playerObj)->GetInterface(playerObj, XA_IID_VOLUME, &playerVolItf);

    qDebug() << "Get volume interface succeeded?" << (res == XA_RESULT_SUCCESS);

    // get the Android buffer queue interface
    res = (*playerObj)->GetInterface(playerObj, XA_IID_ANDROIDBUFFERQUEUESOURCE, &playerBQItf);

    qDebug() << "Get buffer queue interface succeeded?" << (res == XA_RESULT_SUCCESS);

    // specify which events we want to be notified of
    res = (*playerBQItf)->SetCallbackEventsMask(playerBQItf, XA_ANDROIDBUFFERQUEUEEVENT_PROCESSED);

    qDebug() << "Setting callback events mask succeeded?" << (res == XA_RESULT_SUCCESS);

    // register the callback from which OpenMAX AL can retrieve the data to play
    res = (*playerBQItf)->RegisterCallback(playerBQItf, ::AndroidBufferQueueCallback, this);

    qDebug() << "Registering buffer queue callback succeeded?" << (res == XA_RESULT_SUCCESS);

    // we want to be notified of the video size once it's found, so we register a callback for that
    res = (*playerStreamInfoItf)->RegisterStreamChangeCallback(playerStreamInfoItf,
            ::StreamChangeCallback, this);

    qDebug() << "Registering stream change callback succeeded?" << (res == XA_RESULT_SUCCESS);

    // enqueue the initial buffers
    if (!enqueueInitialBuffers(false))
        return false;

    qDebug() << "Enqueued initial buffers";

    // prepare the player
    res = (*playerPlayItf)->SetPlayState(playerPlayItf, XA_PLAYSTATE_PAUSED);

    qDebug() << "Preparing player (setting play state paused) succeeded?" << (res == XA_RESULT_SUCCESS);

    // set the volume
    res = (*playerVolItf)->SetVolumeLevel(playerVolItf, 0);

    qDebug() << "Setting volume level succeeded?" << (res == XA_RESULT_SUCCESS);

    return true;
}

OmxContext::~OmxContext()
{
    if (!file)
        return;

    // destroy streaming media player object, and invalidate all associated interfaces
    if (playerObj != NULL) {
        (*playerObj)->Destroy(playerObj);
        playerObj = NULL;
        playerPlayItf = NULL;
        playerBQItf = NULL;
        playerStreamInfoItf = NULL;
        playerVolItf = NULL;
    }

    // destroy output mix object, and invalidate all associated interfaces
    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}

void OmxContext::setPaused(bool paused)
{
    XAresult res;

    // make sure the streaming media player was created
    if (NULL != playerPlayItf) {
        // set the player's state
        res = (*playerPlayItf)->SetPlayState(playerPlayItf, paused ?
                XA_PLAYSTATE_PAUSED : XA_PLAYSTATE_PLAYING);
    }
}

void OmxContext::render(QOpenGLContext *context, QWindow *surface)
{
    context->makeCurrent(surface);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (currentFrame >= 0) {
        surfaceTexture->updateTexImage();

        if (!program) {
            qDebug() << "Received first video frame, starting to render";

            program = new QOpenGLShaderProgram;

            program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                    "attribute highp vec4 vertex;\n"
                    "attribute highp vec2 texCoord;\n"
                    "varying highp vec2 coord;\n"
                    "void main(void)\n"
                    "{\n"
                    "   coord = texCoord;\n"
                    "   gl_Position = vertex;\n"
                    "}");
            program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                    "#extension GL_OES_EGL_image_external : require\n"
                    "uniform samplerExternalOES tex;\n"
                    "varying highp vec2 coord;\n"
                    "void main(void)\n"
                    "{\n"
                    "   gl_FragColor = texture2D(tex, coord);\n"
                    "}");

            program->link();

            vertexLocation = program->attributeLocation("vertex");
            texCoordLocation = program->attributeLocation("texCoord");
            textureLocation = program->uniformLocation("tex");

            qDebug() << "attribute locations" << vertexLocation << texCoordLocation;
            qDebug() << "uniform location" << textureLocation;
        }

        program->bind();

        static GLfloat const triangleVertices[] = {
            -1, -3,
            3, 1,
            -1, 1
        };

        static GLfloat const triangleTexCoords[] = {
            0, 2,
            2, 0,
            0, 0
        };

        program->enableAttributeArray(vertexLocation);
        program->setAttributeArray(vertexLocation, triangleVertices, 2);
        program->enableAttributeArray(texCoordLocation);
        program->setAttributeArray(texCoordLocation, triangleTexCoords, 2);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        program->disableAttributeArray(vertexLocation);
        program->disableAttributeArray(texCoordLocation);
    }

    context->swapBuffers(surface);
}
