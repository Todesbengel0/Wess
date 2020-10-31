#ifndef INPUTREGISTRY_H
#define INPUTREGISTRY_H

#include "keyboardinput.h"
#include "mouseinput.h"

#ifndef SG_NO_VRPN
#include "trackinginput.h"
#include "flystickinput.h"
#endif


class InputRegistry
{
public:
    /*!
     * \brief getInstance
     * Singleton-Abruf, Thread sicher
     * \return Die Instanz der InputRegistry
     */
    static InputRegistry& getInstance();

    /*!
     * \brief getKeyboardInput
     * Erstellt eine KeyboardInput-Instanz, falls nicht vorhanden, und gibt diese zurück
     * \return Eine KeyboardInput-Instanz
     */
    KeyboardInput* getKeyboardInput();

    /*!
     * \brief getMouseInput
     * Erstellt eine MouseInput-Instanz, falls nicht vorhanden, und gibt diese zurück
     * \return Eine MouseInput-Instanz
     */
    MouseInput* getMouseInput();

#ifndef SG_NO_VRPN
    /*!
     * \brief getVrpnInput
     * Erstellt eine VRPNInput-Instanz, falls nicht vorhanden, und gibt diese zurück
     * \return Eine VRPNInput-Instanz
     */
    TrackingInput* getTrackingInput();

    /*!
     * \brief getFlystickInput
     * Erstellt eine FlystickInput-Instanz, falls nicht vorhanden, und gibt diese zurück
     * \return Eine FlystickInput-Instanz
     */
    FlystickInput* getFlystickInput();

    bool hasFlystickInput();              /*!< Prüft, ob eine FlystickInput-Instanz vorhanden ist */
    bool hasTrackingInput();              /*!< Prüft, ob eine VRPNInput-Instanz vorhanden ist */
#endif

    bool hasKeyboardInput();              /*!< Prüft, ob eine KeyboardInput-Instanz vorhanden ist */
    bool hasMouseInput();                 /*!< Prüft, ob eine MouseInput-Instanz vorhanden ist */
private:
    explicit InputRegistry();
    virtual ~InputRegistry();

    KeyboardInput* m_keyIn;
    MouseInput* m_mouseIn;
#ifndef SG_NO_VRPN
    FlystickInput* m_flyIn;
    TrackingInput* m_trackIn;
#endif
};

#endif // INPUTREGISTRY_H
