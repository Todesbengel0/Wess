#include "sunlight.h"
#include "scenemanager.h"

SunLight::SunLight()
{

}

void SunLight::beforeTraverseChilds(SGObjectVisitor& /*visitor*/)
{
    if (isActive())
    {
        SceneManager::instance()->getActiveContext()->addActiveLight(this);

        getTransformation().activate();

        // Licht ist aktiv => Aktualisiere Richtung im Kamerakoordinatensystem (KKS)
        QMatrix4x4 worldViewMatrix = SceneManager::instance()->getActiveContext()->getDisplayConfiguration()->getViewMatrix();
        worldViewMatrix *= SceneManager::instance()->getActiveContext()->getModelMatrix();

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

