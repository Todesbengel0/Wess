#pragma once

#include <node.h>
#include <transformation.h>
#include <drawable.h>
#include <simplecube.h>

//////////////////////////////////////////////////////////////////////////
// Klasse: Chessboard
// Beschreibung: Hauptklasse des Schachbretts
// Autor: Vinzenz Funk
//////////////////////////////////////////////////////////////////////////

class Chessboard
{
// ctor / dtor
public:
	Chessboard(float size, float height);
	~Chessboard();

// access
public:
	Node* Init();

// getters / setters
public:
	Node* GetRoot() const;
	Transformation* GetRootTrafo() const;

// members
private:
	const float mSize;
	const float mHeight;

// scene objects
private:
	Transformation* mtRoot;
	Node* mnRoot;

	// bottom base frame of the board
	SimpleCube* mgBaseFrame;
	Drawable* mdBaseFrame;
	Node* mnBaseFrame;
};
