#include "shadowlight.h"
#include "scenemanager.h"
#include "shadowmaprenderingsgobjectvisitor.h"

GLuint ShadowLight::shadowMap() const
{
    return mTexture;
}

ShadowLight::ShadowLight() : RenderConfiguration(), Renderer(new ShadowMapRenderingSGObjectVisitor()),
    mCamera(new Camera())
{
}

void ShadowLight::init(int shadowResX, int shadowResY, Shader* shadedObjectShader, unsigned short contextNr, unsigned short sceneNr)
{
    mShadedObjectShader = shadedObjectShader;
    mContextNr = contextNr;
    mSceneNr = sceneNr;
    SceneManager::instance()->setActiveContext(mContextNr);
    RenderingContext* lContext = SceneManager::instance()->getActiveContext();
    lContext->setCamera(mCamera);
    lContext->setRenderConfig(this);

    mShadowMapResolutionX = shadowResX;
    mShadowMapResolutionY = shadowResY;

    mViewportCount = 1;

    QMatrix4x4 lProjMat;
    lProjMat.perspective(40.,1.,1.,100.);
    mProjectionsMatrices.push_front(lProjMat);
    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(mCamera->getPosition(), mCamera->getPosition() + mCamera->getViewDir(), mCamera->getUpDir());

    static_cast<ShadowMapRenderingSGObjectVisitor*>(mSGObjectVisitor)->setShadedObjectShader(shadedObjectShader);

    // set as prerenderer
    this->setAsPreRenderer();

    // initialize framebuffer and texture
    // The framebuffer regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    mFbo = 0;
    glFunctions->glGenFramebuffers(1, &mFbo);
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glFunctions->glActiveTexture(GL_TEXTURE6);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glFunctions->glGenTextures(1, &mTexture);
    glFunctions->glBindTexture(GL_TEXTURE_2D, mTexture);
    glFunctions->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowResX, shadowResY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glFunctions->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef QT_OPENGL_ES
     glFunctions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTexture, 0, 0);
#else
     glFunctions->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mTexture, 0);
     glFunctions->glDrawBuffer(GL_NONE); // No color buffer gets drawn.
#endif

}

void ShadowLight::renderScene()
{
    // da hier der shader gewechselt wird den globalen rendering state invalidieren
    RenderingState::getInstance().resetState();

    mShadedObjectShader->enableShader();
    mShadedObjectShader->shaderProgram()->setUniformValue("buildingShadowmap", true);
    mShadedObjectShader->setShaderUniforms();

    // render the shadowmap
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

    glFunctions->glEnable(GL_DEPTH_TEST);
    glFunctions->glClearColor(0.0, 0.0, 0.0, 0.0);
//    glFunctions->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mTexture, 0);
    glFunctions->glClear(GL_DEPTH_BUFFER_BIT);

    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(mCamera->getPosition(), mCamera->getPosition() + mCamera->getViewDir(), mCamera->getUpDir());
    mProjectionsMatrices[0].setToIdentity();
    mProjectionsMatrices[0].perspective(40.0f, 1.0f, 1.0f, 100.f);

    // Do frame
    Renderer::renderScene();

    mShadowModelMatrix.setToIdentity();

    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mShadedObjectShader->enableShader();
    mShadedObjectShader->shaderProgram()->setUniformValue("buildingShadowmap", false);
    mShadedObjectShader->setShaderUniforms();
}

void ShadowLight::update(Camera* camera)
{
    Q_UNUSED(camera);
}

void ShadowLight::setViewport()
{
    glFunctions->glViewport(0, 0, mShadowMapResolutionX, mShadowMapResolutionY);
}
