#pragma once

#include "ChessColor.h"

class ChessFieldActor
{
public:
	ChessFieldActor();
	virtual ~ChessFieldActor();

protected:
	void Init(ChessColor color);

// properties
protected:
	ChessColor mFieldColor;

// scene objects
protected:
	Color* mDrawColor;
};
