#include "bulletcollisiondispatcher.h"
#include "bulletphysicobject.h"

BulletCollisionDispatcher::BulletCollisionDispatcher(btCollisionConfiguration *collisionConfiguration)
    :btCollisionDispatcher(collisionConfiguration)
{
}

bool BulletCollisionDispatcher::needsCollision(const btCollisionObject *body0, const btCollisionObject *body1)
{
    bool vNeedsCollision=true;
    vNeedsCollision=btCollisionDispatcher::needsCollision(body0,body1);
    if(vNeedsCollision)
    {
        vNeedsCollision=BulletCollisionDispatcher::proveCollision(body0,body1);
    }
    return vNeedsCollision;
}

bool BulletCollisionDispatcher::proveCollision(const btCollisionObject *body0, const btCollisionObject *body1)
{
    unsigned long long int v_body0group=((BulletPhysicObject*)(body0->getUserPointer()))->getCollisionGroup();
    unsigned long long int v_body1mask=((BulletPhysicObject*)(body1->getUserPointer()))->getCollisionMask();
    if(v_body0group & v_body1mask)
    {
        return true;
    }
    else
    {
        return false;
    }
}
