#include "stdafx.h"
#include "EnvSphere.h"

EnvSphere::EnvSphere()
	: mgMesh(nullptr)
	, mDraw(nullptr)
{
}

EnvSphere::~EnvSphere()
{
	delete mDraw;
	delete mgMesh;
}

Node* EnvSphere::Init()
{
	mtRoot.scale(3.0f, 3.0f, 3.0f);
	mtRoot.rotate(90.0f, 0.0f, 1.0f, 0.0f);

	mgMesh = new TriangleMesh(":/models/env_sphere.obj");
	mDraw = new Drawable(mgMesh);

	mDraw->getProperty<Texture>()->loadPicture(":/textures/environment.jpg");
	mDraw->setShader(ShaderManager::getShader(":/shaders/Environment.vert", ":/shaders/Environment.frag"));

	auto n = new Node(&mtRoot);
	n->addChild(new Node(mDraw));
	return n;
}
