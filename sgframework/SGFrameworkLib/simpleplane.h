#ifndef _SIMPLEPLANE_H
#define _SIMPLEPLANE_H
/*
 *  SimplePlane.h
 *  FBOMotionBlur
 *
 *  Created by bdr on 14.09.10.
 *  Copyright 2010 Hochschule Kempten. All rights reserved.
 *
 */

#include "geometry.h"

//! Zeichnen einer einfachen Fläche in vorgegebener Größe und Anzahl von Unterteilungen (Netz)
class SimplePlane : public Geometry
{
public:
    SimplePlane(float size) {mXSize = size;mYSize = size;mDivs = 1;}
    SimplePlane(float sizeX, float sizeY) {mXSize = sizeX;mYSize = sizeY;mDivs = 1;}
    SimplePlane(float sizeX, float sizeY, unsigned int divs) {mXSize = sizeX;mYSize = sizeY;mDivs = divs;}
    virtual void render(Drawable* d) override;

protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;

protected:
    float mXSize;
    float mYSize;
    unsigned int mDivs;
};

//inline SimplePlane::SimplePlane(float size)
//{
//    mXSize = size;
//    mYSize = size;
//    mDivs = 1;
//}

//inline SimplePlane::SimplePlane(float sizeX, float sizeY)
//{
//    mXSize = sizeX;
//    mYSize = sizeY;
//    mDivs = 1;
//}

//inline SimplePlane::SimplePlane(float sizeX, float sizeY, unsigned int divs)
//{
//    mXSize = sizeX;
//    mYSize = sizeY;
//    mDivs = divs;
//}

#endif
