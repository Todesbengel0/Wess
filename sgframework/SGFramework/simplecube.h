#ifndef SIMPLECUBE_H
#define SIMPLECUBE_H

#include "geometry.h"

//! Geometrie eines einfachen Würfels
class SimpleCube : public Geometry
{
protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;

private:
    GLfloat mWidth, mHeight, mDepth;

    void init(GLfloat width, GLfloat height, GLfloat depth);

public:
    SimpleCube();
    SimpleCube(GLfloat size);
    SimpleCube(GLfloat width, GLfloat height, GLfloat depth);
    float getW(){return mWidth;}
    virtual void render(Drawable* d) override;
    virtual bool isTriangleList() const override;
    ~SimpleCube();
};

#endif
