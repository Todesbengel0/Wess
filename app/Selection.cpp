#include "stdafx.h"
#include "Selection.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"



Selection::Selection(float fieldSize, ChessBoard* chessBoard)
    : mFieldSize(fieldSize),
      mFigureSelected(false),
      mIsPromoting(false),
      mChessBoard(chessBoard),
      mCurrentField(nullptr),
      mSelectedFigure(nullptr),
      mShownFigure(nullptr)
{
	mPositionX = 0;
	mPositionZ = 0;
}

Node* Selection::Init() {
	constexpr float y = 3000.0f;
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

void Selection::MoveUp()
{
	if (mPositionZ < 7) {
		mPositionZ += 1;
		mCurrentField = setField();

        if (mSelectedFigure != nullptr)
            mSelectedFigure->MovePosition(0, -mFieldSize);

        //mfPawn->MovePosition(0, -mFieldSize);
        //mfRook->MovePosition(0, -mFieldSize);
        //mfKnight->MovePosition(0, -mFieldSize);
        //mfBishop->MovePosition(0, -mFieldSize);
        //mfQueen->MovePosition(0, -mFieldSize);
        //mfKing->MovePosition(0, -mFieldSize);
	}
}

void Selection::MoveDown()
{
	if (mPositionZ > 0) {
		mPositionZ -= 1;
		mCurrentField = setField();

        if (mSelectedFigure != nullptr)
            mSelectedFigure->MovePosition(0, mFieldSize);

        //mfPawn->MovePosition(0, mFieldSize);
        //mfRook->MovePosition(0, mFieldSize);
        //mfKnight->MovePosition(0, mFieldSize);
        //mfBishop->MovePosition(0, mFieldSize);
        //mfQueen->MovePosition(0, mFieldSize);
        //mfKing->MovePosition(0, mFieldSize);
	}
}

void Selection::MoveLeft()
{
	if (mPositionX > 0) {
		mPositionX -= 1;
		mCurrentField = setField();

        if (mSelectedFigure != nullptr)
            mSelectedFigure->MovePosition(-mFieldSize, 0);

        //mfPawn->MovePosition(-mFieldSize, 0);
        //mfRook->MovePosition(-mFieldSize, 0);
        //mfKnight->MovePosition(-mFieldSize, 0);
        //mfBishop->MovePosition(-mFieldSize, 0);
        //mfQueen->MovePosition(-mFieldSize, 0);
        //mfKing->MovePosition(-mFieldSize, 0);
	}
}

void Selection::MoveRight()
{
	if (mPositionX < 7) {
		mPositionX += 1;
		mCurrentField = setField();

        if (mSelectedFigure != nullptr)
            mSelectedFigure->MovePosition(mFieldSize, 0);

        //mfPawn->MovePosition(mFieldSize, 0);
        //mfRook->MovePosition(mFieldSize, 0);
        //mfKnight->MovePosition(mFieldSize, 0);
        //mfBishop->MovePosition(mFieldSize, 0);
        //mfQueen->MovePosition(mFieldSize, 0);
        //mfKing->MovePosition(mFieldSize, 0);
	}
}

void Selection::SelectFigure()
{
    if (mSelectedFigure != nullptr)
    { // set chesspiece to new position and deselect it
        if (mChessBoard->SetFigureOnField(mSelectionX, mSelectionZ, mPositionX, mPositionZ)) {
            if ((mPositionZ != 0 && mPositionZ != 7) || mSelectedFigure->GetType()!=0)
            {
                // unselect chesspiece
                mSelectedFigure->SetHighlighted(false);
                mSelectedFigure = nullptr;
                return;
            }
            // promotion
            PromotionInit();
		}
        return;
    }
    // select chesspiece
    mSelectedFigure = mChessBoard->GetFigure(mPositionX, mPositionZ);

    if (mSelectedFigure != nullptr) {
        mSelectedFigure->SetHighlighted(true);

        mSelectionX = mPositionX;
        mSelectionZ = mPositionZ;
    }
}

void Selection::PromotionInit()
{
    mIsPromoting = true;
    float tempX = mSelectedFigure->GetPosition().x(), tempZ = mSelectedFigure->GetPosition().y();
    mShownFigure = mfQueen;
    mSelectedFigure->SetPosition(mShownFigure->GetPosition().x(), mShownFigure->GetPosition().y());
    mShownFigure->SetHighlighted(true);
    mShownFigure->SetPosition(tempX, tempZ);
}

void Selection::ShowNext()
{
    switch(mShownFigure->GetType())
    {
    case tKnight:
        ShowFigure(mfBishop);
        return;
    case tBishop:
        ShowFigure(mfRook);
        return;
    case tRook:
        ShowFigure(mfQueen);
        return;
    case tQueen:
        ShowFigure(mfKnight);
        return;
    default:
        return;
    }
}

void Selection::ShowPrevious()
{
    switch(mShownFigure->GetType())
    {
    case tKnight:
        ShowFigure(mfQueen);
        return;
    case tBishop:
        ShowFigure(mfKnight);
        return;
    case tRook:
        ShowFigure(mfBishop);
        return;
    case tQueen:
        ShowFigure(mfRook);
        return;
    default:
        return;
    }
}

void Selection::ShowFigure(Figure *shownFigure)
{
    this->PromotionReset();
    float tempX = mSelectedFigure->GetPosition().x(), tempZ = mSelectedFigure->GetPosition().y();
    mShownFigure = shownFigure;
    mSelectedFigure->SetPosition(mShownFigure->GetPosition().x(), mShownFigure->GetPosition().y());
    mShownFigure->SetHighlighted(true);
    mShownFigure->SetPosition(tempX, tempZ);
}

void Selection::PromotionReset()
{
    float tempX = mSelectedFigure->GetPosition().x(), tempZ = mSelectedFigure->GetPosition().y();
    mSelectedFigure->SetPosition(mShownFigure->GetPosition().x(), mShownFigure->GetPosition().y());
    mShownFigure->SetHighlighted(false);
    mShownFigure->SetPosition(tempX, tempZ);
}

void Selection::PromotionFinit()
{
    this->PromotionReset();
    mSelectedFigure->SetHighlighted(false);
    mSelectedFigure = nullptr;
    mChessBoard->Promote(mPositionX, mPositionZ, mShownFigure->GetType());
    mShownFigure = nullptr;
    mIsPromoting = false;
}





void Selection::keyboard(int, int)
{
	auto key_in = InputRegistry::getInstance().getKeyboardInput();

    if (key_in->isKeyPressed('j'))
        mIsPromoting?ShowPrevious():MoveLeft();
    else if (key_in->isKeyPressed('i'))
        mIsPromoting?ShowNext():MoveUp();
    else if (key_in->isKeyPressed('k'))
        mIsPromoting?ShowPrevious():MoveDown();
    else if (key_in->isKeyPressed('l'))
        mIsPromoting?ShowNext():MoveRight();
    else if (key_in->isKeyPressed('+'))
        mIsPromoting?PromotionFinit():SelectFigure();
}
