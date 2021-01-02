#pragma once

//////////////////////////////////////////////////////////////////////////
// Klasse: Game
// Beschreibung: Hauptklasse des gesamten Spiels
// Autor: Vinzenz Funk
//////////////////////////////////////////////////////////////////////////

#include "Chessboard.h"
#include "HelperAxes.h"

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

// initialization
public:
	Node* Init(Camera* cam);

// scene objects
private:
	Camera* mCam;

	PointLight* mLight;

	Transformation mtRoot;
	Node* mnRoot;

	HelperAxes* mHelperAxes;

	ChessBoard* mChessboard;
};

