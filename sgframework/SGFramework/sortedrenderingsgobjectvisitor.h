#ifndef SORTEDRENDERINGSGOBJECTVISITOR_H
#define SORTEDRENDERINGSGOBJECTVISITOR_H

#include "sgobjectvisitor.h"

//! Implementiert einen SGObjectVisitor welcher beim durchlaufen des Szenengraphens Shapes zu einer ShapeList hinzugügt
//! und nach dem durchlaufen des Szenengraphens diese in sortierter reihenfolge Rendert.
class SortedRenderingSGObjectVisitor : public SGObjectVisitor
{
public:
    //! Wird aufgerufen wenn das Node traversiert wird bevor die kinder des Nodes traversiert werden.
    //! Wird in Drawable::beforeTraverseChilds() aufgerufen.
    //! Hier wird die benötigte Anzahl an Shapes zum ShapeBuffer hinzugefügt und ShapeList::updateDrawableShape()
    //! aufgerufen sodass die Shapes zur ShapeList hinzugefügt werden.
    virtual void beforeTraverseChilds(Drawable& drawable) override;

    //! Wird aufgerufen nachdem im Renderer alle Nodes traversiert wurden. \sa Renderer::renderScene()
    //! Rendert die zuvor zur ShapeList hinzugefügten Shapes.
    virtual void afterTraverseScene() override;
};

#endif // SORTEDRENDERINGSGOBJECTVISITOR_H
