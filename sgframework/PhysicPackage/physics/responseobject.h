#ifndef TRIGGERRESPONSEOBJECT_H
#define TRIGGERRESPONSEOBJECT_H

#include <QList>

struct CollisionPointInfo;
class PhysicObject;

//Base class which triggers an callback for a specific object which has the following arguments (PhysicObject* &aPhysicObjectA,PhysicObject* &aPhysicObjectB,QList<CollisionPointInfo> &aCollPointInfoList)
class ResponseObject
{
public:
    ResponseObject(){}
    virtual void triggerObject(PhysicObject* &aPhysicObjectA,PhysicObject* &aPhysicObjectB,QList<CollisionPointInfo> &aCollPointInfoList)=0;
};

//Template class which takes the objects class and a function pointer, which triggerObject should call on the object
template<class T>
class SpecificResponseObject:public ResponseObject
{
    T *mObject;
    void (T::*functionPointer)(PhysicObject *&,PhysicObject *&,QList<CollisionPointInfo> &);
public:
    SpecificResponseObject(T *aObject,void (T::*aFunctionPointer)(PhysicObject *&,PhysicObject *&,QList<CollisionPointInfo> &)):ResponseObject(){
        mObject=aObject;
        functionPointer=aFunctionPointer;
    }
    void triggerObject(PhysicObject* &aPhysicObjectA,PhysicObject* &aPhysicObjectB,QList<CollisionPointInfo> &aCollPointInfoList){
        ((*mObject).*functionPointer)(aPhysicObjectA,aPhysicObjectB,aCollPointInfoList);
    }
};

#endif // TRIGGERRESPONSEOBJECT_H
