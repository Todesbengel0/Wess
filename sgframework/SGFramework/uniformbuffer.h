#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include "opengl.h"

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLExtraFunctions>
#include <QMap>

#ifdef Q_OS_ANDROID
#include <GLES3/gl31.h>
#endif

//! Die UniformBufferklasse bietet Funktionen an, um OpenGL uniform buffer objects zu erzeugen. \n
//! Beispiel:
//! \code
//! QOpenGLShaderProgram shaderProgram;
//! UniformBuffer uniformBuffer;
//! const GLchar* uniformNames[] = {
//!     "materialAmbient",
//!     "materialDiffuse",
//!     "materialSpecular"
//! };
//!
//! uniformBuffer.create(shaderProgram, "materialUniforms", uniformNames); // Erzeugt uniform buffer object "materialUniforms" und allokiert benötigten Speicher
//! uniformBuffer.bind(); // Bindet Buffer für Schreibvorgang
//! uniformBuffer.write(...); // Schreibt Daten in Buffer
//! ...
//! uniformBuffer.release(); // Löst Buffer
//!
//! uniformBuffer.bindBase(); // Bindet Daten im Buffer
//! \endcode
class UniformBuffer
{
public:
    //! Dieses enum bestimmt das usage pattern eines UniformBuffer Objekts.
    enum UsagePattern
    {
        StreamDraw = GL_STREAM_DRAW,
        StaticDraw = GL_STATIC_DRAW,
        DynamicDraw = GL_DYNAMIC_DRAW
    };

    //! Erzeugt ein neues uniform buffer object mit dem UsagePattern usage (standardmäßig DynamicDraw).
    //! \param uniformBlockBinding bestimmt den binding point des uniform buffer objects.
    //!
    //! Hinweis: Dieser Konstruktor erzeugt lediglich eine UniformBufferinstanz.
    //! Das eigentliche uniform buffer object im OpenGL server wirt nicht erzeugt,
    //! bevor \sa UniformBuffer::create() aufgerufen wurde.
    UniformBuffer(GLuint uniformBlockBinding = 0, UsagePattern usage = DynamicDraw);

    //! Zerstört das uniform buffer object.
    ~UniformBuffer();

    //! Zerstört das uniform buffer object.
    void destroy();

    //! Bindet eine über uniformIndex bestimmte Anzahl an Uniformvariablen im Shader,
    //! die mit dem uniform buffer object verknüpft sind.
    //! \param uniformIndex bestimmt den Startindex innerhalb des Uniformblocks im Shader,
    //! ab dem gebunden wird.
    //!
    //! Gibt false zurück, falls das Binden nicht möglich ist, sonst true.
    bool bindRange(int uniformIndex = 0);

    //! Bindet Uniformvariablen im shader in einem bestimmten Bereich, die mit dem uniform buffer object verknüpft sind.
    //! \param uniformIndex bestimmt den Startindex innerhalb des Uniformblocks im Shader.
    //! \param uniformCount bestimmt wie viele uniform variablen im Shader gebunden werden.
    //!
    //! Gibt false zurück, falls das Binden nicht möglich ist, sonst true.
    bool bindRange(int uniformIndex, int uniformCount);

    //! Bindet alle Uniformvariablen im Shader, die mit diesem uniform buffer object verknüpft sind.
    //!
    //! Gibt false zurück, falls das Binden nicht möglich ist, sonst true.
    bool bindBase();

    //! Bindet das uniform buffer object dieses UniformBuffers zum Schreiben an das derzeitige OpenGL Kontext.
    //! \param uniformIndex bestimmt, ab welcher Uniformvariable des zugehörigen
    //! Uniformblocks im Shader der Schreibvorgang beginnen soll.
    //!
    //! Gibt false zurück, falls das Binden nicht möglich ist, sonst true.
    bool bind(int uniformIndex = 0);

    //! Bindet das uniform buffer object dieses UniformBuffers zum Schreiben an das derzeitige OpenGL Kontext.
    //! \param uniformIndex bestimmt, ab welcher Uniformvariable des zugehörigen
    //! Uniformblocks im Shader der Schreibvorgang beginnt.
    //! \param uniformCount gibt an, wie viele Uniformvariablen innerhalb des zugehörigen Uniformblocks
    //! vom Schreibvorgang betroffen sind.
    //!
    //! Gibt false zurück, falls das Binden nicht möglich ist, sonst true.
    bool bind(int uniformIndex, int uniformCount);

    //! Löst das uniform buffer object dieses UniformBuffers vom derzeitigen OpenGL Kontext und beendet den Schreibvorgang.
    //!
    //! Diese methode sollte nach bind() aufgerufen werden, wenn der Schreibvorgang beendet ist.
    void release();

    //! Ersetzt dataSize Bytes dieses Buffers mit dem Inhalt von data.
    //! uniformIndex bezeichnet den index der Uniformvariable innerhalb des Uniformblocks im Shader.
    bool write(int uniformIndex, const void* data, int dataSize);

    //! Bindet shaderProgram an das uniform buffer object dieses UniformBuffers.
    //! Gibt Uniformblock index des zugehörigen Uniformblocks aus shader zurück, falls der
    //! zugehörige Uniformblock gefunden wird. Gibt sonst UINT_MAX zurück.
    GLuint addShaderProgram(const QOpenGLShaderProgram& shaderProgram);

