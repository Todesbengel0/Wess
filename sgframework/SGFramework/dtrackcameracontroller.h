#ifndef DTRACKCAMERACONTROLLER_H
#define DTRACKCAMERACONTROLLER_H

#include "cameracontroller.h"

class DTrackCameraController : public CameraController
{
public:
    DTrackCameraController(Camera* camera, QString device);

protected:
    virtual void controlCamera() override;

private:
    QString m_device;

};

#endif // DTRACKCAMERACONTROLLER_H
