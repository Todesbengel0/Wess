#ifndef DYNAMICCHARACTERWITHCAM_H
#define DYNAMICCHARACTERWITHCAM_H
#include "dynamiccharacter.h"

class ICameraAccess;

/*!
 * \author Sebastian Vesenmayer
 *
 * \brief The DynamicCharacterWithCam class
 *
 * An object of this class is a dynamic character, which has a camera attached that follows it
 */
class DynamicCharacterWithCam :public DynamicCharacter
{
public:
    DynamicCharacterWithCam(int a_PhysicEngineSlot,IGeometryAccess* a_Geometry);
    DynamicCharacterWithCam(PhysicEngine *a_PhysicEngine,IGeometryAccess* a_Geometry);
    //Sets the Camera which follows the character
    virtual void setCam(ICameraAccess *a_Camera);
    //Sets the relative cam Position to the Players Bounding Box
    virtual void setRelativeCamPosition(float a_X,float a_Y,float a_Z);
    //Sets the yaw-Angle (X-Axis) to let the character look up or down without changing move direction
    virtual void setUpDownView(float a_Angle);
    //Overloaded function to move the character and to set current camera position relative to the character
    virtual void moveCharacter(float a_TimeStep, const unsigned long long a_MoveFlags);
protected:
    ICameraAccess* m_Camera;//!< Pointer to Cam
    float m_RelativeCamPosition[3];//!< Relative Position to Cam
};

#endif // DYNAMICCHARACTERWITHCAM_H
