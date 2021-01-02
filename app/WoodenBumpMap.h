#pragma once

class WoodenBumpMap : public Texture
{
public:
	WoodenBumpMap();
	~WoodenBumpMap();

// drawable overrides
public:
	//! \see DrawableProperty::getId()
	static DrawablePropertyId getPropertyId();

	//! \see DrawableProperty::getId()
	virtual DrawablePropertyId getId() const override;
};
