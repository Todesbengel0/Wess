#ifndef INDEXEDGEOMETRY_H
#define INDEXEDGEOMETRY_H

#include "geometry.h"

//! Eine Geometrie welche ihre daten Indiziert apspeichert sodass diese kompakter abgelegt sind.
class IndexedGeometry : public Geometry
{
public:
    IndexedGeometry();
    IndexedGeometry(const QVector<QVector4D>& vertices, const QVector<QVector3D>& normals, const QVector<QVector4D>& vertexColors,
                    const QVector<QVector2D>& textureCoordinates, const QVector<GLuint>& indices);
    virtual void prepareVertexArrayObject(Shader& shader, QOpenGLVertexArrayObject& vao) override;
    virtual void prepareVertexBuffers() override;
    virtual void destroyVertexBuffers() override;
    virtual void render(Drawable* d) override;

    //! Gibt die Vertices als Dreiecksliste zurück. Also nicht geindexed.
    virtual QVector<QVector4D> getTriangleList() override;

protected:
    //! Überschrieben um die Indices beim Initialisieren der Geometrie zu erstellen.
    virtual void fillIndices(QVector<GLuint>& indices);

private:
    QVector<GLuint> mIndices;
    QOpenGLBuffer mIndexBuf;
};

#endif // INDEXEDGEOMETRY_H
