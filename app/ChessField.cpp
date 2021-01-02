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

	// make more "wooden-looking"
	const QVector3D wood_col(ColClamp(158), ColClamp(115), ColClamp(83));
	const QVector3D field_col(GetDrawColor());
	const QVector3D mat_col(Lerp(field_col, wood_col, 0.4f));

	WoodenPiece::Init(mDraw, mat_col, 2.0f);

//#ifdef _DEBUG
//	// reference field bottom left
//	if (tx < 0.1f && tz > -0.1f)
//		mDraw->getProperty<Color>()->setValue(1.0f, 0.0f, 0.0f);
//#endif // _DEBUG

	mTrans.translate(tx, 0.0f, tz);

	auto nTrans = new Node(&mTrans);
	nTrans->addChild(new Node(mDraw));
	
	return nTrans;
}
