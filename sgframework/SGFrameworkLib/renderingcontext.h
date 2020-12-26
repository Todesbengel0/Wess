#ifndef RENDERINGCONTEXT_H
#define RENDERINGCONTEXT_H

#include <QList>

#include "opengl.h"
#include "node.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "shader.h"
#include "uniformbuffer.h"
#include "renderconfiguration.h"

class ShadowLight;
class SunLight;
class PointLight;
class SpotLight;

//! Der RenderingContext hält alles, was einen Rendering-Vorgang GLOBAL bestimmt. Das heißt im wesentlichen wird dort
//! für den aktuellen Rendering-Durchlauf der Zustand gespeichert: Kamera, Lichtquellen, Material, Blending. Diese
//! Attribute werden dann so an die jeweiligen Shader übergeben. Mit dem Kontext wird bestimmt WIE eine Szene gerendert
//! wird. Die Kontexte werden vom SceneManager verwaltet, wie die Szenen selbst (Scene) auch. Der RenderingManager
//! kümmert sich nur um die Renderer selbst.
class RenderingContext
{
public:
    RenderingContext() :mUniformBuffer(sUniformBufferBinding), mCam(NULL), mRenderConfig(NULL) {init();}
    RenderingContext(Camera* pCamera)  :mUniformBuffer(sUniformBufferBinding), mCam(NULL), mRenderConfig(NULL) {init();mCam = pCamera;}
    virtual ~RenderingContext();
    Camera* getCamera() const {return mCam;}
    void setCamera(Camera* pCamera);
    void setRenderConfig(RenderConfiguration *pConfig);
    void addActiveLight(SunLight* light);
    void addActiveLight(PointLight* light);
    void addActiveLight(SpotLight* light);
    void resetContext();
    virtual void setUniforms(QOpenGLShaderProgram* s);
    Shader* getDefaultShader() {return mDefaultShader;}
    void setShadowLight(ShadowLight* l) {mShadowLight = l;}
    ShadowLight* getShadowLight() {return mShadowLight;}
    void setBlending();

    void setModelMatrix(const QMatrix4x4& m) {mModelMatrix = m;}
    const QMatrix4x4& getModelMatrix() const {return mModelMatrix;}
    void multiplyRightModelMatrix(const QMatrix4x4& m) {mModelMatrix *= m;}

    //! Gibt die Instanz der DisplayConfiguration zurück um auf die berechneten Matrizen zugreifen zu können
    RenderConfiguration* getDisplayConfiguration(){return mRenderConfig;}

    //! Aktualisiert die Projektions- und Viewmatrix
    void update();
private:
    QVector<SunLight*> mActiveSunLights;
    QVector<PointLight*> mActivePointLights;
    QVector<SpotLight*> mActiveSpotLights;
    QMatrix4x4 mModelMatrix;
    UniformBuffer mUniformBuffer;
    Camera* mCam;
    Shader* mDefaultShader;
    ShadowLight* mShadowLight;
    RenderConfiguration* mRenderConfig;
    static const int sMaxLights;
    static const int sUniformBufferBinding;

    void createUniformBuffer(QOpenGLShaderProgram& shaderProgram);
    void updateSunLightsBufferData(int indexOffset);
    void updatePointLightsBufferData(int indexOffset);
    void updateSpotLightsBufferData(int indexOffset);
    bool haveSunLightsChanges() const;
    bool havePointLightsChanges() const;
    bool haveSpotLightsChanges() const;

    virtual void init();
};


#endif // RENDERINGCONTEXT_H
