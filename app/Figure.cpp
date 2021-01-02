#include "stdafx.h"
#include "Figure.h"

Figure::Figure()
	: mgMesh(nullptr)
	, mInitialX(-1)
	, mInitialZ(-1)
{
}

Figure::~Figure() = default;

Node* Figure::Init(ChessColor color, int initialX /*= -1*/, int initialZ /*= -1*/)
{
	mInitialX = initialX;
	mInitialZ = initialZ;

	auto nTrans = new Node(&mtPosition);

	mgMesh = GetLoadMesh(GetMeshFilePath());

	ChessFieldActor::Init(color, mgMesh);
	WoodenPiece::Init(mDraw, GetDrawColor(), 10.0f);

	OnInit();

	auto nRotation = new Node(&mtRotation);
	nRotation->addChild(new Node(mDraw));

	nTrans->addChild(nRotation);
	return nTrans;
}

// implementer defined
void Figure::OnInit()
{
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

Transformation& Figure::GetRotationTrafo()
{
	return mtRotation;
}

int Figure::GetInitialX() const
{
	return mInitialX;
}

int Figure::GetInitialZ() const
{
    return mInitialZ;
}

void Figure::MovePosition(float tx, float tz)
{
    mtPosition.translate(tx, 0.0f, tz);
}
