#include "stdafx.h"
#include "Selection.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"



Selection::Selection(float fieldSize, ChessBoard* chessBoard):
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
	float y = 25.0f;
	mfPawn = new Pawn();
	mfPawn->SetPosition(-2.5, y);
	mfRook = new Rook();
	mfRook->SetPosition(-1.5, y);
	mfKnight = new Knight();
	mfKnight->SetPosition(-0.5f, y);
	mfBishop = new Bishop();
	mfBishop->SetPosition(0.5f, y);
	mfQueen = new Queen();
	mfQueen->SetPosition(1.5, y);
	mfKing = new King();
	mfKing->SetPosition(2.5, y);

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

Selection::~Selection() = default;

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

		if (mSelectedFigure != nullptr) {
			mSelectedFigure->step(0, -mFieldSize);
		}

		mfPawn->step(0, -mFieldSize);
		mfRook->step(0, -mFieldSize);
		mfKnight->step(0, -mFieldSize);
		mfBishop->step(0, -mFieldSize);
		mfQueen->step(0, -mFieldSize);
		mfKing->step(0, -mFieldSize);
	}
}

void Selection::moveDown()
{
	if (mPositionZ > 0) {
		mPositionZ -= 1;
		mCurrentField = setField();

		if (mSelectedFigure != nullptr) {
			mSelectedFigure->step(0, mFieldSize);
		}

		mfPawn->step(0, mFieldSize);
		mfRook->step(0, mFieldSize);
		mfKnight->step(0, mFieldSize);
		mfBishop->step(0, mFieldSize);
		mfQueen->step(0, mFieldSize);
		mfKing->step(0, mFieldSize);
	}
}

void Selection::moveLeft()
{
	if (mPositionX > 0) {
		mPositionX -= 1;
		mCurrentField = setField();

		if (mSelectedFigure != nullptr) {
			mSelectedFigure->step(-mFieldSize, 0);
		}

		mfPawn->step(-mFieldSize, 0);
		mfRook->step(-mFieldSize, 0);
		mfKnight->step(-mFieldSize, 0);
		mfBishop->step(-mFieldSize, 0);
		mfQueen->step(-mFieldSize, 0);
		mfKing->step(-mFieldSize, 0);
	}
}

void Selection::moveRight()
{
	if (mPositionX < 7) {
		mPositionX += 1;
		mCurrentField = setField();

		if (mSelectedFigure != nullptr) {
			mSelectedFigure->step(mFieldSize, 0);
		}

		mfPawn->step(mFieldSize, 0);
		mfRook->step(mFieldSize, 0);
		mfKnight->step(mFieldSize, 0);
		mfBishop->step(mFieldSize, 0);
		mfQueen->step(mFieldSize, 0);
		mfKing->step(mFieldSize, 0);
	}
}

void Selection::selectFigure()
{
	if (mSelectedFigure != nullptr) { // set chesspiece to new position and deselect it 
		if (mChessBoard->SetFigureOnField(mSelectionX, mSelectionZ, mPositionX, mPositionZ)) {
			// unselect chesspiece
			mSelectedFigure->SetHighlighted(false);
			mSelectedFigure = nullptr;
		}
	}
	else {  
		// select chesspiece
		mSelectedFigure = mChessBoard->GetFigure(mPositionX, mPositionZ);

		if (mSelectedFigure != nullptr) {
			mSelectedFigure->SetHighlighted(true);

			mSelectionX = mPositionX;
			mSelectionZ = mPositionZ;
		}
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
