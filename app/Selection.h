#pragma once
#include "ChessField.h"
#include "Figure.h"
#include "ChessBoard.h"

class Selection
{

public:
	Selection(float fieldSize, ChessField* chessField);
	~Selection();
	Node* Init();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	ChessField* setField();

private:
	float mFieldSize;
	int mPositionX;
	int mPositionZ;
	bool mFigureSelected;

	Transformation mtPosition;

	Chessboard* mChessBoard;
	ChessField* mCurrentField;
	
	Figure* mfPawn;
	Figure* mfRook;
	Figure* mfKnight;
	Figure* mfBishop;
	Figure* mfQueen;
	Figure* mfKing;
};

