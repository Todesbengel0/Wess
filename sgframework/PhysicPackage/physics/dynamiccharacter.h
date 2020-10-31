#ifndef DYNAMICCHARAKTER_H
#define DYNAMICCHARAKTER_H
#include <QVector3D>

class IGeometryAccess;

namespace MovementFlag
{
/*!
 * \brief The MovementFlags enum
 * defines the flags to move an dynamic character in the simulation world
 */
enum MovementFlags
{
    Forward=1<<0,
    Backward=1<<1,
    TurnLeft=1<<2,
    TurnRight=1<<3,
    Jump=1<<4,
    StraveLeft=1<<5,
    StraveRight=1<<6,
    WalkFast=1<<7,
    WalkSlow=1<<8,
    FastTurn=1<<9,
    SlowTurn=1<<10
};
}

class PhysicEngine;

//! Realisiert einen über die Physik-Engine steuerbaren Character
class DynamicCharacter
{
protected:
    DynamicCharacter(int a_PhysicEngineSlot,IGeometryAccess* a_Geometry);
    DynamicCharacter(PhysicEngine* a_PhysicEngine,IGeometryAccess* a_Geometry);
    virtual ~DynamicCharacter();
    virtual bool canStandUp()=0;
    virtual bool canJump()=0;
public:
    virtual IGeometryAccess *getGeometryAccess();
    /*!
     * \brief getPosition3DVector
     * \return returns the position of the character as a 3d vector;
     */
    virtual QVector3D getPosition3DVector();
    /*!
     * \brief getYRotation4x4Matrix
     * \return gets rotation matrix arround the y-axis
     */
    virtual QMatrix4x4 getYRotation4x4Matrix();
    /*!
     * \brief getYRotationDegrees
     * \return get the rotaion of the y-axis in degrees
     */
    virtual float getYRotationDegrees();
protected:
    void initializeCharacter();
    virtual void interpolateSpeeds(const float &a_MaxSpeed, const float &a_TimeStep, const float &a_PositiveNegative, QVector3D a_Direction, QVector3D &a_Velocity);
public:
    /*!
     * \brief lockLinearAxis
     * locks axis so an object cannot move in an axis direction, must be set before it is registered in th ephysic engine
     * \param a_X
     * \param a_Y
     * \param a_Z
     */
    virtual void lockLinearAxis(bool a_X,bool a_Y, bool a_Z);

    /*!
     * \brief moveCharacter
     * moves the character dependend on the time and the set movement flags
     * \param a_TimeStep time in milli seconds
     * \param a_MoveFlags MovementFlag::MovementFlags
     */
    virtual void moveCharacter(float a_TimeStep, const unsigned long long a_MoveFlags);

    //Sets the Accelarationfactor, how fast the character accelarates in every direction
    virtual void setAccelarationFactor(const float a_AccelarationFactor);
    //Sets the factor how fast the character brakes on the ground
    virtual void setBrakeFactor(const float a_BrakeFactor);
    //No functio atm
    virtual void setJumpMoveFactor(const float a_JumpMoveFactor);
    //Sets the speed how fast the player jumps in the air
    virtual void setJumpSpeed(const float a_JumpSpeed);
    virtual void setMaxBackwardNormalSpeed(const float a_BackwardSpeed);
    virtual void setMaxBackwardSlowSpeed(const float a_BackwardSlowSpeed);
    virtual void setMaxBackwardSprintSpeed(const float a_BackwardSprintSpeed);
    virtual void setMaxForwardNormalSpeed(const float a_ForwardSpeed);
    virtual void setMaxForwardSlowSpeed(const float a_ForwardSlowSpeed);
    virtual void setMaxForwardSprintSpeed(const float a_ForwardSprintSpeed);
    virtual void setMaxStraveNormalSpeed(const float a_StraveSpeed);
    virtual void setMaxStraveSlowSpeed(const float a_StraveSlowSpeed);
    virtual void setMaxStraveSprintSpeed(const float a_StraveSprintSpeed);
    virtual void setMaxTurnNormalSpeed(const float a_TurnSpeed);
    virtual void setMaxTurnSlowSpeed(const float a_TurnSlowSpeed);
    virtual void setMaxTurnSprintSpeed(const float a_TurnSprintSpeed);

    //Sets Position and Orientation of the character
    virtual void setPosition(const QMatrix4x4 a_Position);
    virtual void setPosition(const QVector3D a_Position);
    virtual void setYAngle(const float a_YAngle);
protected:
    virtual void setYRotation(const float a_Rotation);

    virtual void updateMoveDirection();
protected:
    bool m_XAxisLocked;
    bool m_YAxisLocked;
    bool m_ZAxisLocked;

    float m_AccelarationFactor; //Factor how Fast Player walks on per ms

    float m_BrakeFactor; //Factor how fast Player decellarate

    QVector3D m_CurrentXVelocity; //Current Velocity on Player x-axis
    QVector3D m_CurrentZVelocity; //Current velocity on players y-axis

    QVector3D m_ForwardVector;//Direction where the Player is looking

    QVector3D m_LeftSideVector;// Vector the Points to the left for strave Calculation

    float m_JumpMoveFactor; //Air Movement Influence per ms
    float m_JumpSpeed; //Jump up Speed

    float m_MaxBackwardSpeed;//Maximum Speed going Backwards
    float m_MaxBackwardSlowSpeed;
    float m_MaxBackwardSprintSpeed;//Maximum  Backward Sprint Speed
    float m_MaxForwardSpeed;//Maximum Speed in Forward Direction
    float m_MaxForwardSlowSpeed;//Slow Forward Speed
    float m_MaxForwardSprintSpeed;//Maximum Forward Sprint Speed
    float m_MaxStraveSpeed;//Maximum Strave Speed
    float m_MaxStraveSlowSpeed;
    float m_MaxStraveSprintSpeed;//Maximum Strave Sprint Speed
    float m_MaxTurnSpeed;//Current Turn Speed
    float m_MaxTurnSlowSpeed; //Factor for Slow Turning
    float m_MaxTurnSprintSpeed;//Maximum Turn Speed in degrees per ms

    float m_YAngle; //Angle Rotation arround the Y-Axis
    float m_XViewAngle; //View Rotation -> does not change move direction

    static const QVector3D s_NegativeXNormal;
    static const QVector3D s_NegativeZNormal;

    IGeometryAccess* m_Geometry;
};

#endif // DYNAMICCHARAKTER_H
