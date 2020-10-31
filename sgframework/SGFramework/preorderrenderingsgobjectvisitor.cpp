#include "preorderrenderingsgobjectvisitor.h"

#include "renderingstate.h"
#include "scenemanager.h"

void PreOrderRenderingSGObjectVisitor::beforeTraverseChilds(Drawable& drawable)
{
    if (drawable.isTransparent() == mRenderTransparent)
    {
        renderDrawable(drawable);
    }
}

void PreOrderRenderingSGObjectVisitor::afterTraverseScene()
{
    // zweiter durchlauf starten in dem transparente objekte gerendert werden
    mRenderTransparent = true;
    SceneManager::instance()->getActiveScene()->getRoot()->traverse(*this);

    // zurücksetzen sodass im nächsten frame beim ersten traversieren die opaken objekte gerendert werden
    mRenderTransparent = false;
}

void PreOrderRenderingSGObjectVisitor::renderDrawable(Drawable& drawable)
{
        Shader& shader = *drawable.getShader();
        Geometry& geometry = *drawable.getGeometry();

        RenderingState::getInstance().resetState();
        RenderingState::getInstance().bindDrawable(drawable, shader, geometry);


        QMatrix4x4 lViewMatrix = SceneManager::instance()->getActiveContext()->getDisplayConfiguration()->getViewMatrix();
        // Set the uniform variable on shader program
        ShapeList::bindShapeUniforms(drawable.getWorldMatrix(), lViewMatrix, shader);

        // Rendern
        geometry.render(&drawable);

}
