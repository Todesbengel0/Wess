#ifndef BULLETDISCRETEDYNAMICSWORLD_H
#define BULLETDISCRETEDYNAMICSWORLD_H
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include <QThreadPool>
#include <QList>

//#define USE_SINGLETHREAD

class BulletParallelCollisionCallback;
class BulletPhysicEngine;

class BulletDiscreteDynamicsWorld:public btDiscreteDynamicsWorld
{
private:
    QList<BulletParallelCollisionCallback*> m_CallbackThreads;

    int m_NumberOfThreads;
    int m_NumOfManifolds;

    BulletPhysicEngine* m_PhysicEngine;

    QThreadPool m_ThreadPool;
public:
    BulletDiscreteDynamicsWorld(BulletPhysicEngine *a_PhysicEngine, btDispatcher* dispatcher, btBroadphaseInterface* pairCache, btConstraintSolver* constraintSolver, btCollisionConfiguration* collisionConfiguration);
    ~BulletDiscreteDynamicsWorld();
    virtual void internalSingleStepSimulation(btScalar timeStep);
};

#endif // BULLETDISCRETEDYNAMICSWORLD_H
