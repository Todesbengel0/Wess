#include <QOpenGLContext>
#include <QTimer>
#include <QPoint>
#include <QTime>
#include <QApplication>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QApplication>
#include <QPainter>
#include <QtAlgorithms>

#include "openglwidget.h"
#include "opengl.h"
#include "scene.h"
#include "mainwindow.h"
#include "inputregistry.h"
#include "physicengineticker.h"
#include "scenemanager.h"


SGFGLFUNCTIONS *glFunctions = NULL;

// statische Klassenvariablen von Window initialisieren
QVector<OverlayWidget*> OpenGLWidget::sOverlayWidgets;
QVector<Listener*> OpenGLWidget::sListener;
QVector<ReshapeListener*> OpenGLWidget::sReshapeListener;
QVector<IdleObserver*> OpenGLWidget::sIdleObserver;
OpenGLWidget* OpenGLWidget::sInstance = NULL;

OpenGLWidget::OpenGLWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    //Note that this constructor should not contain any calls to OpenGL resources
    mFrames = 0;
    sInstance = this;
    mGLVersionValid = false;

    qDebug() << "Widget instantiated." << this;

    // This timer drives "idle" updates for animations
    QTimer* timerI = new QTimer(this);
    connect(timerI, SIGNAL(timeout()), this, SLOT(idle()));
    timerI->start(1);

    // This timer drives the scene updates
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

    // This timer drives FPS measuring
    QTimer* timerFPS = new QTimer(this);
    connect(timerFPS, SIGNAL(timeout()), this, SLOT(calcFPS()));
    timerFPS->start(1000);
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{
    qDebug() << "Initializing OpenGL.";

    if (context())
    {
        initializeOpenGLFunctions();
#ifdef QT_OPENGL_ES
        if (context()->isOpenGLES() && context()->format().majorVersion() >= 3)
        {
            glFunctions = context()->functions();
            mGLVersionValid = true;
        }
#else
#ifdef SG_USE_OPENGL_3_3_FUNCTIONS
        if (context()->format().majorVersion() > 3 ||
            (context()->format().majorVersion() == 3 && context()->format().minorVersion() >= 3))
#else
        if (context()->format().majorVersion() >= 4)
#endif
        {
            glFunctions = context()->versionFunctions<SGFGLFUNCTIONS>();
            if (glFunctions)
            {
                mGLVersionValid = true;
            }
            else
            {
                qDebug()
                  << "Die OpenGL Funktionen konten nicht initialisiert werden obwohl die openGL version hoch genug ist";
            }
        }
        else
        {
            qDebug() << "Die erkannte OpenGL Version ist zu niedrig!";
        }
#endif
        qDebug() << "Context with versions created: " << context()->format().majorVersion() << context()->format().minorVersion();
    }
    else
    {
        qDebug() << "Context is Zero.";
    }

    // Initialize Scenemanager Singleton
    SceneManager::instance()->init();
    new PhysicEngineTicker();

    // Cursor in die Mitte setzen, hilfreich z.B. für Kamera mit Maussteuerung
    QPoint lMiddle = QPoint(width() / 2, height() / 2);
    this->cursor().setPos(this->mapToGlobal(lMiddle));
    resizeGL(width(), height());

#ifdef SG_LOG_OPENGL_DEBUG_MASSAGES
    if (!mOpenGLDebugLogger.initialize())
    {
        qWarning(
          "QOpenGLDebugLogger konte nicht initialisiert werden wahrscheinlich fehlt die passende OpenGL extension.");
    }
    else
    {
        mOpenGLDebugLogger.enableMessages(); // enable all messages
        connect(&mOpenGLDebugLogger, &QOpenGLDebugLogger::messageLogged, this, &OpenGLWidget::logOpenGLDebugMessage);
        mOpenGLDebugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }
#endif // SG_LOG_OPENGL_DEBUG_MASSAGES
}

