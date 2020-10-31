#include "sortedrenderingsgobjectvisitor.h"
#include "scenemanager.h"

void SortedRenderingSGObjectVisitor::beforeTraverseChilds(Drawable& drawable)
{
    ShapeList& shapeList = SceneManager::instance()->getActiveScene()->getShapeList();
    // Anhand des Traverse indexes bestimmen wie oft dieses Drawable innerhalb des Baumes vorhanden ist.
    // Anhand des Index wird hier die entsprechende Anzahl an Shapes erstellt.
    const int traverseIndex = drawable.incrementTraverseIndex();
    ShapeBuffer& shapes = drawable.getShapes();
    if (traverseIndex > shapes.getSize())
    {
        shapes.addAfterCurrent(drawable);
    }

    shapes.moveToNext();
    shapeList.updateDrawableShape(drawable);
}

void SortedRenderingSGObjectVisitor::afterTraverseScene()
{
    ShapeList& shapeList = SceneManager::instance()->getActiveScene()->getShapeList();
    shapeList.afterTraverse();

    // Zweiter Durchlauf: Opake Objekte rendern
    glFunctions->glDepthMask(GL_TRUE);
    shapeList.renderOpakeShapes();

    // Dritter Durchlauf: Transparente Objekte rendern
    glFunctions->glDepthMask(GL_FALSE);
    shapeList.renderTransparentShapes();
    glFunctions->glDepthMask(GL_TRUE);
}
