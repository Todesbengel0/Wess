/*
 *  Light.h
 *  robot_licht
 *
 *  Created by bdr on 18.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#ifndef _LICHT_H_
#define _LICHT_H_

#include "transformationbase.h"
#include "sgobject.h"
#include "flags.h"
#include <QVector4D>

//! Implementierung von Lichtquellen für den Szenengraph
//! Auch hier gilt wieder, dass es in OpenGL keine eigentliche Implementierung der Wirkung von Lichtquellen gibt.
//! Stattdessen ist jeder Shader selbst hierfür verantwortlich. Der Szenegraph übernimmt über die Klasse Light lediglich
//! die Übergabe der Lichtquellen an die Shader. Dies geschieht in RenderingContext::setUniforms
class Light : public SGObject
{
public:
    //! Dieses enum bestimmt, welche Uniformvariablenwerte sich seit dem letzten Frame verändert haben.
    enum ChangeFlag
    {
        // Enums keine Werte zuweisen. Werden als Bitmaske verwendet
        AmbientChanged,
        DiffuseChanged,
        SpecularChanged,

        ChangeFlagCount // Muss immer an letzter Stelle stehen. Bezeichnet Anzahl an change flags
    };

    //! Zerstört diese Lichtinstanz
    virtual ~Light();

    //! Gibt die Transformation des Lichts zurück.
    TransformationBase& getTransformation() {return mTransformation;}

    //! Gibt die Transformation des Lichts zurück.
    const TransformationBase& getTransformation() const {return mTransformation;}

    //! Ambiente Farbe (r, g, b) setzen
    void setAmbient(float red, float green, float blue);

    //! Diffuse Farbe (r, g, b) setzen
    void setDiffuse(float red, float green, float blue);

    //! Spiegelnde Farbe (r, g, b) setzen
    void setSpecular(float red, float green, float blue);

    //! Aktiviert das Licht.
    void turnOn();

    //! Deaktiviert das Licht.
    void turnOff();

    //! Ambiente Farbe lesen
    const QVector3D& getAmbient() const {return mAmbient;}

    //! Diffuse Farbe lesen
    const QVector3D& getDiffuse() const {return mDiffuse;}

    //! Spiegelnde Farbe lesen
    const QVector3D& getSpecular() const {return mSpecular;}

    //! Setzt ChangeFlags zurück.
    void acceptChanges() {mChangeFlags.resetFlags();}

    //! Gibt true zurück, falls sich Uniformvariablen seit dem Aufruf der Methode acceptChanges() verändert haben, sonst false.
    bool hasChanges() const {return mChangeFlags.hasAnyFlagSet();}

    //! Gibt true zurück, falls das Licht aktiv (an) ist, sonst false.
    bool isActive() const {return mOn;}

protected:
    Light();

private:
    TransformationBase mTransformation;

    // light attributes
    QVector3D mAmbient;       // Aclarri
    QVector3D mDiffuse;       // Dcli
    QVector3D mSpecular;      // Scli

    Flags<ChangeFlag> mChangeFlags;
    bool mOn;
};

//inline TransformationBase& Light::getTransformation()
//{
//    return mTransformation;
//}

//inline const TransformationBase& Light::getTransformation() const
//{
//    return mTransformation;
//}

//inline const QVector3D &Light::getAmbient() const
//{
//    return mAmbient;
//}

//inline const QVector3D &Light::getDiffuse() const
//{
//    return mDiffuse;
//}

//inline const QVector3D &Light::getSpecular() const
//{
//    return mSpecular;
//}

//inline void Light::acceptChanges()
//{
//    mChangeFlags.resetFlags();
//}

//inline bool Light::hasChanges() const
//{
//    return mChangeFlags.hasAnyFlagSet();
//}

//inline bool Light::isActive() const
//{
//    return mOn;
//}

#endif
