#include "keyboardcameracontroller.h"
#include "inputregistry.h"

KeyboardCameraController::KeyboardCameraController(Camera* camera) :
    CameraController(camera)
{
}

void KeyboardCameraController::controlCamera()
{
    QVector3D deltaPosition;

    KeyboardInput* keyIn = InputRegistry::getInstance().getKeyboardInput();
    if (keyIn->isKeyPressed('w'))
    {
        deltaPosition += mCamera->getViewDir() * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('s'))
    {
        deltaPosition -= mCamera->getViewDir() * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('a'))
    {
        deltaPosition -= mCamera->getRightDir() * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('d'))
    {
        deltaPosition += mCamera->getRightDir() * mMoveSpeed;
    }
    if (keyIn->isKeyPressed(Qt::Key_Left))
    {
        mYaw -= mRotSpeed;
    }
    if (keyIn->isKeyPressed(Qt::Key_Right))
    {
        mYaw += mRotSpeed;
    }
    if (keyIn->isKeyPressed('e'))
    {
        mRoll += mRotSpeed;
    }
    if (keyIn->isKeyPressed('q'))
    {
        mRoll -= mRotSpeed;
    }
    if (keyIn->isKeyPressed(Qt::Key_Up))
    {
        mPitch -= mRotSpeed;
    }
    if (keyIn->isKeyPressed(Qt::Key_Down))
    {
        mPitch += mRotSpeed;
    }
    if (keyIn->isKeyPressed('x'))
    {
        deltaPosition -= QVector3D(1.f, 0.f, 0.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('X'))
    {
        deltaPosition += QVector3D(1.f, 0.f, 0.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('y'))
    {
        deltaPosition -= QVector3D(0.f, 1.f, 0.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('Y'))
    {
        deltaPosition += QVector3D(0.f, 1.f, 0.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('z'))
    {
        deltaPosition -= QVector3D(0.f, 0.f, 1.f) * mMoveSpeed;
    }
    if (keyIn->isKeyPressed('Z'))
    {
        deltaPosition += QVector3D(0.f, 0.f, 1.f) * mMoveSpeed;
    }

    mCamera->setRotation(mYaw, mPitch, mRoll);
    mCamera->setPosition(mCamera->getPosition() + deltaPosition);
}
