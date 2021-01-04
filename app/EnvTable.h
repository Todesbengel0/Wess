#pragma once

#include "WoodenPiece.h"

class EnvTable : public WoodenPiece
{
public:
	EnvTable();
	~EnvTable();

public:
	Node* Init();

private:
	Transformation mtRoot;
	TriangleMesh* mgMesh;
	Drawable* mDraw;
};
