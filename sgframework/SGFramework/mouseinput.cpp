#include "mouseinput.h"
#include "scenemanager.h"
#include "openglwidget.h"

MouseInput::MouseInput() :
    mMouseButtons(0),
    mMousePos(QPoint(0, 0))
{}

const QPoint& MouseInput::getMousePosition()
{
    return mMousePos;
}

QVector3D MouseInput::getRayFromMousePosition()
{
    float width = OpenGLWidget::getInstance()->width();
    float height = OpenGLWidget::getInstance()->height();
    QPoint mousePos = mMousePos;
    float x = ((2.f * mousePos.x()) / width) - 1.f;
    float y = ((2.f * mousePos.y()) / height) - 1.f;

    QVector4D ray = QVector4D(x, y, -1.f, 1.f);

    //Camera* cam = SceneManager::instance()->getActiveContext()->getCamera();
    const QMatrix4x4& projectionMatrix = SceneManager::instance()->getActiveContext()->getDisplayConfiguration()->getProjectionMatrix();
    const QMatrix4x4& viewMatrix = SceneManager::instance()->getActiveContext()->getDisplayConfiguration()->getViewMatrix();

    ray = projectionMatrix.inverted() * ray;
    ray.setW(0.f);
    ray.setZ(-1.f);

    ray = viewMatrix.inverted() * ray;

    return ray.toVector3D().normalized();
}

QVector2D MouseInput::getMouseMove()
{
    QVector2D ret(mMousePos - mMouseOldPos);
    mMouseOldPos = mMousePos; // Move ist "verbraucht" - wird 0
    return (ret);
}

bool MouseInput::isMouseButtonPressed(int mouseButton)
{
    return (mMouseButtons & mouseButton) != 0;
}

int MouseInput::getMouseWheelDelta()
{
    int ret = mMouseWheelDelta;
    mMouseWheelDelta = 0;
    return ret;
}

void MouseInput::setMousePosition(const QPoint& pos)
{
    mMouseOldPos = mMousePos;
    mMousePos = pos;
}

void MouseInput::setMouseButtons(int mouseButtons)
{
    mMouseButtons = mouseButtons;
}

void MouseInput::addMouseWheelDelta(int p)
{
    mMouseWheelDelta += p;
}
