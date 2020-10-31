#include "dtrackcameracontroller.h"
#include "inputregistry.h"
#include "vrpndevicedtrack.h"

DTrackCameraController::DTrackCameraController(Camera* camera, QString device) :
    CameraController(camera)
{
    VRPNDeviceDTrack::getInstance();
    m_device = device;
}

void DTrackCameraController::controlCamera()
{
    if(m_device == "Flystick")
    {
        QVector3D headDiff = InputRegistry::getInstance().getFlystickInput()->getFsPosDiff();
        QVector3D camPosition = mCamera->getPosition();
        camPosition += mCamera->getRightDir() * (-headDiff.x()*100);
        camPosition += mCamera->getUpDir() * (-headDiff.y()*100);
        camPosition += -mCamera->getViewDir() * (-headDiff.z()*100);
        mCamera->setPosition(camPosition);

        InputRegistry::getInstance().getFlystickInput()->calculateTRViewDirAngles();
        mPitch += InputRegistry::getInstance().getFlystickInput()->getTRViewDirPitchAngle() / 25.0f;
        mYaw += InputRegistry::getInstance().getFlystickInput()->getTRViewDirRollAngle() / 25.0f;
        //float yawDiff = InputRegistry::getInstance().getFlystickInput()->getTRViewDirYawAngle() / 25.0;
        mCamera->setRotation(mYaw, mPitch, 0.f);
    }
    else if(m_device == "Trackingbrille")
    {
        QVector3D headDiff = InputRegistry::getInstance().getTrackingInput()->getTrHeadPosDiff();
//        qDebug() << "TRHeadPosDiff: " << headDiff;
        mCamera->setPosition(mCamera->getPosition() + QVector3D(-headDiff.x()*100, 0, headDiff.z()*100));

        InputRegistry::getInstance().getTrackingInput()->calculateTRViewDirAngles();
        mPitch += InputRegistry::getInstance().getTrackingInput()->getTRViewDirPitchAngle() / 10.0f;
        mYaw += InputRegistry::getInstance().getTrackingInput()->getTRViewDirRollAngle() / 10.0f;
        //float yawDiff = InputRegistry::getInstance().getTrackingInput()->getTRViewDirYawAngle() / 10.0;
        mCamera->setRotation(mYaw, mPitch, 0.f);
    }
    else
    {
        qDebug() << m_device << " nicht gefunden, nur Flystick oder Trackingbrille vorhanden!";
    }
}
