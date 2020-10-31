#include "opengl.h"
#include "shader.h"
//#include <iostream>
#include <QFile>

#include "scenemanager.h"
#include "light.h"
#include "shaderfileloader.h"

QString Shader::sShaderName;

Shader::Shader()
{
}

Shader::~Shader()
{
    Q_ASSERT_X(false, "Shader::~Shader",
               "Löschen von Shadern ist nicht erlaubt, da es vom ShaderManager verwaltet wird.");
}

void Shader::compileLinkShader()
{
    qDebug() << "Compiling " << mVertexSource << mFragmentSource << mGeometrySource << mTcSource << mTeSource;
    compileLinkShader(mVertexSource, mFragmentSource, mGeometrySource, mTcSource, mTeSource);
}

void Shader::compileLinkShader(const QString& vertexFilepath, const QString& fragmentFilepath,
                               const QString& geometryFilepath, const QString& tcsPath, const QString& tesPath)
{
    mCompLinkOK = true;
    if (shaderProgram() == NULL)
    {
        mShaderProgram = new QOpenGLShaderProgram();
    }

    QString vertexSourceCode = loadShaderAndAddDirectives(vertexFilepath);
    if(!shaderProgram()->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSourceCode))
    {
        mCompLinkOK = false;
        qDebug() << "Failed to compile vertex shader";
        qDebug() << "File: " << vertexFilepath;
        qDebug() << "Log: " << shaderProgram()->log();
        return;
    }

    if (!geometryFilepath.isEmpty())
    {
        QString geometrySourceCode = loadShaderAndAddDirectives(geometryFilepath);
        if (!shaderProgram()->addShaderFromSourceCode(QOpenGLShader::Geometry, geometrySourceCode))
        {
            mCompLinkOK = false;
            qDebug() << "Failed to compile geometry shader!";
            qDebug() << "File: " << geometryFilepath;
            qDebug() << "Log: " << shaderProgram()->log();

            return;
        }
    }
    if (!tcsPath.isEmpty())
    {
        QString tcsSourceCode = loadShaderAndAddDirectives(tcsPath);
        if(!shaderProgram()->addShaderFromSourceCode(QOpenGLShader::TessellationControl, tcsSourceCode))
        {
            mCompLinkOK = false;
            qDebug() << "Failed to compile tesselation control shader!";
            qDebug() << "File: " << tcsPath;
            qDebug() << "Log: " << shaderProgram()->log();

            return;
        }
    }
    if (!tesPath.isEmpty())
    {
        QString tesSourceCode = loadShaderAndAddDirectives(tesPath);
        if (!shaderProgram()->addShaderFromSourceCode(QOpenGLShader::TessellationEvaluation, tesSourceCode))
        {
            mCompLinkOK = false;
            qDebug() << "Failed to compile tesselation evaluation shader!";
            qDebug() << "File: " << tesPath;
            qDebug() << "Log: " << shaderProgram()->log();

            return;
        }
    }
    //    if (!tcsPath.isEmpty())
    //        addShaderwoQtSupport(GL_TESS_CONTROL_SHADER, tcsPath);
    //    if (!tesPath.isEmpty())
    //        addShaderwoQtSupport(GL_TESS_EVALUATION_SHADER, tesPath);
    //    if (!geometryFilepath.isEmpty())
    //        addShaderwoQtSupport(GL_GEOMETRY_SHADER, geometryFilepath);

    QString fragmentSourceCode = loadShaderAndAddDirectives(fragmentFilepath);
    if(!shaderProgram()->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSourceCode))
    {
        mCompLinkOK = false;
        qDebug() << "Failed to compile fragment shader!";
        qDebug() << "File: " << fragmentFilepath;
        qDebug() << "Log: " << shaderProgram()->log();

        return;
    }

    if (!shaderProgram()->link())
    {
        mCompLinkOK = false;
        qDebug() << "Failed to link shader! Log: " << shaderProgram()->log();
        return;
    }
}

void Shader::initialize()
{
    Shader::sShaderName = "basic";
    mShaderProgram = NULL;

    // setze Prio für das Sortieren der Shapelist
    setPriority(333);

    compileLinkShader();
}

void Shader::initialize(const QString& vPath, const QString& fPath, const QString& gPath, const QString& tcsPath, const QString& tesPath)
{
    Shader::sShaderName = vPath.section('/', -1);
    mVertexSource = vPath;
    mFragmentSource = fPath;
    mGeometrySource = gPath;
    mTcSource = tcsPath;
    mTeSource = tesPath;
    mShaderProgram = NULL;

    // setze Prio für das Sortieren der Shapelist
    setPriority(333);

    compileLinkShader();
}

QString Shader::loadShaderAndAddDirectives(const QString& pFilePath)
{
    QString lCode = ShaderFileLoader::loadShaderFromFileQt(pFilePath);
    lCode.prepend(QLatin1String(SGFSHADERVERSIONSTRING));
    return lCode;
}

void Shader::addShaderwoQtSupport(GLenum type, const QString& path)
{
    //    QFile codeFile(path);
    QString code;
    GLuint shader;
    GLint result;
    char error[2048];
    QByteArray bytes;
    char* ptr;

    //    if (!codeFile.open(QIODevice::ReadOnly))
    //        qCritical() << "Unable to open Shader " << path;
    //    QTextStream in(&codeFile);
    //    code = in.readAll();
    code = loadShaderAndAddDirectives(path);

    // QString to char * conversion is ugly!
    bytes = code.toLatin1();

    shader = glFunctions->glCreateShader(type);
    ptr = bytes.data();
    glFunctions->glShaderSource(shader, 1, (const char**)&ptr, NULL);
    glFunctions->glCompileShader(shader);
    // Returns the results of the shader compilation.
    glFunctions->glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    // Display shader errors if any.
    if (!result)
    {
        // Get the error message and display it.
        glFunctions->glGetShaderInfoLog(shader, sizeof(error), NULL, error);
        qCritical() << "Error in Shader " << path << ": " << error;
        return;
    }

    // Attach to the effect's context.
    glFunctions->glAttachShader(mShaderProgram->programId(), shader);
}

void Shader::listActiveUniforms()
{
    int total = -1;
    glFunctions->glGetProgramiv(this->shaderProgram()->programId(), GL_ACTIVE_UNIFORMS, &total);
    qDebug() << "Listing " << total << " active uniforms";
    for (int i = 0; i < total; ++i)
    {
        int name_len = -1, num = -1;
        GLenum type = GL_ZERO;
        char name[100];
        glFunctions->glGetActiveUniform(this->shaderProgram()->programId(), GLuint(i), sizeof(name) - 1, &name_len,
                                        &num, &type, name);
        name[name_len] = 0;
        qDebug() << name;
    }
}

DrawablePropertyId Shader::getId() const
{
    return getPropertyId();
}

void Shader::enableShader()
{
    if (mCompLinkOK)
    {
        if (!shaderProgram()->bind())
        {
            qDebug() << "Shader program konnte nicht gebunden werden.";
        }
    }
}

//! Setzen aller (!) Uniforms des Shaders. Abgeleitete Klassen sollten diese Methode überschreiben,
//! um ihre eigenen Uniforms zu setzen. Gegebenenfalls dann die Methode dieser Basisklasse rufen, um
//! die "Standard"-Uniforms setzen zu lassen (Lichtquellen, etc.)
void Shader::setShaderUniforms()
{
    SceneManager::instance()->getActiveContext()->setUniforms(shaderProgram());
}

void Shader::disableShader()
{
    shaderProgram()->release();
}
