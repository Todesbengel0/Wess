#ifndef SIMPLESPHERE_H
#define SIMPLESPHERE_H

#include "indexedgeometry.h"

//! Zeichnen einer einfachen Kugel mit vorgegebener Größe und Auflösung in layer und höhe
class SimpleSphere : public IndexedGeometry
{
public:
    SimpleSphere(float size = 1.0, int subdivisionHorizontal = 25, int subdivisionVertical = 25);
    virtual ~SimpleSphere();

protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;
    virtual void fillIndices(QVector<GLuint>& indices) override;

private:
    void init(float size, int subdivisionHorizontal, int subdivisionVertical);
    int mSubdivisionHorizontal;
    int mSubdivisionVertical;
    float mSize;
};

#endif // SIMPLESPHERE_H
