#pragma once
#include "ChessField.h"
#include "Figure.h"
#include "ChessBoard.h"

class Selection : public Listener
{

public:
	Selection(float fieldSize, Chessboard* chessBoard);
	~Selection();
	Node* Init();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void keyboard(int, int) override;
	
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

