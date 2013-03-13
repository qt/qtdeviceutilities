#include "omxnode.h"

#include <QtGui/QOpenGLContext>
#include <QtQuick/qsgtexture.h>

#include <QTimer>

static const char omx_texture_material_vertex[] =
           "uniform highp mat4 qt_Matrix;                      \n"
           "attribute highp vec4 qt_VertexPosition;            \n"
           "attribute highp vec2 qt_VertexTexCoord;            \n"
           "varying highp vec2 qt_TexCoord;                    \n"
           "void main() {                                      \n"
           "    qt_TexCoord = qt_VertexTexCoord;               \n"
           "    gl_Position = qt_Matrix * qt_VertexPosition;   \n"
           "}";


static const char omx_texture_material_fragment[] =
           "#extension GL_OES_EGL_image_external : require     \n"
           "varying highp vec2 qt_TexCoord;                    \n"
           "uniform samplerExternalOES qt_Texture;             \n"
           "uniform lowp float qt_Opacity;                     \n"
           "void main() {                                      \n"
           "    gl_FragColor = texture2D(qt_Texture, qt_TexCoord) * qt_Opacity; \n"
           "}";

QList<QByteArray> OmxTextureMaterial::attributes() const
{
    QList<QByteArray> attributeList;
    attributeList << "qt_VertexPosition";
    attributeList << "qt_VertexTexCoord";
    return attributeList;
}

void OmxTextureMaterial::updateState(const OmxTextureState *newState, const OmxTextureState *oldState)
{
    Q_UNUSED(oldState);
    newState->player->updateTexture();
}

const char *OmxTextureMaterial::vertexShader() const
{
    return omx_texture_material_vertex;
}

const char *OmxTextureMaterial::fragmentShader() const
{
    return omx_texture_material_fragment;
}

OmxNode::OmxNode(OmxPlayer *player)
    : m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    , m_player(player)
    , m_initialized(false)
{
    m_textureMaterial = OmxTextureMaterial::createMaterial();
    m_textureMaterial->state()->player = player;

    setGeometry(&m_geometry);
    setMaterial(m_textureMaterial);

    setFlag(UsePreprocess, true);
}

OmxNode::~OmxNode()
{
    delete m_textureMaterial;
}

void OmxNode::preprocess()
{
}

void OmxNode::updateTexture()
{
    printf("OmxNode::updateTexture()\n");
}

void OmxNode::setRect(const QRectF &rect)
{
    if (m_rect == rect)
        return;

    printf("OmxNode::setRect(%f %f %f %f)\n", rect.x(), rect.y(), rect.width(), rect.height());
    m_rect = rect;

    QRectF sourceRect(0, 0, 1, 1);
    QSGGeometry::updateTexturedRectGeometry(&m_geometry, m_rect, sourceRect);
}

OmxItem::OmxItem()
    : m_player(OmxPlayer::create())
    , m_hasFrame(false)
    , m_initialized(false)
    , m_sourceWidth(0)
    , m_sourceHeight(0)
    , m_paused(false)
{
    connect(m_player, SIGNAL(frameAvailable()), this, SLOT(triggerRender()));
    connect(m_player, SIGNAL(videoSize(int, int)), this, SLOT(videoSize(int, int)));

    setFlag(ItemHasContents, true);
}

OmxItem::~OmxItem()
{
    delete m_player;
}

void OmxItem::triggerRender()
{
    m_hasFrame = true;
    update();
}

void OmxItem::videoSize(int w, int h)
{
    m_sourceWidth = w;
    m_sourceHeight = h;

    emit sourceWidthChanged();
    emit sourceHeightChanged();
}

void OmxItem::setSource(const QString &source)
{
    if (m_initialized || source == m_source)
        return;

    m_source = source;

    m_initialized = m_player->initialize(m_source.toLocal8Bit());

    // start playing if not paused
    if (m_initialized && !paused())
        m_player->setPaused(false);

    emit sourceChanged();
}

QSGNode *OmxItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (!m_hasFrame)
        return 0;

    OmxNode *node;
    if (oldNode)
        node = static_cast<OmxNode *>(oldNode);
    else
        node = new OmxNode(m_player);

    node->setRect(boundingRect());
    node->markDirty(QSGNode::DirtyMaterial);

    return node;
}

void OmxItem::setPaused(bool p)
{
    if (p == m_paused)
        return;
    m_player->setPaused(p);
    m_paused = p;
    emit pausedChanged();
}
