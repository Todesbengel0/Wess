#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "camera.h"
#include "idleobserver.h"

class CameraController : public IdleObserver
{
public:
    CameraController(Camera* camera);

    Camera* getCamera();
    void setCamera(Camera* camera);

    float getMoveSpeed() const;
    void setMoveSpeed(float moveSpeed);
    float getRotSpeed() const;
    void setRotSpeed(float rotSpeed);

    void doIt() override;

protected:
    Camera* mCamera;
    float mMoveSpeed;
    float mRotSpeed;

    float mYaw;
    float mPitch;
    float mRoll;

    virtual void controlCamera() = 0;
};

#endif // CAMERACONTROLLER_H
