#pragma once

#include "ChessFieldActor.h"

class ChessField : public ChessFieldActor
{
public:
	ChessField();
	~ChessField();

public:
	Node* Init(Geometry* geo, ChessColor color, float tx, float tz);

private:
	Transformation mTrans;
};
