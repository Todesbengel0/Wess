#include "stdafx.h"
#include "WoodenBumpMap.h"

WoodenBumpMap::WoodenBumpMap()
	: Texture(":/textures/wood_norm.png", 2)
{
	setNameString("NormalMap");
}

WoodenBumpMap::~WoodenBumpMap() = default;

DrawablePropertyId WoodenBumpMap::getPropertyId()
{
	return DrawablePropertyIdGeter::getIdOf<WoodenBumpMap>();
}

DrawablePropertyId WoodenBumpMap::getId() const
{
	return getPropertyId();
}
