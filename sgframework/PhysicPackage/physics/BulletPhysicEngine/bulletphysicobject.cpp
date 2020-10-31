#include "bulletphysicobject.h"
#include "bulletphysicengine.h"

BulletPhysicObject::BulletPhysicObject(IGeometryAccess *a_Geometry)
    :PhysicObject(a_Geometry),m_MotionState(0),m_RigidBody(0)
{
}

BulletPhysicObject::~BulletPhysicObject()
{
    PhysicObject** a_ConstraintPhysicObjectA=new PhysicObject* [m_ConstraintPairList.size()];
    for(int i=0;i<m_ConstraintPairList.size();i++)
    {
        a_ConstraintPhysicObjectA[i]=m_ConstraintPairList[i].m_PhysicObject;
    }
    for(int i=0,j=m_ConstraintPairList.size();i<j;i++)
    {
        m_PhysicEngine->deleteConstraints(this,a_ConstraintPhysicObjectA[i]);
    }
    delete [] a_ConstraintPhysicObjectA;

    m_PhysicEngine->removePhysicObject(this);

    delete m_MotionState;
    m_MotionState=0;

    if(m_RigidBody)
        delete (m_RigidBody->getCollisionShape());

    delete m_RigidBody;
    m_RigidBody=0;
}

void BulletPhysicObject::addConstraintPair(ConstraintPair a_ConstraintPair)
{
    m_ConstraintPairList.append(a_ConstraintPair);
}

void BulletPhysicObject::clearForces()
{
    if(m_RigidBody)
        m_RigidBody->clearForces();
}

QMatrix4x4 BulletPhysicObject::getEngineModelMatrix()
{
    if(m_RigidBody)
    {
        btTransform vTransformation;
        m_MotionState->getWorldTransform(vTransformation);
        btScalar vMatrix[16];
        vTransformation.getOpenGLMatrix(vMatrix);
        QMatrix4x4 v4x4Matrix((float)vMatrix[0],(float)vMatrix[4],(float)vMatrix[8],(float)vMatrix[12]
                ,(float)vMatrix[1],(float)vMatrix[5],(float)vMatrix[9],(float)vMatrix[13]
                ,(float)vMatrix[2],(float)vMatrix[6],(float)vMatrix[10],(float)vMatrix[14]
                ,(float)vMatrix[3],(float)vMatrix[7],(float)vMatrix[11],(float)vMatrix[15]);
        v4x4Matrix*=this->_hullTransformationInverse;
        return v4x4Matrix;
    }
    qDebug()<<"BulletPhysicObject::getEngineModelMatrix(): No Ridgid Body! Returned Identity Matrix!";
    QMatrix4x4 v_Identity;
    v_Identity.setToIdentity();
    return v_Identity;
}

QVector3D BulletPhysicObject::getForce()
{
    if(m_RigidBody)
    {
        btVector3 v_Force=m_RigidBody->getTotalForce();
        return QVector3D(v_Force.x(),v_Force.y(),v_Force.z());
    }
    return QVector3D();
}

float BulletPhysicObject::getFriction()
{
    if(m_RigidBody)
    {
        return (float)(m_RigidBody->getFriction());
    }
    return 0.f;
}

QVector3D BulletPhysicObject::getLinearVelocity()
{
    if(m_RigidBody)
    {
        btVector3 v_LinearVelocity=m_RigidBody->getLinearVelocity();
        return QVector3D(v_LinearVelocity.x(),v_LinearVelocity.y(),v_LinearVelocity.z());
    }
    return QVector3D();
}

QVector3D BulletPhysicObject::getAngularVelocity()
{
    if(m_RigidBody)
    {
        btVector3 v_angleVelocity=m_RigidBody->getAngularVelocity();
        return QVector3D(v_angleVelocity.x(),v_angleVelocity.y(),v_angleVelocity.z());
    }
    return QVector3D();
}

btMotionState *BulletPhysicObject::getMotionState()
{
    return m_MotionState;
}

float BulletPhysicObject::getRestitution()
{
    if(m_RigidBody)
    {
        return (float)(m_RigidBody->getRestitution());
    }
    return 0.f;
}