void OpenGLWidget::paintGL()
{
    mFrames++;

    if (mGLVersionValid)
    {
        SceneManager::instance()->renderScene();
    }
    else
    {
        // If no glfunctions available go into color buffer test mode
        static float sTestIntensity = 0.f;
        static int sTestRGB = 0;
        static QElapsedTimer sTimer = QElapsedTimer();
        if (sTestIntensity < 1.f)
        {
            sTestIntensity += 0.0005f * sTimer.restart();
            switch (sTestRGB)
            {
            case 0:
                glClearColor(1.f - sTestIntensity, sTestIntensity, 0.f, 1.f);
                break;
            case 1:
                glClearColor(0.f, 1.f - sTestIntensity, sTestIntensity, 1.f);
                break;
            case 2:
                glClearColor(sTestIntensity, 0.f, 1.f - sTestIntensity, 1.f);
                break;
            default:
                break;
            }
        }
        else
        {
            sTestIntensity = 0.f;
            sTestRGB = (sTestRGB + 1) % 3;
        }
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void OpenGLWidget::calcFPS()
{
    static QTime lTime;
    static bool start = true;
    static QWidget* lMainWindow = NULL;

    // Find Mainwindow - otherwise have to get parent widgets as parameter through whole hierarchy
    if (lMainWindow == NULL)
    {
        foreach (QWidget* widget, qApp->topLevelWidgets())
        {
            if (widget->inherits("QMainWindow"))
            {
                lMainWindow = widget;
            }
        }
    }

    if (start)
    {
        lTime.start();
        start = false;
    }
    else if (lTime.elapsed() > 0)
    {
        int lFPS = (mFrames * 1000) / lTime.elapsed();

        QString str = QString(QApplication::instance()->applicationName() + " - " + " (%1 fps)").arg(lFPS);
        qobject_cast<QMainWindow*>(lMainWindow)->setWindowTitle(str);

        // emit signal for Display of FPS in UI
        if (OpenGLWidget::getInstance())
        {
            emit OpenGLWidget::getInstance()->sigFPS(lFPS);
        }

        mFrames = 0;
        lTime.restart();
    }
}

void OpenGLWidget::resizeGL(int w, int h)
{
    int lWidth = this->devicePixelRatio() * w;
    int lHeight = this->devicePixelRatio() * h;

    QVector<ReshapeListener*>::iterator i;

    if (!OpenGLWidget::sReshapeListener.empty())
    {
        for (i = OpenGLWidget::sReshapeListener.begin(); i != OpenGLWidget::sReshapeListener.end(); ++i)
        {
            (*i)->reshape(lWidth, lHeight);
        }
    }
}

void OpenGLWidget::idle()
{
    QVector<IdleObserver*>::iterator i;

    for (i = OpenGLWidget::sIdleObserver.begin(); i != OpenGLWidget::sIdleObserver.end(); ++i)
    {
        (*i)->doIt();
    }
}

void OpenGLWidget::keyboard(int key, int modifier)
{
    QVector<Listener*>::iterator i;

    for (i = OpenGLWidget::sListener.begin(); i != OpenGLWidget::sListener.end(); ++i)
    {
        (*i)->keyboard(key, modifier);
    }
}

QPoint OpenGLWidget::convertCoord(const QPoint& glutp)
{
    int h = height() - 1;
    QPoint res(glutp.x(), h - glutp.y());
    return res;
}

#ifdef SG_LOG_OPENGL_DEBUG_MASSAGES
void OpenGLWidget::logOpenGLDebugMessage(const QOpenGLDebugMessage& message)
{
    if (message.message().startsWith(QString("API_ID_REDUNDANT_FBO")))
    {
        //Folgende Nachricht ignorieren, da sie von der Implementierung von QOpenGLWindow selbst erzeugt wird (mehrfaches binden des Default FB)
        // QOpenGLDebugMessage("APISource", 8, "API_ID_REDUNDANT_FBO performance warning has been generated. Redundant state change in glBindFramebuffer API call, FBO 0, \"\", already bound.", "LowSeverity", "PerformanceType")
        return;
    }
    else if (message.message().startsWith(QString("Texture state usage warning:")))
    {
        //Diese Meldung ignorieren, liegt wohl an (Nicht-)Verwendung der Texture Unit 0, wenn man dennoch hoehere, z.B. 6 fuer Shadow Maps verwendet.
        //QOpenGLDebugMessage("APISource", 131204, "Texture state usage warning: Texture 0 is base level inconsistent. Check texture size.", "LowSeverity", "OtherType")
        return;
    }
    else if (message.severity() == QOpenGLDebugMessage::NotificationSeverity)
    {
        //Notifications ignorieren
        return;
    }

    qWarning() << message;
}
#endif

void OpenGLWidget::mouseMotion(int x, int y)
{
    QVector<Listener*>::iterator i;
    QPoint dummy(x, y);

    dummy = this->convertCoord(dummy);
    InputRegistry::getInstance().getMouseInput()->setMousePosition(dummy);
    for (i = OpenGLWidget::sListener.begin(); i != OpenGLWidget::sListener.end(); ++i)
    {
        (*i)->mouseMotion(dummy);
    }
}

void OpenGLWidget::addOverlayWidget(OverlayWidget* overlayWidget)
{
    OpenGLWidget::sOverlayWidgets.push_front(overlayWidget);
}

void OpenGLWidget::orderOverlayWidgets()
{
    qSort(OpenGLWidget::sOverlayWidgets.begin(), OpenGLWidget::sOverlayWidgets.end(), [](OverlayWidget* a, OverlayWidget* b) -> bool { return a->getLayer() < b->getLayer(); });

    for(OverlayWidget* overlayWidget : OpenGLWidget::sOverlayWidgets)
        overlayWidget->raise();
}

bool OpenGLWidget::hasFocusedOverlayWidget()
{
    for(OverlayWidget* overlayWidget : OpenGLWidget::sOverlayWidgets)
        if(overlayWidget->hasFocus())
            return true;
    return false;
}

void OpenGLWidget::addListener(Listener* d)
{
    OpenGLWidget::sListener.push_front(d);
}

void OpenGLWidget::addIdleObserver(IdleObserver* d)
{
    OpenGLWidget::sIdleObserver.push_front(d);
}

void OpenGLWidget::addReshapeListener(ReshapeListener* d)
{
    OpenGLWidget::sReshapeListener.push_front(d);
}

OpenGLWidget* OpenGLWidget::getInstance()
{
    if (!OpenGLWidget::sInstance)
    {
        qDebug() << "Window not properly instantiated!" << endl;
    }
    return OpenGLWidget::sInstance;
}

void OpenGLWidget::keyPressEvent(QKeyEvent* e)
{
    // Koordinaten in Qt leider nicht verfuegbar...
    // Qt liefert immer Großbuchstaben und dann einen Modifier
    // Umrechnen!

    int key = e->key();

    if (InputRegistry::getInstance().hasKeyboardInput()) {

        if (e->modifiers() != Qt::ShiftModifier && key <= 'Z' && key >= 'A')
        {
            key = tolower(key);
        }
        InputRegistry::getInstance().getKeyboardInput()->setKeyDown(key, ( int )(e->modifiers()));
    }
    keyboard(key, e->modifiers());
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (InputRegistry::getInstance().hasKeyboardInput()) {
        InputRegistry::getInstance().getKeyboardInput()->setKeyUp(event->key(), ( int )(event->modifiers()));
        if (event->modifiers() != Qt::ShiftModifier && event->key() <= 'Z' && event->key() >= 'A')
        {
            InputRegistry::getInstance().getKeyboardInput()->setKeyUp(tolower(event->key()), ( int )(event->modifiers()));
        }

        // Überprüfe ob Betriebssystem abhängige tastenkombination für Vollbild betätigt wurde.
        // Unter Windows F11, OS X Ctrl+Meta+F, KDE F11 ... siehe http://doc.qt.io/qt-5/qkeysequence.html
        if (event->matches(QKeySequence::FullScreen))
        {
            SceneManager* sceneManager = SceneManager::instance();
            sceneManager->setFullScreen(!sceneManager->isInFullScreenMode());
        }
    }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent* event)
{
    mouseMotion(event->x(), event->y());
    InputRegistry::getInstance().getMouseInput()->setMouseButtons(( int )(event->buttons()));
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    QVector<Listener*>::iterator i;
    QPoint dummy = convertCoord(event->pos());

    InputRegistry::getInstance().getMouseInput()->setMousePosition(dummy);
    InputRegistry::getInstance().getMouseInput()->setMouseButtons(( int )(event->buttons()));

    if (event->button() == Qt::LeftButton)
    {
        for (i = OpenGLWidget::sListener.begin(); i != OpenGLWidget::sListener.end(); ++i)
        {
            (*i)->mouseLeftUp(dummy);
        }
    }
}

void OpenGLWidget::mousePressEvent(QMouseEvent* event)
{
    QVector<Listener*>::iterator i;
    QPoint dummy = convertCoord(event->pos());

    InputRegistry::getInstance().getMouseInput()->setMousePosition(dummy);
    InputRegistry::getInstance().getMouseInput()->setMouseButtons(( int )(event->buttons()));

    if (event->button() == Qt::LeftButton)
    {
        for (i = OpenGLWidget::sListener.begin(); i != OpenGLWidget::sListener.end(); ++i)
        {
            (*i)->mouseLeftDown(dummy);
        }
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent* e)
{
    InputRegistry::getInstance().getMouseInput()->addMouseWheelDelta(e->angleDelta().y());
}
