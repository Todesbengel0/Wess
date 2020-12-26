#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H
#include <QMatrix4x4>

namespace PhysicState
{
enum PhysicState{
    Static=0,/*Object will not move during runtime*/
    Dynamic=1,/*Object will move freely during runtime*/
    Kinematic=2/*Object can be moved but will not respond to any forces*/
};
}

namespace PhysicType
{
enum PhysicType{
    Object=0,/*Any Object in the Physic World which reacts on forces,collisions and scenegraph transformations*/
    Trigger=1,/*Executes a registered Callback, i.e. Open a Door if you stand in front of it*/
    Player=2/*Object is a Character which is moved by the player or KI*/
};
}

//Collision Groups specify which PhysicObject can collide with another, if the corresponding mask matches an other group it can collider withanother group
//this is important for collactables which will maybe not have an effect on enemies
namespace CollisionGroup
{

enum CollisionGroup{
    #define BIT(n) 1<<(n-1)

    Null=0x0,
    Static=BIT(1),
    Dynamic=BIT(2),
//    Player=BIT(3),
//    Trigger=BIT(4),
//    Constraints=BIT(5),
//    Ball=BIT(6),
//    CraneCube=BIT(7),
//    Cue=BIT(8),
//    Chair=BIT(9),
//    GameControls=BIT(10),
//    GameElements=BIT(11),
//    GameAccel=BIT(12),
//    GameJump=BIT(13),
//    GameFinish=BIT(14)

    #undef BIT
};
}

//Enum for different kind of collision hulls
namespace CollisionHull
{
enum CollisionHull{
    BoxAABB=0,//Axis aligned bounding box from vertices of a geometry
    BoxHalfExtends=1,//Box from Half extends set by the user
    BoxCovarianceFromPoints=2,//Oriented matched box computed by the engine
    SphereAABB=3,//Sphere from AABB from vertices
    SphereRadius=4,//Sphere with radius set by the user
    SphereWelzl=5,//Automatic algorhythm which generates the smallest sphere of an Object (slow)
    CylinderSegmentRadius=6,
    CapsuleSegmentRadius=7,
    ConeSegmentRadius=8,
    Multisphere=9,//Bounding box which consists of multiple spheres ust be set by user
    ConvexHull=10,//AUtomatic generation of convex hull of a mesh, optimal for meshes <100 triangles
    BVHTriangleMesh=11/*Binary Volume Hierarchie Triangle Mesh for Static Environment*/
};
}

class IGeometryAccess;
class ResponseObject;
struct CollisionPointInfo;
class PhysicObjectConstructionInfo;
class PhysicEngine;
class SphereConstructionInfo;
class ShapeConstructionInfo;

//Interface which should describe a rigid body
class PhysicObject
{
    friend class PhysicEngine;
public:
    PhysicObject(IGeometryAccess *a_Geometry);
    virtual ~PhysicObject();

    //adds a registered, removed physic object to its engine again
    virtual void addToPhysicEngine();
    //adds a respones object to the physic engine which calls a callback funtion pointer
    void addResponseObject(ResponseObject *a_Object);

    //clears all forces of the rigid body
    virtual void clearForces()=0;
    void collisionCallback(PhysicObject*& a_CollideWith, QList<CollisionPointInfo> &a_CollPointInfo);

    //deletes the construction info passed by the user, is called autoatically
    void deleteConstructionInfo();

    //gets the center of the mass transformation of an physicobject
    QMatrix4x4 getCenterOfMassTransformation();
    //gives back the collision group which the physic object belongs to
    unsigned long long getCollisionGroup();
    //gives back the collision mask which says with which physic objects group it can collide
    unsigned long long getCollisionMask();
    PhysicObjectConstructionInfo* getConstructionInfo();
    //returns the modelmatrix calculated by the physic engine
    virtual QMatrix4x4 getEngineModelMatrix()=0;
    virtual QVector3D getForce()=0;
    //gets the friction applied for contacts, it reduces the speed of an physicobject when it has contac
    virtual float getFriction()=0;
    IGeometryAccess* getGeometry();
    virtual QVector3D getLinearVelocity()=0;
    virtual QVector3D getAngularVelocity()=0;
    //returns the state static kinematic or dynamic
    unsigned int getPhysicState();
    //returns the type object, trigger, player
    unsigned int getPhysicType();
    //gets the bounciness off an physic object when it is colliding
    virtual float getRestitution()=0;
    //gets the factor how fast the physicobject brakes whenn it is rolling
    virtual float getRollingFriction()=0;
    virtual QVector3D getTorque()=0;

    //register the physic object to its engine, must be called for one time
    void registerPhysicObject();
    //removes it from the engine if it has been registered or added again
    void removeFromPhysicEngine();

