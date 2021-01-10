#pragma once

#include "WoodenPiece.h"

class BoardFrame : public WoodenPiece
{
public:
	BoardFrame();
	~BoardFrame();

public:
	Node* Init(GLfloat width, GLfloat height, GLfloat depth);
	Transformation& GetTrafo();

private:
	Transformation mtRoot;
	SimpleCube* mGeo;
	Drawable* mDraw;
};
