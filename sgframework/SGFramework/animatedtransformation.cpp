/*
 *  AnimatedTransformation.cpp
 *  AccuBuffer
 *
 *  Created by bdr on 13.09.10.
 *  Copyright 2010 Hochschule Kempten. All rights reserved.
 *
 */

#include "animatedtransformation.h"

void AnimatedTransformation::doIt()
{
    this->translate(mDx, mDy, mDz);
    this->rotate(mAngle, mRx, mRy, mRz);
}

AnimatedTransformation::AnimatedTransformation(float dx, float dy, float dz, float angle, float rx, float ry, float rz)
    : Transformation()
{
    mDx = dx;
    mDy = dy;
    mDz = dz;
    mAngle = angle;
    mRx = rx;
    mRy = ry;
    mRz = rz;
}
