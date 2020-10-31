// Geometry.h
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QOpenGLBuffer>

#include "node.h"
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include "drawableproperty.h"
#include "vertexformat.h"
#include <QVector2D>
#include "opengl.h"

class Drawable;

//! Geometry ist die Basisklasse für alle Objekte im Szenengraphen, die die geometrischen Daten eines zu zeichenenden
//! Objektes enthalten. Dazu gehören alle Attribute, die mit der Geometrie zu tun haben, also auch Normalen,
//! Texturkoordinaten, etc. eine Farbe nur sinnvoll pro Vertex kann ebenfalls festgelegt werden, alternativ kann die
//! Color property verwendet werden um eine einfabige Geometry zu erhalten.
//! Jedes Drawable im Szenengraph muss ein Geometry-Objekt haben, um gezeichnet werden zu können. Mehrere Drawable
//! können sich aber eine Geometry teilen!
class Geometry : public DrawableProperty
{
public:
    Geometry();
    virtual ~Geometry();

    //! Buffer für das Rendern binden
    void bindBuffer(Shader& shader);

    //! Buffer nach dem Rendern freigeben
    void unbindBuffer();

    //! Objekt zeichnen
    virtual void render(Drawable* d);

    //! Geometry-Basis Initialisieren. Wenn geometry noch nicht initialisiert wurde, werden die Geometriedaten befüllt sowie
    //! die Buffer erstellt und befüllt.
    void initializeGeometryBase();
    //! Wird beim ändern der Geometry eines Drawables aufgerufen, um Optimierungen
    //! (GeometryOptimizer) durchführen zu können und das VertexFormat nur einmal pro Drawable zu setzen.
    //! Ruft initializeGeometryBase
    void initializeGeometry(Drawable& drawable);

    //! Buffer, Vertex Array Objects sowie alle Vertices, Colors, Normals und Textur Koordinaten löschen.
    void deInitGeometry();

    //! Getter für die Vertices der Geometry.
    const QVector<QVector4D>& getVertices() const;

    //! Getter für die Normalen der Geometry.
    const QVector<QVector3D>& getNormals() const;

    //! Getter für die Textur-Koordinaten der Geometry.
    const QVector<QVector2D>& getTextureCoordinates() const;

    //! Getter für die Vertex-Farben der Geometry.
    const QVector<QVector4D>& getVertexColors() const;

    //! \see DrawableProperty::getId()
    static DrawablePropertyId getPropertyId() {return DrawablePropertyIdGeter::getIdOf<Geometry>();}

    //! \see DrawableProperty::getId()
    virtual DrawablePropertyId getId() const override;

    //! Getter für information über die Existenz einzelner Vertex Attribute.
    const VertexFormat& getVertexFormat() const {return mVertexFormat;}

    //! Gibt an ob die Geometriedaten als Dreiecksliste abgelegt und gerendert werden. Also ob glDrawArrays mit
    //! 'GL_TRIANGLES' aufgerufen wird oder ob es z.B. Triangle-Strips. Inerhalb von GeometryOptimizer werden nur
    //! Geometrien optimiert die als Dreiecksliste abgelegt sind sodas wenn diese Funktion false zurückgibt keine
    //! optimierungen auf diese ausgeführt werden.
    virtual bool isTriangleList() const;

    //! Gibt an ob die Geometriedaten Interleaved in einem Buffer gespeichert sind. \see InterleavedGeometry,
    //! IndexedInterleavedGeometry
    virtual bool isInterleaved() const;

    //! Gibt eine liste der Vertices der Geometry zurück bei der immer 3 Vertices ein Dreieck bilden. Bei normalen
    //! Geometryen die als Dreiecksliste abgelegt sind ist diese Methode equivalent zu getVertices(). Wenn die daten
    //! jedoch geindexed (IndexedGeometry) oder interleaved (InterleavedGeometry) abgespeichert sind gibt getVertices()
    //! nicht die richtigen bzw. garkeine daten zurück aber getTriangleList() gibt die Vertices als Dreiecksliste
    //! zurück.
    virtual QVector<QVector4D> getTriangleList();

protected:
    //! Überschrieben um die Vertices beim Initialisieren der Geometrie zu erstellen.
    virtual void fillVertices(QVector<QVector4D>& vertices);

