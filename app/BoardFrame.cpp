#include "stdafx.h"
#include "BoardFrame.h"

BoardFrame::BoardFrame()
	: mGeo(nullptr)
	, mDraw(nullptr)
{
}

BoardFrame::~BoardFrame()
{
	delete mDraw;
	delete mGeo;
}

Node* BoardFrame::Init(GLfloat width, GLfloat height, GLfloat depth)
{
	mGeo = new SimpleCube(width, height, depth);
	mDraw = new Drawable(mGeo);

	//const QVector3D frame_col(ColClamp(158), ColClamp(115), ColClamp(83));
	const QVector3D frame_col(ColClamp(120), ColClamp(87), ColClamp(63));

	WoodenPiece::Init(mDraw, frame_col, 2.0f);

	auto n = new Node(&mtRoot);
	n->addChild(new Node(mDraw));
	return n;
}

Transformation& BoardFrame::GetTrafo()
{
	return mtRoot;
}
