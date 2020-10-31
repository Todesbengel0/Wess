#include "dynamiccharacter.h"
#include "physics.h"
#include "igeometryaccess.h"

const QVector3D DynamicCharacter::s_NegativeXNormal=QVector3D(-1.f,0.f,0.f);
const QVector3D DynamicCharacter::s_NegativeZNormal=QVector3D(0.f,0.f,-1.f);

DynamicCharacter::DynamicCharacter(int a_PhysicEngineSlot, IGeometryAccess *a_Geometry)
{
    m_Geometry=a_Geometry;
    PhysicEngineManager::getPhysicEngineBySlot(a_PhysicEngineSlot)->createNewPhysicObject(a_Geometry);
    m_Geometry->getPhysicObject()->setPhysicType(PhysicType::Player);
    m_Geometry->getPhysicObject()->setPhysicState(PhysicState::Dynamic);

    initializeCharacter();
}

DynamicCharacter::DynamicCharacter(PhysicEngine *a_PhysicEngine, IGeometryAccess *a_Geometry)
{
    m_Geometry=a_Geometry;
    a_PhysicEngine->createNewPhysicObject(a_Geometry);
    m_Geometry->getPhysicObject()->setPhysicType(PhysicType::Player);
    m_Geometry->getPhysicObject()->setPhysicState(PhysicState::Dynamic);

    initializeCharacter();
}

DynamicCharacter::~DynamicCharacter()
{
}

IGeometryAccess *DynamicCharacter::getGeometryAccess()
{
    return m_Geometry;
}

QVector3D DynamicCharacter::getPosition3DVector()
{
    float* v_MMatrix=m_Geometry->getModelMatrix().data();
    return QVector3D(v_MMatrix[12],
            v_MMatrix[13],
            v_MMatrix[14]);
}

QMatrix4x4 DynamicCharacter::getYRotation4x4Matrix()
{
    QMatrix4x4 v_RotMatrix= m_Geometry->getModelMatrix();
    float *v_MatrixData=v_RotMatrix.data();
    v_MatrixData[12]=0;
    v_MatrixData[13]=0;
    v_MatrixData[14]=0;
    return v_RotMatrix;
}

float DynamicCharacter::getYRotationDegrees()
{
    return m_YAngle;
}

void DynamicCharacter::initializeCharacter()
{
    m_XAxisLocked=false;
    m_YAxisLocked=false;
    m_ZAxisLocked=false;
    this->m_YAngle=0.f;
    this->m_XViewAngle = 0.f;
    this->m_AccelarationFactor=5.5f/1000.f;
    this->m_BrakeFactor=50.f/1000.f;
    this->m_JumpMoveFactor=0.1f/1000;
    this->m_JumpSpeed=5.f;
    this->m_ForwardVector=QVector3D(0.f,0.f,-1.f);
    this->m_CurrentXVelocity=QVector3D(0.f,0.f,0.f);
    this->m_CurrentZVelocity=QVector3D(0.f,0.f,0.f);
    this->m_MaxForwardSpeed=5.f;
    this->m_MaxForwardSprintSpeed=10.f;
    this->m_MaxBackwardSpeed=2.5f;
    this->m_MaxBackwardSprintSpeed=5.f;
    this->m_MaxStraveSpeed=7.5f;
    this->m_MaxStraveSprintSpeed=15.f;
    this->m_MaxTurnSprintSpeed=180.f/1000.f;
    this->m_MaxForwardSlowSpeed=2.5f;
    this->m_MaxBackwardSlowSpeed=1.25f;
    this->m_MaxStraveSlowSpeed=3.25f;
    this->m_MaxTurnSpeed=90.f/1000.f;
    this->m_MaxTurnSlowSpeed=22.5f/1000.f;
    this->m_LeftSideVector=QVector3D(-1.f,0.f,0.f);
}

