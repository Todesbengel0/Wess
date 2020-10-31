#ifndef BUMPMAP_H
#define BUMPMAP_H

#include "texture.h"

class Drawable;

//! BumpMap liest eine Textur ein, die als Bumpmap interpretiert werden soll.
//! Setzt voraus, dass der jeweilige Shader die Textur richtig interpretiert!
class BumpMap : public Texture
{
public:
    //! Als default wird die Textureinheit 1 für BumpMaps verwendet.
    BumpMap(int unit = 1) : Texture(unit){setNameString("bumpMap");}
    BumpMap(const QString& pfad, int unit = 1) : Texture(pfad, unit) {mTextureUnit = unit; setNameString("bumpMap");}

    //! \see DrawableProperty::getId()
    static DrawablePropertyId getPropertyId(){return DrawablePropertyIdGeter::getIdOf<BumpMap>();}

    //! \see DrawableProperty::getId()
    virtual DrawablePropertyId getId() const override {return getPropertyId();}
};

//inline BumpMap::BumpMap(int unit) : Texture(unit)
//{
//}

//inline BumpMap::BumpMap(const QString& pfad, int unit) : Texture(pfad, unit)
//{
//    mTextureUnit = unit;
//}

//inline DrawablePropertyId BumpMap::getPropertyId()
//{
//    return DrawablePropertyIdGeter::getIdOf<BumpMap>();
//}

#endif // BUMPMAP_H
