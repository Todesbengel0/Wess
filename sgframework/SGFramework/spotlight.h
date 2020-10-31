#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "light.h"
#include <QVector2D>

//! Die SpotLight Klasse beschreibt ein Spotlicht.
class SpotLight : public Light
{
public:
    //! Dieses enum bestimmt, welche Uniformvariablenwerte sich seit dem letzten Frame verändert haben.
    enum ChangeFlag
    {
        // Enums keine Werte zuweisen. Werden als Bitmaske verwendet
        ViewPositionChanged,
        ViewDirectionChanged,
        AttenuationsChanged,
        FalloffAngleChanged,
        FalloffExponentChanged,

        ChangeFlagCount // Muss immer an letzter Stelle stehen. Bezeichnet Anzahl an change flags
    };

    //! Erzeugt eine Instanz von SpotLight mit \param falloffAngle, \param falloffExponent, \param linearAttenuation und \param quadraticAttenuation.
    SpotLight(float falloffAngle = 45.0f, float falloffExponent = 0.001f, float linearAttenuation = 0.01f, float quadraticAttenuation = 0.001f);

    //! Licht beim Traversieren des Baumes zu aktiven Lichtern hinzufügen (wenn Licht aktiviert ist).
    //! \sa SGObject::beforeTraverseChilds()
    virtual void beforeTraverseChilds(SGObjectVisitor& visitor) override;

    //! Gibt Position des Spotlichts im Kamerakoordinatensystem zurück.
    const QVector3D& getViewPosition() const {return mViewPosition;}

    //! Gibt Richtung des Spotlichts im Kamerakoordinatensystem zurück.
    const QVector3D& getViewDirection() const {return mViewDirection;}

    //! Gibt den linearen sowie quadratischen Abschwächungsfaktor des Spotlichts zurück.
    const QVector2D& getAttenuations() const {return mAttenuations;}

    //! Gibt den Vignettierungswinkel des Spotlichts zurück.
    float getFalloffAngle() const {return mFalloffAngle;}

    //! Gibt den Vignettierungsexponenten des Spotlichts zurück.
    float getFalloffExponent() const {return mFalloffExponent;}

    //! Setzt den linearen Abschwächungsfaktor auf \param linearAttenuation.
    void setLinearAttenuation(float linearAttenuation) {mAttenuations.setX(linearAttenuation);mChangeFlags.addFlag(AttenuationsChanged);}

    //! Setzt den quadratischen Abschwächungsfaktor auf \param quadraticAttenuation.
    void setQuadraticAttenuation(float quadraticAttenuation) {mAttenuations.setY(quadraticAttenuation);mChangeFlags.addFlag(AttenuationsChanged);}

    //! Setzt den Vignettierungswinkel des Spotlichts auf \param falloffAngle.
    void setFalloffAngle(float falloffAngle) {mFalloffAngle = falloffAngle;mChangeFlags.addFlag(FalloffAngleChanged);}

    //! Setzt den Vignettierungsexponenten des Spotlichts auf \param falloffExponent.
    void setFalloffExponent(float falloffExponent) {mFalloffExponent = falloffExponent;mChangeFlags.addFlag(FalloffExponentChanged);}

    //! Setzt ChangeFlags zurück.
    void acceptChanges() {Light::acceptChanges();mChangeFlags.resetFlags();}

    //! Gibt true zurück, falls sich Uniformvariablen seit dem Aufruf der Methode acceptChanges() verändert haben, sonst false.
    bool hasChanges() const {return Light::hasChanges() | mChangeFlags.hasAnyFlagSet();}

private:
    QVector3D mViewPosition;
    QVector3D mViewDirection;
    QVector2D mAttenuations;
    float mFalloffAngle;
    float mFalloffExponent;
    Flags<ChangeFlag> mChangeFlags;
};

//inline const QVector3D& SpotLight::getViewPosition() const
//{
//    return mViewPosition;
//}

//inline const QVector3D& SpotLight::getViewDirection() const
//{
//    return mViewDirection;
//}

//inline const QVector2D& SpotLight::getAttenuations() const
//{
//    return mAttenuations;
//}

//inline float SpotLight::getFalloffAngle() const
//{
//    return mFalloffAngle;
//}

//inline float SpotLight::getFalloffExponent() const
//{
//    return mFalloffExponent;
//}

//inline void SpotLight::setLinearAttenuation(float linearAttenuation)
//{
//    mAttenuations.setX(linearAttenuation);
//    mChangeFlags.addFlag(AttenuationsChanged);
//}

//inline void SpotLight::setQuadraticAttenuation(float quadraticAttenuation)
//{
//    mAttenuations.setY(quadraticAttenuation);
//    mChangeFlags.addFlag(AttenuationsChanged);
//}

//inline void SpotLight::setFalloffAngle(float falloffAngle)
//{
//    mFalloffAngle = falloffAngle;
//    mChangeFlags.addFlag(FalloffAngleChanged);
//}

//inline void SpotLight::setFalloffExponent(float falloffExponent)
//{
//    mFalloffExponent = falloffExponent;
//    mChangeFlags.addFlag(FalloffExponentChanged);
//}

//inline void SpotLight::acceptChanges()
//{
//    Light::acceptChanges();
//    mChangeFlags.resetFlags();
//}

//inline bool SpotLight::hasChanges() const
//{
//    return Light::hasChanges() | mChangeFlags.hasAnyFlagSet();
//}

#endif // SPOTLIGHT_H
