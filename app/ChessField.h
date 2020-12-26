#pragma once

#include "ChessColor.h"

class ChessField
{
public:
	ChessField();
	~ChessField();

public:
	Node* Init(Geometry* geo, ChessColor color, float tx, float tz);

private:
	Transformation mTrans;
	Drawable* mDraw;

	ChessColor mColor;
};
