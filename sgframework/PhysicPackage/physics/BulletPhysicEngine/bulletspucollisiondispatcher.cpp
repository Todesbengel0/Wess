#include "bulletspucollisiondispatcher.h"
#include "bulletcollisiondispatcher.h"

BulletSpuCollisionDispatcher::BulletSpuCollisionDispatcher(btThreadSupportInterface *threadInterface, unsigned int maxNumOutstandingTasks, btCollisionConfiguration *collisionConfiguration)
    :SpuGatheringCollisionDispatcher(threadInterface,maxNumOutstandingTasks,collisionConfiguration)
{
}

bool BulletSpuCollisionDispatcher::needsCollision(const btCollisionObject *body0, const btCollisionObject *body1)
{
    bool vNeedsCollision=true;
    vNeedsCollision=SpuGatheringCollisionDispatcher::needsCollision(body0,body1);
    if(vNeedsCollision)
    {
        vNeedsCollision=BulletCollisionDispatcher::proveCollision(body0,body1);
    }
    return vNeedsCollision;
}

//bool BulletSpuCollisionDispatcher::needsResponse(const btCollisionObject *body0, const btCollisionObject *body1)
//{
//    return false;
//}
