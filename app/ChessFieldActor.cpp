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

	const QVector3D draw_col = GetDrawColor();
	mDrawColor = new Color(draw_col.x(), draw_col.y(), draw_col.z());
	mDraw->setProperty<Color>(mDrawColor);
}

ChessColor ChessFieldActor::GetFieldColor() const
{
	return mFieldColor;
}

QVector3D ChessFieldActor::GetDrawColorByFieldColor(ChessColor color)
{
	switch (color)
	{
		case White:
			return QVector3D(1.0f, 1.0f, 0.9f);

		case Black:
			return QVector3D(0.1f, 0.1f, 0.1f);
	}

	assert(false);
	return QVector3D(1.0f, 1.0f, 1.0f);
}

QVector3D ChessFieldActor::GetDrawColor() const
{
	return GetDrawColorByFieldColor(mFieldColor);
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
