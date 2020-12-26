#ifndef PREORDERRENDERINGSGOBJECTVISITOR_H
#define PREORDERRENDERINGSGOBJECTVISITOR_H

#include "sgobjectvisitor.h"

//! Implementiert einen SGObjectVisitor, welcher direkt beim Durchlaufen (traversieren) des Szenengraphens rendert.
//! Zusätzlich werden alle Shader, Geometrien und andere Properties eines Drawables immer neu gebunden, unabhängig davon dass das zuvor
//! gerenderte Drawable evtl. bereits dieselbe Property hatte (es wird RenderingState::resetState() pro Drawable aufgerufen).
//! Transparente objekte werden in einem zweiten Durchlauf des Szenengraphens gerendert und ebenfalls nicht sortiert,
//! sodass diese sich gegenseitig falsch überlappen können. Das PreOrder Rendering mit PreOrderRenderingSGObjectVisitor
//! ist langsamer als das sortierte Rendern über SortedRenderingSGObjectVisitor und sollte deshalb nur
//! verwendet werden, wenn die Reihenfolge des Renderns wichtig ist, man Performanzevergleiche durchführt oder Bugs
//! sucht.
class PreOrderRenderingSGObjectVisitor : public SGObjectVisitor
{
public:
    //! Wird aufgerufen wenn der Node traversiert wird bevor die Kinder des Nodes traversiert werden.
    //! Wird in Drawable::beforeTraverseChilds() aufgerufen. \see SGObjectVisitor::beforeTraverseChilds()
    //! Hier wird das Drawable direkt über renderDrawable() gerendert.
    virtual void beforeTraverseChilds(Drawable& drawable) override;

    //! Wird aufgerufen nachdem im Renderer alle Nodes traversiert wurden. \sa Renderer::renderScene()
    //! Hier wird der zweite Durchlauf des Szenengraphens zum rendern von Transparenten Objekten gestartet.
    //! Beim 2. Durchlauf wird wieder beforeTraverseChilds() dismal werden jedoch nur Transparente Objekte gerendert.
    virtual void afterTraverseScene() override;

private:
    //! Uniforms, Shader ... Binden und dei Geometry render.
    void renderDrawable(Drawable& drawable);

    //! Gibt an ob gerade Transparente oder Opake Drawables gerendert werden sollen.
    bool mRenderTransparent = false; // beginne mit opaken objekten
};

#endif // PREORDERRENDERINGSGOBJECTVISITOR_H