btRigidBody *BulletPhysicObject::getRigidBody()
{
    return m_RigidBody;
}

float BulletPhysicObject::getRollingFriction()
{
    if(this->m_RigidBody)
    {
        return (float)(m_RigidBody->getRollingFriction());
    }
    return 0.f;
}

QVector3D BulletPhysicObject::getTorque()
{
    if(m_RigidBody)
    {
        btVector3 v_Force=m_RigidBody->getTotalTorque();
        return QVector3D(v_Force.x(),v_Force.y(),v_Force.z());
    }
    return QVector3D();
}

QList<btTypedConstraint *> BulletPhysicObject::removeConstraintPairs(PhysicObject *a_OtherPhysicObject)
{
    QList<btTypedConstraint*> v_RemovedConstraints;
    for(int i=0,j=m_ConstraintPairList.size();i<j;i++)
    {
        if((m_ConstraintPairList.at(i).m_PhysicObject)==a_OtherPhysicObject)
        {
            QList<ConstraintPair>*v_OtherGeoPairList=&((BulletPhysicObject*)a_OtherPhysicObject)->m_ConstraintPairList;
            for(int k=0,l=v_OtherGeoPairList->size();k<l;k++)
            {
                if(v_OtherGeoPairList->at(k).m_PhysicObject==this)
                {
                    v_OtherGeoPairList->removeAt(k);
                    k-=1;
                    l-=1;
                }
            }
            v_RemovedConstraints.append(m_ConstraintPairList.at(i).m_Constraint);
            m_ConstraintPairList.removeAt(i);
            i-=1;
            j-=1;
        }
    }
    return v_RemovedConstraints;
}

void BulletPhysicObject::setActivation(bool a_IsActive)
{
    if(m_RigidBody)
    {
        if(a_IsActive)
            m_RigidBody->activate();
        else
            m_RigidBody->forceActivationState(WANTS_DEACTIVATION);
    }
}

void BulletPhysicObject::setAlwaysActive(bool a_AlwaysActive)
{
    if(m_RigidBody)
    {
        if(a_AlwaysActive)
        {
            m_RigidBody->forceActivationState(DISABLE_DEACTIVATION);
            m_RigidBody->activate(true);
        }
        else
        {
            m_RigidBody->forceActivationState(WANTS_DEACTIVATION);
            m_RigidBody->activate(true);
        }
    }
}

void BulletPhysicObject::setAngularFactor(QVector3D a_AngularFactor)
{
    if(m_RigidBody)
    {
        btVector3 v_Vector=btVector3((btScalar)(a_AngularFactor.x()),(btScalar)(a_AngularFactor.y()),(btScalar)(a_AngularFactor.z()));
        m_RigidBody->setAngularFactor(v_Vector);
    }
}

void BulletPhysicObject::setAngularVelocity(QVector3D a_Velocity)
{
    if(m_RigidBody)
    {
        btVector3 v_Vector=btVector3((btScalar)(a_Velocity.x()),(btScalar)(a_Velocity.y()),(btScalar)(a_Velocity.z()));
        m_RigidBody->activate(true);
        m_RigidBody->setAngularVelocity(v_Vector);
    }
}

