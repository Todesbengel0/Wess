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
	mCurrentField(nullptr)
{
	mPositionX = 0;
	mPositionZ = 0;


}

Node* Selection::Init() {
	mfPawn = new Pawn();
	mfRook = new Rook();
	mfKnight = new Knight();
	mfBishop = new Bishop();
	mfQueen = new Queen();
	mfKing = new King();

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
}
