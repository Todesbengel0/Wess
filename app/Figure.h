#pragma once

#include "ChessColor.h"

class Figure
{
public: 
	Figure();
	~Figure();

public:
	Node* Init(ChessColor color);
	static void FreeMeshes();
protected:
	virtual QString GetMeshFilePath() const = 0;
private:
	static TriangleMesh* GetLoadMesh(const QString& file_path);

public:
	void SetPosition(float tx, float tz);

private:
	float mSize;
	ChessColor mColor;

	Transformation mtPosition;
	Drawable* mdFigure;
	TriangleMesh* mgMesh;

	static QMap<QString, TriangleMesh*> msgMeshes;
};

