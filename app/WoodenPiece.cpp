#include "stdafx.h"
#include "WoodenPiece.h"

#include "WoodenShader.h"
#include "WoodenPatternMap.h"

WoodenPiece::WoodenPiece()
	: mShader(ShaderManager::getShader<WoodenShader>(":/shaders/Wooden.vert", ":/shaders/Wooden.frag"))
	, mMat(nullptr)
{
}

WoodenPiece::~WoodenPiece()
{
	//delete mMat;	// sadly crashes on UniformBuffer dtor
};

void WoodenPiece::Init(Drawable* draw, QVector3D base_color, float shininess)
{
	mMat = new Material;
	mMat->setDiffuse(base_color.x(), base_color.y(), base_color.z());
	base_color /= 4.0f;
	mMat->setAmbient(base_color.x(), base_color.y(), base_color.z());
	mMat->setSpecular(1.0f, 1.0f, 1.0f);
	mMat->setShininess(shininess);

	draw->setProperty(mMat);
	//draw->setProperty(new Texture(":/textures/wood_pattern.png"));
	if (!msPatternMap)
		msPatternMap = new WoodenPatternMap;
	draw->setProperty(msPatternMap);
	//draw->setProperty(&mShader->GetTexNormal());
	draw->setShader(mShader);
}

WoodenShader* WoodenPiece::GetShader()
{
	return mShader;
}

WoodenPatternMap* WoodenPiece::msPatternMap;
