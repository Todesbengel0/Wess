#include "stdafx.h"
#include "Selection.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"



Selection::Selection(float fieldSize, Chessboard* chessBoard):
	mFieldSize(fieldSize),
	mFigureSelected(false),
	mChessBoard(chessBoard),
	mCurrentField(nullptr),
	mSelectedFigure(nullptr)	
{
	mPositionX = 0;
	mPositionZ = 0;


}

Node* Selection::Init() {
	mfPawn = new Pawn();
	mfPawn->SetPosition(-2.5, 15);
	mfRook = new Rook();
	mfRook->SetPosition(-1.5, 15);
	mfKnight = new Knight();
	mfKnight->SetPosition(-0.5f, 15);
	mfBishop = new Bishop();
	mfBishop->SetPosition(0.5f, 15);
	mfQueen = new Queen();
	mfQueen->SetPosition(1.5, 15);
	mfKing = new King();
	mfKing->SetPosition(2.5, 15);

	mCurrentField = mChessBoard->GetField(mPositionX, mPositionZ);
	mCurrentField->SetHighlighted(true); // set shader to highlighted;

	auto nTrans = new Node(&mtPosition);
	nTrans->addChild(mfPawn->Init(White));
	nTrans->addChild(mfRook->Init(White));
	nTrans->addChild(mfKnight->Init(White));
	nTrans->addChild(mfBishop->Init(White));
	nTrans->addChild(mfQueen->Init(White));
	nTrans->addChild(mfKing->Init(White));
	return nTrans;
}

Selection::~Selection()
{
}

ChessField* Selection::setField() {
	mCurrentField->SetHighlighted(false); // set shader back to normal 

	ChessField* field = mChessBoard->GetField(mPositionX, mPositionZ);
	field->SetHighlighted(true); // set shader to highlighted;
	return field;
}

void Selection::moveUp()
{
	if (mPositionZ < 7) {
		mPositionZ += 1;
		mCurrentField = setField();
	}
}

void Selection::moveDown()
{
	if (mPositionZ > 0) {
		mPositionZ -= 1;
		mCurrentField = setField();
	}
}

void Selection::moveLeft()
{
	if (mPositionX > 0) {
		mPositionX -= 1;
		mCurrentField = setField();
	}
}

void Selection::moveRight()
{
	if (mPositionX < 7) {
		mPositionX += 1;
		mCurrentField = setField();
	}
}

void Selection::selectFigure()
{
	if (mSelectedFigure != nullptr) {
		mSelectedFigure->SetHighlighted(false);
	}

	mSelectedFigure = mChessBoard->GetFigure(mPositionX, mPositionZ);

	if (mSelectedFigure != nullptr) {
		mSelectedFigure->SetHighlighted(true);
	}
}





void Selection::keyboard(int, int)
{
	auto key_in = InputRegistry::getInstance().getKeyboardInput();

	if (key_in->isKeyPressed('j'))
	{
		moveLeft();
	}
	else if (key_in->isKeyPressed('i'))
	{
		moveUp();
	}
	else if (key_in->isKeyPressed('k'))
	{
		moveDown();
	}
	else if (key_in->isKeyPressed('l'))
	{
		moveRight();
	}

	else if (key_in->isKeyPressed('+')) {
		selectFigure();
	}
}
