#include "uniformbuffer.h"

UniformBuffer::UniformBuffer(GLuint uniformBlockBinding, UsagePattern usage) :
    mUsage(usage),
    mUniformBlockBinding(uniformBlockBinding)
{
}

UniformBuffer::~UniformBuffer()
{
    destroy();
}

void UniformBuffer::destroy()
{
    if (mBufferId && glFunctions)
    {
        glFunctions->glDeleteBuffers(1, &mBufferId);
        mBufferId = 0;
    }
}

bool UniformBuffer::bindRange(int uniformIndex)
{
    return bindRange(uniformIndex, mOffsets.size() - uniformIndex);
}

bool UniformBuffer::bindRange(int uniformIndex, int uniformCount)
{
    if (mOffsets.isEmpty())
    {
        // Buffer ist leer, nichts zu tun
        return false;
    }

    Q_ASSERT_X(uniformIndex >= 0 && uniformIndex < mOffsets.size(), "UniformBuffer::bindRange", "Konnte Buffer nicht binden. Gegebener Index ist außerhalb des Uniformbereichs.");
    const int offset = mOffsets[uniformIndex];
    const GLsizeiptr dataSize = calculateDataSize(uniformIndex, uniformCount);
    glFunctions->glBindBufferRange(GL_UNIFORM_BUFFER, mUniformBlockBinding, mBufferId, offset, dataSize);

    return true;
}

bool UniformBuffer::bindBase()
{
    if (mOffsets.isEmpty())
    {
        // Buffer ist leer, nichts zu tun
        return false;
    }

    // Binde buffer an uniform block binding point
    glFunctions->glBindBufferBase(GL_UNIFORM_BUFFER, mUniformBlockBinding, mBufferId);

    return true;
}

bool UniformBuffer::bind(int uniformIndex)
{
    return bind(uniformIndex, mOffsets.size() - uniformIndex);
}

bool UniformBuffer::bind(int uniformIndex, int uniformCount)
{
    if (mOffsets.isEmpty())
    {
        // Buffer ist leer, nichts zu tun
        return false;
    }

    Q_ASSERT_X(uniformIndex >= 0 && uniformIndex < mOffsets.size(), "UniformBuffer::bind", "Konnte Schreibvorgang nicht starten. Gegebener Index ist außerhalb des Uniformbereichs.");
    glFunctions->glBindBuffer(GL_UNIFORM_BUFFER, mBufferId);
    const int offset = mOffsets[uniformIndex];
    const GLsizeiptr dataSize = calculateDataSize(uniformIndex, uniformCount);
    mGpuAccessibleData = glFunctions->glMapBufferRange(GL_UNIFORM_BUFFER, offset, dataSize, GL_MAP_WRITE_BIT);

    return true;
}

void UniformBuffer::release()
{
    if (mOffsets.isEmpty())
    {
        // Buffer ist leer, nichts zu tun
        return;
    }

    glFunctions->glUnmapBuffer(GL_UNIFORM_BUFFER);
    glFunctions->glBindBuffer(GL_UNIFORM_BUFFER, 0);
    mGpuAccessibleData = nullptr;
}

bool UniformBuffer::write(int uniformIndex, const void* data, int dataSize)
{
    if (mOffsets.isEmpty())
    {
        // Buffer ist leer, nichts zu tun
        return false;
    }

    Q_ASSERT_X(uniformIndex >= 0 && uniformIndex < mOffsets.size(), "UniformBuffer::write", "Konnte nicht in Buffer schreiben. Gegebener Index ist außerhalb des Uniformbereichs.");
    const int offset = mOffsets[uniformIndex];
    Q_ASSERT_X(dataSize <= (mBlockSize - offset), "UniformBuffer::write", "Konnte nicht in Buffer schreiben. Gegebene Datengröße überschreitet Buffergröße.");
    Q_ASSERT_X(mGpuAccessibleData, "UniformBuffer::write", "Konnte nicht in buffer schreiben. Bitte UniformBuffer::bind vor dieser Methode aufrufen.");
    const GLubyte* ubyteData = reinterpret_cast<const GLubyte*>(data);
    GLubyte* ubyteGpuAccessibleData = reinterpret_cast<GLubyte*>(mGpuAccessibleData);
    std::copy(&ubyteData[0], &ubyteData[dataSize], &ubyteGpuAccessibleData[offset]);

    return true;
}

