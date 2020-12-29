#include "stdafx.h"
#include "Figure.h"


Figure::Figure()
	: mdFigure(nullptr)
	, mgMesh(nullptr)
{
}

Figure::~Figure()
{
	delete mdFigure;
}

Node* Figure::Init(ChessColor color)
{
	ChessFieldActor::Init(color);

	mgMesh = GetLoadMesh(GetMeshFilePath());
	mdFigure = new Drawable(mgMesh);
	mdFigure->setProperty<Color>(mDrawColor);

	auto nTrans = new Node(&mtPosition);
	nTrans->addChild(new Node(mdFigure));

	return nTrans;
}

QMap<QString, TriangleMesh*> Figure::msgMeshes;

TriangleMesh* Figure::GetLoadMesh(const QString& file_path)
{
	auto it = msgMeshes.find(file_path);
	if (it == msgMeshes.end())
	{
		// load mesh
		auto mesh = new TriangleMesh(file_path);
		msgMeshes.insert(file_path, mesh);
		return mesh;
	}

	// already loaded mesh
	return it.value();
}

void Figure::FreeMeshes()
{
	for (auto mesh : msgMeshes)
		delete mesh;
	msgMeshes.clear();
}

void Figure::SetPosition(float tx, float tz)
{
	mtPosition.resetTrafo();
	mtPosition.translate(tx, 0.0f, tz);
}
