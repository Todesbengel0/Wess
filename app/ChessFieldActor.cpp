#include "stdafx.h"
#include "ChessFieldActor.h"

ChessFieldActor::ChessFieldActor()
	: mFieldColor(ChessColor::Black)
	, mDrawColor(nullptr)
{
}

ChessFieldActor::~ChessFieldActor()
{
	delete mDrawColor;
}

void ChessFieldActor::Init(ChessColor color)
{
	mFieldColor = color;

	switch (mFieldColor)
	{
		case White:
			mDrawColor = new Color(1.0f, 1.0f, 0.9f);
			break;

		case Black:
			mDrawColor = new Color(0.1f, 0.1f, 0.1f);
			break;

		default:
			assert(false);
			break;
	}
}
