/*
 *  AnimatedTransformation.h
 *  AccuBuffer
 *
 *  Created by bdr on 13.09.10.
 *  Copyright 2010 Hochschule Kempten. All rights reserved.
 *
 */

#ifndef _ANIMATEDTRANSFORMATION_H_
#define _ANIMATEDTRANSFORMATION_H_

#include "transformation.h"
#include "idleobserver.h"

//! AnimatedTransformation ist eine Transformation, die in jedem Zeitintervall = "Tick" die
//! eingestellte Transformation weiterverschiebt oder rotiert.
class AnimatedTransformation : public Transformation, public IdleObserver
{
public:
    AnimatedTransformation(float dx, float dy, float dz, float angle, float rx, float ry, float rz);
    virtual void doIt() override;

private:
    float mDx;
    float mDy;
    float mDz;
    float mAngle;
    float mRx;
    float mRy;
    float mRz;
};

#endif
