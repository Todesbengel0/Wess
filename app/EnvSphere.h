#pragma once

class EnvSphere
{
public:
	EnvSphere();
	~EnvSphere();

public:
	Node* Init();

private:
	Transformation mtRoot;
	TriangleMesh* mgMesh;
	Drawable* mDraw;
};