void BulletPhysicObject::setEngineModelMatrix(QMatrix4x4 &a_ModelMatrix)
{
    if(this->m_RigidBody)
    {
        QMatrix4x4 v_MMPlusHullMatrix=a_ModelMatrix*this->_hullTransformation;
        btTransform hullTransformation;
        btTransform massPointTrans;

#ifdef BT_USE_DOUBLE_PRECISION
        btScalar vMatrixScalar[16];
        for( int i=0;i<16;i++)
        {
            vMatrixScalar[i]=(btScalar)v_MMPlusHullMatrix.data()[i];
        }
        hullTransformation.setFromOpenGLMatrix(vMatrixScalar);

        for( int i=0;i<16;i++)
        {
            vMatrixScalar[i]=(btScalar)m_CenterOfMassTransformation.data()[i];
        }
        massPointTrans.setFromOpenGLMatrix(this->m_CenterOfMassTransformation.data());
#else
        hullTransformation.setFromOpenGLMatrix(v_MMPlusHullMatrix.data());
        massPointTrans.setFromOpenGLMatrix(this->m_CenterOfMassTransformation.data());
#endif
        int v_CollisionFlags=m_RigidBody->getCollisionFlags();
        if(v_CollisionFlags & btCollisionObject::CF_KINEMATIC_OBJECT)
        {
            m_MotionState->setWorldTransform(hullTransformation);
            m_RigidBody->activate(true);
        }
        else if(v_CollisionFlags & btCollisionObject::CF_CHARACTER_OBJECT)
        {
            m_MotionState->setWorldTransform(hullTransformation);
            m_RigidBody->setCenterOfMassTransform(hullTransformation*massPointTrans);
            m_RigidBody->activate(true);
        }
        else
        {
            m_MotionState->setWorldTransform(hullTransformation);
            m_RigidBody->setCenterOfMassTransform(hullTransformation*massPointTrans);
        }
    }
}

void BulletPhysicObject::setForce(QVector3D a_Force)
{
    (void)a_Force;
}

void BulletPhysicObject::setFriction(float a_Friction)
{
    if(m_RigidBody)
    {
        if(a_Friction>1.f)
        {
            a_Friction=1.f;
        }
        else if(a_Friction<0.f)
        {
            a_Friction=0.f;
        }
        m_RigidBody->setFriction((btScalar)a_Friction);
    }
}

void BulletPhysicObject::setGravity(QVector3D a_Gravity)
{
    if(m_RigidBody)
    {
        m_RigidBody->setGravity(btVector3((btScalar)(a_Gravity.x()),(btScalar)(a_Gravity.y()),(btScalar)(a_Gravity.z())));
    }
}

void BulletPhysicObject::setLinearFactor(QVector3D a_LinearFactor)
{
    if(m_RigidBody)
    {
        btVector3 v_Vector=btVector3((btScalar)(a_LinearFactor.x()),(btScalar)(a_LinearFactor.y()),(btScalar)(a_LinearFactor.z()));
        m_RigidBody->setLinearFactor(v_Vector);
    }
}

void BulletPhysicObject::setLinearVelocity(QVector3D a_Velocity)
{
    if(m_RigidBody)
    {
        btVector3 v_Vector=btVector3((btScalar)(a_Velocity.x()),(btScalar)(a_Velocity.y()),(btScalar)(a_Velocity.z()));
        m_RigidBody->activate(true);
        m_RigidBody->setLinearVelocity(v_Vector);
    }
}

void BulletPhysicObject::setPhysicState(unsigned int a_PhysicState)
{
    PhysicObject::setPhysicState(a_PhysicState);
}

void BulletPhysicObject::setPhysicType(unsigned int a_PhysicType)
{
    PhysicObject::setPhysicType(a_PhysicType);
}

void BulletPhysicObject::setRestitution(float a_Restitution)
{
    if(m_RigidBody)
    {
        if(a_Restitution>1.f)
        {
            a_Restitution=1.f;
        }
        else if(a_Restitution<0.f)
        {
            a_Restitution=0.f;
        }
        m_RigidBody->setRestitution((btScalar)a_Restitution);
    }
}

void BulletPhysicObject::setRollingFriction(float a_RollingFriction)
{
    if(m_RigidBody)
    {
        if(a_RollingFriction>1.f)
        {
            a_RollingFriction=1.f;
        }
        else if(a_RollingFriction<0.f)
        {
            a_RollingFriction=0.f;
        }
        m_RigidBody->setRollingFriction((btScalar)a_RollingFriction);
    }
}

void BulletPhysicObject::setRigidBody(btRigidBody *a_Rigidbody)
{
    this->m_RigidBody=a_Rigidbody;
}

void BulletPhysicObject::setTorque(QVector3D a_Torque)
{
    (void)a_Torque;
}

void BulletPhysicObject::setMotionState(btMotionState *a_MotionState)
{
    this->m_MotionState=a_MotionState;
}