    //! Erzeugt das uniform buffer object.
    //! Diese Methode holt sich zusätzlich aus dem shaderProgram den zu uniformBlockName zugehörigen uniform block index.
    //! uniformNames bestimmt die Namen einzelner Uniformvariablen innerhalb des Uniformblocks im Shader des shaderPrograms.
    //!
    //! Gibt false zurück, falls kein Uniformblock im Shader des shaderPrograms mit dem Namen uniformBlockName gefunden werden kann.
    //! Gibt false zurück, falls das erste Element in uniformNames nicht im Uniformblock gefunden werden kann.
    //! Gibt sonst true zurück.
    //!
    //! Hinweis: Uniformnamen im Array uniformNames müssen in derselben Reihenfolge, wie sie im Shader im Speicher liegen, angegeben werden.
    template <int uniformCount>
    inline bool create(const QOpenGLShaderProgram& shaderProgram, const GLchar* uniformBlockName, const GLchar* (&uniformNames)[uniformCount]);

    //! Gibt OpenGL id des uniform buffer objects zurück.
    GLuint getBufferId() const {return mBufferId;}

    //! Gibt binding point des uniform buffer objects zurück.
    GLuint getUniformBlockBinding() const {return mUniformBlockBinding;}

    //! Gibt UsagePattern des UniformBuffers zurück.
    UsagePattern getUsagePattern() const {return mUsage;}

    //! Gibt die Anzahl an Uniformvariablen im Uniformblock zurück.
    int getUniformCount() const {return mOffsets.size();}

    //! Gibt true zurück, falls dieser UniformBuffer erzeugt wurde, gibt sonst false zurück.
    bool isCreated() const {return mBufferId != 0;}

private:
    void initialize();
    GLsizeiptr calculateDataSize(int uniformIndex, int uniformCount) const;

#ifdef QT_DEBUG
    void validateOffsets();
#endif // QT_DEBUG

    QMap<const QOpenGLShaderProgram*, GLuint> mShaderPrograms;
    QVector<GLint> mOffsets;
    const GLchar* mUniformBlockName = nullptr;
    GLvoid* mGpuAccessibleData = nullptr;
    UsagePattern mUsage;
    GLuint mBufferId = 0;
    GLuint mUniformBlockBinding;
    GLint mBlockSize;

//Für Android Extrafunctions binden, sonst geht kein Zugriff auf GLES3 Funktionen
#ifdef Q_OS_ANDROID
    QOpenGLExtraFunctions *glFunctions;
#endif
};

template <int uniformCount>
inline bool UniformBuffer::create(const QOpenGLShaderProgram& shaderProgram, const GLchar* uniformBlockName, const GLchar* (&uniformNames)[uniformCount])
{
    // Code basiert auf https://www.packtpub.com/books/content/opengl-40-using-uniform-blocks-and-uniform-buffer-objects,
    // http://www.geeks3d.com/20140704/gpu-buffers-introduction-to-opengl-3-1-uniform-buffers-objects/,
    // http://learnopengl.com/#!Advanced-OpenGL/Advanced-GLSL und
    // http://www.opentk.com/node/2926
    mUniformBlockName = uniformBlockName;
#ifdef QT_OPENGL_ES
    //ES3 Funktionen bekommen wir nur über die "ExtraFunctions", transparent für Desktop OpenGL
    this->glFunctions = QOpenGLContext::currentContext()->extraFunctions();
#endif

    const GLuint blockIndex = addShaderProgram(shaderProgram);
    if (blockIndex == UINT_MAX)
    {
        // Gegebenes shader program enthält kein zugehöriges uniform block, nichts zu tun
        return false;
    }

    // Hole indices der einzelnen uniforms aus uniform block des shader programs
    GLuint indices[uniformCount];
    glFunctions->glGetUniformIndices(shaderProgram.programId(), uniformCount, uniformNames, indices);
    if (indices[0] == UINT_MAX)
    {
        // Den gegebenen Uniformnamen konnten keine indices zugewiesen werden
        // => Gegebenes shader program hat entweder keinen uniform buffer oder die gegebenen uniformnamen wurden nicht gefunden
        // => Nichts zu tun
        return false;
    }

    // Hole byte offsets der uniforms innerhalb des uniform blocks
    mOffsets.resize(uniformCount);
    glFunctions->glGetActiveUniformsiv(shaderProgram.programId(), uniformCount, indices, GL_UNIFORM_OFFSET, mOffsets.data());

    // Hole Buffergröße in bytes
    glFunctions->glGetActiveUniformBlockiv(shaderProgram.programId(), blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &mBlockSize);

    initialize();
    glFunctions->glBindBuffer(GL_UNIFORM_BUFFER, mBufferId);

    // Allokiere benötigten speicher
    glFunctions->glBufferData(GL_UNIFORM_BUFFER, mBlockSize, nullptr, mUsage);

#ifdef QT_DEBUG
    validateOffsets();
#endif // QT_DEBUG

    glFunctions->glBindBuffer(GL_UNIFORM_BUFFER, 0);

    return true;
}

//inline GLuint UniformBuffer::getBufferId() const
//{
//    return mBufferId;
//}

//inline GLuint UniformBuffer::getUniformBlockBinding() const
//{
//    return mUniformBlockBinding;
//}

//inline UniformBuffer::UsagePattern UniformBuffer::getUsagePattern() const
//{
//    return mUsage;
//}

//inline int UniformBuffer::getUniformCount() const
//{
//    return mOffsets.size();
//}

//inline bool UniformBuffer::isCreated() const
//{
//    return mBufferId != 0;
//}

#endif // UNIFORMBUFFER_H
