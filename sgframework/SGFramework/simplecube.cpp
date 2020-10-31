#include "simplecube.h"

SimpleCube::SimpleCube()
{
    init(1.f, 1.f, 1.f);
}

SimpleCube::SimpleCube(GLfloat size)
{
    init(size, size, size);
}

SimpleCube::SimpleCube(GLfloat width, GLfloat height, GLfloat depth)
{
    init(width, height, depth);
}

void SimpleCube::init(GLfloat width, GLfloat height, GLfloat depth)
{
    this->mWidth = width;
    this->mHeight = height;
    this->mDepth = depth;
}

void SimpleCube::fillVertices(QVector<QVector4D>& vertices)
{
    GLfloat vWidth = mWidth / 2.f;
    GLfloat vHeight = mHeight / 2.f;
    GLfloat vDepth = mDepth / 2.f;

    vertices.clear();
    // Front
    vertices.append(QVector4D(vWidth, vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, -vHeight, vDepth, 1.0f));

    // Back
    vertices.append(QVector4D(-vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, -vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, -vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, -vDepth, 1.0f));

    // Left
    vertices.append(QVector4D(-vWidth, vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, -vDepth, 1.0f));

    // Right
    vertices.append(QVector4D(vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, -vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, -vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, -vHeight, -vDepth, 1.0f));

    // Top
    vertices.append(QVector4D(vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, vHeight, vDepth, 1.0f));

    // Bottom
    vertices.append(QVector4D(vWidth, -vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, -vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(vWidth, -vHeight, -vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, vDepth, 1.0f));
    vertices.append(QVector4D(-vWidth, -vHeight, -vDepth, 1.0f));
}

void SimpleCube::fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& /*vertices*/)
{
    normals.clear();
    // Front
    normals.append(QVector3D(0.0f, 0.0f, 1.0f));
    normals.append(QVector3D(0.0f, 0.0f, 1.0f));
    normals.append(QVector3D(0.0f, 0.0f, 1.0f));
    normals.append(QVector3D(0.0f, 0.0f, 1.0f));
    normals.append(QVector3D(0.0f, 0.0f, 1.0f));
    normals.append(QVector3D(0.0f, 0.0f, 1.0f));

    // Back
    normals.append(QVector3D(0.0f, 0.0f, -1.0f));
    normals.append(QVector3D(0.0f, 0.0f, -1.0f));
    normals.append(QVector3D(0.0f, 0.0f, -1.0f));
    normals.append(QVector3D(0.0f, 0.0f, -1.0f));
    normals.append(QVector3D(0.0f, 0.0f, -1.0f));
    normals.append(QVector3D(0.0f, 0.0f, -1.0f));

    // Left
    normals.append(QVector3D(-1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(-1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(-1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(-1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(-1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(-1.0f, 0.0f, 0.0f));

    // Right
    normals.append(QVector3D(1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(1.0f, 0.0f, 0.0f));
    normals.append(QVector3D(1.0f, 0.0f, 0.0f));

    // Top
    normals.append(QVector3D(0.0f, 1.0f, 0.0f));
    normals.append(QVector3D(0.0f, 1.0f, 0.0f));
    normals.append(QVector3D(0.0f, 1.0f, 0.0f));
    normals.append(QVector3D(0.0f, 1.0f, 0.0f));
    normals.append(QVector3D(0.0f, 1.0f, 0.0f));
    normals.append(QVector3D(0.0f, 1.0f, 0.0f));

    // Bottom
    normals.append(QVector3D(0.0f, -1.0f, 0.0f));
    normals.append(QVector3D(0.0f, -1.0f, 0.0f));
    normals.append(QVector3D(0.0f, -1.0f, 0.0f));
    normals.append(QVector3D(0.0f, -1.0f, 0.0f));
    normals.append(QVector3D(0.0f, -1.0f, 0.0f));
    normals.append(QVector3D(0.0f, -1.0f, 0.0f));
}

void SimpleCube::fillTexCoords(QVector<QVector2D>& texCoords)
{
    // front
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 0.f));

    // back
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 0.f));

    // left
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 0.f));
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));

    // right
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 0.f));

    // top
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
    texCoords.append(QVector2D(1.f, 0.f));

    // bottom
    texCoords.append(QVector2D(1.f, 0.f));
    texCoords.append(QVector2D(1.f, 1.f));
    texCoords.append(QVector2D(0.f, 1.f));
    texCoords.append(QVector2D(1.f, 0.f));
    texCoords.append(QVector2D(0.f, 1.f));
    texCoords.append(QVector2D(0.f, 0.f));
}

void SimpleCube::render(Drawable* /*d*/)
{
    glFunctions->glDrawArrays(GL_TRIANGLES, 0, 36);
}

SimpleCube::~SimpleCube()
{
}

bool SimpleCube::isTriangleList() const
{
    return true;
}
