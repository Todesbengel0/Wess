#include <qmath.h>
#include "simpleellipse.h"

void SimpleEllipse::fillVertices(QVector<QVector4D>& vertices)
{
    QVector2D last(0, mHeight);

    vertices.clear();

    for (int i = 0; i < mSegments; i++)
    {
        float x, y;
        x = sin(2 * M_PI / mSegments * (i + 1)) * mWidth;
        y = cos(2 * M_PI / mSegments * (i + 1)) * mHeight;
        vertices.append(QVector4D(last, 0.f, 1.0f));
        vertices.append(QVector4D(x, y, .0f, 1.0f));
        vertices.append(QVector4D(0.f, 0.f, 0.f, 1.0f));
        last = QVector2D(x, y);
    }
}

void SimpleEllipse::fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& /*vertices*/)
{
    normals.clear();
    for (int i = 0; i < 3 * mSegments; i++)
    {
        normals.append(QVector3D(0.f, 0.f, 1.f));
    }
}

void SimpleEllipse::fillTexCoords(QVector<QVector2D>& texCoords)
{
    QVector2D last = QVector2D(0, 1);

    texCoords.clear();

    // TODO Umparametrieren -> [0,1]
    for (int i = 1; i < mSegments; i++)
    {
        float x, y;
        x = sin(2 * M_PI / mSegments * i);
        y = cos(2 * M_PI / mSegments * i);
        texCoords.append(last);
        texCoords.append(QVector2D(x, y));
        texCoords.append(QVector2D(0.f, 0.f));
    }
}

//! Rendern einer Ellipse mit einer vorgegebenen Unterteilung (mSegments)
void SimpleEllipse::render(Drawable* /*d*/)
{
    glFunctions->glDrawArrays(GL_TRIANGLES, 0, 3 * (mSegments));
}

SimpleEllipse::~SimpleEllipse()
{
}
