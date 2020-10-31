#include "tessplane.h"
#include "drawable.h"

void TessPlane::init()
{
    mInnerLevels = 1.0f;
    mOuterLevels = 1.0f;
}

void TessPlane::render(Drawable* d)
{
    QOpenGLShaderProgram* shaderProgram = d->getShader()->shaderProgram();
    shaderProgram->setPatchVertexCount(3);
    shaderProgram->setUniformValue("TessLevelOuter", mOuterLevels);
    shaderProgram->setUniformValue("TessLevelInner", mInnerLevels);

#ifdef GL_PATCHES
    glFunctions->glDrawArrays(GL_PATCHES, 0, mVertices.size());
#endif
    //    glDrawArrays(GL_TRIANGLES,0,6);
}

void TessPlane::fillVertices(QVector<QVector4D>& vertices)
{
    float width = mXSize;
    float height = mYSize;
    vertices.clear();
    // Tri one
    vertices.append(QVector4D(-0.5f * width, -0.5f * height, 0.f, 1.0f));
    vertices.append(QVector4D(0.5f * width, -0.5f * height, 0.f, 1.0f));
    vertices.append(QVector4D(-0.5f * width, 0.5f * height, 0.f, 1.0f));
    vertices.append(QVector4D(-0.5f * width, 0.5f * height, 0.f, 1.0f));
    vertices.append(QVector4D(0.5f * width, -0.5f * height, 0.f, 1.0f));
    vertices.append(QVector4D(0.5f * width, 0.5f * height, 0.f, 1.0f));
}

void TessPlane::fillTexCoords(QVector<QVector2D>& texCoords)
{
    texCoords.clear();
    texCoords.append(QVector2D(0.0, 0.0));
    texCoords.append(QVector2D(1.0, 0.0));
    texCoords.append(QVector2D(0.0, 1.0));
    texCoords.append(QVector2D(0.0, 1.0));
    texCoords.append(QVector2D(1.0, 0.0));
    texCoords.append(QVector2D(1.0, 1.0));
}

void TessPlane::keyboard(int key, int /*modifier*/)
{
    switch (key)
    {
    case '1':
        mInnerLevels--;
        break;
    case '2':
        mInnerLevels++;
        break;
    case '3':
        mOuterLevels--;
        break;
    case '4':
        mOuterLevels++;
        break;
    }
}