void DynamicCharacter::interpolateSpeeds(const float &a_MaxSpeed, const float &a_TimeStep, const float &a_PositiveNegative, QVector3D a_Direction, QVector3D &a_Velocity)
{
    a_Direction*=a_PositiveNegative;
    float v_MoveDirection=QVector3D::dotProduct(a_Velocity,a_Direction);
    float v_VectorLength=a_Velocity.length();
    if(v_MoveDirection>=0)
    {
        if(v_VectorLength>a_MaxSpeed)
        {
            a_Velocity.normalize();
            a_Velocity=a_Velocity*(v_VectorLength-(this->m_BrakeFactor*a_TimeStep)/v_VectorLength);
            v_VectorLength=a_Velocity.length();
            if(v_VectorLength<a_MaxSpeed)
            {
                a_Velocity.normalize();
                a_Velocity=a_Velocity*a_MaxSpeed;
            }
        }
        else
        {
            a_Velocity+=a_TimeStep*this->m_AccelarationFactor*a_Direction*(a_MaxSpeed-v_VectorLength);
            v_VectorLength=a_Velocity.length();
            if(v_VectorLength>a_MaxSpeed)
            {
                a_Velocity.normalize();
                a_Velocity=a_Velocity*a_MaxSpeed;
            }
        }
    }
    else
    {
        QVector3D v_BrakeVelocity=this->m_BrakeFactor*a_TimeStep/v_VectorLength*a_Direction;
        if(v_BrakeVelocity.length()<a_Velocity.length())
        {
            a_Velocity+=v_BrakeVelocity;
        }
        else
        {
            a_Velocity-=a_Velocity;
        }
    }
}

void DynamicCharacter::lockLinearAxis(bool a_X, bool a_Y, bool a_Z)
{
    m_XAxisLocked=a_X;
    m_YAxisLocked=a_Y;
    m_ZAxisLocked=a_Z;
}

