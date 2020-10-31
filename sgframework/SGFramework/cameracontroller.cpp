#include "cameracontroller.h"

CameraController::CameraController(Camera* camera) :
    IdleObserver(),
    mCamera(camera),
    mMoveSpeed(1.f / 6.f),
    mRotSpeed(0.1f),
    mYaw(0.f),
    mPitch(0.f),
    mRoll(0.f)
{
}

Camera* CameraController::getCamera()
{
    return mCamera;
}
void CameraController::setCamera(Camera* camera)
{
    mCamera = camera;
}

void CameraController::setMoveSpeed(float moveSpeed)
{
    mMoveSpeed = moveSpeed;
}

float CameraController::getMoveSpeed() const
{
    return mMoveSpeed;
}

void CameraController::setRotSpeed(float rotSpeed)
{
    mRotSpeed = rotSpeed;
}

float CameraController::getRotSpeed() const
{
    return mRotSpeed;
}

void CameraController::doIt()
{
    controlCamera();
}
