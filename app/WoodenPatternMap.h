#pragma once

class WoodenPatternMap : public Texture
{
public:
	WoodenPatternMap();
	~WoodenPatternMap();

// drawable overrides
public:
	//! \see DrawableProperty::getId()
	static DrawablePropertyId getPropertyId();

	//! \see DrawableProperty::getId()
	DrawablePropertyId getId() const override;
};
