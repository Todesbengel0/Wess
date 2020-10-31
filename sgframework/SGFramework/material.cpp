/*
 *  Material.cpp
 *  robot
 *
 *  Created by bdr on 18.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#include <string.h>
#include "material.h"
#include "scenemanager.h"

const int Material::sUniformBufferBinding = 0;

Material::Material() :
    mUniformBuffer(sUniformBufferBinding)
{
    setAmbient(0.3f, 0.3f, 0.3f, 1.f); // set default ambient color
    setDiffuse(0.5f, 0.5f, 0.5f, 1.f);
    setSpecular(0.7f, 0.7f, 0.7f, 1.f);
    setShininess(1.f);
    setEmission(0.f, 0.f, 0.f, 1.f);

    // setze Prio für das Sortieren der Shapelist. Setzt Uniforms beim aktivieren also relativ billig.
    setPriority(1);
}

void Material::setAmbient(float red, float green, float blue, float alpha)
{
    QVector4D t(red, green, blue, alpha);

    mAmbient = t;
    mChangeFlags.addFlag(AmbientChanged);
}

void Material::setDiffuse(float red, float green, float blue, float alpha)
{
    QVector4D t(red, green, blue, alpha);

    mDiffuse = t;
    mChangeFlags.addFlag(DiffuseChanged);
}

void Material::setSpecular(float red, float green, float blue, float alpha)
{
    QVector4D t(red, green, blue, alpha);

    mSpecular = t;
    mChangeFlags.addFlag(SpecularChanged);
}

void Material::setEmission(float red, float green, float blue, float alpha)
{
    QVector4D t(red, green, blue, alpha);

    mEmission = t;
    mChangeFlags.addFlag(EmissionChanged);
}

void Material::setShininess(float shininess)
{
    mShininess = shininess;
    mChangeFlags.addFlag(ShininessChanged);
}

void Material::turnOn(Shader* shader)
{
    setUniforms(shader);
}

DrawablePropertyId Material::getId() const
{
    return getPropertyId();
}

void Material::setUniforms(Shader* shader)
{
    QOpenGLShaderProgram& shaderProgram = *shader->shaderProgram();
    if (mUniformBuffer.isCreated())
    {
        mUniformBuffer.addShaderProgram(shaderProgram);
        if (mChangeFlags.hasAnyFlagSet())
        {
            // Daten haben sich verändert => Aktualisiere Daten im Buffer.
            updateBufferData();

            // Updates sind abgeschlossen => ChangeFlags können zurückgesetzt werden.
            mChangeFlags.resetFlags();
        }
    }
    else
    {
        // Uniform buffer existiert noch nicht => Erzeuge uniform buffer und setze Daten.
        createUniformBuffer(shaderProgram);
        updateBufferData();
        mChangeFlags.resetFlags();
    }

    mUniformBuffer.bindBase();
}

void Material::createUniformBuffer(QOpenGLShaderProgram& shaderProgram)
{
    const GLchar* uniformNames[] = {
        "material.ambient",
        "material.diffuse",
        "material.specular",
        "material.emission",
        "material.shininess"
    };

    mUniformBuffer.create(shaderProgram, "MaterialUniforms", uniformNames);
}

void Material::updateBufferData()
{
    if (mUniformBuffer.bind())
    {
        int i = -1;
        mUniformBuffer.write(++i, &mAmbient, sizeof(mAmbient));
        mUniformBuffer.write(++i, &mDiffuse, sizeof(mDiffuse));
        mUniformBuffer.write(++i, &mSpecular, sizeof(mSpecular));
        mUniformBuffer.write(++i, &mEmission, sizeof(mEmission));
        mUniformBuffer.write(++i, &mShininess, sizeof(mShininess));
        mUniformBuffer.release();
    }
}
