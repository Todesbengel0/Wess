#ifndef BULLETCOLLISIONDISPATCHER_H
#define BULLETCOLLISIONDISPATCHER_H
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"

class BulletCollisionDispatcher:public btCollisionDispatcher
{
public:
    BulletCollisionDispatcher(btCollisionConfiguration* collisionConfiguration);
    bool needsCollision(const btCollisionObject *body0, const btCollisionObject *body1);
    static bool proveCollision(const btCollisionObject *body0, const btCollisionObject *body1);
};

#endif // BULLETCOLLISIONDISPATCHER_H
