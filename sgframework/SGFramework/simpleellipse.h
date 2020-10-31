/*
 *  SimpleEllipse.h
 *  fernglas
 *
 *  Created by bdr on 07.10.10.
 *  Copyright 2010 Hochschule Kempten. All rights reserved.
 *
 */

#include "geometry.h"

//! Zeichnet eine einfache Ellipse
class SimpleEllipse : public Geometry
{
protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;

public:
    SimpleEllipse() {mWidth = mHeight = 1.0;mSegments = 20;}
    //! Breite, Höhe und Angabe der Segmente für die "Auflösung"
    SimpleEllipse(float width, float height, int segments) {mWidth = width;mHeight = height;mSegments = segments;}
    virtual void render(Drawable* d) override;
    virtual ~SimpleEllipse();

private:
    float mWidth;
    float mHeight;
    int mSegments;
};

//inline SimpleEllipse::SimpleEllipse()
//{
//    mWidth = mHeight = 1.0;
//    mSegments = 20;
//}

//inline SimpleEllipse::SimpleEllipse(float width, float height, int segments)
//{
//    mWidth = width;
//    mHeight = height;
//    mSegments = segments;
//}
