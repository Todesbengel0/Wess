#include "bulletdynamiccharacter.h"

BulletDynamicCharacter::BulletDynamicCharacter(int a_PhysicEngineSlot,IGeometryAccess *a_Geometry)
    :DynamicCharacter(a_PhysicEngineSlot,a_Geometry)
{
}

BulletDynamicCharacter::BulletDynamicCharacter(PhysicEngine* a_PhysicEngine,IGeometryAccess *a_Geometry)
    :DynamicCharacter(a_PhysicEngine,a_Geometry)
{
}

inline bool BulletDynamicCharacter::canJump()
{
    return true;
}

inline bool BulletDynamicCharacter::canStandUp()
{
    return true;
}
