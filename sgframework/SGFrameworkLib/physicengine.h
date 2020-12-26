#ifndef PHYSICENGINE_H
#define PHYSICENGINE_H

/*!
  @file physicengine.h
  @author Vesenmayer Sebastian
  @brief This File Contains the Interface for a Physic Engine.
  @details If you want to Use a Physic Engine you must Subclass the\n
  two Classes PhysicEngine and PhysicObject. A Physic Object Represents\n
  an Object in the PhysicEngine and is used to Change things like the ModelMatrix\n
  or the Shape for an Geometric Object in the Szenegraph.
*/

#include <QVector3D>

class DynamicCharacter;
class DynamicCharacterWithCam;
class PhysicEngineManager;
class PhysicObject;
class IGeometryAccess;

class PhysicEngine
{
    friend class PhysicEngineManager;
protected:
    PhysicEngine();
    virtual ~PhysicEngine();

    virtual void addAsBoxAABB(PhysicObject *a_PhysicObject)=0;
    virtual void addAsBVHTriangleMesh(PhysicObject *a_PhysicObject)=0;
    virtual void addAsCone(PhysicObject *a_PhysicObject)=0;
    virtual void addAsConvexHull(PhysicObject *a_PhysicObject)=0;
    virtual void addAsCovarianceBoxFromPoints(PhysicObject *a_PhysicObject)=0;
    virtual void addAsCustomBox(PhysicObject *a_PhysicObject)=0;
    virtual void addAsCustomSphere(PhysicObject *a_PhysicObject)=0;
    virtual void addAsMultisphere(PhysicObject *a_PhysicObject)=0;   
    virtual void addAsSphereAABB(PhysicObject *a_PhysicObject)=0;
    virtual void addAsWelzlSphere(PhysicObject *a_PhysicObject)=0;
public:
    //Adds a hinge between two PhysicObjects with a given Point in each and an axis, and maximum angles and other parameters
    virtual void addHingeConstraint(PhysicObject* a_PhysicObjectA,PhysicObject* a_PhysicObjectB,
                                    QVector3D a_PivotInA,QVector3D a_PivotInB,
                                    QVector3D a_AxisInA,QVector3D a_AxisInB,
                                    float a_LowLimit,float a_HighLimit,float a_Softness=0.9f,float a_BiasFactor=0.3f,float a_RelaxationFactor=1.f)=0;
    //adds a physicobject, which was registered and removed before
    virtual void addPhysicObject(PhysicObject *a_PhysicObject)=0;
    //Adds a Point to Point connection between two physicObjects
    virtual void addPointToPointConstraint(PhysicObject *a_PhysicObjectA, QVector3D a_PivotInsideA, PhysicObject *a_PhysicObjectB, QVector3D a_PivotInsideB)=0;

    //Creates a new Dynamic Character
    virtual DynamicCharacter* createNewDynamicCharacter(IGeometryAccess *a_Geometry)=0;
    //Creates a Dynamic Character which can hold a following cam
    virtual DynamicCharacterWithCam* createNewDynamicCharacterWithCam(IGeometryAccess *a_Geometry)=0;
    //Creates a new PhysicObject for a given Geometry
    virtual PhysicObject* createNewPhysicObject(IGeometryAccess *a_Geometry)=0;

    virtual void debugDraw()=0;

    //Returns true if engine did an internal tick
    virtual bool hasTicked();

    //pauses the physic simulation
    virtual void pauseSimulation()=0;

    //does a raytest from a start point to an end point and gives back the closest physicobject
    virtual PhysicObject* rayTestClosestBody(QVector3D a_From, QVector3D a_To)=0;
    //does a ray test from a start point to an end point and gives back a list of all physicobjects which touches the ray
    virtual QList<PhysicObject*> rayTestAllBodys(QVector3D a_From, QVector3D a_To)=0;
    //registers a given physiobject to the physicengine, has to be called once for initalization
    void registerPhysicObject(PhysicObject* a_PhysicObject);
    //Deletes a Connection between two PhysicObjects
    virtual void deleteConstraints(PhysicObject* a_PhysicObjectA,PhysicObject* a_PhysicObjectB)=0;
    //removes a Physicobject from the Physicengine, can be added per addPhysicObject again
    virtual void removePhysicObject(PhysicObject *a_PhysicObject)=0;
    //resumes the physic simulation if it was paused
    virtual void resumeSimulation()=0;

    virtual void setGravityForNewObjects(QVector3D a_Gravity)=0;
    virtual void setGravityForAllObjects(QVector3D a_Gravity)=0;
    virtual void setTicked(bool a_HasTicked=true);
    //no effect atm
    virtual void setFrequency(float a_Frequency)=0;

    //ticks the engine for th given time
    virtual void tick(int timeElapsedMS)=0;
    //sets if the matrices have to be  updated, is called by the physic engine
    virtual void updateMatrix()=0;
    //toggles the debug drawer on and off
    void toggleDebugDrawer(bool a_DebugDrawOn);

protected:
    bool m_DebugDrawOn;
    bool m_HasTicked;
};

#endif // PHYSICENGINE_H
