#ifndef BULLETSPUCOLLISIONDISPATCHER_H
#define BULLETSPUCOLLISIONDISPATCHER_H
#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"

class BulletSpuCollisionDispatcher:public SpuGatheringCollisionDispatcher
{
public:
    BulletSpuCollisionDispatcher(class	btThreadSupportInterface*	threadInterface, unsigned int	maxNumOutstandingTasks,btCollisionConfiguration* collisionConfiguration);
    bool needsCollision(const btCollisionObject *body0, const btCollisionObject *body1);
//    bool needsResponse(const btCollisionObject *body0, const btCollisionObject *body1);
};

#endif // BULLETSPUCOLLISIONDISPATCHER_H
