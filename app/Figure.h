#pragma once

#include "ChessFieldActor.h"

class Figure : public ChessFieldActor
{
// ctor / dtor
public: 
	Figure();
	~Figure();

// initialization
public:
	Node* Init(ChessColor color);
	static void FreeMeshes();
protected:
	virtual QString GetMeshFilePath() const = 0;
private:
	static TriangleMesh* GetLoadMesh(const QString& file_path);

// getters / setters
public:
	void SetPosition(float tx, float tz);
	void step(float x, float z);

// properties
private:
	float mSize;

// scene objects
private:
	Transformation mtPosition;
	TriangleMesh* mgMesh;

	static QMap<QString, TriangleMesh*> msgMeshes;
};

