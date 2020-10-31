#include "shadowmapproperty.h"
#include "scenemanager.h"

ShadowMapProperty::ShadowMapProperty()
    : mHomgenusToTexCoordinates(0.5, 0.0, 0.0, 0.5, 0.0, 0.5, 0.0, 0.5, 0.0, 0.0, 0.5, 0.5, 0.0, 0.0, 0.0, 1.0)
{
}

void ShadowMapProperty::turnOn(Shader* shader)
{
    if (SceneManager::instance()->getActiveContext()->getShadowLight() != NULL)
    {
            QMatrix4x4 m = SceneManager::instance()->getActiveContext()->getShadowLight()->getModelMatrix();
            QMatrix4x4 mv = SceneManager::instance()->getActiveContext()->getShadowLight()->getViewMatrix() * m;

            QMatrix4x4 depthBiasMVP =   // This matrix calculates texture coordinates
              mHomgenusToTexCoordinates // from the vertexposition
              * SceneManager::instance()->getActiveContext()->getShadowLight()->getProjectionMatrix() * mv;

            shader->shaderProgram()->setUniformValue("homMVPLight", depthBiasMVP);
            shader->shaderProgram()->setUniformValue("MVLight", mv);
            shader->shaderProgram()->setUniformValue("NLight", mv.normalMatrix());
            shader->shaderProgram()->setUniformValue("shadowMap", 6);
            glFunctions->glActiveTexture(GL_TEXTURE6);
            glFunctions->glBindTexture(GL_TEXTURE_2D, SceneManager::instance()->getActiveContext()->getShadowLight()->shadowMap());
    }
}

void ShadowMapProperty::turnOff(Shader* /*shader*/)
{
    glFunctions->glActiveTexture(GL_TEXTURE6);
    glFunctions->glBindTexture(GL_TEXTURE_2D, 0);
}

DrawablePropertyId ShadowMapProperty::getId() const
{
    return getPropertyId();
}
