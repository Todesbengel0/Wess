#include "stdafx.h"
#include "ActorHighlight.h"

ActorHighlight::ActorHighlight()
	: mHighlighted(false)
	, mHighlightColor(1.0f, 0.0f, 0.0f)
	, mHighlightFactor(0.2f, 0.4f)
{
}

ActorHighlight::~ActorHighlight() = default;

DrawablePropertyId ActorHighlight::getPropertyId()
{
	return DrawablePropertyIdGeter::getIdOf<ActorHighlight>();
}

DrawablePropertyId ActorHighlight::getId() const
{
	return getPropertyId();
}

void ActorHighlight::turnOn(Shader* shader)
{
	shader->shaderProgram()->setUniformValue("Highlighted", mHighlighted ? 1.0f : 0.0f);
	shader->shaderProgram()->setUniformValue("HighlightCol", mHighlightColor);
	shader->shaderProgram()->setUniformValue("HighlightFactor", mHighlightFactor);
}

void ActorHighlight::SetHighlighted(bool highlighted /*= true*/)
{
	mHighlighted = highlighted;
}

bool ActorHighlight::GetHighlighted() const
{
	return mHighlighted;
}

void ActorHighlight::SetHighlightColor(const QVector3D& col)
{
	mHighlightColor = col;
}

const QVector3D& ActorHighlight::GetHighlightColor() const
{
	return mHighlightColor;
}

void ActorHighlight::SetHighlightFactor(const QVector2D& factor_range)
{
	mHighlightFactor = factor_range;
}

const QVector2D& ActorHighlight::SetHighlightFactor() const
{
	return mHighlightFactor;
}
