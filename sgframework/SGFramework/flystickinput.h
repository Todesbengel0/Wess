#ifndef FLYSTICKINPUT_H
#define FLYSTICKINPUT_H

#define SG_NO_KEY 0 /*!< Standardwert falls keine Taste gedrückt*/
#define FS_MAX_BUTTONS 7 /*!< Anzahl der Buttons auf Flystick*/

#include <QElapsedTimer>
#include <QPoint>
#include <QQuaternion>
#include <QVector3D>
#include <vector>

class FlystickInput
{
public:
    FlystickInput();

    bool isFsButtonPressed(int button); /*!< prüft, ob Fs-Taste Nr. key (von links) gedrückt wurde. */
    void setFsButtonDown(int button); /*!< wird beim Drücken von Fs-Taste aufgerufen. */
    void setFsButtonUp(int button); /*!< wird beim Loslassen von Fs-Taste aufgerufen. */
    void setFsAnalogstickPos(const QPoint& pos); /*!< wird beim Bewegen des Fs-Analogsticks aufgerufen. */
    void setFsPos(const QVector3D& position); /*!< wird beim Bewegen des Flysticks aufgerufen. */
    void setFsOrientation(const QQuaternion& orientation); /*!< wird beim Bewegen des Flysticks aufgerufen. */

    QPoint getFsAnalogstickPos(); /*!< gibt Position des Fs-Analogsticks zurück. */
    QVector3D getFsPos(); /*!< gibt Position des Flysticks zurück. */
    QVector3D getFsPosDiff();
    QQuaternion getFsOrientation(); /*!< gibt Orientierung des Flysticks als Quaternion zurück. */

    float getFsVelocity(); /*!< gibt aktuelle Geschwindigkeit des Flysticks in Einheiten/s zurück. */
    QVector3D getFsVelVec(); /*!< gibt aktuelle Geschwindigkeit des Flysticks als Vektor zurück. */

    void calculateTRViewDirAngles();

    float getTRViewDirPitchAngle();
    float getTRViewDirRollAngle();
    float getTRViewDirYawAngle();

private:
    QQuaternion mFsOrientation;
    QQuaternion mFsOldOrientation;
    QVector3D mFsPos;
    QVector3D mFsOldPos;

    // -------------zur Geschwindigkeitsberechnung ------------
    QVector3D mFsLastPositions[2];
    int mFsLastPosTime[2];
    bool mFsPosSavedLastTick;
    int mFsLastSavedPos;
    QElapsedTimer mFsTimeElapsed; /*!< Verstrichene Zeit seit Erzeugung der Klasse. */
    // --------------------------------------------------------

    QPoint mFsAnalogstickPos; /*!< Speicher für Fs-Analogstick-Position in PROZENT des
                                 Maximalwertes. */
    QPoint mFsAnalogstickOldPos; /*!< Speicher für vorherige Fs-Analogstick-Position. */
    int mFsButtons[FS_MAX_BUTTONS]; /*!< Speicher für Fs-Buttons. */

    float m_pitch;
    float m_roll;
    float m_yaw;
};

#endif // FLYSTICKINPUT_H
