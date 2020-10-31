#ifndef SUNLIGHT_H
#define SUNLIGHT_H

#include "light.h"

//! Die SunLight Klasse beschreibt ein Sonnenlicht.
class SunLight : public Light
{
public:
    //! Dieses enum bestimmt, welche Uniformvariablenwerte sich seit dem letzten Frame verändert haben.
    enum ChangeFlag
    {
        // Enums keine Werte zuweisen. Werden als Bitmaske verwendet
        ViewDirectionChanged,

        ChangeFlagCount // Muss immer an letzter Stelle stehen. Bezeichnet Anzahl an change flags
    };

    //! Erzeugt eine Instanz von SunLight
    SunLight();

    //! Licht beim Traversieren des Baumes zu aktiven Lichtern hinzufügen (wenn Licht aktiviert ist).
    //! \sa SGObject::beforeTraverseChilds()
    virtual void beforeTraverseChilds(SGObjectVisitor& visitor) override;

    //! Gibt die Richtung des Sonnenlichts im Kamerakoordinatensystem zurück.
    const QVector3D& getViewDirection() const {return mViewDirection;}

    //! Setzt ChangeFlags zurück.
    void acceptChanges() {Light::acceptChanges();mChangeFlags.resetFlags();}

    //! Gibt true zurück, falls sich Uniformvariablen seit dem Aufruf der Methode acceptChanges() verändert haben, sonst false.
    bool hasChanges() const {return Light::hasChanges() | mChangeFlags.hasAnyFlagSet();}

private:
    QVector3D mViewDirection;
    Flags<ChangeFlag> mChangeFlags;
};

//inline const QVector3D& SunLight::getViewDirection() const
//{
//    return mViewDirection;
//}

//inline void SunLight::acceptChanges()
//{
//    Light::acceptChanges();
//    mChangeFlags.resetFlags();
//}

//inline bool SunLight::hasChanges() const
//{
//    return Light::hasChanges() | mChangeFlags.hasAnyFlagSet();
//}

#endif // SUNLIGHT_H
