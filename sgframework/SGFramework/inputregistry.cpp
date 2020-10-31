#include "inputregistry.h"

InputRegistry::InputRegistry() :
    m_keyIn(nullptr),
    m_mouseIn(nullptr)
#ifndef SG_NO_VRPN
    , m_flyIn(nullptr),
    m_trackIn(nullptr)
#endif
{}

InputRegistry::~InputRegistry()
{
    if (hasKeyboardInput())
        delete m_keyIn;

    if (hasMouseInput())
        delete m_mouseIn;

#ifndef SG_NO_VRPN
    if (hasFlystickInput())
        delete m_flyIn;

    if (hasTrackingInput())
        delete m_trackIn;
#endif
}

InputRegistry& InputRegistry::getInstance()
{
    static InputRegistry instance;
    return instance;
}

KeyboardInput* InputRegistry::getKeyboardInput()
{
    if (!hasKeyboardInput())
        m_keyIn = new KeyboardInput();
    return m_keyIn;
}

MouseInput* InputRegistry::getMouseInput()
{
    if (!hasMouseInput())
        m_mouseIn = new MouseInput();
    return m_mouseIn;
}

#ifndef SG_NO_VRPN
FlystickInput* InputRegistry::getFlystickInput()
{
    if (!hasFlystickInput())
        m_flyIn = new FlystickInput();
    return m_flyIn;
}

bool InputRegistry::hasTrackingInput()
{
    return m_trackIn != nullptr;
}

TrackingInput* InputRegistry::getTrackingInput()
{
    if (!hasTrackingInput())
        m_trackIn = new TrackingInput();
    return m_trackIn;
}

bool InputRegistry::hasFlystickInput()
{
    return m_flyIn != nullptr;
}
#endif

bool InputRegistry::hasKeyboardInput()
{
    return m_keyIn != nullptr;
}

bool InputRegistry::hasMouseInput()
{
    return m_mouseIn != nullptr;
}
