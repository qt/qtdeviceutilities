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
