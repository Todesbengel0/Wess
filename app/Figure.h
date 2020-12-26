#pragma once

#include "ChessColor.h"

class Figure
{
public: 
	Figure(ChessColor color);
	~Figure();

	void SetPosition(int x, int y);

private:
	float mSize;
	ChessColor mColor;

	Transformation mtPosition;
	Drawable* mdFigure;
	Geometry* mgMesh;
};

