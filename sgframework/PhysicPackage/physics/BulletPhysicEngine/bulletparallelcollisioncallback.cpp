#include "bulletparallelcollisioncallback.h"
#include "../collisionpointinfo.h"
#include "../physicobject.h"

#include <QDebug>


BulletParallelCollisionCallback::BulletParallelCollisionCallback(btDispatcher *a_Dispatcher, int *a_NumberOfManifolds, int a_NumberOfThreads,int a_ThreadId)
{
    m_Dispatcher=a_Dispatcher;
    m_NumberOfManifolds=a_NumberOfManifolds;
    m_NumberOfThreads=a_NumberOfThreads;
    m_ThreadId=a_ThreadId;
    m_ContactManifold=0;
    m_NumberOfContacts=0;
    m_PhysicObjA=0;
    m_PhysicObjB=0;
    m_CollisionObjA=0;
    m_CollisionObjB=0;
    setAutoDelete(false);
}

void BulletParallelCollisionCallback::run()
{
    for(int i=m_ThreadId;i<*m_NumberOfManifolds;i+=m_NumberOfThreads)
    {
        m_ContactManifold=m_Dispatcher->getManifoldByIndexInternal(i);
        m_CollisionObjA=const_cast<btCollisionObject*>(m_ContactManifold->getBody0());
        m_CollisionObjB=const_cast<btCollisionObject*>(m_ContactManifold->getBody1());
        if(!m_CollisionObjA->isActive() && !m_CollisionObjB->isActive())
        {
            continue;
        }
        m_NumberOfContacts=m_ContactManifold->getNumContacts();
        QList<CollisionPointInfo> vCollisionInfoListA;
        QList<CollisionPointInfo> vCollisionInfoListB;
        for(int j=0;j<m_NumberOfContacts;j++)
        {
            btManifoldPoint vPoint=m_ContactManifold->getContactPoint(j);
            CollisionPointInfo vCollPointInfo;
            if((vCollPointInfo.m_Distance=(float)(vPoint.m_distance1))<=0.0f)
            {
                vCollPointInfo.m_PositionA=QVector3D(vPoint.m_positionWorldOnA.x(),vPoint.m_positionWorldOnA.y(),vPoint.m_positionWorldOnA.z());
                vCollPointInfo.m_PositionB=QVector3D(vPoint.m_positionWorldOnB.x(),vPoint.m_positionWorldOnB.y(),vPoint.m_positionWorldOnB.z());
                vCollPointInfo.m_NormalOnB=QVector3D(vPoint.m_normalWorldOnB.x(),vPoint.m_normalWorldOnB.y(),vPoint.m_normalWorldOnB.z());
                vCollisionInfoListA.append(vCollPointInfo);
                m_CopyVector=vCollPointInfo.m_PositionB;
                vCollPointInfo.m_PositionB=vCollPointInfo.m_PositionA;
                vCollPointInfo.m_PositionA=m_CopyVector;
                vCollPointInfo.m_NormalOnB*=-1;
                vCollisionInfoListB.append(vCollPointInfo);
            }
        }
        if(!vCollisionInfoListA.isEmpty())
        {
            m_PhysicObjA=static_cast<PhysicObject*>(m_CollisionObjA->getUserPointer());
            m_PhysicObjB=static_cast<PhysicObject*>(m_CollisionObjB->getUserPointer());
            m_PhysicObjA->collisionCallback(m_PhysicObjB,vCollisionInfoListA);
            m_PhysicObjB->collisionCallback(m_PhysicObjA,vCollisionInfoListB);
        }
    }
}
