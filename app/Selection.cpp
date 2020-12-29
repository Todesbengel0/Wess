#include "stdafx.h"
#include "Selection.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"



Selection::Selection(float fieldSize, ChessField* chessField):
	mFieldSize(fieldSize),
	mFigureSelected(false),
	mCurrentField(chessField)
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
	//mCurrentField; // set shader back to normal 

	ChessField* field = mChessBoard->GetField(mPositionX, mPositionZ);
	//field; // set shader to highlighted;
	return field;
}

void Selection::moveUp()
{
	mPositionZ -= 1;
	mCurrentField = setField();
}

void Selection::moveDown()
{
	mPositionZ += 1;
	mCurrentField = setField();
}

void Selection::moveLeft()
{
	mPositionX -= 1;
	mCurrentField = setField();
}

void Selection::moveRight()
{
	mPositionX += 1;
	mCurrentField = setField();
}
