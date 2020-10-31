#ifndef TRACKINGINPUT_H
#define TRACKINGINPUT_H

#include <QQuaternion>
#include <QVector3D>


class TrackingInput
{
public:
    TrackingInput();

    void setTrViewDirection(const QQuaternion &pos);  /*!< wird bei Änderung der Trackingbrille-Blickrichtung aufgerufen. */
    void setTrHeadPos(const QVector3D &pos);          /*!< wird bei Änderung der Trackingbrille-Kopfposition aufgerufen. */

    QVector3D getTrHeadPos();                         /*!< gibt Trackingbrille-Kopfposition zurück. */
    QVector3D getTrHeadPosDiff();                     /*!< gibt Differenz aus derzeitiger und vorheriger Tracking-
                                                            brille-Kopfposition zurück. */
    QQuaternion getTrViewDirection();                 /*!< gibt Blickrichtung der Trackingbrille zurück. */
    QQuaternion getTrViewDirectionDiff();             /*!< gibt Differenz der Blickrichtung der Tracking-
                                                            brille zurück. */
    void calculateTRViewDirAngles();

    float getTRViewDirPitchAngle();
    float getTRViewDirRollAngle();
    float getTRViewDirYawAngle();

private:
    QQuaternion mTrViewDirection;                     /*!< Speicher für Blickrichtung der Trackingbrille. */
    QQuaternion mTrViewOldDirection;                  /*!< Speicher für vorherige Blickrichtung der Trackingbrille */
    float mTrViewDirTotalAngles[3]; // yaw, pitch, roll
    bool mTrViewDirectionWasUsed;
    QVector3D mTrHeadPos;                             /*!< Speicher für Trackingbrille-Kopfposition. */
    QVector3D mTrHeadOldPos;                          /*!< Speicher für vorherige Trackingbrille-Kopfposition. */

    float m_pitch;
    float m_roll;
    float m_yaw;

    bool m_hasChanged;

};

#endif // TRACKINGINPUT_H
