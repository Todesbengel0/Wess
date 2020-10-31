#include "geometry.h"
#include "drawable.h"
#include <map>
#include "scene.h"
#include "color.h"
#include "shader.h"
#include "vertexformatmanager.h"

Geometry::Geometry()
    : mVertexPositionBuffer(QOpenGLBuffer::VertexBuffer), mVertexColorBuffer(QOpenGLBuffer::VertexBuffer)
{

    // setze Prio für das Sortieren der Shapelist
    setPriority(25);
}

Geometry::~Geometry()
{
}

const QVector<QVector4D>& Geometry::getVertices() const
{
    Q_ASSERT_X(!isInterleaved(), "Geometry::getVertices()",
               "Auf die Vertices einer Interleavt gespeicherten geometry kann nur über InterleavedGeometry::getData() "
               "oder InterleavedGeometry::getTriangleList() zugegriffen werden!");
    return mVertices;
}

const QVector<QVector3D>& Geometry::getNormals() const
{
    Q_ASSERT_X(!isInterleaved(), "Geometry::getNormals()", "Auf die Normalen einer Interleaved gespeicherten geometry "
                                                           "kann nur über InterleavedGeometry::getData() zugegriffen "
                                                           "werden!");
    return mVertexNormals;
}

const QVector<QVector2D>& Geometry::getTextureCoordinates() const
{
    Q_ASSERT_X(!isInterleaved(), "Geometry::getTextureCoordinates()",
               "Auf die Textur-Koordinaten einer Interleavt gespeicherten geometry kann nur über "
               "InterleavedGeometry::getData() zugegriffen werden!");
    return mTextureCoordinates;
}

const QVector<QVector4D>& Geometry::getVertexColors() const
{
    Q_ASSERT_X(!isInterleaved(), "Geometry::getVertexColors()",
               "Auf die Vertex-Farben einer Interleavt gespeicherten geometry "
               "kann nur über InterleavedGeometry::getData() zugegriffen "
               "werden!");
    return mVertexColors;
}

void Geometry::fillVertices(QVector<QVector4D>& /*vertices*/)
{
}

void Geometry::fillColors(QVector<QVector4D>& /*colors*/)
{
    // Standard-Farben werden in Shader basic.vert bestimmt.
}

void Geometry::fillNormals(QVector<QVector3D>& /*normals*/, QVector<QVector4D>& /*vertices*/)
{
}

void Geometry::fillTexCoords(QVector<QVector2D>& /*texCoords*/)
{
}

void Geometry::deInitGeometry()
{
    mVertices.clear();
    mVertexNormals.clear();
    mTextureCoordinates.clear();
    mVertexColors.clear();
    mGeometryInitialized = false;
    for (auto it = mVAOs.begin(); it != mVAOs.end(); ++it)
    {
        QOpenGLVertexArrayObject* vao = it.value();
        vao->destroy();
        delete vao;
    }

    mVAOs.clear();

    destroyVertexBuffers();
}

DrawablePropertyId Geometry::getId() const
{
    return getPropertyId();
}

bool Geometry::isTriangleList() const
{
    return false;
}

bool Geometry::isInterleaved() const
{
    return false;
}

QVector<QVector4D> Geometry::getTriangleList()
{
    return mVertices;
}

void Geometry::bindBuffer(Shader& shader)
{
    QOpenGLVertexArrayObject*& vao = mVAOs[&shader];
    if (!vao)
    {
        // Vertex array object für aktuellen shader existiert nicht => Erzeuge vao (Es muss 1 vao pro shader existieren)
        vao = new QOpenGLVertexArrayObject;
        prepareVertexArrayObject(shader, *vao);
    }

    mCurrentVAO = vao;
    mCurrentVAO->bind();
}

void Geometry::unbindBuffer()
{
    Q_ASSERT_X(mCurrentVAO, "Geometry::unbindBuffer",
               "Unable to unbind buffer. Call Geometry::bindBuffer before you call this method");
    mCurrentVAO->release();
}

void Geometry::render(Drawable* /*d*/)
{
}

void Geometry::initializeGeometryBase()
{
    if (!mGeometryInitialized)
    {
        fillVertices(mVertices);
        fillColors(mVertexColors);
        fillNormals(mVertexNormals, mVertices);
        fillTexCoords(mTextureCoordinates);

        prepareVertexBuffers();

        mGeometryInitialized = true;
        //        qDebug() << "Geo success.";
    }
}

void Geometry::initializeGeometry(Drawable& drawable)
{
    this->initializeGeometryBase();
    // Setze entsprechendes Vertex Format. mVertexFormat kann nicht verwendet werden da beim sortieren von Drawablen
    // anhand ihrer Properties lediglich die adresse betrachtet wird. Somit wird über den VertexFormatManager eine
    // globale eindeutige instanz pro VertexFormat erstellt und zurückgegeben.
    drawable.setProperty<VertexFormat>(VertexFormatManager::getVertexFormat(mVertexFormat));
}

void Geometry::prepareVertexBuffers()
{
    if (!mVertices.isEmpty())
    {
        initBuffer(mVertexPositionBuffer, mVertices, VertexFormat::FormatPosition);
    }

    if (!mVertexColors.isEmpty())
    {
        initBuffer(mVertexColorBuffer, mVertexColors, VertexFormat::FormatColor);
    }

    if (!mVertexNormals.isEmpty())
    {
        initBuffer(mNormalBuffer, mVertexNormals, VertexFormat::FormatNormal);
    }

    if (!mTextureCoordinates.isEmpty())
    {
        initBuffer(mTextureCoordinatesBuffer, mTextureCoordinates, VertexFormat::FormatTextureCoodinate);
    }
}

void Geometry::prepareVertexArrayObject(Shader& shader, QOpenGLVertexArrayObject& vao)
{
    // Create a VAO for this "object"
    vao.create();
    vao.bind();

    QOpenGLShaderProgram* shaderProgram = shader.shaderProgram();
    if (!mVertices.isEmpty())
    {
        mVertexPositionBuffer.bind();
        shaderProgram->enableAttributeArray("vertexPosition");
        shaderProgram->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 4);
    }

    if (!mVertexColors.isEmpty())
    {
        mVertexColorBuffer.bind();
        shaderProgram->enableAttributeArray("vertexColor");
        shaderProgram->setAttributeBuffer("vertexColor", GL_FLOAT, 0, 4);
    }

    if (!mVertexNormals.isEmpty())
    {
        mNormalBuffer.bind();
        shaderProgram->enableAttributeArray("vertexNormal");
        shaderProgram->setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);
    }

    if (!mTextureCoordinates.isEmpty())
    {
        mTextureCoordinatesBuffer.bind();
        shaderProgram->enableAttributeArray("textureCoords");
        shaderProgram->setAttributeBuffer("textureCoords", GL_FLOAT, 0, 2);
    }

    //    d->getShader()->listActiveUniforms();
}

void Geometry::destroyVertexBuffers()
{
    mVertexPositionBuffer.destroy();
    mVertexColorBuffer.destroy();
    mNormalBuffer.destroy();
    mTextureCoordinatesBuffer.destroy();
}
