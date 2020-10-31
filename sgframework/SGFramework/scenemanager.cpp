#include "qdesktopwidget.h"
#include "scenemanager.h"
#include "screenrenderer.h"
#include "texture.h"

// Vom Designer generierten Header laden, damit Signals verbunden werden können
#include "ui_glwithwidgets.h"

SceneManager* SceneManager::sInstance = NULL;

SceneManager* SceneManager::instance()
{
    if (sInstance == NULL)
    {
        sInstance = new SceneManager;
    }
    return sInstance;
}

SceneManager::SceneManager()
{
    // prüfe eimalig ob die propertyIds richtig gesetzt wurden
    Q_ASSERT_X(Shader::getPropertyId() == DrawablePropertyId::Shader, "SceneManager::SceneManager",
               "Die PropertyId des Shaders ist nicht wie gewünscht DrawablePropertyId::Shader");
    Q_ASSERT_X(Texture::getPropertyId() == DrawablePropertyId::Texture, "SceneManager::SceneManager",
               "Die PropertyId der Textur ist nicht wie gewünscht DrawablePropertyId::Texture");
    Q_ASSERT_X(Geometry::getPropertyId() == DrawablePropertyId::Geometry, "SceneManager::SceneManager",
               "Die PropertyId der Geometry ist nicht wie gewünscht DrawablePropertyId::Geometry");

    mRenderManager = new RenderManager();
    mCurContext = 0;
    mActiveScene = 0;
}

QMainWindow* SceneManager::getMainWindow() const
{
    return mMainWindow;
}

RenderManager* SceneManager::getRenderManager()
{
    if (mRenderManager)
    {
        return mRenderManager;
    }
    else
    {
        qDebug() << "No Rendermanager in SceneManager!";
        return NULL;
    }
}

void SceneManager::free()
{
    for (int i = 0; i < mContexts.size(); i++)
    {
        delete mContexts[i];
    }
}

RenderingContext* SceneManager::getActiveContext()
{
    if (mContexts.empty())
    {
        qDebug() << "No valid Context set in Scenemanager.";
        return (NULL);
    }
    return mContexts[mCurContext];
}

void SceneManager::connectUiSignals(Ui::GLwithWidgets* /*pGLwithWidgets*/)
{
    // Hier können die Signals des UI mit eigenen QObject Slots verbunden werden und umgekehrt
}

void SceneManager::setWindowReference(MainWindow* pQMainWindow, Ui::GLwithWidgets* pGLwithWidgets)
{
    mGLwithWidgets = pGLwithWidgets;
    mMainWindow = pQMainWindow;
}

void SceneManager::setFullScreen(bool fullScreen)
{
    if (fullScreen == mMainWindow->isFullScreen())
    {
        // nichts zu tun da schon im richtigen modus
        return;
    }

    if (fullScreen)
    {
        mMainWindow->showFullScreen();
        mMainWindow->setCursor(Qt::BlankCursor);
        mGLwithWidgets->myGLWidget->setGeometry(QApplication::desktop()->screenGeometry());
        mGLwithWidgets->myGLWidget->showFullScreen();
    }
    else
    {
        mMainWindow->showNormal();
        mMainWindow->setCursor(Qt::ArrowCursor);
        mGLwithWidgets->myGLWidget->showNormal();
    }
}

unsigned short SceneManager::setActiveContext(unsigned short nr)
{
    unsigned int oldContext = mCurContext;
    if (mContexts.size() > nr)
    {
        mCurContext = nr;
    }
    else
    {
        qDebug() << "Invalid Renderingcontext.";
    }
    //    if (nr != 0 || oldContext != 0)
    //        qDebug() << "Context is " << nr << ", was " << oldContext;
    return (oldContext);
}

unsigned short SceneManager::setActiveScene(unsigned short nr)
{
    unsigned int oldScene = mActiveScene;
    if (mScenes.size() > nr)
    {
        mActiveScene = nr;
    }
    else
    {
        qDebug() << "Invalid Scene.";
    }
    return (oldScene);
}

void SceneManager::renderScene()
{
    mRenderManager->renderScene();
}

void SceneManager::init()
{
    // Call init method of concrete Scene
    SceneManager::instance()->initScenes();
}
