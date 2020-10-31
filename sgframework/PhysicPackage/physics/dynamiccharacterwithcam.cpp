#include "dynamiccharacterwithcam.h"
#include "icameraaccess.h"
#include "igeometryaccess.h"

DynamicCharacterWithCam::DynamicCharacterWithCam(int a_PhysicEngineSlot, IGeometryAccess *a_Geometry)
    :DynamicCharacter(a_PhysicEngineSlot,a_Geometry)
{
    this->setRelativeCamPosition(0.f,0.f,0.f);
    m_Camera=0;
}

DynamicCharacterWithCam::DynamicCharacterWithCam(PhysicEngine* a_PhysicEngine, IGeometryAccess *a_Geometry)
    :DynamicCharacter(a_PhysicEngine,a_Geometry)
{
    this->setRelativeCamPosition(0.f,0.f,0.f);
    m_Camera=0;
}

void DynamicCharacterWithCam::setCam(ICameraAccess *a_Camera)
{
    m_Camera=a_Camera;
}

void DynamicCharacterWithCam::setRelativeCamPosition(float a_X, float a_Y, float a_Z)
{
    m_RelativeCamPosition[0]=a_X;
    m_RelativeCamPosition[1]=a_Y;
    m_RelativeCamPosition[2]=a_Z;
}

void DynamicCharacterWithCam::setUpDownView(float a_Angle)
{
    this->m_XViewAngle = a_Angle;
}

void DynamicCharacterWithCam::moveCharacter(float a_TimeStep, const unsigned long long a_MoveFlags)
{
    DynamicCharacter::moveCharacter(a_TimeStep,a_MoveFlags);
    if(m_Camera)
    {
        m_Camera->setYAngleTripod(-m_YAngle);
        m_Camera->setXAngleTripod(-m_XViewAngle);
        float *v_MMatrix=m_Geometry->getModelMatrix().data();
        QVector3D v_Trans(m_RelativeCamPosition[0],m_RelativeCamPosition[1],m_RelativeCamPosition[2]);
        QMatrix4x4 v_RotatePos;
        v_RotatePos.rotate(m_YAngle,0.f,1.f,0.f);
        v_Trans=v_RotatePos*v_Trans;
        m_Camera->setTranslationTripod(v_MMatrix[12]+v_Trans.x(),
                v_MMatrix[13]+v_Trans.y(),
                v_MMatrix[14]+v_Trans.z());
    }
}
