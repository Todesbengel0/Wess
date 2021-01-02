#include "stdafx.h"
#include "ChessFieldActor.h"

#include "ActorHighlight.h"

ChessFieldActor::ChessFieldActor()
	: mFieldColor(ChessColor::Black)
	, mHighlighted(false)
	, mDraw(nullptr)
{
}

ChessFieldActor::~ChessFieldActor()
{
	delete mDraw;
}

void ChessFieldActor::Init(ChessColor color, Geometry* geo)
{
	mFieldColor = color;
	mDraw = new Drawable(geo);

	mDraw->getProperty<ActorHighlight>()->SetHighlightColor(QVector3D(0.8f, 0.0f, 0.0f));
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

	mDraw->getProperty<ActorHighlight>()->SetHighlighted(mHighlighted);
}
