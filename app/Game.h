#pragma once

//////////////////////////////////////////////////////////////////////////
// Klasse: Game
// Beschreibung: Hauptklasse des gesamten Spiels
// Autor: Vinzenz Funk
//////////////////////////////////////////////////////////////////////////

#include "Chessboard.h"

class Game
{
// ctor / dtor
public:
	Game();
	~Game();

// access
public:
	static Game& GetInstance();	// singleton
	Node* Init();

// scene objects
private:
	Transformation* mtRoot;
	Node* mnRoot;

	Chessboard* mChessboard;
	Node* mnChessboard;
};

