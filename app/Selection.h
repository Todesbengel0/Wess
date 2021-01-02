#pragma once
#include "ChessField.h"
#include "Figure.h"
#include "ChessBoard.h"

class Selection : public Listener
{

public:
	Selection(float fieldSize, ChessBoard* chessBoard);
	~Selection();
	Node* Init();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
	void selectFigure();

	void keyboard(int, int) override;
	
	ChessField* setField();

private:
	float mFieldSize;
	int mPositionX;
	int mPositionZ;
	bool mFigureSelected;
	int mSelectionX;
	int mSelectionZ;

	Transformation mtPosition;

	ChessBoard* mChessBoard;
	ChessField* mCurrentField;
	
	Figure* mSelectedFigure;
	Figure* mfPawn;
	Figure* mfRook;
	Figure* mfKnight;
	Figure* mfBishop;
	Figure* mfQueen;
	Figure* mfKing;
};

