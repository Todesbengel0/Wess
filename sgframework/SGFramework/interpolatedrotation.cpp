#include "interpolatedrotation.h"

void InterpolatedRotation::rotateStart(float angle, QVector3D axis)
{
    mStartMatrix.rotate(angle, axis);
    mStartQuat *= QQuaternion::fromAxisAndAngle(axis, angle);
}

void InterpolatedRotation::rotateEnd(float angle, QVector3D axis)
{
    mEndMatrix.rotate(angle, axis);
    mEndQuat *= QQuaternion::fromAxisAndAngle(axis, angle);
}

InterpolatedRotation::InterpolatedRotation()
{
    mStartMatrix.setToIdentity();
    mEndMatrix.setToIdentity();
    mT = 0.0f;
    mSpeed = 0.01f;
    mBackandForth = false;
    mRepresentasQuat = true;
}

void InterpolatedRotation::doIt()
{
    mT += mSpeed;
    QMatrix4x4& modelMatrix = this->getModelMatrix();
    if ((mT >= 1.0f) || (mT < 0.0f))
    {
        if (mBackandForth)
        {
            mSpeed = -mSpeed;
        }
        else
        {
            mT = 0.0f;
        }
    }
    modelMatrix.setToIdentity();
    if (mRepresentasQuat)
    {
        modelMatrix.rotate(QQuaternion::nlerp(mStartQuat, mEndQuat, mT));
    }
    else
    {
        modelMatrix = (1 - mT) * mStartMatrix + mT * mEndMatrix;
    }
}
