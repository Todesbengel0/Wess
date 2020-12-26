#include "stdafx.h"
#include "Figure.h"


Figure::Figure()
	: mColor(ChessColor::Black)
	, mdFigure(nullptr)
	, mgMesh(nullptr)
{
}

Figure::~Figure()
{
	delete mdFigure;
	delete mgMesh;
}

Node* Figure::Init(ChessColor color)
{
	mColor = color;

	// TODO: optimization: figures should share same meshes
	mgMesh = new TriangleMesh(GetMeshFilePath());
	mdFigure = new Drawable(mgMesh);

	auto nTrans = new Node(&mtPosition);
	nTrans->addChild(new Node(mdFigure));

	return nTrans;
}

void Figure::SetPosition(float tx, float tz)
{
	mtPosition.resetTrafo();
	mtPosition.translate(tx, 0.0f, tz);
}
