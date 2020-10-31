#include "color.h"
#include "shader.h"

//! Die 4 Farbwerte für Rot, Grün, Blau und Transparenz in Vektor v setzen
Color::Color(float red, float green, float blue, float alpha) : mChanged(true), mColor(red, green, blue, alpha)
{
}

//! Farbwerte setzen
//!\param r Rot
//!\param g Grün
//!\param b Blau
//!\param a Transparenz
void Color::setValue(float red, float green, float blue, float alpha)
{
    mChanged = true;
    mColor[0] = red;
    mColor[1] = green;
    mColor[2] = blue;
    mColor[3] = alpha;
}

DrawablePropertyId Color::getId() const
{
    return getPropertyId();
}

void Color::turnOn(Shader* shader)
{
    shader->shaderProgram()->setUniformValue("color", mColor);
}
