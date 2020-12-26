#pragma once

//////////////////////////////////////////////////////////////////////////
// Klasse: Game
// Beschreibung: Hauptklasse des gesamten Spiels
// Autor: Vinzenz Funk
//////////////////////////////////////////////////////////////////////////

#include "Chessboard.h"

class Game
{
// ctor / dtor singleton
private:
	Game();
public:
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	~Game();

	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;


// access
public:
	static Game& GetInstance();	// singleton
	void gameloop();

// initialization
public:
	Node* Init(Camera* cam);

// scene objects
private:
	Camera* mCam;

	Transformation mtRoot;
	Node* mnRoot;

	Chessboard* mChessboard;
};

