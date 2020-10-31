/*
 *  Material.h
 *  robot
 *
 *  Created by bdr on 18.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "drawableproperty.h"
#include "uniformbuffer.h"
#include "flags.h"
#include <QVector4D>

//! Speichern und rendern eines Materials für beleuchtete Objekte
class Material : public DrawableProperty
{
public:
    Material();
    //! Ambiente Farbe (r, g, b, a) setzen
    void setAmbient(float red, float green, float blue, float alpha = 1.0f);

    //! Diffuse Farbe (r, g, b, a) setzen
    void setDiffuse(float red, float green, float blue, float alpha = 1.0f);

    //! Spiegelnde Farbe (r, g, b, a) setzen
    void setSpecular(float red, float green, float blue, float alpha = 1.0f);

    //! Emississionsfarbe (r, g, b, a) setzen
    void setEmission(float red, float green, float blue, float alpha = 1.0f);

    //! Spiegelkoeffizient setzen
    void setShininess(float shininess);

    //! Setzt die Uniforms des Materials. \see DrawableProperty::turnOn()
    virtual void turnOn(Shader* shader) override;

    //! Ambiente Farbe lesen
    const QVector4D& getAmbient() {return mAmbient;}

    //! Diffuse Farbe lesen
    const QVector4D& getDiffuse() {return mDiffuse;}

    //! Spiegelnde Farbe lesen
    const QVector4D& getSpecular() {return mSpecular;}

    //! Emississionsfarbe lesen
    const QVector4D& getEmission() {return mEmission;}

    //! Spiegelkoeffizient lesen
    float getShininess() {return mShininess;}

    //! \see DrawableProperty::getId()
    static DrawablePropertyId getPropertyId() {return DrawablePropertyIdGeter::getIdOf<Material>();}

    //! \see DrawableProperty::getId()
    virtual DrawablePropertyId getId() const override;

private:
    enum ChangeFlag
    {
        // Enums keine Werte zuweisen. Werden als Bitmaske verwendet
        AmbientChanged,
        DiffuseChanged,
        SpecularChanged,
        EmissionChanged,
        ShininessChanged
    };

    void setUniforms(Shader* shader);
    void createUniformBuffer(QOpenGLShaderProgram& shaderProgram);
    void updateBufferData();

    UniformBuffer mUniformBuffer;
    QVector4D mAmbient;
    QVector4D mDiffuse;
    QVector4D mSpecular;
    QVector4D mEmission;
    float mShininess;
    Flags<ChangeFlag> mChangeFlags;

    static const int sUniformBufferBinding;
};

//inline const QVector4D& Material::getAmbient()
//{
//    return mAmbient;
//}

//inline const QVector4D& Material::getDiffuse()
//{
//    return mDiffuse;
//}

//inline const QVector4D& Material::getSpecular()
//{
//    return mSpecular;
//}

//inline const QVector4D& Material::getEmission()
//{
//    return mEmission;
//}

//inline float Material::getShininess()
//{
//    return mShininess;
//}

//inline DrawablePropertyId Material::getPropertyId()
//{
//    return DrawablePropertyIdGeter::getIdOf<Material>();
//}

#endif
