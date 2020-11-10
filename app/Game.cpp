#include "Game.h"

Game::Game()
	: mtRoot(nullptr)
	, mnRoot(nullptr)
	, mChessboard(nullptr)
	, mnChessboard(nullptr)
{
}

Game::~Game()
{
	// root node delete,
	// this destroys scene graph and all nodes!
	// freeing node memory in any component beyond this point is UB
	delete mnChessboard;

	delete mChessboard;

	delete mtRoot;
}

// singleton access, thread safe by c++11 std
Game& Game::GetInstance()
{
	static Game game;
	return game;
}

Node* Game::Init()
{
	mtRoot = new Transformation;
	mnRoot = new Node(mtRoot);

	// chessboard
	mChessboard = new Chessboard(10.0f, 0.5f);
	mnChessboard = mChessboard->Init();
	mnRoot->addChild(mnChessboard);

	return mnRoot;
}
