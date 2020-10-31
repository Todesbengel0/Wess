#include <QtGlobal>
#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include "mainwindow.h"

#undef main

#ifdef Q_OS_IOS
extern "C" int qtmn(int argc, char *argv[])
#elseif Q_OS_ANDROID
extern "C" int qtmn(int argc, char *argv[])
#else
int main( int argc, char* argv[] )
#endif
{
    // Specify the OpenGL format we wish to use
    // According to the documentation of QOpenGLWidget this should be done
    // before instantiation of QApplication
    QSurfaceFormat format;
    format.setDepthBufferSize(24);

    //Unter Windows Verwendung von Desktop OpenGL erzwingen
    //sonst bekommt man - warum auch immer - bei manchen Konfigurationen einen Kontext mit OpenGL ES
#if defined(Q_OS_WIN32) || defined(Q_OS_Win64)
    //a.setAttribute(Qt::AA_UseDesktopOpenGL);
#endif

    //Versionsunterscheidung für OpenGL ES
    if (format.renderableType() != QSurfaceFormat::OpenGLES)
    {
#ifdef SG_USE_OPENGL_3_3_FUNCTIONS
        format.setVersion(3, 3);
#else
        format.setVersion(4, 0);
#endif
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
    else
    {
        format.setVersion(3, 0);
    }

#ifdef SG_LOG_OPENGL_DEBUG_MASSAGES
    format.setOption(QSurfaceFormat::DebugContext);
#endif

    QSurfaceFormat::setDefaultFormat(format);
//    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    QApplication a( argc, argv );

    MainWindow w;

    //Notwendig, um Qt-Resources in statischen Bibliotheken verfügbar zu machen.
    Q_INIT_RESOURCE(displayconfig);
    w.show();
    return a.exec();
}
