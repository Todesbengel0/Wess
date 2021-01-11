#pragma once
#include "ChessField.h"
#include "Figure.h"
#include "Chessboard.h"

class Selection : public Listener
{

public:
	Selection(float fieldSize, ChessBoard* chessBoard);
	~Selection();
	Node* Init();

	void keyboard(int, int) override;
	
	ChessField* setField();

//Figure and Field Selection
private:
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void SelectFigure();

//Promotion
private:
    void PromotionInit();
    void ShowNext();
    void ShowPrevious();
    void ShowFigure(Figure* shownFigure);
    void PromotionReset();
    void PromotionFinit();

private:
	float mFieldSize;
	int mPositionX;
	int mPositionZ;
	bool mFigureSelected;
	int mSelectionX;
	int mSelectionZ;
    bool mIsPromoting;

	Transformation mtPosition;

	ChessBoard* mChessBoard;
	ChessField* mCurrentField;
	
	Figure* mSelectedFigure;
    Figure* mViewableFigures[6];

    //Promotion
    Figure* mShownFigure;
};

