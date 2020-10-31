#include "shadowmaprenderingsgobjectvisitor.h"
#include "scenemanager.h"

void ShadowMapRenderingSGObjectVisitor::afterTraverseScene()
{
    Q_ASSERT(mShadedObjectShader);
    ShapeList& shapeList = SceneManager::instance()->getActiveScene()->getShapeList();
    shapeList.afterTraverse();

    // Zweiter Durchlauf: Opake Objekte rendern
    glFunctions->glDepthMask(GL_TRUE);
    shapeList.renderOpakeGeometries(*mShadedObjectShader);

    // Dritter Durchlauf: Transparente Objekte rendern
    glFunctions->glDepthMask(GL_FALSE);
    shapeList.renderTransparentGeometries(*mShadedObjectShader);
    glFunctions->glDepthMask(GL_TRUE);
}
