/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://qt.digia.com/
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://qt.digia.com/
**
****************************************************************************/
#include "omxnode.h"

#include <QtGui/QOpenGLContext>
#include <QtQuick/qsgtexture.h>
#include <QtQuick/qquickwindow.h>

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
    , m_paused(false)
    , m_sourceWidth(0)
    , m_sourceHeight(0)
{
    connect(m_player, SIGNAL(frameAvailable()), this, SLOT(triggerRender()));
    connect(m_player, SIGNAL(videoSize(int, int)), this, SLOT(videoSize(int, int)));

    setFlag(ItemHasContents, true);
}

void OmxItem::itemChange(ItemChange change, const ItemChangeData &)
{
    if (change == ItemSceneChange) {
        QQuickWindow *win = window();
        if (!win)
            return;

        // Connect the beforeRendering signal to our paint function.
        // Since this call is executed on the rendering thread it must be
        // a Qt::DirectConnection
        connect(win, SIGNAL(beforeRendering()), this, SLOT(beforeRendering()), Qt::DirectConnection);
    }
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
    emit sourceChanged();
    update();
}

void OmxItem::beforeRendering()
{
    if (m_initialized || m_source.isNull())
        return;

    m_initialized = m_player->initialize(m_source.toLocal8Bit());

    GLuint tid;
    glGenTextures(1, &tid);

    // start playing if not paused
    if (m_initialized && !paused())
        m_player->setPaused(false);
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
