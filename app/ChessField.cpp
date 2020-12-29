#include "stdafx.h"
#include "ChessField.h"

ChessField::ChessField()
{
}

ChessField::~ChessField()
{
}

Node* ChessField::Init(Geometry* geo, ChessColor color, float tx, float tz)
{
	// all fields share same geometry
	ChessFieldActor::Init(color, geo);

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
