#include "physicaccessablecamera.h"

PhysicAccessableCamera::PhysicAccessableCamera(): Camera()
{
    mXAngle = 0.f;
    mYAngle = 0.f;
}

void PhysicAccessableCamera::setYAngleTripod(float yAngle)
{
    mYAngle = yAngle;
    setRotation(mYAngle, mXAngle, 0.f);
}

void PhysicAccessableCamera::setXAngleTripod(float xAngle)
{
    mXAngle = xAngle;
    setRotation(mYAngle, mXAngle, 0.f);
}

void PhysicAccessableCamera::setTranslationTripod(float x, float y, float z)
{
    setPosition(QVector3D(x,y,z));
}
