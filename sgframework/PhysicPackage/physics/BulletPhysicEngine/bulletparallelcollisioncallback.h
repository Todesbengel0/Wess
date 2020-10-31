#ifndef BULLETPARALLELCOLLISIONCALLBACK_H
#define BULLETPARALLELCOLLISIONCALLBACK_H
#include <QRunnable>
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include <QVector3D>

class PhysicObject;

class BulletParallelCollisionCallback:public QRunnable
{
private:
    PhysicObject* m_PhysicObjA;
    PhysicObject* m_PhysicObjB;
    btCollisionObject* m_CollisionObjA;
    btCollisionObject* m_CollisionObjB;
    btDispatcher* m_Dispatcher;
    btPersistentManifold  *m_ContactManifold;
    int *m_NumberOfManifolds;
    int m_NumberOfThreads;
    int m_NumberOfContacts;
    int m_ThreadId;
    QVector3D m_CopyVector;
public:
    BulletParallelCollisionCallback(btDispatcher* a_Dispatcher, int *a_NumberOfManifolds, int a_NumberOfThreads, int a_ThreadId);
    void run();
};

#endif // BULLETPARALLELCOLLISIONCALLBACK_H
