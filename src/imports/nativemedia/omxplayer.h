#ifndef OMXPLAYER_H
#define OMXPLAYER_H

#include <QObject>

class OmxPlayer : public QObject
{
    Q_OBJECT
public:
    virtual ~OmxPlayer() = 0;

    virtual void updateTexture() = 0;
    virtual bool initialize(const QByteArray &filename) = 0;
    virtual bool hasFrame() = 0;
    virtual void setPaused(bool paused) = 0;

    static OmxPlayer *create();

signals:
    void frameAvailable();
    void videoSize(int w, int h);
};

#endif