void DynamicCharacter::moveCharacter(float a_TimeStep,const unsigned long long a_MoveFlags)
{
    float v_XYZMovement[]={0.f,0.f,0.f};
    QVector3D v_LastVelocity=m_Geometry->getPhysicObject()->getLinearVelocity();
    bool v_CanJump=this->canJump();
    if(a_MoveFlags & MovementFlag::TurnLeft)
    {
        if(~a_MoveFlags & MovementFlag::TurnRight)
        {
            float v_TurnDegrees=this->m_MaxTurnSpeed;
            if(a_MoveFlags & MovementFlag::SlowTurn)
            {
                if(~a_MoveFlags & MovementFlag::FastTurn)
                {
                    v_TurnDegrees=this->m_MaxTurnSlowSpeed;
                }
            }
            else if(a_MoveFlags & MovementFlag::FastTurn)
            {
                    v_TurnDegrees=this->m_MaxTurnSprintSpeed;
            }
            m_YAngle+=v_TurnDegrees*a_TimeStep;
            this->setYRotation(m_YAngle);
        }
    }
    else if(a_MoveFlags & MovementFlag::TurnRight)
    {
        float v_TurnDegrees=this->m_MaxTurnSpeed;
        if(a_MoveFlags & MovementFlag::SlowTurn)
        {
            if(~a_MoveFlags & MovementFlag::FastTurn)
            {
                v_TurnDegrees=this->m_MaxTurnSlowSpeed;
            }
        }
        else if(a_MoveFlags & MovementFlag::FastTurn)
        {
                v_TurnDegrees=this->m_MaxTurnSprintSpeed;
        }
        else
        {
            v_TurnDegrees=this->m_MaxTurnSpeed;
        }
        m_YAngle+=-v_TurnDegrees*a_TimeStep;
        this->setYRotation(m_YAngle);
    }
    if(v_CanJump)
    {
        if(a_MoveFlags & MovementFlag::Forward)
        {
            if(~a_MoveFlags & MovementFlag::Backward)
            {
                if(a_MoveFlags & MovementFlag::WalkFast)
                {
                    if(~a_MoveFlags & MovementFlag::WalkSlow)
                    {
                        interpolateSpeeds(m_MaxForwardSprintSpeed,a_TimeStep,1.f,m_ForwardVector,m_CurrentZVelocity);
                    }
                    else
                    {
                        interpolateSpeeds(m_MaxForwardSpeed,a_TimeStep,1.f,m_ForwardVector,m_CurrentZVelocity);
                    }
                }
                else if (a_MoveFlags & MovementFlag::WalkSlow)
                {
                    interpolateSpeeds(m_MaxForwardSlowSpeed,a_TimeStep,1.f,m_ForwardVector,m_CurrentZVelocity);
                }
                else
                {
                    interpolateSpeeds(m_MaxForwardSpeed,a_TimeStep,1.f,m_ForwardVector,m_CurrentZVelocity);
                }

                if(a_MoveFlags & (MovementFlag::TurnLeft |MovementFlag::TurnRight))
                {
                    v_XYZMovement[0]+=this->m_CurrentZVelocity.x()-(this->m_CurrentZVelocity.x()*0.2f);
                    v_XYZMovement[2]+=this->m_CurrentZVelocity.z()-(this->m_CurrentZVelocity.z()*0.2f);
                }
                else
                {
                    v_XYZMovement[0]+=this->m_CurrentZVelocity.x();
                    v_XYZMovement[2]+=this->m_CurrentZVelocity.z();
                }
            }
        }
        else if(a_MoveFlags & MovementFlag::Backward)
        {
            if(a_MoveFlags & MovementFlag::WalkFast)
            {
                if(~a_MoveFlags & MovementFlag::WalkSlow)
                {
                    interpolateSpeeds(m_MaxBackwardSprintSpeed,a_TimeStep,-1.f,m_ForwardVector,m_CurrentZVelocity);
                }
                else
                {
                    interpolateSpeeds(m_MaxBackwardSpeed,a_TimeStep,-1.f,m_ForwardVector,m_CurrentZVelocity);
                }
            }
            else if (a_MoveFlags & MovementFlag::WalkSlow)
            {
                interpolateSpeeds(m_MaxBackwardSlowSpeed,a_TimeStep,-1.f,m_ForwardVector,m_CurrentZVelocity);
            }
            else
            {
                interpolateSpeeds(m_MaxBackwardSpeed,a_TimeStep,-1.f,m_ForwardVector,m_CurrentZVelocity);
            }
            if(a_MoveFlags & (MovementFlag::TurnLeft |MovementFlag::TurnRight))
            {
                v_XYZMovement[0]+=this->m_CurrentZVelocity.x()-(this->m_CurrentZVelocity.x()*0.2f);
                v_XYZMovement[2]+=this->m_CurrentZVelocity.z()-(this->m_CurrentZVelocity.z()*0.2f);
            }
            else
            {
                v_XYZMovement[0]+=this->m_CurrentZVelocity.x();
                v_XYZMovement[2]+=this->m_CurrentZVelocity.z();
            }
        }
        if(a_MoveFlags & MovementFlag::StraveLeft)
        {
            if(~a_MoveFlags & MovementFlag::StraveRight)
            {
                if(a_MoveFlags & MovementFlag::WalkFast)
                {
                    if(~a_MoveFlags & MovementFlag::WalkSlow)
                    {
                        interpolateSpeeds(m_MaxStraveSprintSpeed,a_TimeStep,1,m_LeftSideVector,m_CurrentXVelocity);
                    }
                    else
                    {
                        interpolateSpeeds(m_MaxStraveSpeed,a_TimeStep,1,m_LeftSideVector,m_CurrentXVelocity);
                    }
                }
                else if (a_MoveFlags & MovementFlag::WalkSlow)
                {
                    interpolateSpeeds(m_MaxStraveSlowSpeed,a_TimeStep,1,m_LeftSideVector,m_CurrentXVelocity);
                }
                else
                {
                    interpolateSpeeds(m_MaxStraveSpeed,a_TimeStep,1,m_LeftSideVector,m_CurrentXVelocity);
                }

                if(a_MoveFlags & (MovementFlag::TurnLeft |MovementFlag::TurnRight))
                {
                    v_XYZMovement[0]+=this->m_CurrentXVelocity.x()-(this->m_CurrentXVelocity.x()*0.2f);
                    v_XYZMovement[2]+=this->m_CurrentXVelocity.z()-(this->m_CurrentXVelocity.z()*0.2f);
                }
                else
                {
                    v_XYZMovement[0]+=this->m_CurrentXVelocity.x();
                    v_XYZMovement[2]+=this->m_CurrentXVelocity.z();
                }
            }
        }
        else if(a_MoveFlags & MovementFlag::StraveRight)
        {
            if(a_MoveFlags & MovementFlag::WalkFast)
            {
                if(~a_MoveFlags & MovementFlag::WalkSlow)
                {
                    interpolateSpeeds(m_MaxStraveSprintSpeed,a_TimeStep,-1,m_LeftSideVector,m_CurrentXVelocity);
                }
                else
                {
                    interpolateSpeeds(m_MaxStraveSpeed,a_TimeStep,-1,m_LeftSideVector,m_CurrentXVelocity);
                }
            }
            else if (a_MoveFlags & MovementFlag::WalkSlow)
            {
                interpolateSpeeds(m_MaxStraveSlowSpeed,a_TimeStep,-1,m_LeftSideVector,m_CurrentXVelocity);
            }
            else
            {
                interpolateSpeeds(m_MaxStraveSpeed,a_TimeStep,-1,m_LeftSideVector,m_CurrentXVelocity);
            }
            if(a_MoveFlags & (MovementFlag::TurnLeft |MovementFlag::TurnRight))
            {
                v_XYZMovement[0]+=this->m_CurrentXVelocity.x()-(this->m_CurrentXVelocity.x()*0.2f);
                v_XYZMovement[2]+=this->m_CurrentXVelocity.z()-(this->m_CurrentXVelocity.z()*0.2f);
            }
            else
            {
                v_XYZMovement[0]+=this->m_CurrentXVelocity.x();
                v_XYZMovement[2]+=this->m_CurrentXVelocity.z();
            }
        }
        if(a_MoveFlags & MovementFlag::Jump)
        {
            v_XYZMovement[1]=this->m_JumpSpeed;
        }
        if(((~a_MoveFlags & MovementFlag::Forward) && (~a_MoveFlags & MovementFlag::Backward)) ||
                ((a_MoveFlags & MovementFlag::Forward) && (a_MoveFlags & MovementFlag::Backward)) )
        {
            float v_MoveDirection=QVector3D::dotProduct(this->m_CurrentZVelocity,this->m_ForwardVector);
            float v_VectorLength=this->m_CurrentZVelocity.length();
            if(v_MoveDirection>0)            {
                QVector3D v_BrakeVelocity=this->m_BrakeFactor*a_TimeStep/v_VectorLength*this->m_ForwardVector;
                if(v_BrakeVelocity.length()<m_CurrentZVelocity.length())
                {
                    this->m_CurrentZVelocity-=v_BrakeVelocity;
                }
                else
                {
                    this->m_CurrentZVelocity-=this->m_CurrentZVelocity;
                }
            }
            else if (v_MoveDirection<0)
            {
                QVector3D v_BrakeVelocity=this->m_BrakeFactor*a_TimeStep/v_VectorLength*this->m_ForwardVector;
                if(v_BrakeVelocity.length()<m_CurrentZVelocity.length())
                {
                    this->m_CurrentZVelocity+=v_BrakeVelocity;
                }
                else
                {
                    this->m_CurrentZVelocity-=this->m_CurrentZVelocity;
                }
            }
            v_XYZMovement[0]+=m_CurrentZVelocity.x();
            v_XYZMovement[2]+=m_CurrentZVelocity.z();
        }
        if(((~a_MoveFlags & MovementFlag::StraveLeft) && (~a_MoveFlags & MovementFlag::StraveRight))||
                ((a_MoveFlags & MovementFlag::StraveLeft) && (a_MoveFlags & MovementFlag::StraveRight)))
        {
            float v_MoveDirection=QVector3D::dotProduct(this->m_CurrentXVelocity,this->m_LeftSideVector);
            float v_VectorLength=this->m_CurrentXVelocity.length();
            if(v_MoveDirection>0)
            {
                QVector3D v_BrakeVelocity=this->m_BrakeFactor*a_TimeStep/v_VectorLength*this->m_LeftSideVector;
                if(v_BrakeVelocity.length()<m_CurrentXVelocity.length())
                {
                    this->m_CurrentXVelocity-=v_BrakeVelocity;
                }
                else
                {
                    this->m_CurrentXVelocity-=this->m_CurrentXVelocity;
                }

            }
            else if (v_MoveDirection<0)
            {
                QVector3D v_BrakeVelocity=this->m_BrakeFactor*a_TimeStep/v_VectorLength*this->m_LeftSideVector;
                if(v_BrakeVelocity.length()<m_CurrentXVelocity.length())
                {
                    this->m_CurrentXVelocity+=v_BrakeVelocity;
                }
                else
                {
                    this->m_CurrentXVelocity-=this->m_CurrentXVelocity;
                }
            }
            v_XYZMovement[0]+=m_CurrentXVelocity.x();
            v_XYZMovement[2]+=m_CurrentXVelocity.z();
        }
        if(!m_XAxisLocked)
            v_LastVelocity.setX(v_XYZMovement[0]);
        if(!m_ZAxisLocked)
            v_LastVelocity.setZ(v_XYZMovement[2]);
        if(v_XYZMovement[1]!=0.f && !m_YAxisLocked)
        {
            v_LastVelocity.setY(v_XYZMovement[1]);
        }
        m_Geometry->getPhysicObject()->setLinearVelocity(v_LastVelocity);
    }
    else
    {

    }
}

