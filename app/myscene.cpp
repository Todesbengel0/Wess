#include <opengl.h>
#include <openglwidget.h>
#include <screenrenderer.h>
#include <scenemanager.h>
#include <transformation.h>
#include <keyboardtransformation.h>
#include <mousekeyboardcameracontroller.h>
#include <color.h>
#include <simplecube.h>

#include <QDir>

#include "ui_dockwidget.h"

Node *InitScene();

void SceneManager::initScenes()
{
    auto lDockWidget = new QDockWidget(QString("FPS"), SceneManager::getMainWindow());

    auto cam = new Camera();
    auto camController = new MouseKeyboardCameraController(cam);
    Q_UNUSED(camController);
    auto myContext=new RenderingContext(cam);
    unsigned int myContextNr = SceneManager::instance()->addContext(myContext);
    unsigned int myScene = SceneManager::instance()->addScene(InitScene());
    auto myRenderer = new ScreenRenderer(myContextNr, myScene);
	Q_UNUSED(myRenderer);

    //Vorsicht: Die Szene muss initialisiert sein, bevor das Fenster verändert wird (Fullscreen)
    SceneManager::instance()->setActiveScene(myScene);
    SceneManager::instance()->setActiveContext(myContextNr);
//    SceneManager::instance()->setFullScreen();

    auto lDock = new Ui_FPSWidget();
    lDock->setupUi(lDockWidget);
    lDockWidget->resize(200,100);
    SceneManager::getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, lDockWidget);
    lDockWidget->show();
    QObject::connect(OpenGLWidget::getInstance(), SIGNAL(sigFPS(int)), lDock->lcdNumber, SLOT(display(int)));
}

Node* InitScene()
{
    auto root = new KeyboardTransformation();
    auto nroot = new Node(root);

	// for resource location
	static const QDir cur_dir(SRCDIR);
	auto model_path = cur_dir.relativeFilePath("../models/...");

    auto test_cube = new Drawable(new SimpleCube(2.0f, 2.0f, 2.0f));
    test_cube->getProperty<Color>()->setValue(0.5f, 1.0f, 0.5f);

    nroot->addChild(new Node(test_cube));
    return nroot;
}
