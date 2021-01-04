#include "stdafx.h"
#include "Game.h"

Game::Game()
	: mCam(nullptr)
	, mLight(nullptr)
	, mnRoot(nullptr)
	, mHelperAxes(nullptr)
	, mChessboard(nullptr)
	, mgEnvironmentSphere(nullptr)
	, mdEnvironmentSphere(nullptr)
{
}

Game::~Game()
{
	// root node delete,
	// this destroys scene graph and all nodes!
	// freeing node memory elsewhere is UB
	delete mnRoot;

	// free chessboard & related memory
	delete mChessboard;

	// free environment
	delete mdEnvironmentSphere;
	delete mgEnvironmentSphere;

	// free helper axes
	delete mHelperAxes;

	// free lights
	delete mLight;
}

// singleton access, thread safe by c++11 std
Game& Game::GetInstance()
{
	static Game game;
	return game;
}

Node* Game::Init(Camera* cam)
{
	mCam = cam;
	mnRoot = new Node(&mtRoot);

	// lights
	mLight = new PointLight();
	mLight->getTransformation().translate(0.0f, 10.0f, 0.0f);
	mLight->setDiffuse(0.9f, 0.9f, 0.9f);
	mLight->setSpecular(0.6f, 0.6f, 0.6f);
	mLight->setAmbient(0.6f, 0.6f, 0.6f);
	mLight->turnOn();
	mnRoot->addChild(new Node(mLight));

	// helper axes
#ifdef _DEBUG
	mHelperAxes = new HelperAxes;
	mHelperAxes->GetTrafo()->translate(0.0f, 5.0f, 0.0f);
	mnRoot->addChild(mHelperAxes->Init());
#endif // _DEBUG

	// chessboard
	mChessboard = new ChessBoard(10.0f, 0.5f);
	mnRoot->addChild(mChessboard->Init());

	// environment
	mgEnvironmentSphere = new TriangleMesh(":/models/env_sphere.obj");
	mdEnvironmentSphere = new Drawable(mgEnvironmentSphere);
	mdEnvironmentSphere->getProperty<Texture>()->loadPicture(":/textures/environment.jpg");
	mdEnvironmentSphere->setShader(ShaderManager::getShader(":/shaders/Environment.vert", ":/shaders/Environment.frag"));
	auto nEnvironmentSphere = new Node(&mtEnvironmentSphere);
	nEnvironmentSphere->addChild(new Node(mdEnvironmentSphere));
	mnRoot->addChild(nEnvironmentSphere);


	// initial camera position
	//mCam->setPosition(-15.0f, 8.0f, -15.0f);
	//mCam->lookAt(QVector3D(0.0f, 0.0f, 0.0f));
	//mCam->setRotation(0.0f, 25.0f, 0.0f);	// only works with no camera controller attached

	return mnRoot;
}
