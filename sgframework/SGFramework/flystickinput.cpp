#include "flystickinput.h"
#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <QTime>
#include <cmath>

#include "trackinginput.h"
#include "vrpndevicedtrack.h"

FlystickInput::FlystickInput()
    : mFsOrientation(QQuaternion(0, 0, 0, 0))
    , mFsOldOrientation(QQuaternion(0, 0, 0, 0))
    , mFsPos(QVector3D(0, 0, 0))
    , mFsOldPos(QVector3D(0, 0, 0))
    , mFsAnalogstickPos(QPoint(0, 0))
    , mFsAnalogstickOldPos(QPoint(0, 0))
    , mFsLastPositions{ QVector3D(0, 0, 0), QVector3D(0, 0, 0) }
    , mFsPosSavedLastTick(true)
    , mFsLastSavedPos(0)
{
    for (int i = 0; i < FS_MAX_BUTTONS; i++)
    {
        mFsButtons[i] = SG_NO_KEY;
        qDebug() << "Button " << i << "angemeldet!";
    }
    mFsTimeElapsed.start();
}

bool FlystickInput::isFsButtonPressed(int button)
{
    if (button < FS_MAX_BUTTONS)
    {
        if (mFsButtons[button] != SG_NO_KEY)
        {
            return true;
        }
    }
    return false;
}

void FlystickInput::setFsButtonDown(int button)
{
    if (button < FS_MAX_BUTTONS)
    {
        mFsButtons[button] = button + 1;
        qDebug() << mFsButtons[button];
    }
}

void FlystickInput::setFsButtonUp(int button)
{
    if (button < FS_MAX_BUTTONS)
    {
        mFsButtons[button] = SG_NO_KEY;
    }
}

void FlystickInput::setFsAnalogstickPos(const QPoint& pos)
{
    mFsAnalogstickOldPos = mFsAnalogstickPos;
    mFsAnalogstickPos = pos;
}

void FlystickInput::setFsPos(const QVector3D& position)
{
    mFsOldPos = mFsPos;
    mFsPos = position;
    QVector3D mFsPosDiff = mFsPos - mFsOldPos;
    if(mFsPosDiff.x() < 0.001 && mFsPosDiff.z() < 0.001)
    {
        mFsPos = mFsOldPos;
        return;
    }
    if (!mFsPosSavedLastTick)
    {
        mFsLastSavedPos = (mFsLastSavedPos == 0) ? 1 : 0;
        mFsLastPositions[mFsLastSavedPos] = mFsPos;
        mFsLastPosTime[mFsLastSavedPos] = mFsTimeElapsed.elapsed();
        mFsPosSavedLastTick = true;
    }
    else
    {
        mFsPosSavedLastTick = false;
    }
}

void FlystickInput::setFsOrientation(const QQuaternion& orientation)
{
    mFsOldOrientation = mFsOrientation;
    mFsOrientation = orientation;
}

QPoint FlystickInput::getFsAnalogstickPos() { return mFsAnalogstickPos; }
QVector3D FlystickInput::getFsPos()
{
    TrackerInfo lTrack;
    lTrack = VRPNDeviceDTrack::getInstance()->getFlightStickPosition();
    //return mFsPos - mFsOldPos;
    mFsPos = QVector3D(lTrack.m_Position[0], lTrack.m_Position[1], lTrack.m_Position[2]);
    return(mFsPos);
}

QVector3D FlystickInput::getFsPosDiff()
{
    static unsigned int lOneTime = 0;
    QVector3D lHeadPos = this->getFsPos();
    QVector3D lDiff = lHeadPos - mFsOldPos;
    mFsOldPos = lHeadPos;

    if (lOneTime == 0)
    {
        lOneTime++;
        return(QVector3D(.0f,.0f,.0f));
    }
    else
        return lDiff;
}

QQuaternion FlystickInput::getFsOrientation() { return mFsOrientation; }

float FlystickInput::getFsVelocity()
{
    float v            = fabs((mFsLastPositions[0] - mFsLastPositions[1]).length() / (mFsLastPosTime[0] - mFsLastPosTime[1]) * 10.f);
    return(true);
}

QVector3D FlystickInput::getFsVelVec()
{
    return mFsLastPositions[mFsLastSavedPos] - mFsLastPositions[(mFsLastSavedPos == 1) ? 0:1];
}


void FlystickInput::calculateTRViewDirAngles()
{
    float pitch = 0, yaw = 0, roll = 0;
    mFsOldOrientation.getEulerAngles(&pitch, &yaw, &roll);
    float pitch2 = 0, yaw2 = 0, roll2 = 0;
    mFsOrientation.getEulerAngles(&pitch2, &yaw2, &roll2);
    m_pitch = pitch2 - pitch;
    m_yaw = yaw2 - yaw;
    m_roll = roll2 - roll;
}

float FlystickInput::getTRViewDirPitchAngle()
{

    return m_pitch;
}

float FlystickInput::getTRViewDirRollAngle()
{
    return m_roll;
}

float FlystickInput::getTRViewDirYawAngle()
{
    return m_yaw;
}



