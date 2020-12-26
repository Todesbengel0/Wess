#ifndef MODELTRANSFORMATION_H
#define MODELTRANSFORMATION_H

#include "transformationbase.h"
#include "drawableproperty.h"

/*!
 * \brief Implementiert eine Transformation die sich lediglich auf das Drawable auswirkt an dass sie angehängt wird.
 * \sa Transformation, DrawableProperty
 */
class ModelTransformation : public TransformationBase, public DrawableProperty
{
public:
    //! Wendet die Transformation auf die Globale ModelMatrix an. \see TransformationBase::activate()
    //! \sa DrawableProperty::turnOn()
    virtual void turnOn(Shader* shader) override;

    //! Entfernt die Transformation von der Globale ModelMatrix. \see TransformationBase::deActivate()
    //! \sa DrawableProperty::turnOff()
    virtual void turnOff(Shader* shader) override;

    //! \see DrawableProperty::getId()
    static DrawablePropertyId getPropertyId() {return DrawablePropertyIdGeter::getIdOf<ModelTransformation>();}

    //! \see DrawableProperty::getId()
    virtual DrawablePropertyId getId() const override;
};

#endif // MODELTRANSFORMATION_H
