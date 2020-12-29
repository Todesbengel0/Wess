#include "stdafx.h"
#include "ChessField.h"

ChessField::ChessField()
	: mDraw(nullptr)
{
}

ChessField::~ChessField()
{
	delete mDraw;
}

Node* ChessField::Init(Geometry* geo, ChessColor color, float tx, float tz)
{
	ChessFieldActor::Init(color);

	// all fields share same geometry
	mDraw = new Drawable(geo);
	mDraw->setProperty<Color>(mDrawColor);

#ifdef _DEBUG
	// reference field bottom left
	if (tx < 0.1f && tz > -0.1f)
		mDraw->getProperty<Color>()->setValue(1.0f, 0.0f, 0.0f);
#endif // _DEBUG

	mTrans.translate(tx, 0.0f, tz);

	auto nTrans = new Node(&mTrans);
	nTrans->addChild(new Node(mDraw));
	
	return nTrans;
}
