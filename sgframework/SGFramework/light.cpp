/*
 *  Light.cpp
 *  robot_licht
 *
 *  Created by bdr on 18.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include "scenemanager.h"
#include "light.h"

Light::~Light()
{
}

void Light::turnOn()
{
    mOn = true;
}

void Light::turnOff()
{
    mOn = false;
}

//! Initialisieren
Light::Light()
{
    mOn = true;

    // setting default light values
    setAmbient(0.1f, 0.1f, 0.1f);
    setDiffuse(0.3f, 0.3f, 0.3f);
    setSpecular(0.7f, 0.7f, 0.7f);
}

void Light::setAmbient(float red, float green, float blue)
{
    QVector3D t(red, green, blue);

    mAmbient = t;
    mChangeFlags.addFlag(AmbientChanged);
}

void Light::setDiffuse(float red, float green, float blue)
{
    QVector3D t(red, green, blue);

    mDiffuse = t;
    mChangeFlags.addFlag(DiffuseChanged);
}

void Light::setSpecular(float red, float green, float blue)
{
    QVector3D t(red, green, blue);

    mSpecular = t;
    mChangeFlags.addFlag(SpecularChanged);
}
