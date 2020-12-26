#ifndef SGOBJECT_H
#define SGOBJECT_H

class SGObjectVisitor;

//! Basisklasse aller Szenengraph Objekte die einem Node angehängt werden können. \sa Node, Drawable, Transformation
class SGObject
{
public:
    virtual ~SGObject();

protected:
    //! SGObject darf nicht direkt instanziiert werden.
    SGObject();

    //! Wird aufgerufen wenn das Node traversiert wird befor die kinder des Nodes traversiert werden.
    virtual void beforeTraverseChilds(SGObjectVisitor& visitor);

    //! Wird aufgerufen wenn das Node traversiert wird nachdem die kinder des Nodes traversiert wurden.
    virtual void afterTraverseChilds();

private:
    //! Node darf beforeTraverseChilds() sowie afterTraverseChilds() aufrufen
    friend class Node;
};

#endif // SGOBJECT_H
