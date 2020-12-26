#ifndef PHYSICACCESSABLECAMERA_H
#define PHYSICACCESSABLECAMERA_H

#include "camera.h"
#include "icameraaccess.h"

class PhysicAccessableCamera : public Camera, public ICameraAccess
{

public:
    PhysicAccessableCamera();

    // PhysicEngine Interface Implementierungen
    virtual void setYAngleTripod(float yAngle) override;
    virtual void setXAngleTripod(float xAngle) override;
    virtual void setTranslationTripod(float x, float y, float z) override;

private:
    float mYAngle;
    float mXAngle;
};

#endif // PHYSICACCESSABLECAMERA_H
