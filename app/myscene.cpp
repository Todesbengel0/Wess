#include "stdafx.h"
#include <openglwidget.h>
#include <screenrenderer.h>
#include <scenemanager.h>
#include <keyboardtransformation.h>
#include <mousekeyboardcameracontroller.h>

// controls if the frame should have the right side FPS widget
//#define HAVE_DOCKWIDGET

#ifdef HAVE_DOCKWIDGET
#include "ui_dockwidget.h"
#endif // HAVE_DOCKWIDGET

Node *InitScene(Camera* cam);

void SceneManager::initScenes()
{
#ifdef HAVE_DOCKWIDGET
    auto lDockWidget = new QDockWidget(QString("FPS"), SceneManager::getMainWindow());
#endif // HAVE_DOCKWIDGET

	auto cam = new Camera();
	cam->setNearPlane(0.01f);

	auto camController = new MouseKeyboardCameraController(cam);
	Q_UNUSED(camController);

    auto myContext=new RenderingContext(cam);
    unsigned int myContextNr = SceneManager::instance()->addContext(myContext);
    unsigned int myScene = SceneManager::instance()->addScene(InitScene(cam));
    auto myRenderer = new ScreenRenderer(myContextNr, myScene);
	Q_UNUSED(myRenderer);

    //Vorsicht: Die Szene muss initialisiert sein, bevor das Fenster verändert wird (Fullscreen)
    SceneManager::instance()->setActiveScene(myScene);
    SceneManager::instance()->setActiveContext(myContextNr);
//    SceneManager::instance()->setFullScreen();

#ifdef HAVE_DOCKWIDGET
    auto lDock = new Ui_FPSWidget();
    lDock->setupUi(lDockWidget);
    lDockWidget->resize(200,100);
    SceneManager::getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, lDockWidget);
    lDockWidget->show();
    QObject::connect(OpenGLWidget::getInstance(), SIGNAL(sigFPS(int)), lDock->lcdNumber, SLOT(display(int)));
#endif // HAVE_DOCKWIDGET
}

#include "Game.h"

Node* InitScene(Camera* cam)
{
	return Game::GetInstance().Init(cam);
}
