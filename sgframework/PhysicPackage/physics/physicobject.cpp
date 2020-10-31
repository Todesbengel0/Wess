#include "physicobject.h"
#include "physics.h"
#include "igeometryaccess.h"
#include "responseobject.h"
#include "collisionpointinfo.h"

PhysicObject::PhysicObject(IGeometryAccess *a_Geometry)
{
    this->m_Geometry=a_Geometry;
    this->m_CenterOfMassTransformation.setToIdentity();
    _hullTransformation.setToIdentity();
    if(a_Geometry->isStaticGeometry())
    {
        this->m_PhysicState=PhysicState::Static;
        this->m_CollisionGroup=CollisionGroup::Static;
        this->m_CollisionMask=CollisionGroup::Dynamic;
    }
    else
    {
        this->m_PhysicState=PhysicState::Dynamic;
        this->m_CollisionGroup=CollisionGroup::Dynamic;
        this->m_CollisionMask=CollisionGroup::Static | CollisionGroup::Dynamic;
    }
    this->m_PhysicType=PhysicType::Object;
    this->m_ConsructionInfo=0;
}

PhysicObject::~PhysicObject()
{
    m_Geometry->setPhysicObject(0);
    m_Geometry=0;
}

void PhysicObject::addToPhysicEngine()
{
    m_PhysicEngine->addPhysicObject(this);
}

void PhysicObject::addResponseObject(ResponseObject* a_Object)
{
    this->m_ResponseObjectList.append(a_Object);
}

void PhysicObject::collisionCallback(PhysicObject *&a_CollideWith, QList<CollisionPointInfo> &a_CollPointInfo)
{
//    If you Use read or write operations on the other Geometry, be sure the methods are threadsafe!
    for(int i=0,j=this->m_ResponseObjectList.size();i<j;i++)
    {
        PhysicObject* vTemp=this;
        this->m_ResponseObjectList.at(i)->triggerObject(vTemp,a_CollideWith,a_CollPointInfo);
    }
}

void PhysicObject::deleteConstructionInfo()
{
    if(m_ConsructionInfo)
    {
        delete m_ConsructionInfo;
        m_ConsructionInfo=0;
    }
}

QMatrix4x4 PhysicObject::getCenterOfMassTransformation()
{
    return this->m_CenterOfMassTransformation;
}

unsigned long long PhysicObject::getCollisionGroup()
{
    return this->m_CollisionGroup;
}

unsigned long long PhysicObject::getCollisionMask()
{
    return this->m_CollisionMask;
}

PhysicObjectConstructionInfo *PhysicObject::getConstructionInfo()
{
    return this->m_ConsructionInfo;
}

IGeometryAccess *PhysicObject::getGeometry()
{
    return this->m_Geometry;//TODO Drawable ersetzen
}

unsigned int PhysicObject::getPhysicState()
{
    return this->m_PhysicState;
}

unsigned int PhysicObject::getPhysicType()
{
    return this->m_PhysicType;
}

void PhysicObject::registerPhysicObject()
{
    m_PhysicEngine->registerPhysicObject(this);
}

void PhysicObject::removeFromPhysicEngine()
{
    m_PhysicEngine->removePhysicObject(this);
}

void PhysicObject::setCenterOfMassTransformation(QMatrix4x4 &a_ModelMatrix)
{
    m_CenterOfMassTransformation=a_ModelMatrix;
    _centerOfMassTransformationInverse=m_CenterOfMassTransformation.inverted();
}

void PhysicObject::setCollisionGroup(unsigned long long a_CollisionGroup)
{
    this->m_CollisionGroup=a_CollisionGroup;
}

void PhysicObject::setCollisionMask(unsigned long long a_CollisionMask)
{
    this->m_CollisionMask=a_CollisionMask;
}

void PhysicObject::setConstructionInfo(PhysicObjectConstructionInfo *a_ConsructionInfo)
{
    this->m_ConsructionInfo=a_ConsructionInfo;
}

/**
  Sets the 4x4 ModelMatrix for Geometry Object which is contained in\n
  the PhysicObject.
  @return void
  @param aModelMatrix The new 4x4 Modelmatrix for Geometry Object.
  */
void PhysicObject::setGeometryModelMatrix(QMatrix4x4 *a_ModelMatrix)
{
    *a_ModelMatrix*=_hullTransformationInverse;
    this->m_Geometry->setModelMatrix(*a_ModelMatrix);
}

void PhysicObject::setHullTransformation(QMatrix4x4 &modelMatrix)
{
    _hullTransformation=modelMatrix;
    _hullTransformationInverse=_hullTransformation.inverted();
}

void PhysicObject::setPhysicState(unsigned int a_PhysicState)
{
    this->m_PhysicState=a_PhysicState;
    switch(a_PhysicState)
    {
    case PhysicState::Static:
        this->m_Geometry->setStaticGeometry(true);
        break;
    default:
        this->m_Geometry->setStaticGeometry(false);
        break;
    }
}

void PhysicObject::setPhysicType(unsigned int a_PhysicType)
{
    this->m_PhysicType=a_PhysicType;
}