    //sets the activation state for an object so it results callbacks again if true or not if false
    virtual void setActivation(bool a_IsActive)=0;
    //sets the activtion state as allways active, so that it produces allways callback by contact
    virtual void setAlwaysActive(bool a_AllwaysActive)=0;
    //sets the multiplication of speed for each axis, which the physicobject can roll
    virtual void setAngularFactor(QVector3D a_AngularFactor)=0;
    //sets the rotaional velocity of an physic object
    virtual void setAngularVelocity(QVector3D a_Velocity)=0;
    //sets the transformation of the mass center of the physic object
    virtual void setCenterOfMassTransformation(QMatrix4x4 &a_ModelMatrix);
    //sets the collision groups the physic object should belong to
    virtual void setCollisionGroup(unsigned long long a_CollisionGroup);
    //sets the mask with which the physic object can collide
    virtual void setCollisionMask(unsigned long long a_CollisionMask);
    //sets the construction info for generating the collision hull
    void setConstructionInfo(PhysicObjectConstructionInfo *a_ConsructionInfo);
    //sets the matrix in the engine
    virtual void setEngineModelMatrix(QMatrix4x4 &a_ModelMatrix)=0;
    //sets the friction of an body how fast it decellarate by contact
    virtual void setFriction(float a_Friction)=0;
    virtual void setForce(QVector3D a_Force)=0;
    //sets the gravity for the physic object
    virtual void setGravity(QVector3D a_Gravity)=0;
    //sets the model matrix for the geometry interface
    virtual void setGeometryModelMatrix(QMatrix4x4 *a_ModelMatrix);
    virtual void setHullTransformation(QMatrix4x4 &modelMatrix);
    //sets the factor of movement for linear axis
    virtual void setLinearFactor(QVector3D a_LinearFactor)=0;
    //sets the linear velocity
    virtual void setLinearVelocity(QVector3D a_Velocity)=0;
    virtual void setPhysicState(unsigned int a_PhysicState);
    virtual void setPhysicType(unsigned int a_PhysicType);
    //sets the bounciness of an object when it collides
    virtual void setRestitution(float a_Restitution)=0;
    //sets the brake factor when the object is rolling on other physicobjects
    virtual void setRollingFriction(float a_RollingFriction)=0;
    virtual void setTorque(QVector3D a_Torque)=0;

protected:
    unsigned long long int m_CollisionGroup;
    unsigned long long int m_CollisionMask;
    PhysicObjectConstructionInfo *m_ConsructionInfo;

    IGeometryAccess *m_Geometry;

    QMatrix4x4 m_CenterOfMassTransformation;
    QMatrix4x4 _centerOfMassTransformationInverse;
    QMatrix4x4 _hullTransformation;
    QMatrix4x4 _hullTransformationInverse;

    PhysicEngine* m_PhysicEngine;
    unsigned int m_PhysicState;
    unsigned int m_PhysicType;

    QList<ResponseObject*> m_ResponseObjectList;
};

//The construction info sets data like collision hull, initial friction, mass point, mass, initial restitution etc.
class PhysicObjectConstructionInfo
{
private:
    QVector3D m_BoxHalfExtends;

    bool m_CcdActive;
    unsigned long long int m_CollisionHull;
    float m_ConeHeigth;
    float m_ConeRadius;

    float m_Friction;

    QVector3D m_LocalInertiaPoint;

    float m_Mass;
    QMatrix4x4 m_MidPointTransformation;
    QList<SphereConstructionInfo> m_MulitSphereInfo;

    float m_Restitution;
    float m_RollingFriction;

    float m_SphereRadius;
public:
    PhysicObjectConstructionInfo();
    virtual ~PhysicObjectConstructionInfo();

    QVector3D getBoxHalfExtends();
    bool getCcdActivation();
    unsigned long long getCollisionHull();
    float getConeHeight();
    float getConeRadius();
    float getFriction();
    QVector3D getLocalInertiaPoint();
    float getMass();
    QMatrix4x4 getMidpointTransformation();
    QList<SphereConstructionInfo> getMultiSphereConstructionInfo();
    float getRestitution();
    float getRollingFriction();
    float getSphereRadius();

    //sets the half extends in xyz for a manual generated bounding box
    void setBoxHalfExtends(QVector3D a_BoxHalfExtends);
    //sets continous collision detection on or off, avoids ghosting if activated
    void setCcdActivation(bool a_CcdActive);
    //sets the collision hull from collision hull enum
    void setCollisionHull(unsigned long long a_CollisionHull);
    //sets the height of a cone
    void setConeHeight(float a_ConeHeight);
    //sets the radius of a cone
    void setConeRadius(float a_ConeRadius);
    //sets the initial friction between physic objects
    void setFriction(float a_Friction);
    //sets the mass point in the object the object rotates arround it
    void setLocalInertiaPoint(QVector3D a_LocalInertiaPoint);
    //sets the mass, should not be set tooo high
    void setMass(float a_Mass);
    //sets a transformation for the midpoint
    void setMidpointTransformation(QMatrix4x4 a_MidpointTransformation);
    //sets the construction info for multisphere
    void setMultiSphereConstructionInfo(QList<SphereConstructionInfo> a_MultiSphereInfo);
    void setRestitution(float a_Restitution);
    void setRollingFriction(float a_RollingFriction);
    //sets the radius of a manual created sphere
    void setSphereRadius(float a_SphereRadius);
};

class ShapeConstructionInfo
{
public:
    ShapeConstructionInfo();
    virtual ~ShapeConstructionInfo();
protected:
    QMatrix4x4 m_MidpointTransformation;
public:
    QMatrix4x4 getMidpointTransformation();

    void setMidpointTransformation(QMatrix4x4 a_MidpointTransformation);
};

class SphereConstructionInfo : public ShapeConstructionInfo
{
public:
    SphereConstructionInfo();
    ~SphereConstructionInfo();
private:
    float m_SphereRadius;
public:
    float getSphereRadius();

    void setSphereRadius(float a_SphereRadius);
};

#endif // PHYSICOBJECT_H