GLuint UniformBuffer::addShaderProgram(const QOpenGLShaderProgram& shaderProgram)
{
    Q_ASSERT_X(mUniformBlockName, "UniformBuffer::addShaderProgram", "Konnte shader program nicht hinzufügen. Bitte UniformBuffer::create vor dieser Methode aufrufen.");
    GLuint& blockIndex = mShaderPrograms[&shaderProgram];
    if (blockIndex > 0)
    {
        // ShaderProgram ist bereits an Binding point gebunden => Nichts zu tun.
        return blockIndex - 1;
    }

    // Hole index des uniform blocks aus shader program
    GLuint newBlockIndex = glFunctions->glGetUniformBlockIndex(shaderProgram.programId(), mUniformBlockName);
    if (newBlockIndex == UINT_MAX)
    {
        // Gegebenes shader program enthält kein zugehöriges uniform block, nichts zu tun
        return newBlockIndex;
    }

    // Binde uniform block des shader programs an binding point
    blockIndex = newBlockIndex;
    glFunctions->glUniformBlockBinding(shaderProgram.programId(), blockIndex, mUniformBlockBinding);

    // Stelle sicher, dass blockIndex nicht 0 ist (Zur Überprüfung, ob ein Shader Program bereits gebunden wurde).
    ++blockIndex;

    return blockIndex - 1;
}

void UniformBuffer::initialize()
{
#ifdef QT_DEBUG
    GLint maxUniformBufferBindings;
    glFunctions->glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUniformBufferBindings);
    Q_ASSERT_X(static_cast<GLint>(mUniformBlockBinding) < maxUniformBufferBindings, "UniformBuffer::create", "Konnte uniform buffer nicht erzeugen. Maximale Anzahl an möglichen uniform buffers ist überschritten.");
#endif // QT_DEBUG

    if (!mBufferId)
    {
        // UBO existiert noch nicht => Erzeuge UBO
        glFunctions->glGenBuffers(1, &mBufferId);
    }
}

GLsizeiptr UniformBuffer::calculateDataSize(int uniformIndex, int uniformCount) const
{
    // Ermittle Datengröße anhand gegebenem Uniformindex sowie gegebener Uniformanzahl.
    const int lastIndex = uniformIndex + uniformCount;
    const GLsizeiptr dataSize = lastIndex == mOffsets.size() ? mBlockSize - mOffsets[uniformIndex] : mOffsets[lastIndex] - mOffsets[uniformIndex];
    Q_ASSERT_X(dataSize <= (mBlockSize - mOffsets[uniformIndex]), "UniformBuffer::calculateDataSize", "Konnte Datengröße nicht ermitteln. Gegebene Datengröße überschreitet Buffergröße.");

    return dataSize;
}

#ifdef QT_DEBUG
void UniformBuffer::validateOffsets()
{
    // Stelle sicher, dass die uniformnamen in der richtigen Reihenfolge mitgegeben wurden, damit später beim Schreiben in den Buffer keine Probleme auftreten.
    // Dadurch wird zudem ein sequentieller Speicherzugriff wahrscheinlicher => Bessere Performance bei UBOs.
    const int offsetCount = mOffsets.size();
    for (int i = 1; i < offsetCount; ++i)
    {
        Q_ASSERT_X(mOffsets[i] > mOffsets[i - 1], "UniformBuffer::validateOffsets", "Konnte uniform buffer nicht erzeugen. Gegebene Uniformnamen wurden in der falschen Reihenfolge angegeben.");
    }
}

#endif // QT_DEBUG
