#pragma once

#include "drawableproperty.h"

class ActorHighlight : public DrawableProperty
{
// ctor / dtor
public:
	ActorHighlight();
	~ActorHighlight();

// drawable property
public:
	//! \see DrawableProperty::getId()
	static DrawablePropertyId getPropertyId();
	//! \see DrawableProperty::getId()
	DrawablePropertyId getId() const override;
	void turnOn(Shader* shader) override;

// getters / setters
public:
	void SetHighlighted(bool highlighted = true);
	bool GetHighlighted() const;
	void SetHighlightColor(const QVector3D& col);
	const QVector3D& GetHighlightColor() const;
	void SetHighlightFactor(const QVector2D& factor_range);
	const QVector2D& SetHighlightFactor() const;

// properties
private:
	bool mHighlighted;
	QVector3D mHighlightColor;
	QVector2D mHighlightFactor;
};
