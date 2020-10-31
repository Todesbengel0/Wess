#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "light.h"
#include <QVector2D>

//! Die PointLight Klasse beschreibt ein Punktlicht.
class PointLight : public Light
{
public:
    //! Dieses enum bestimmt, welche Uniformvariablenwerte sich seit dem letzten Frame verändert haben.
    enum ChangeFlag
    {
        // Enums keine Werte zuweisen. Werden als Bitmaske verwendet
        ViewPositionChanged,
        AttenuationsChanged,

        ChangeFlagCount // Muss immer an letzter Stelle stehen. Bezeichnet Anzahl an change flags
    };

    //! Erzeugt eine Instanz von PointLight mit \param linearAttenuation und \param quadraticAttenuation.
    PointLight(float linearAttenuation = 0.01f, float quadraticAttenuation = 0.001f);

    //! Licht beim Traversieren des Baumes zu aktiven Lichtern hinzufügen (wenn Licht aktiviert ist).
    //! \sa SGObject::beforeTraverseChilds()
    virtual void beforeTraverseChilds(SGObjectVisitor& visitor) override;

    //! Gibt Position des Punktlichts im Kamerakoordinatensystem zurück.
    const QVector3D& getViewPosition() const {return mViewPosition;}

    //! Gibt den linearen sowie quadratischen Abschwächungsfaktor des Punktlichts zurück.
    const QVector2D& getAttenuations() const {return mAttenuations;}

    //! Setzt den linearen Abschwächungsfaktor auf \param linearAttenuation.
    void setLinearAttenuation(float linearAttenuation) {mAttenuations.setX(linearAttenuation);mChangeFlags.addFlag(AttenuationsChanged);}

    //! Setzt den quadratischen Abschwächungsfaktor auf \param quadraticAttenuation.
    void setQuadraticAttenuation(float quadraticAttenuation) {mAttenuations.setY(quadraticAttenuation);mChangeFlags.addFlag(AttenuationsChanged);}

    //! Setzt ChangeFlags zurück.
    void acceptChanges() {Light::acceptChanges();mChangeFlags.resetFlags();}

    //! Gibt true zurück, falls sich Uniformvariablen seit dem Aufruf der Methode acceptChanges() verändert haben, sonst false.
    bool hasChanges() const {return Light::hasChanges() | mChangeFlags.hasAnyFlagSet();}

private:
    QVector3D mViewPosition;
    QVector2D mAttenuations;
    Flags<ChangeFlag> mChangeFlags;
};

//inline const QVector3D& PointLight::getViewPosition() const
//{
//    return mViewPosition;
//}

//inline const QVector2D& PointLight::getAttenuations() const
//{
//    return mAttenuations;
//}

//inline void PointLight::setLinearAttenuation(float linearAttenuation)
//{
//    mAttenuations.setX(linearAttenuation);
//    mChangeFlags.addFlag(AttenuationsChanged);
//}

//inline void PointLight::setQuadraticAttenuation(float quadraticAttenuation)
//{
//    mAttenuations.setY(quadraticAttenuation);
//    mChangeFlags.addFlag(AttenuationsChanged);
//}

//inline void PointLight::acceptChanges()
//{
//    Light::acceptChanges();
//    mChangeFlags.resetFlags();
//}

//inline bool PointLight::hasChanges() const
//{
//    return Light::hasChanges() | mChangeFlags.hasAnyFlagSet();
//}

#endif // POINTLIGHT_H
