#ifndef SHADOWLIGHT_H
#define SHADOWLIGHT_H

#include "opengl.h"
#include "renderconfiguration.h"
#include "scene.h"
#include "texture.h"
#include "renderer.h"
#include "camera.h"

//! Klasse, die als PreRenderer eine ShadowMap in ein FBO rendert, das später als Textur verwendet werden kann
class ShadowLight : public RenderConfiguration, public Renderer
{
private:
    Shader* mShadedObjectShader;
    GLuint mTexture;
    GLuint mFbo;
    QMatrix4x4 mShadowModelMatrix;
    GLsizei mShadowMapResolutionX = 1;
    GLsizei mShadowMapResolutionY = 1;
    Camera *mCamera;

public:
    ShadowLight();
    void init(int shadowResX, int shadowResY, Shader* shadedObjectShader, unsigned short contextNr, unsigned short sceneNr);

    void setPosition(const QVector3D& position) {mCamera->setPosition(position);}
    void setRotation(float yaw, float pitch, float roll) {mCamera->setRotation(yaw, pitch, roll);}
    void lookAt(const QVector3D& pEye, const QVector3D& pCenter, const QVector3D& pUp) {mViewMatrix.setToIdentity(); mViewMatrix.lookAt(pEye,pCenter,pUp);}
    // overwrite Renderer::renderScene()
    virtual void renderScene() override;

    GLuint shadowMap() const;

    inline void setModelMatrix(const QMatrix4x4& m) {mShadowModelMatrix = m;}
    inline const QMatrix4x4& getModelMatrix() const {return mShadowModelMatrix;}
    inline void multiplyRightModelMatrix(const QMatrix4x4& m) {mShadowModelMatrix *= m;}

    //! aktualisiert die Matrizen der Camera. Überschreibt RenderConfiguration::update(), da die view matrix nicht
    //! (neu) berechnet wird.
    virtual void update(Camera* camera) override; // Methode aus RenderConfiguration überschreiben
    //! Standardviewport für Shadowlight setzen -> Methode aus RenderConfiguration überschreiben
    virtual void setViewport() override;
};

#endif // SHADOWLIGHT_H
