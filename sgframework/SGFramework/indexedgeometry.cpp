#include "indexedgeometry.h"

IndexedGeometry::IndexedGeometry() : mIndexBuf(QOpenGLBuffer::IndexBuffer)
{
}

IndexedGeometry::IndexedGeometry(const QVector<QVector4D>& vertices, const QVector<QVector3D>& normals,
                                 const QVector<QVector4D>& vertexColors, const QVector<QVector2D>& textureCoordinates,
                                 const QVector<GLuint>& indices)
    : mIndexBuf(QOpenGLBuffer::IndexBuffer)
{
    mVertices = vertices;
    mVertexNormals = normals;
    mVertexColors = vertexColors;
    mTextureCoordinates = textureCoordinates;
    mIndices = indices;
}

void IndexedGeometry::fillIndices(QVector<GLuint>& /*indices*/)
{
}

void IndexedGeometry::prepareVertexBuffers()
{
    fillIndices(mIndices);

    Geometry::prepareVertexBuffers();

    if (!mIndices.isEmpty())
    {
        initBuffer(mIndexBuf, mIndices, VertexFormat::FormatIndex);
    }
}

void IndexedGeometry::prepareVertexArrayObject(Shader& shader, QOpenGLVertexArrayObject& vao)
{
    Geometry::prepareVertexArrayObject(shader, vao);
    if (!mIndices.isEmpty())
    {
        mIndexBuf.bind();
    }
}

void IndexedGeometry::destroyVertexBuffers()
{
    Geometry::destroyVertexBuffers();
    mIndexBuf.destroy();
    mIndices.clear();
}

void IndexedGeometry::render(Drawable* /*d*/)
{
    glFunctions->glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}

QVector<QVector4D> IndexedGeometry::getTriangleList()
{
    QVector<QVector4D> deindexedVertices;
    deindexedVertices.reserve(mIndices.size());
    for (int i = 0; i < mIndices.size(); ++i)
    {
        deindexedVertices.append(mVertices[mIndices[i]]);
    }

    return deindexedVertices;
}
