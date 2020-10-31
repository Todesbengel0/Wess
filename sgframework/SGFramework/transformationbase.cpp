#include "transformationbase.h"
#include "scenemanager.h"
#include "shadowlight.h"

TransformationBase::TransformationBase()
{
}

void TransformationBase::translate(float tx, float ty, float tz)
{
    mModelMatrix.translate(QVector3D(tx, ty, tz));
}
void TransformationBase::rotate(float angle, const QVector3D& axis)
{
    mModelMatrix.rotate(angle, axis);
}
void TransformationBase::rotate(float angle, float x, float y, float z)
{
    mModelMatrix.rotate(angle, x, y, z);
}
void TransformationBase::scale(const QVector3D& scale)
{
    mModelMatrix.scale(scale);
}
void TransformationBase::scale(float sx, float sy, float sz)
{
    mModelMatrix.scale(sx, sy, sz);
}
void TransformationBase::resetTrafo()
{
    mModelMatrix.setToIdentity();
}

void TransformationBase::activate()
{
    // Save M-Matrix of Scene
    mMTemp = SceneManager::instance()->getActiveContext()->getModelMatrix();
    SceneManager::instance()->getActiveContext()->multiplyRightModelMatrix(mModelMatrix);

    // Save MV-Light-Matrix of Scene
    if (SceneManager::instance()->getActiveContext()->getShadowLight() != NULL)
    {
        mMLTemp = SceneManager::instance()->getActiveContext()->getShadowLight()->getModelMatrix();
        SceneManager::instance()->getActiveContext()->getShadowLight()->multiplyRightModelMatrix(mModelMatrix);
    }
}

void TransformationBase::deActivate()
{
    SceneManager::instance()->getActiveContext()->setModelMatrix(mMTemp);

    if (SceneManager::instance()->getActiveContext()->getShadowLight() != NULL)
    {
        SceneManager::instance()->getActiveContext()->getShadowLight()->setModelMatrix(mMLTemp);
    }
}

void TransformationBase::setModelMatrix(const QMatrix4x4& modelMatrix)
{
    mModelMatrix = modelMatrix;
}
