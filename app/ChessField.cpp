#include "stdafx.h"
#include "ChessField.h"

ChessField::ChessField()
	: mDraw(nullptr)
	, mColor(ChessColor::Black)
{
}

ChessField::~ChessField()
{
	delete mDraw;
}

Node* ChessField::Init(Geometry* geo, ChessColor color, float tx, float tz)
{
	mColor = color;

	// all fields share same geometry
	mDraw = new Drawable(geo);

	switch (color)
	{
		case White:
			mDraw->getProperty<Color>()->setValue(1.0f, 1.0f, 1.0f);
			break;
		case Black:
			mDraw->getProperty<Color>()->setValue(0.0f, 0.0f, 0.0f);
			break;
		default:
			assert(false);
			break;
	}

#ifdef _DEBUG
	// reference field bottom left
	if (tx < -3.0f && tz < -3.0f)
		mDraw->getProperty<Color>()->setValue(1.0f, 0.0f, 0.0f);
#endif // _DEBUG

	mTrans.translate(tx, 0.0f, tz);

	auto nTrans = new Node(&mTrans);
	nTrans->addChild(new Node(mDraw));
	
	return nTrans;
}
