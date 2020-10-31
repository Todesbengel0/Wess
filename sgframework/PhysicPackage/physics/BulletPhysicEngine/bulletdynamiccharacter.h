#ifndef BULLETDYNAMICCHARACTER_H
#define BULLETDYNAMICCHARACTER_H
#include "../dynamiccharacter.h"

class BulletDynamicCharacter :public DynamicCharacter
{
public:
    BulletDynamicCharacter(int a_PhysicEngineSlot, IGeometryAccess *a_Geometry);
    BulletDynamicCharacter(PhysicEngine *a_PhysicEngine, IGeometryAccess *a_Geometry);
protected:
    inline bool canJump();
    inline bool canStandUp();
};

#endif // BULLETDYNAMICCHARACTER_H
