#include "pointlight.h"
#include "scenemanager.h"

PointLight::PointLight(float linearAttenuation, float quadraticAttenuation) :
    mAttenuations(linearAttenuation, quadraticAttenuation)
{
}

void PointLight::beforeTraverseChilds(SGObjectVisitor& /*visitor*/)
{
    if (isActive())
    {
        SceneManager::instance()->getActiveContext()->addActiveLight(this);

        getTransformation().activate();

        // Licht ist aktiv => Aktualisiere Position im Kamerakoordinatensystem (KKS)
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

        getTransformation().deActivate();
    }
}

