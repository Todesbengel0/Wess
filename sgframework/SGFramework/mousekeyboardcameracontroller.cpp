#include <QVector2D>

#include "mousekeyboardcameracontroller.h"
#include "inputregistry.h"
#include "openglwidget.h"
#include "scenemanager.h"

MouseKeyboardCameraController::MouseKeyboardCameraController(Camera* camera)
    : KeyboardCameraController(camera), mFollowMouse(false), mRightMouseButtonPressed(false)
{
}

void MouseKeyboardCameraController::controlCamera()
{
    KeyboardCameraController::controlCamera();
    const QVector2D mouseMoveVector = InputRegistry::getInstance().getMouseInput()->getMouseMove();

    if (!OpenGLWidget::getInstance()->geometry().contains(OpenGLWidget::getInstance()->mapFromGlobal(QCursor::pos())))
    {
        // Wenn das Fenster nicht das aktuelle Fenster ist -> Kamerasteuerung abrechen und Maus wieder sichtbar
        // machen.
        mFollowMouse = false;
        OpenGLWidget::getInstance()->setCursor(Qt::ArrowCursor);
        return;
    }

    if (InputRegistry::getInstance().getMouseInput()->isMouseButtonPressed(Qt::RightButton))
    {
        if (!mRightMouseButtonPressed)
        {
            // Rechte maustaste wurde zum ersten mal gedrückt
            mRightMouseButtonPressed = true;
            mFollowMouse = !mFollowMouse;
        }
    }
    else
    {
        mRightMouseButtonPressed = false;
    }

    // Kamerasteuerung ausführen
    if (mFollowMouse || SceneManager::instance()->isInFullScreenMode())
    {
        if (SceneManager::instance()->isInFullScreenMode())
        {
            // Im Vollbild modus mFollowMouse auf false setzen, sodass der Cursor beim Verlassen des
            // Vollbilds wieder sichtbar wird.
            mFollowMouse = false;
        }

        OpenGLWidget& window = *OpenGLWidget::getInstance();
        window.setCursor(Qt::BlankCursor); // Mauszeiger ausblenden
        QRect geometry = window.geometry();
        if (mouseMoveVector.lengthSquared() < geometry.height() * geometry.bottom() / 10.0f)
        {
            // große bewegungen ignorieren da diese meist durch das Zentrieren des Mauszeigers entstehen und
            // nicht durch bewegung der maus.
            mYaw += mouseMoveVector.x() * mRotSpeed;
            mPitch -= mouseMoveVector.y() * mRotSpeed;
            mCamera->setRotation(mYaw, mPitch, 0.f);
        }

        QPoint localCursorPos = window.mapFromGlobal(QCursor::pos());
        // verkleinere das Rechteck auf jeder seite um 50, sodass der Cursor bei schnellen Bewegungen nicht so leicht aus
        // dem Fenster hinaus glitchen kann.
        geometry.adjust(50, 50, -50, -50);
        if (!geometry.contains(localCursorPos))
        {
            // Mauszeiger in die Mitte des Fensters setzen, sodass neue Bewegungen der Maus erfast werden können.
            QCursor::setPos(window.mapToGlobal(geometry.center()));
        }
    }
    else
    {
        OpenGLWidget::getInstance()->setCursor(Qt::ArrowCursor);
    }
}