PhysicObjectConstructionInfo::PhysicObjectConstructionInfo()
{
    this->m_BoxHalfExtends=QVector3D(1.f,1.f,1.f);
    this->m_CcdActive=false;
    this->m_ConeRadius=1.f;
    this->m_ConeHeigth=1.f;
    this->m_CollisionHull=CollisionHull::BoxAABB;
    this->m_Friction=0.7f;
    this->m_LocalInertiaPoint=QVector3D(0.f,0.f,0.f);
    this->m_Mass=10.f;
    this->m_MidPointTransformation.setToIdentity();
    this->m_Restitution=0.5f;
    this->m_RollingFriction=0.2f;
    this->m_SphereRadius=1.0f;
}

PhysicObjectConstructionInfo::~PhysicObjectConstructionInfo()
{
}

QVector3D PhysicObjectConstructionInfo::getBoxHalfExtends()
{
    return this->m_BoxHalfExtends;
}

bool PhysicObjectConstructionInfo::getCcdActivation()
{
    return this->m_CcdActive;
}

unsigned long long PhysicObjectConstructionInfo::getCollisionHull()
{
    return this->m_CollisionHull;
}

float PhysicObjectConstructionInfo::getConeHeight()
{
    return this->m_ConeHeigth;
}

float PhysicObjectConstructionInfo::getConeRadius()
{
    return this->m_ConeRadius;
}

float PhysicObjectConstructionInfo::getFriction()
{
    return this->m_Friction;
}

QVector3D PhysicObjectConstructionInfo::getLocalInertiaPoint()
{
    return this->m_LocalInertiaPoint;
}

float PhysicObjectConstructionInfo::getMass()
{
    return this->m_Mass;
}

QMatrix4x4 PhysicObjectConstructionInfo::getMidpointTransformation()
{
    return this->m_MidPointTransformation;
}

float PhysicObjectConstructionInfo::getRestitution()
{
    return this->m_Restitution;
}

float PhysicObjectConstructionInfo::getRollingFriction()
{
    return this->m_RollingFriction;
}

float PhysicObjectConstructionInfo::getSphereRadius()
{
    return this->m_SphereRadius;
}

void PhysicObjectConstructionInfo::setBoxHalfExtends(QVector3D a_BoxHalfExtends)
{
    this->m_BoxHalfExtends=a_BoxHalfExtends;
}

void PhysicObjectConstructionInfo::setCcdActivation(bool a_CcdActive)
{
    this->m_CcdActive=a_CcdActive;
}

void PhysicObjectConstructionInfo::setCollisionHull(unsigned long long a_CollisionHull)
{
    this->m_CollisionHull=a_CollisionHull;
}

void PhysicObjectConstructionInfo::setConeHeight(float a_ConeHeight)
{
    this->m_ConeHeigth=a_ConeHeight;
}

void PhysicObjectConstructionInfo::setConeRadius(float a_ConeRadius)
{
    this->m_ConeRadius=a_ConeRadius;
}

void PhysicObjectConstructionInfo::setFriction(float a_Friction)
{
    this->m_Friction=a_Friction;
}

void PhysicObjectConstructionInfo::setLocalInertiaPoint(QVector3D a_LocalInertiaPoint)
{
    this->m_LocalInertiaPoint=a_LocalInertiaPoint;
}

void PhysicObjectConstructionInfo::setMass(float a_Mass)
{
    this->m_Mass=a_Mass;
}

void PhysicObjectConstructionInfo::setMidpointTransformation(QMatrix4x4 a_MidpointTransformation)
{
    this->m_MidPointTransformation=a_MidpointTransformation;
}

void PhysicObjectConstructionInfo::setMultiSphereConstructionInfo(QList<SphereConstructionInfo> a_MultiSphereInfo)
{
    this->m_MulitSphereInfo=a_MultiSphereInfo;
}

void PhysicObjectConstructionInfo::setRestitution(float a_Restitution)
{
    this->m_Restitution=a_Restitution;
}

void PhysicObjectConstructionInfo::setRollingFriction(float a_RollingFriction)
{
    if(a_RollingFriction<=1.f && a_RollingFriction >=0.f)
    {
        this->m_RollingFriction=a_RollingFriction;
    }
}

void PhysicObjectConstructionInfo::setSphereRadius(float a_SphereRadius)
{
    this->m_SphereRadius=a_SphereRadius;
}

ShapeConstructionInfo::ShapeConstructionInfo()
{
    this->m_MidpointTransformation.setToIdentity();
}

ShapeConstructionInfo::~ShapeConstructionInfo()
{
}

QMatrix4x4 ShapeConstructionInfo::getMidpointTransformation()
{
    return this->m_MidpointTransformation;
}

void ShapeConstructionInfo::setMidpointTransformation(QMatrix4x4 a_MidpointTransformation)
{
    this->m_MidpointTransformation=a_MidpointTransformation;
}

SphereConstructionInfo::SphereConstructionInfo()
    :ShapeConstructionInfo()
{
    this->m_SphereRadius=1.0f;
}

SphereConstructionInfo::~SphereConstructionInfo()
{
}

float SphereConstructionInfo::getSphereRadius()
{
    return this->m_SphereRadius;
}

void SphereConstructionInfo::setSphereRadius(float a_SphereRadius)
{
    this->m_SphereRadius=a_SphereRadius;
}

QList<SphereConstructionInfo> PhysicObjectConstructionInfo::getMultiSphereConstructionInfo()
{
    return this->m_MulitSphereInfo;
}
