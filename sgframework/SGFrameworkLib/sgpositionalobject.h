#ifndef SGPOSITIONALOBJECT_H
#define SGPOSITIONALOBJECT_H
#include "sgobject.h"
#include <QMatrix4x4>

class SGPositionalObject : public SGObject
{
public:
    SGPositionalObject();

    //! Get WorldMatrix. Die WorldMatrix enhält die Transformation eines Positional im Welt-Koordinatensystem. Diese Matrix
    //! wird beim traversieren des Baumes aktualisiert.
    const QMatrix4x4& getWorldMatrix(){return mWorldMatrix;}

protected:
    //! Wird aufgerufen wenn das Node traversiert wird bevor die kinder des Nodes traversiert werden.
    //! Ruft unter anderem SGObjectVisitor::beforeTraverseChilds() auf. \see SGObjectVisitor::beforeTraverseChilds()
    virtual void beforeTraverseChilds(SGObjectVisitor& visitor) override;

    QMatrix4x4 mWorldMatrix;
};

#endif // SGPOSITIONALOBJECT_H
