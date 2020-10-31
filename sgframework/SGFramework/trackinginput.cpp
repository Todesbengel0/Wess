#include "trackinginput.h"
#include "vrpndevicedtrack.h"

TrackingInput::TrackingInput() :
    mTrViewDirection(QQuaternion(0, 0, 0, 0)),
    mTrViewOldDirection(QQuaternion(0, 0, 0, 0))
{}

void TrackingInput::setTrViewDirection(const QQuaternion &pos)
{
    mTrViewOldDirection = mTrViewDirection;
    mTrViewDirection = pos;
}

void TrackingInput::setTrHeadPos(const QVector3D &pos)
{
    mTrHeadOldPos = mTrHeadPos;
    mTrHeadPos = pos;
    m_hasChanged = true;
}

QVector3D TrackingInput::getTrHeadPos()
{
    TrackerInfo lTrack;
    lTrack = VRPNDeviceDTrack::getInstance()->getHeadPosition();
    mTrHeadPos = QVector3D(lTrack.m_Position[0], lTrack.m_Position[1], lTrack.m_Position[2]);
    return mTrHeadPos;
}

QVector3D TrackingInput::getTrHeadPosDiff()
{
    QVector3D lHeadPos = this->getTrHeadPos();
    QVector3D lDiff = lHeadPos - mTrHeadOldPos;
    mTrHeadOldPos = lHeadPos;
    return lDiff;

//    if(test.x() < 0.0001 && test.z() < 0.0001)
//        return mTrHeadPos - mTrHeadPos;
//    return mTrHeadPos - mTrHeadOldPos;
}

QQuaternion TrackingInput::getTrViewDirection()
{
    return mTrViewDirection;
}

QQuaternion TrackingInput::getTrViewDirectionDiff()
{
    return mTrViewOldDirection - mTrViewDirection;
}


void TrackingInput::calculateTRViewDirAngles()
{
    float pitch = 0, yaw = 0, roll = 0;
    float pitch2 = 0, yaw2 = 0, roll2 = 0;

    mTrViewOldDirection.getEulerAngles(&pitch, &yaw, &roll);

    mTrViewDirection.getEulerAngles(&pitch2, &yaw2, &roll2);

    m_pitch = pitch2 - pitch;
    m_yaw = yaw2 - yaw;
    m_roll = roll2 - roll;
}

float TrackingInput::getTRViewDirPitchAngle()
{

    return m_pitch;
}

float TrackingInput::getTRViewDirRollAngle()
{
    return m_roll;
}

float TrackingInput::getTRViewDirYawAngle()
{
    return m_yaw;
}
