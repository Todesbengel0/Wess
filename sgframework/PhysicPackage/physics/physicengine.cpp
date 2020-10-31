#include "physicengine.h"
#include "physicobject.h"

PhysicEngine::PhysicEngine()
{
    m_DebugDrawOn=false;
    m_HasTicked=true;
}

PhysicEngine::~PhysicEngine()
{
}

bool PhysicEngine::hasTicked()
{
    return m_HasTicked;
}

void PhysicEngine::registerPhysicObject(PhysicObject *a_PhysicObject)
{
    PhysicObjectConstructionInfo* v_ConstructionInfo=a_PhysicObject->getConstructionInfo();
    if(!v_ConstructionInfo)
    {
        v_ConstructionInfo=new PhysicObjectConstructionInfo();
        a_PhysicObject->setConstructionInfo(v_ConstructionInfo);
    }
    unsigned long long int v_CollisionHull=v_ConstructionInfo->getCollisionHull();
    switch(v_CollisionHull)
    {
    case CollisionHull::BoxAABB:
        addAsBoxAABB(a_PhysicObject);
        break;
    case CollisionHull::BoxCovarianceFromPoints:
        addAsCovarianceBoxFromPoints(a_PhysicObject);
        break;
    case CollisionHull::BoxHalfExtends:
        addAsCustomBox(a_PhysicObject);
        break;
    case CollisionHull::BVHTriangleMesh:
        addAsBVHTriangleMesh(a_PhysicObject);
        break;
    case CollisionHull::ConeSegmentRadius:
        addAsCone(a_PhysicObject);
        break;
    case CollisionHull::ConvexHull:
        addAsConvexHull(a_PhysicObject);
        break;
    case CollisionHull::Multisphere:
        addAsMultisphere(a_PhysicObject);
        break;
    case CollisionHull::SphereAABB:
        addAsSphereAABB(a_PhysicObject);
        break;
    case CollisionHull::SphereRadius:
        addAsCustomSphere(a_PhysicObject);
        break;
    case CollisionHull::SphereWelzl:
        addAsWelzlSphere(a_PhysicObject);
        break;
    default:
        qDebug()<<"[WARNING]Geometry::registerPhysicObject(): No correct info to register Geometry to PHYSIC_ENGINE! Registration aborted!";
        break;
    }
    //Delete the Construction Info after Rigid Body Construction
    a_PhysicObject->deleteConstructionInfo();
}

void PhysicEngine::setTicked(bool a_HasTicked)
{
    m_HasTicked=a_HasTicked;
}

void PhysicEngine::toggleDebugDrawer(bool a_DebugDrawOn)
{
    m_DebugDrawOn=a_DebugDrawOn;
}