void DynamicCharacter::setAccelarationFactor(const float a_AccelarationFactor)
{
    m_AccelarationFactor=a_AccelarationFactor;
}

void DynamicCharacter::setBrakeFactor(const float a_BrakeFactor)
{
    m_BrakeFactor=a_BrakeFactor;
}

void DynamicCharacter::setJumpMoveFactor(const float a_JumpMoveFactor)
{
    m_JumpMoveFactor=a_JumpMoveFactor;
}

void DynamicCharacter::setJumpSpeed(const float a_JumpSpeed)
{
    m_JumpSpeed=a_JumpSpeed;
}

void DynamicCharacter::setMaxBackwardNormalSpeed(const float a_BackwardSpeed)
{
    m_MaxBackwardSpeed=a_BackwardSpeed;
}

void DynamicCharacter::setMaxBackwardSlowSpeed(const float a_BackwardSlowSpeed)
{
    m_MaxBackwardSlowSpeed=a_BackwardSlowSpeed;
}

void DynamicCharacter::setMaxBackwardSprintSpeed(const float a_BackwardSprintSpeed)
{
    m_MaxBackwardSprintSpeed=a_BackwardSprintSpeed;
}

void DynamicCharacter::setMaxForwardNormalSpeed(const float a_ForwardSpeed)
{
    m_MaxForwardSpeed=a_ForwardSpeed;
}

