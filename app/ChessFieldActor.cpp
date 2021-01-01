#include "stdafx.h"
#include "ChessFieldActor.h"

ChessFieldActor::ChessFieldActor()
	: mFieldColor(ChessColor::Black)
	, mHighlighted(false)
	, mDrawColor(nullptr)
	, mDraw(nullptr)
{
}

ChessFieldActor::~ChessFieldActor()
{
	delete mDrawColor;
	delete mDraw;
}

void ChessFieldActor::Init(ChessColor color, Geometry* geo)
{
	mFieldColor = color;
	mDraw = new Drawable(geo);

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

	mDraw->setProperty<Color>(mDrawColor);
}

ChessColor ChessFieldActor::GetFieldColor() const
{
	return mFieldColor;
}

void ChessFieldActor::SetHighlighted(bool highlighted)
{
	if (highlighted == mHighlighted)
		return;

	mHighlighted = highlighted;

	if (mHighlighted)
	{
		static Color col_highlighted(0.8f, 0.0f, 0.0f);
		mDraw->setProperty<Color>(&col_highlighted);
	}
	else
	{
		mDraw->setProperty<Color>(mDrawColor);
	}
}
