#include "stdafx.h"
#include "Figure.h"


Figure::Figure()
	: mgMesh(nullptr)
{
}

Figure::~Figure()
{
}

Node* Figure::Init(ChessColor color)
{
	mgMesh = GetLoadMesh(GetMeshFilePath());

	ChessFieldActor::Init(color, mgMesh);

	auto nTrans = new Node(&mtPosition);
	nTrans->addChild(new Node(mDraw));

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

void Figure::step(float tx, float tz)
{
	mtPosition.translate(tx, 0.0f, tz);
}
