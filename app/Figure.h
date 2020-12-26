#pragma once

#include "ChessColor.h"

class Figure
{
public: 
	Figure();
	~Figure();

public:
	Node* Init(ChessColor color);
protected:
	virtual QString GetMeshFilePath() const = 0;

public:
	void SetPosition(float tx, float tz);

private:
	float mSize;
	ChessColor mColor;

	Transformation mtPosition;
	Drawable* mdFigure;
	TriangleMesh* mgMesh;
};

