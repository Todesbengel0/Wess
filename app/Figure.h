#pragma once

#include "ChessFieldActor.h"
#include "WoodenPiece.h"

class Figure : public ChessFieldActor, public WoodenPiece
{
// ctor / dtor
public: 
	Figure();
	~Figure();

// initialization
public:
	Node* Init(ChessColor color, int initialX = -1, int initialZ = -1);
	static void FreeMeshes();
protected:
	virtual QString GetMeshFilePath() const = 0;
	virtual void OnInit();
private:
	static TriangleMesh* GetLoadMesh(const QString& file_path);

// getters / setters
public:
	void SetPosition(float tx, float tz);
	void step(float x, float z);
protected:
	Transformation& GetRotationTrafo();
	int GetInitialX() const;
	int GetInitialZ() const;

// properties
private:
	float mSize;
	int mInitialX;
	int mInitialZ;

// scene objects
private:
	Transformation mtRotation;
	Transformation mtPosition;
	TriangleMesh* mgMesh;

	static QMap<QString, TriangleMesh*> msgMeshes;
};

