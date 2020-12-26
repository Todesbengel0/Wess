#ifndef SHADOWMAPPROPERTY_H
#define SHADOWMAPPROPERTY_H

#include <drawableproperty.h>
#include "shadowlight.h"

//! Property, die einem Objekt eine ShadowMap anhängt. In einem Prerendering (siehe ShadowLight) wird eine Textur
//! erzeugt, die dann hier gebunden wird. Außerdem werden die entsprechenden Uniforms für den passenden Shader gesetzt.
class ShadowMapProperty : public DrawableProperty
{
private:
    QMatrix4x4 mHomgenusToTexCoordinates;

public:
    ShadowMapProperty();

    // DrawableProperty interface
    virtual void turnOn(Shader* shader) override;
    virtual void turnOff(Shader* shader) override;
    static inline DrawablePropertyId getPropertyId() {return DrawablePropertyIdGeter::getIdOf<ShadowMapProperty>();}
    virtual DrawablePropertyId getId() const override;
};

//inline DrawablePropertyId ShadowMapProperty::getPropertyId()
//{
//    return DrawablePropertyIdGeter::getIdOf<ShadowMapProperty>();
//}

#endif // SHADOWMAPPROPERTY_H