void DynamicCharacter::setMaxForwardSlowSpeed(const float a_ForwardSlowSpeed)
{
    m_MaxForwardSlowSpeed=a_ForwardSlowSpeed;
}

void DynamicCharacter::setMaxForwardSprintSpeed(const float a_ForwardSprintSpeed)
{
    m_MaxForwardSprintSpeed=a_ForwardSprintSpeed;
}

void DynamicCharacter::setMaxStraveNormalSpeed(const float a_StraveSpeed)
{
    m_MaxStraveSpeed=a_StraveSpeed;
}

void DynamicCharacter::setMaxStraveSlowSpeed(const float a_StraveSlowSpeed)
{
    m_MaxStraveSlowSpeed=a_StraveSlowSpeed;
}

void DynamicCharacter::setMaxStraveSprintSpeed(const float a_StraveSprintSpeed)
{
    m_MaxStraveSprintSpeed=a_StraveSprintSpeed;
}

void DynamicCharacter::setMaxTurnNormalSpeed(const float a_TurnSpeed)
{
    m_MaxTurnSpeed=a_TurnSpeed;
}

void DynamicCharacter::setMaxTurnSlowSpeed(const float a_TurnSlowSpeed)
{
    m_MaxTurnSlowSpeed=a_TurnSlowSpeed;
}

