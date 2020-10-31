/*
 *  Transformation.cpp
 *  solar_oo
 *
 *  Created by bdr on 11.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */

#include "transformation.h"

Transformation::Transformation()
{
}

void Transformation::beforeTraverseChilds(SGObjectVisitor& /*visitor*/)
{
    activate();
}

void Transformation::afterTraverseChilds()
{
    deActivate();
}
