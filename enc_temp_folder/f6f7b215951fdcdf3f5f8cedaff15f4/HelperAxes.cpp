#include "stdafx.h"
#include "HelperAxes.h"

HelperAxes::HelperAxes()
	: mgArrow(nullptr)
{
	memset(mdAxes, 0, sizeof mdAxes);
}

HelperAxes::~HelperAxes()
{
	delete mgArrow;
}

Node* HelperAxes::Init()
{
	auto nRoot = new Node(&mtRoot);

	constexpr float arrow_size = 3.0f;
	constexpr float arrow_width = arrow_size * 0.2f;

	mgArrow = new SimpleCube(arrow_size, arrow_width, arrow_width);

	// X
	{
		mtAxes[0].translate((arrow_size - arrow_width) * 0.5f, 0.0f, 0.0f);
		auto nTX = new Node(&mtAxes[0]);
		mdAxes[0] = new Drawable(mgArrow);
		mdAxes[0]->getProperty<Color>()->setValue(1.0f, 0.0f, 0.0f);
		nTX->addChild(new Node(mdAxes[0]));
		nRoot->addChild(nTX);
	}

	// Y
	{
		mtAxes[1].translate(0.0f, (arrow_size - arrow_width) * 0.5f, 0.0f);
		mtAxes[1].rotate(90.0f, 0.0f, 0.0f, 1.0f);
		auto nTY = new Node(&mtAxes[1]);
		mdAxes[1] = new Drawable(mgArrow);
		mdAxes[1]->getProperty<Color>()->setValue(0.0f, 1.0f, 0.0f);
		nTY->addChild(new Node(mdAxes[1]));
		nRoot->addChild(nTY);
	}

	// Z
	{
		mtAxes[2].translate(0.0f, 0.0f, (arrow_size - arrow_width) * 0.5f);
		mtAxes[2].rotate(90.0f, 0.0f, 1.0f, 0.0f);
		auto nTZ = new Node(&mtAxes[2]);
		mdAxes[2] = new Drawable(mgArrow);
		mdAxes[2]->getProperty<Color>()->setValue(0.0f, 0.0f, 1.0f);
		nTZ->addChild(new Node(mdAxes[2]));
		nRoot->addChild(nTZ);
	}

	return nRoot;
}

Transformation* HelperAxes::GetTrafo()
{
	return &mtRoot;
}
