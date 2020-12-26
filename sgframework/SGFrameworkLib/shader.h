#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLShaderProgram>
#include "drawableproperty.h"
#include "opengl.h"

//! Hier werden die Shaderprogramme erzeugt und in den Szenengraph eingebunden. Die Klasse besteht einerseits aus
//! "Convenience"-Methoden zum Laden und Übersetzen der Shader-Quelldateien. Außerdem wird für das binden der Programme
//! zum richtigen Zeitpunkt sowie das Setzen der Uniform-Variablen gesorgt.
class Shader : public DrawableProperty
{
public:
    Shader();
    virtual void enableShader();
    virtual void setShaderUniforms();
    void disableShader();

    QOpenGLShaderProgram* shaderProgram() {return mShaderProgram;}

    virtual const QString& getShaderName() {return sShaderName;}
    void listActiveUniforms();
    bool isOK() {return mCompLinkOK;}
    static DrawablePropertyId getPropertyId() {return DrawablePropertyIdGeter::getIdOf<Shader>();}
    virtual DrawablePropertyId getId() const override;

protected:
    virtual ~Shader();

    QString mVertexSource;
    QString mFragmentSource;
    QString mGeometrySource;
    QString mTeSource;
    QString mTcSource;
    QOpenGLShaderProgram* mShaderProgram;

    void addShaderwoQtSupport(GLenum type, const QString& path);
    void compileLinkShader();
    void compileLinkShader(const QString& vertexFilepath, const QString& fragmentFilepath,
                           const QString& geometryFilepath, const QString& tcsPath, const QString& tesPath);
    QString loadShaderAndAddDirectives(const QString& pFilePath);

    static QString sShaderName;
    bool mCompLinkOK;

private:
    void initialize();
    void initialize(const QString& vPath, const QString& fPath, const QString& gPath = QString(),
                    const QString& tcsPath = QString(), const QString& tesPath = QString());

    friend class ShaderManager;
};

//inline QOpenGLShaderProgram* Shader::shaderProgram()
//{
//    return mShaderProgram;
//}

//inline const QString& Shader::getShaderName()
//{
//    return sShaderName;
//}

//inline bool Shader::isOK()
//{
//    return mCompLinkOK;
//}

//inline DrawablePropertyId Shader::getPropertyId()
//{
//    return DrawablePropertyIdGeter::getIdOf<Shader>();
//}

#endif // SHADER_H
