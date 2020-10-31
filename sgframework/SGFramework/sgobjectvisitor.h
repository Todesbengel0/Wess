#ifndef SGOBJECTVISITOR_H
#define SGOBJECTVISITOR_H

#include "sgobject.h"
#include "drawable.h"

//! Basisklasse der Beobachter (Visitor). Durch die Beobachter wird die Anpassung des Verhaltes beim traversieren des
//! Szenographens (der Nodes) ermöglicht. So kann beispielswiese leicht zwischen Sortiertem und Nicht-Sortiertem
//! Rendering gewechselt werden.
class SGObjectVisitor
{
public:
    virtual ~SGObjectVisitor() = default;

    //! Wird aufgerufen wenn das Node traversiert wird, bevor die kinder des Nodes traversiert werden.
    //! Wird in Drawable::beforeTraverseChilds() aufgerufen. \sa Drawable::beforeTraverseChilds()
    virtual void beforeTraverseChilds(Drawable& drawable);

    //! Wird aufgerufen nachdem im Renderer alle Nodes traversiert wurden. \sa Renderer::renderScene()
    virtual void afterTraverseScene();

protected:
    //! SGObjectVisitor darf nicht direkt instanziiert werden.
    SGObjectVisitor() = default;
};

#endif // SGOBJECTVISITOR_H
