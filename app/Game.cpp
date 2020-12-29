#include "stdafx.h"
#include "Game.h"

Game::Game()
	: mCam(nullptr)
	, mnRoot(nullptr)
	, mHelperAxes(nullptr)
	, mChessboard(nullptr)
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

	// free helper axes
	delete mHelperAxes;
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

#ifdef _DEBUG
	mHelperAxes = new HelperAxes;
	mHelperAxes->GetTrafo()->translate(0.0f, 5.0f, 0.0f);
	mnRoot->addChild(mHelperAxes->Init());
#endif // _DEBUG

	// chessboard
	mChessboard = new Chessboard(10.0f, 0.5f);
	mnRoot->addChild(mChessboard->Init());

	// initial camera position
	//mCam->setPosition(-15.0f, 8.0f, -15.0f);
	//mCam->lookAt(QVector3D(0.0f, 0.0f, 0.0f));
	//mCam->setRotation(0.0f, 25.0f, 0.0f);	// only works with no camera controller attached

	return mnRoot;
}
