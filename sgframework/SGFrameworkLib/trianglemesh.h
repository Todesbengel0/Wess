#ifndef _TRIANGLEMESH_H
#define _TRIANGLEMESH_H
#include "geometry.h"

//! Klasse zum Speichern eines Dreiecksnetzes
//! Typischerweise wird dieses aus einer obj-Datei geladen, siehe TriangleMesh::objRead
//! Zu beachten ist, dass eine obj-Datei die Daten indexed enthält, d.h. jedes Datum, z.B. ein Vertex wird einmal mit
//! seiner Geometrie abgelegt und dann über einen ganzzahligen Index referenziert. Da es in OpenGL nicht möglich ist
//! indices pro Vertex-Attribut anzugeben "rollt" diese Klasse die Daten aus
class TriangleMesh : public Geometry
{
public:
    TriangleMesh();
    TriangleMesh(const QString& filePath) {init(filePath);}
    ~TriangleMesh();
    virtual void render(Drawable* d) override;
    virtual bool isTriangleList() const override;
    virtual void init(const QString& filePath);

protected:
    virtual void fillVertices(QVector<QVector4D>& vertices) override;
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices) override;
    virtual void fillTexCoords(QVector<QVector2D>& texCoords) override;
    virtual void copyDataFromObj();
    virtual void fillTris();
    //! Lesen eines Modells aus einer .obj-Datei
    void objRead(QString filepath);
    QList<QVector4D> mVerts;
    QList<QVector3D> mNorms;
    QList<QVector3D> mTexCoords;
    QVector<unsigned int> mIVerts;
    QVector<unsigned int> mINorms;
    QVector<unsigned int> mITexcoords;
    bool mHasNormals;
    bool mHasTexCoords;
};

#endif
