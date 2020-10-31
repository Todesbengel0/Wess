#ifndef BULLETDYNAMICCHARACTERWITHCAM_H
#define BULLETDYNAMICCHARACTERWITHCAM_H
#include "../dynamiccharacterwithcam.h"

class BulletDynamicCharacterWithCam : public DynamicCharacterWithCam
{
public:
    BulletDynamicCharacterWithCam(int a_PhysicEngineSlot, IGeometryAccess *a_Geometry);
    BulletDynamicCharacterWithCam(PhysicEngine *a_PhysicEngine, IGeometryAccess *a_Geometry);
protected:
    inline virtual bool canStandUp();
    inline virtual bool canJump();
};

#endif // BULLETDYNAMICCHARACTERWITHCAM_H
