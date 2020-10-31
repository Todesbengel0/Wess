#ifndef TRIGGER_H
#define TRIGGER_H

class PhysicEngine;
class PhysicObjectConstructionInfo;
class IGeometryAccess;
class ResponseObject;

//Helperclass to realize a trigger
class Trigger
{
public:
    Trigger(int a_PhysicEngineSlot,IGeometryAccess *a_Geometry);
    Trigger(PhysicEngine* a_PhysicEngine,IGeometryAccess *a_Geometry);
    virtual ~Trigger();
    PhysicObjectConstructionInfo* getConstructionInfo();
    void registerTrigger();
    void addResponseObject(ResponseObject *a_ResponseObject);
protected:
    IGeometryAccess* m_Geometry;
};

#endif // TRIGGER_H
