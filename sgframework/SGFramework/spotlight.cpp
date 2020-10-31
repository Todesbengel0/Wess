#include "spotlight.h"
#include "scenemanager.h"

SpotLight::SpotLight(float falloffAngle, float falloffExponent, float linearAttenuation, float quadraticAttenuation) :
    mAttenuations(linearAttenuation, quadraticAttenuation),
    mFalloffAngle(falloffAngle),
    mFalloffExponent(falloffExponent)
{
}

void SpotLight::beforeTraverseChilds(SGObjectVisitor& /*visitor*/)
{
    if (isActive())
    {
        SceneManager::instance()->getActiveContext()->addActiveLight(this);

        getTransformation().activate();

        // Licht ist aktiv => Aktualisiere Position und Richtung im Kamerakoordinatensystem (KKS)
        QMatrix4x4 worldViewMatrix = SceneManager::instance()->getActiveContext()->getDisplayConfiguration()->getViewMatrix();
        worldViewMatrix *= SceneManager::instance()->getActiveContext()->getModelMatrix();

        // Extrahiere Position
        QVector3D viewPosition = worldViewMatrix.column(3).toVector3DAffine();
        if (mViewPosition != viewPosition)
        {
            // View position hat sich seit letztem Frame verändert => Aktualisiere es.
            mViewPosition = viewPosition;
            mChangeFlags.addFlag(ViewPositionChanged);
        }

        // Extrahiere negative Z-Richtung
        QVector3D viewDirection = -worldViewMatrix.column(2).toVector3D();
        viewDirection.normalize();
        if (mViewDirection != viewDirection)
        {
            // View direction hat sich seit letztem Frame verändert => Aktualisiere es.
            mViewDirection = viewDirection;
            mChangeFlags.addFlag(ViewDirectionChanged);
        }

        getTransformation().deActivate();
    }
}

