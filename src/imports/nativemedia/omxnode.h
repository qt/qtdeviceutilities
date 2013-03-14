#ifndef OMXNODE_H
#define OMXNODE_H

#include <QtQuick/qquickitem.h>

#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgsimplematerial.h>

#include "SurfaceTexture.h"

#include "omxplayer.h"

class QSGTexture;

struct OmxTextureState {
    OmxPlayer *player;
};

class OmxNode : public QSGGeometryNode
{
public:
    OmxNode(OmxPlayer *player);
    ~OmxNode();

    void preprocess();

    void setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h) { setRect(QRectF(x, y, w, h)); }

    bool isTextureUpdated() const { return m_textureUpdated; }
    void setTextureUpdated(bool textureUpdated) { m_textureUpdated = textureUpdated; }

private:
    bool m_textureUpdated;

    QSGGeometry m_geometry;
    QSGSimpleMaterial<OmxTextureState> *m_textureMaterial;

    QRectF m_rect;
    OmxPlayer *m_player;
    bool m_initialized;
};

class OmxTextureMaterial : public QSGSimpleMaterialShader<OmxTextureState>
{
    QSG_DECLARE_SIMPLE_SHADER(OmxTextureMaterial, OmxTextureState)
public:
    QList<QByteArray> attributes() const;

    void updateState(const OmxTextureState *newState, const OmxTextureState *oldState);

protected:
    const char *vertexShader() const;
    const char *fragmentShader() const;
};

class OmxItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged FINAL)
    Q_PROPERTY(int sourceWidth READ sourceWidth NOTIFY sourceWidthChanged FINAL)
    Q_PROPERTY(int sourceHeight READ sourceHeight NOTIFY sourceHeightChanged FINAL)
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged FINAL)
public:
    OmxItem();
    virtual ~OmxItem();

    QString source() {
        return m_source;
    }

    int sourceWidth() {
        return m_sourceWidth;
    }

    int sourceHeight() {
        return m_sourceHeight;
    }

    bool paused() { return m_paused; }
    void setPaused(bool p);

    void setSource(const QString &source);

signals:
    void sourceChanged();
    void sourceWidthChanged();
    void sourceHeightChanged();
    void pausedChanged();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    void itemChange(ItemChange change, const ItemChangeData &);

private slots:
    void triggerRender();
    void videoSize(int w, int h);
    void beforeRendering();

private:
    OmxPlayer *m_player;
    bool m_hasFrame;
    bool m_initialized;
    bool m_paused;
    QString m_source;
    int m_sourceWidth;
    int m_sourceHeight;
};

#endif // OMXNODE_H

