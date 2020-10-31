#ifndef COLOR_H
#define COLOR_H

#include "drawableproperty.h"
#include <QVector4D>

//! Speicherung und Rendering von Farben im Szenengraph. Dabei ist daran zu denken,
//! dass die verwendeten Farbwerte nur dann überhaupt eine Rolle spielen können, wenn der gebundenen Shader
//! diese Farbe in irgendeiner Art und Weise verwendet. Sobald Beleuchtung im Spiel ist, wird die Farbe in der
//! Regel nicht mehr verwendet, sondern eine Beschreibung der Oberfläche von Objekten wie in Material.
//! Color ist also nur für sehr einfache Beispiele hilfreich.
//! WICHTIG: Da die Farbe nicht als Zustand o.ä. gesetzt werden kann, muss sie über ein Vertexarray an die jeweiligen
//! Shader übergeben werden. Dies geschieht in der Klasse Geometry, die über die Methode Geometry::fillColors
//! ein VertexArray anlegt und füllt, das dann beim Rendern übergeben wird und pro VERTEX im Shader zur Verfügung steht
//! Siehe hierzu der Shader basic.vert
class Color : public DrawableProperty
{
public:
    Color() = default;
    Color(float red, float green, float blue, float alpha = 1.0f);

    //! Farben als einzeln übergebene Werte setzen: Rot, Grün, Blau, Alpha (Transparenz)
    void setValue(float red, float green, float blue, float alpha = 1.0f);

    //! Farben aus 4-elementigem Vektor setzen
    void setValue(const QVector4D& color){mChanged = true; mColor = color;}

    //! Farbe lesen
    const QVector4D& getValue(){return mColor;}

    //! \see DrawableProperty::getId()
    static DrawablePropertyId getPropertyId(){return DrawablePropertyIdGeter::getIdOf<Color>();}

    //! \see DrawableProperty::getId()
    virtual DrawablePropertyId getId() const override;

    //! Die Uniform-Variable Color setzen. \see DrawableProperty::turnOn()
    virtual void turnOn(Shader* shader) override;

private:
    bool mChanged;
    QVector4D mColor;
};

//inline void Color::setValue(const QVector4D& color)
//{
//    mChanged = true;
//    mColor = color;
//}

//inline const QVector4D& Color::getValue()
//{
//    return mColor;
//}

//inline DrawablePropertyId Color::getPropertyId()
//{
//    return DrawablePropertyIdGeter::getIdOf<Color>();
//}

#endif