void DynamicCharacter::setMaxTurnSprintSpeed(const float a_TurnSprintSpeed)
{
    m_MaxTurnSprintSpeed=a_TurnSprintSpeed;
}

void DynamicCharacter::setPosition(const QMatrix4x4 a_Position)
{
    QMatrix4x4& v_ModelMatrix=m_Geometry->getModelMatrix();
    v_ModelMatrix.setToIdentity();
    const float* v_PosMat=a_Position.data();
    float* v_MMatrix=v_ModelMatrix.data();
    v_MMatrix[12]=v_PosMat[12];
    v_MMatrix[13]=v_PosMat[13];
    v_MMatrix[14]=v_PosMat[14];
    m_Geometry->setModelMatrix(v_ModelMatrix);
}

void DynamicCharacter::setPosition(const QVector3D a_Position)
{
    QMatrix4x4& v_ModelMatrix=m_Geometry->getModelMatrix();
    v_ModelMatrix.setToIdentity();
    float* v_MMatrix=v_ModelMatrix.data();
    v_MMatrix[12]=a_Position.x();
    v_MMatrix[13]=a_Position.y();
    v_MMatrix[14]=a_Position.z();
    m_Geometry->setModelMatrix(v_ModelMatrix);
}

void DynamicCharacter::setYAngle(const float a_YAngle)
{
    m_YAngle=a_YAngle;
}

void DynamicCharacter::setYRotation(const float a_Rotation)
{
    QMatrix4x4& v_ModelMatrix=m_Geometry->getModelMatrix();
    float* v_MMatrix=v_ModelMatrix.data();
    for(int i=0;i<12;i++)
    {
        if(!(i%5))
        {
            v_MMatrix[i]=1;
        }
        else
        {
            v_MMatrix[i]=0;
        }
    }
    v_ModelMatrix.rotate(a_Rotation,0.f,1.f,0.f);
    m_Geometry->setModelMatrix(v_ModelMatrix);
    m_Geometry->getPhysicObject()->setEngineModelMatrix(v_ModelMatrix);
    this->updateMoveDirection();
}

inline void DynamicCharacter::updateMoveDirection()
{
    QMatrix4x4 v_RotationMatrix=getYRotation4x4Matrix();
    float v_OldLength=this->m_CurrentZVelocity.length();
    this->m_ForwardVector=v_RotationMatrix*s_NegativeZNormal;
    this->m_LeftSideVector=v_RotationMatrix*s_NegativeXNormal;
    float v_MoveDirection=QVector3D::dotProduct(this->m_ForwardVector,this->m_CurrentZVelocity);
    if(v_MoveDirection>0)
    {
        this->m_CurrentZVelocity=this->m_ForwardVector*v_OldLength;
    }
    else
    {
        this->m_CurrentZVelocity=-this->m_ForwardVector*v_OldLength;
    }
    v_OldLength=this->m_CurrentXVelocity.length();
    v_MoveDirection=QVector3D::dotProduct(this->m_LeftSideVector,this->m_CurrentXVelocity);
    if(v_MoveDirection>0)
    {
        this->m_CurrentXVelocity=this->m_LeftSideVector*v_OldLength;
    }
    else
    {
        this->m_CurrentXVelocity=-this->m_LeftSideVector*v_OldLength;
    }
}
