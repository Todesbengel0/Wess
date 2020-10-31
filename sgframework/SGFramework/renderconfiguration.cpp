#include "renderconfiguration.h"
#include "xmlparser.h"

RenderConfiguration::RenderConfiguration() :
    ReshapeListener()
{
    mWindowSizeChanged = true;
    mCurrentViewport = 0;
    mViewportCount = 1;
}

void RenderConfiguration::update(Camera* camera)
{
    if(camera->isProjectionMatrixCalcNeeded() || mWindowSizeChanged)
    {
        float aspectRatio = (float)mWidth / (float)mHeight;
        QMatrix4x4 projectionMatrix;
        projectionMatrix.perspective(camera->getAperture(), aspectRatio, camera->getNearPlane(), camera->getFarPlane());
        mProjectionsMatrices.clear();
        mProjectionsMatrices.push_back(projectionMatrix);

        mWindowSizeChanged = false;
    }
    if(camera->isViewMatrixCalcNeeded())
    {
        mViewMatrix.setToIdentity();
        mViewMatrix.lookAt(camera->getPosition(), camera->getPosition() + camera->getViewDir(), camera->getUpDir());
    }
}

bool RenderConfiguration::nextViewport()
{
    if ( mCurrentViewport < mViewportCount-1)
    {
        mCurrentViewport++;
        return true;
    }
    return false;
}

void RenderConfiguration::setViewport()
{
    glFunctions->glViewport(0, 0, mWidth, mHeight);
}

void RenderConfiguration::reshape(int w, int h)
{
    ReshapeListener::reshape(w, h);
    mWindowSizeChanged = true;
}

void RenderConfiguration::cleanUp()
{
    mCurrentViewport = 0;
}

QMatrix4x4 RenderConfiguration::getProjectionMatrix()
{
    if (mCurrentViewport < mProjectionsMatrices.length())
        return mProjectionsMatrices[mCurrentViewport];
    else
    {
        qDebug() << "VP: " << mCurrentViewport << " length: " << mProjectionsMatrices.length();
        QMatrix4x4 lDummy;
        lDummy.perspective(90.,1.,1.,100.);
        qDebug() << "Trying to get projection matrix for non existent viewport.";
        return (lDummy);
    }
}

QMatrix4x4 RenderConfiguration::getViewMatrix()
{
    return mViewMatrix;
}
