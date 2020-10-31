#ifndef MOUSEKEYBOARDCAMERACONTROLLER_H
#define MOUSEKEYBOARDCAMERACONTROLLER_H

#include "keyboardcameracontroller.h"

//! Realisierung einer Kamera, bei der die Blickrichtung über die Bewegung der Maus gesteuert werden kann. Beim 1.
//! Drücken der Rechten Maustaste wird der Mauszeiger ausgeblendet und die Bewegung der maus auf die Kamera übertragen.
//! Ein nochmaliges betätigen der rechten Maustaste macht den Mauszeiger wieder sichtbar und die Kamera wird nicht mehr
//! anhand der Mausbewegung rotiert. Während der Mauszeiger ausgeblendet ist wird er innerhalb des Fensters gehalten.
//! Wenn sich das Fenster im Volbildmodus befindet, wird der Mauszeiger immer ausgeblendet und verfolgt.
class MouseKeyboardCameraController : public KeyboardCameraController
{
public:
    MouseKeyboardCameraController(Camera* camera);

protected:
    virtual void controlCamera() override;

private:
    //! gibt an ob der mauszeiger versteckt, in die mitte zentriert wird und zur rotierund der kammera verwendet wird.
    //! Die variable wird beim drücken der Rechten Maustaste getoggelt.
    bool mFollowMouse = false;

    //! Wird verwendet um zu bestimmen ob die rechte Maustaste zum ersten Mal gedrückt wurde oder ob sie schon zuvor
    //! gedrückt war.
    bool mRightMouseButtonPressed = false;
};

#endif // MOUSEKEYBOARDCAMERACONTROLLER_H
