#include "stdafx.h"
#include "WoodenPatternMap.h"

WoodenPatternMap::WoodenPatternMap()
	: Texture(":/textures/wood_pattern.png", 1)
{
	setNameString("PatternMap");
}

WoodenPatternMap::~WoodenPatternMap() = default;

DrawablePropertyId WoodenPatternMap::getPropertyId()
{
	return DrawablePropertyIdGeter::getIdOf<WoodenPatternMap>();
}

DrawablePropertyId WoodenPatternMap::getId() const
{
	return getPropertyId();
}
