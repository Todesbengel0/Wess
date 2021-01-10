#pragma once
#include "ChessField.h"
#include "Figure.h"
#include "Graveyard.h"


//////////////////////////////////////////////////////////////////////////
// Klasse: Chessboard
// Beschreibung: Hauptklasse des Schachbretts
// Autor: Vinzenz Funk
//////////////////////////////////////////////////////////////////////////

class Selection;


class ChessBoard
{
// ctor / dtor
public:
	ChessBoard(float size, float height);
	~ChessBoard();

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
	Transformation& GetRootTrafo();
	ChessField* GetField(int x, int z); 
	Figure* GetFigure(int x, int z);
	bool SetFigureOnField(int x, int z, int tox, int toz);
    //! Setze die Figur auf ein Feld, ohne irgendwelche Logik zu beachten (Handle with Caution!)
    bool SetFigureOnFieldNoLogic(int x, int z, int tox, int toz);
    float GetFieldSize();
    Figure* GetSpeedyPawn();

// public methods
public:
    //! Promotes a pawn to a figure of a specific type
    void Promote(int x, int z, int type);

// members
private:
	const float mSize;
	const float mHeight;
    float mfield_size;
    bool mIsWhiteTurn;
    //! Bauer, der im letzten regulären Zug, zwei Felder nach vorne bewegt wurde
    Figure* mSpeedyPawn;

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
    Figure* mPromotionFigures[2][4][8];

	//Selection
	Selection* mSelection;

    //Graveyards
    Graveyard* mWhiteGraveyard;
    Graveyard* mBlackGraveyard;
};



template <class Ty>
Ty* ChessBoard::MakeFigure(int x, int z, Node* nFigureRoot, ChessColor color, float field_size)
{
	auto figure = new Ty;

	auto node = figure->Init(color, x, z);
	figure->SetPosition(x * field_size, -z * field_size);	// Z inverted on RHS
	nFigureRoot->addChild(node);

	assert(mFigures[z][x] == nullptr);
	mFigures[z][x] = figure;

	return figure;
}
