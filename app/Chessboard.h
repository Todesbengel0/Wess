#pragma once
#include "ChessField.h"
#include "Figure.h"

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

// initialization
public:
	Node* Init();
private:
	void MakeSideFrame(Node* nTopFrameCenter, float frame_thickness, float frame_height);
	void MakeFields(Node* nTopFrameCenter, float field_size, float frame_height);

// getters / setters
public:
	const Transformation& GetRootTrafo() const;

// members
private:
	const float mSize;
	const float mHeight;

// scene objects
private:
	// root of the chessboard
	Transformation mtRoot;

	// bottom base frame of the board
	SimpleCube* mgBaseFrame;
	Drawable* mdBaseFrame;
	Transformation mtBaseFrame;

	// side frame
	Transformation mtTopFrameCenter;
	SimpleCube* mgSideFrame;
	Drawable* mdSideFrame;
	Transformation mtsSideFrame[4];

	//
	SimpleCube* mgField;
	ChessField mFields[8][8];

	// figure position on chessboard
	Figure* mFigurePosition[8][8];
};
