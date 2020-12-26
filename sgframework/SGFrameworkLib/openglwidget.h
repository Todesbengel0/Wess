#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "opengl.h"
#include <QOpenGLWidget>
#include <list>
#include "listener.h"
#include "reshapelistener.h"
#include "idleobserver.h"
#include "node.h"
#include "overlaywidget.h"

#ifdef SG_LOG_OPENGL_DEBUG_MASSAGES
#include <QOpenGLDebugLogger>
#endif

class SceneManager;

class QOpenGLContext;

//! Ein Qt-Widget, das OpenGL zeichnen kann
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget* parent = 0);
    virtual ~OpenGLWidget();
    virtual void keyPressEvent(QKeyEvent* e) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* e) override;

    //! Instanz holen
    static OpenGLWidget* getInstance();

    //! Fügt ein Widget hinzu, welches über OpenGL gerendert wird
    static void addOverlayWidget(OverlayWidget* overlayWidget);
    static void orderOverlayWidgets();
    static bool hasFocusedOverlayWidget();

    //! Listener einfügen, die jeweils zuletzt registrierten werden zuerst gerufen
    static void addListener(Listener* d);

    //! Eigene Liste für Reshape-Events
    static void addReshapeListener(ReshapeListener* d);

    //! Eigene Liste für Objekte, die bei Idle etwas tun wollen
    static void addIdleObserver(IdleObserver* d);
    QOpenGLContext* getContext() {return context();}
    //! Callbacks
    void keyboard(int key, int modifier);
    void mouse(int button, int state, int x, int y);
    void mouseMotion(int x, int y);

private:
    void initializeGL() override;

protected slots:
    virtual void resizeGL(int w, int h) override;

    //! Slot, der von Qt angestoßen wird um neu zu zeichnen. Prüft, ob ein passender Kontext erzeugt wurde. Falls ja,
    //! wird über SceneManager::render das Rendering angestoßen. Falls nein, werden wechselnde Farben angezeigt, um
    //! das Problem zu visualisieren ;-).
    virtual void paintGL() override;
    void idle();
    void calcFPS();

signals:
    //! Schickt die aktuelle Framerate
    void sigFPS(int);

private:
    static QVector<OverlayWidget*> sOverlayWidgets;
    static QVector<Listener*> sListener;
    static QVector<IdleObserver*> sIdleObserver;
    static QVector<ReshapeListener*> sReshapeListener;
    QPoint convertCoord(const QPoint& glutp);
    static OpenGLWidget* sInstance;
    unsigned int mFrames;
    bool mGLVersionValid;

#ifdef SG_LOG_OPENGL_DEBUG_MASSAGES
    QOpenGLDebugLogger mOpenGLDebugLogger;

    void logOpenGLDebugMessage(const QOpenGLDebugMessage& message);
#endif
};

//inline QOpenGLContext* OpenGLWidget::getContext()
//{
//    return context();
//}

#endif // OPENGLWIDGET_H
