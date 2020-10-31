#include "trigger.h"
#include "igeometryaccess.h"
#include "physics.h"

void initPhysicObject(PhysicObject* a_PhysicObject){
    a_PhysicObject->setPhysicType(PhysicType::Trigger);
    a_PhysicObject->setPhysicState(PhysicState::Static);
    PhysicObjectConstructionInfo* v_ConstructionInfo=new PhysicObjectConstructionInfo();
    v_ConstructionInfo->setBoxHalfExtends(QVector3D(0.5f,0.5f,0.5f));
    v_ConstructionInfo->setCollisionHull(CollisionHull::BoxHalfExtends);
    a_PhysicObject->setConstructionInfo(v_ConstructionInfo);
}

Trigger::Trigger(int a_PhysicEngineSlot, IGeometryAccess *a_Geometry)
{
    m_Geometry=a_Geometry;
    PhysicEngine* v_PhysicEngine=PhysicEngineManager::getPhysicEngineBySlot(a_PhysicEngineSlot);
    PhysicObject* v_PhysicObject=v_PhysicEngine->createNewPhysicObject(m_Geometry);
    initPhysicObject(v_PhysicObject);
}

Trigger::Trigger(PhysicEngine *a_PhysicEngine, IGeometryAccess *a_Geometry)
{
    m_Geometry=a_Geometry;
    PhysicObject* v_PhysicObject=a_PhysicEngine->createNewPhysicObject(m_Geometry);
    initPhysicObject(v_PhysicObject);
}

Trigger::~Trigger()
{
}

PhysicObjectConstructionInfo *Trigger::getConstructionInfo()
{
    return m_Geometry->getPhysicObject()->getConstructionInfo();
}

void Trigger::registerTrigger()
{
    m_Geometry->getPhysicObject()->registerPhysicObject();
}

void Trigger::addResponseObject(ResponseObject *a_ResponseObject)
{
    m_Geometry->getPhysicObject()->addResponseObject(a_ResponseObject);
}

