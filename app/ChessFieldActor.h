#pragma once

#include "ChessColor.h"

class ChessFieldActor
{
// ctor / dtor
public:
	ChessFieldActor();
	virtual ~ChessFieldActor();

// initialization
protected:
	void Init(ChessColor color, Geometry* geo);

// access
public:
	void SetHighlighted(bool highlighted);

// properties
private:
	ChessColor mFieldColor;
	bool mHighlighted;

// scene objects
protected:
	Color* mDrawColor;

	Drawable* mDraw;
};
