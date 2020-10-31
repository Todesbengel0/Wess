/*
 *  ShaderTimed.cpp
 *  Shader
 *
 *  Created by bdr on 05.11.10.
 *  Copyright 2010 Hochschule Kempten. All rights reserved.
 *
 */

#include "shadertimed.h"

ShaderTimed::ShaderTimed()
{
    mTimer.start();
    mTime = 0.0f;
}

void ShaderTimed::doIt()
{
    qint64 elapsed = mTimer.elapsed();
    elapsed %= mMsecsPerIteration;

    mTime = elapsed / static_cast<float>(mMsecsPerIteration);
}

void ShaderTimed::setShaderUniforms()
{
    Shader::setShaderUniforms();
    this->shaderProgram()->setUniformValue("Time", mTime);
}
