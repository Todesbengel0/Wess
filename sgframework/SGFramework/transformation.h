/*
 *  Transformation.h
 *  solar_oo
 *
 *  Created by bdr on 11.03.09.
 *  Copyright 2009 Hochschule Kempten. All rights reserved.
 *
 */
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "transformationbase.h"
#include <QMatrix4x4>
#include "sgobject.h"

//! Abbildung einer aus Translationen und Rotationen zusammen gesetzten Transformation
class Transformation : public SGObject, public TransformationBase
{
public:
    Transformation();

protected:
    //! Transformation auf kinder anwenden. muss von SGObject überschrieben werden damit die transformation lediglich
    //! auf die kinder und nicht auf die nachbarn angewendet wird
    virtual void beforeTraverseChilds(SGObjectVisitor& visitor) override;

    //! Transformation nicht auf Geschwister anwenden. muss von SGObject überschrieben werden damit die transformation
    //! lediglich auf die kinder und nicht auf Geschwister angewendet wird
    virtual void afterTraverseChilds() override;
};

#endif
