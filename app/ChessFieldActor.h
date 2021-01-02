#pragma once

#include "ChessColor.h"

class ChessFieldActor
{
// ctor / dtor
public:
	ChessFieldActor();
	virtual ~ChessFieldActor();

// initialization
protected:
	void Init(ChessColor color, Geometry* geo);

// getters / setters
public:
	ChessColor GetFieldColor() const;
	static QVector3D GetDrawColorByFieldColor(ChessColor color);
	QVector3D GetDrawColor() const;

// access
public:
	void SetHighlighted(bool highlighted);

// properties
private:
	ChessColor mFieldColor;
	bool mHighlighted;

// scene objects
protected:
	Drawable* mDraw;
};
