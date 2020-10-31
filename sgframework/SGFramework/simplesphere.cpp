//#include <cmath>
#include <qmath.h>

#include "simplesphere.h"
#include "color.h"

void SimpleSphere::fillVertices(QVector<QVector4D>& vertices)
{
    vertices.clear();

    // Ringe
    for (int l = 0; l <= mSubdivisionHorizontal; l++)
    {
        float sinc = 2.0 * M_PI / (float)mSubdivisionVertical, s;
        float tinc = M_PI / (float)mSubdivisionHorizontal, t;
        t = l * tinc;

        for (int h = 0; h <= mSubdivisionVertical; h++)
        {
            s = h * sinc;
            vertices.append(QVector4D(mSize * cos(s) * sin(t), mSize * cos(t), mSize * sin(s) * sin(t), 1.0f));
        }
    }

    //    qDebug() << vertices.size() << " verts.";
}

void SimpleSphere::fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices)
{
    normals.clear();
    for (int l = 0; l <= mSubdivisionHorizontal; l++)
    {
        for (int h = 0; h <= mSubdivisionVertical; h++)
        {
            normals.append(QVector3D(vertices[l * (mSubdivisionVertical + 1) + h]));
        }
    }
}

void SimpleSphere::fillTexCoords(QVector<QVector2D>& texCoords)
{
    texCoords.clear();
    for (float l = 0; l <= mSubdivisionHorizontal; l++)
    {
        for (float h = 0; h <= mSubdivisionVertical; h++)
        {
            texCoords.append(QVector2D(l / (float)mSubdivisionHorizontal, h / (float)mSubdivisionVertical));
        }
    }
}

void SimpleSphere::init(float size, int subdivisionHorizontal, int subdivisionVertical)
{
    Q_ASSERT_X(size > 0.0f, "SimpleSphere::init()", "Size muss größer als 0 sein!");
    Q_ASSERT_X(subdivisionHorizontal > 0, "SimpleSphere::init()", "subdivisionHorizontal muss größer als 0 sein!");
    Q_ASSERT_X(subdivisionVertical > 0, "SimpleSphere::init()", "subdivisionVertical muss größer als 0 sein!");
    mSize = size;
    mSubdivisionHorizontal = subdivisionHorizontal;
    mSubdivisionVertical = subdivisionVertical;
}

SimpleSphere::SimpleSphere(float size, int subdivisionHorizontal, int subdivisionVertical)
{
    init(size, subdivisionHorizontal, subdivisionVertical);
}

SimpleSphere::~SimpleSphere()
{
}

void SimpleSphere::fillIndices(QVector<GLuint>& indices)
{
    indices.resize(mSubdivisionVertical * mSubdivisionHorizontal * 6);
    int i = 0;
    for (int l = 0; l < mSubdivisionHorizontal; l++)
    {
        for (int h = 0; h < mSubdivisionVertical; h++)
        {
            indices[i++] = l * (mSubdivisionVertical + 1) + h;
            indices[i++] = (l + 1) * (mSubdivisionVertical + 1) + h;
            indices[i++] = l * (mSubdivisionVertical + 1) + h + 1;
            indices[i++] = l * (mSubdivisionVertical + 1) + h + 1;
            indices[i++] = (l + 1) * (mSubdivisionVertical + 1) + h;
            indices[i++] = (l + 1) * (mSubdivisionVertical + 1) + h + 1;
        }
    }
}
