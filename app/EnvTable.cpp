#include "stdafx.h"
#include "EnvTable.h"

EnvTable::EnvTable()
	: mgMesh(nullptr)
	, mDraw(nullptr)
{
}

EnvTable::~EnvTable()
{
	delete mDraw;
	delete mgMesh;
}

Node* EnvTable::Init()
{
	mgMesh = new TriangleMesh(":/models/table.obj");
	mDraw = new Drawable(mgMesh);

	static QVector3D dark_wood(ColClamp(59), ColClamp(42), ColClamp(31));
	WoodenPiece::Init(mDraw, dark_wood, 1.0f);

	auto n = new Node(&mtRoot);
	n->addChild(new Node(mDraw));
	return n;
}
