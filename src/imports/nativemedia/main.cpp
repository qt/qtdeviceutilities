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
#include <QtGui>

void initializeOMX();
void render(QOpenGLContext *context, QWindow *surface);

class Renderer : public QObject
{
    Q_OBJECT
public:
    Renderer(QWindow *surface)
        : surface(surface)
    {
        context = new QOpenGLContext;

        context->create();
        context->makeCurrent(surface);

        initializeOMX();
        qDebug() << "OMX initialized";
    }

public slots:
    void render() {
        ::render(context, surface);
    }

public:
    QWindow *surface;
    QOpenGLContext *context;
} *renderer;

void triggerRender()
{
    QTimer::singleShot(0, renderer, SLOT(render()));
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QScreen *screen = QGuiApplication::primaryScreen();

    qDebug() << "Screen geometry:" << screen->geometry();

    QWindow window;
    window.setSurfaceType(QWindow::OpenGLSurface);
    window.setGeometry(screen->geometry());
    window.show();

    renderer = new Renderer(&window);

    return app.exec();
}

#include "main.moc"
