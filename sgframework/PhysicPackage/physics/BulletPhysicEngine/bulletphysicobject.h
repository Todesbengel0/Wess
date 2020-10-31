#ifndef BULLETPHYSICOBJECT_H
#define BULLETPHYSICOBJECT_H

#include "btBulletDynamicsCommon.h"
#include <QMatrix4x4>
#include <QList>
#include "../physicobject.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

class BulletPhysicEngine;

struct ConstraintPair
{
    PhysicObject* m_PhysicObject;
    btTypedConstraint* m_Constraint;
};

class BulletPhysicObject : public PhysicObject
{
    friend class BulletPhysicEngine;
private:
    btMotionState* m_MotionState;
    btRigidBody* m_RigidBody;
    QList<ConstraintPair> m_ConstraintPairList;
public:
    BulletPhysicObject(IGeometryAccess* a_Geometry);
    ~BulletPhysicObject();

    void addConstraintPair(ConstraintPair a_ConstraintPair);

    void clearForces();

    QMatrix4x4 getEngineModelMatrix();//TODO
    QVector3D getForce();
    float getFriction();
    QVector3D getLinearVelocity();
    QVector3D getAngularVelocity();
    btMotionState* getMotionState();
    float getRestitution();
    btRigidBody *getRigidBody();
    float getRollingFriction();
    QVector3D getTorque();

    QList<btTypedConstraint*> removeConstraintPairs(PhysicObject *a_OtherPhysicObject);

    void setActivation(bool a_IsActive);
    void setAlwaysActive(bool a_AlwaysActive);
    void setAngularFactor(QVector3D a_AngularFactor);
    void setAngularVelocity(QVector3D a_Velocity);
    void setEngineModelMatrix(QMatrix4x4 &a_ModelMatrix);
    void setForce(QVector3D a_Force);
    void setFriction(float a_Friction);
    void setGravity(QVector3D a_Gravity);
    void setLinearFactor(QVector3D a_LinearFactor);
    void setLinearVelocity(QVector3D a_Velocity);
    void setPhysicState(unsigned int a_PhysicState);
    void setPhysicType(unsigned int a_PhysicType);
    void setRestitution(float a_Restitution);
    void setRollingFriction(float a_RollingFriction);
    void setRigidBody(btRigidBody* a_Rigidbody);
    void setTorque(QVector3D a_Torque);
    void setMotionState(btMotionState* a_MotionState);
};

#endif // BULLETPHYSICOBJECT_H
