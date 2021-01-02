#pragma once

#include "ChessFieldActor.h"
#include "WoodenPiece.h"

class ChessField : public ChessFieldActor, public WoodenPiece
{
public:
	ChessField();
	~ChessField();

public:
	Node* Init(Geometry* geo, ChessColor color, float tx, float tz);

private:
	Transformation mTrans;
};
