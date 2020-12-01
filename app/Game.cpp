#include "stdafx.h"
#include "Game.h"

Game::Game()
	: mnRoot(nullptr)
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
}

// singleton access, thread safe by c++11 std
Game& Game::GetInstance()
{
	static Game game;
	return game;
}

Node* Game::Init()
{
	mnRoot = new Node(&mtRoot);

	// chessboard
	mChessboard = new Chessboard(10.0f, 0.5f);
	mnRoot->addChild(mChessboard->Init());

	return mnRoot;
}
