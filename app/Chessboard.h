#pragma once
#include "ChessField.h"
#include "Figure.h"


//////////////////////////////////////////////////////////////////////////
// Klasse: Chessboard
// Beschreibung: Hauptklasse des Schachbretts
// Autor: Vinzenz Funk
//////////////////////////////////////////////////////////////////////////

class Selection;


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
	void MakeFields(Node* nFigureRoot, float field_size, float frame_height);
	void MakeFigures(Node* nFigureRoot, float field_size);
	template <class Ty>
	Ty* MakeFigure(int x, int z, Node* nFigureRoot, ChessColor color, float field_size);

// getters / setters
public:
	const Transformation& GetRootTrafo() const;
	ChessField* GetField(int x, int z);

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

	Transformation mtFieldRoot;

	// fields
	SimpleCube* mgField;
	ChessField mFields[8][8];

	// figure position on chessboard
	Transformation mtFigureRoot;
	Figure* mFigures[8][8];

	//Selection
	Selection* mSelection;
};



template <class Ty>
Ty* Chessboard::MakeFigure(int x, int z, Node* nFigureRoot, ChessColor color, float field_size)
{
	auto figure = new Ty;

	auto node = figure->Init(color);
	figure->SetPosition(x * field_size, -z * field_size);	// Z inverted on RHS
	nFigureRoot->addChild(node);

	assert(mFigures[z][x] == nullptr);
	mFigures[z][x] = figure;

	return figure;
}
