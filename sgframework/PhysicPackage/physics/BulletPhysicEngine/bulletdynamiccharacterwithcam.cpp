#include "bulletdynamiccharacterwithcam.h"

BulletDynamicCharacterWithCam::BulletDynamicCharacterWithCam(int a_PhysicEngineSlot,IGeometryAccess *a_Geometry)
    :DynamicCharacterWithCam(a_PhysicEngineSlot, a_Geometry)
{
}

BulletDynamicCharacterWithCam::BulletDynamicCharacterWithCam(PhysicEngine* a_PhysicEngine,IGeometryAccess *a_Geometry)
    :DynamicCharacterWithCam(a_PhysicEngine,a_Geometry)
{
}

bool BulletDynamicCharacterWithCam::canStandUp()
{
    return true;
}

bool BulletDynamicCharacterWithCam::canJump()
{
    return true;
}