    //! Überschrieben um die Normalen beim Initialisieren der Geometrie zu erstellen.
    virtual void fillNormals(QVector<QVector3D>& normals, QVector<QVector4D>& vertices);

    //! Überschrieben um die Textur-Koordinaten beim Initialisieren der Geometrie zu erstellen.
    virtual void fillTexCoords(QVector<QVector2D>& texCoords);

    //! Überschrieben um die Vertex-Farben beim Initialisieren der Geometrie zu erstellen.
    virtual void fillColors(QVector<QVector4D>& colors);

    //! Wird beim Initialisieren der Geometrie und erstellt die Buffer. Kann überschriben werden um zusätzliche Buffer
    //! wie z.B. einen IndexBuffer zu erstellen.
    virtual void prepareVertexBuffers();

    //! Initialisieren ein Vertex Array Object für einen Shader.
    //! Wird pro Shader der diese Geometrie rendert einmal aufgerufen.
    //! Kann überschrieben werden um zusätzliche Buffer wie z.B. einen IndexBuffer an das Vertex Array Object
    //! anzuhängen.
    virtual void prepareVertexArrayObject(Shader& shader, QOpenGLVertexArrayObject& vao);

    //! Zerstört alle Buffer der Geometrie. Kann überschrieben werden um zusätzliche Buffer wie z.B. einen IndexBuffer
    //! zu zerstören.
    virtual void destroyVertexBuffers();

    //! Hilfsmethode zur erstellung eines Buffers sowie senden der Daten an den Buffer.
    template <typename T>
    void initBuffer(QOpenGLBuffer& buffer, const QVector<T>& elements, VertexFormat::Format format);

    //! Felder für Geometriedaten
    QVector<QVector4D> mVertices;
    QVector<QVector4D> mVertexColors;
    QVector<QVector3D> mVertexNormals;
    QVector<QVector2D> mTextureCoordinates;

    //! Buffer für die Geometriedaten
    QOpenGLBuffer mVertexPositionBuffer;
    QOpenGLBuffer mVertexColorBuffer;
    QOpenGLBuffer mNormalBuffer;
    QOpenGLBuffer mTextureCoordinatesBuffer;

    //! Speichert informationen über die Existenz einzelner Vertex Attribute.
    //! \see Geometry::initializeGeometry(), Geometry::initBuffer()
    VertexFormat mVertexFormat;

    //! Gibt an ob die Geometrie bereits Inizialisiert ist.
    bool mGeometryInitialized = false;

private:
    //! Ein QOpenGLVertexArrayObject pro Shader da Attribute-Locations nicht konstant sein müssen und der Optimizer
    //! Vertex Attribute aus Shader die diese nicht benötigen weg Optimiert.
    QMap<Shader*, QOpenGLVertexArrayObject*> mVAOs;

    //! Speichert das in bindBuffer() gebundene QOpenGLVertexArrayObject zwischen um es in unbindBuffer() wieder
    //! unbinden zu können.
    QOpenGLVertexArrayObject* mCurrentVAO = nullptr;
};

template <typename T>
void Geometry::initBuffer(QOpenGLBuffer& buffer, const QVector<T>& elements, VertexFormat::Format format)
{
    if (!buffer.isCreated())
    {
        buffer.create();
    }

    buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    buffer.bind();
    buffer.allocate(elements.constData(), elements.size() * sizeof(T));
    mVertexFormat.addVertexFormat(format);
}

//inline DrawablePropertyId Geometry::getPropertyId()
//{
//    return DrawablePropertyIdGeter::getIdOf<Geometry>();
//}

//inline const VertexFormat& Geometry::getVertexFormat() const
//{
//    return mVertexFormat;
//}

#endif
