// Node.cc

#include "node.h"
#include "sgobject.h"
#include <QtGlobal>

void Node::traverse(SGObjectVisitor& visitor)
{
    mSGObject->beforeTraverseChilds(visitor);
    traverseChilds(visitor);
    mSGObject->afterTraverseChilds();
    traverseSiblings(visitor);
}

void Node::traverseChilds(SGObjectVisitor& visitor)
{
    if (mLeftChild)
    {
        mLeftChild->traverse(visitor);
    }
}

void Node::traverseSiblings(SGObjectVisitor& visitor)
{
    if (mRightSibling)
    {
        mRightSibling->traverse(visitor);
    }
}

/*!
 Kinder einfügen. Siblings werden nicht separat eingefügt, sondern beim Einfügen
 eines Kindes werden ggf. bestehende Kinder nach rechts geschoben.
 */
void Node::addChild(Node* child)
{
    if (!mLeftChild)
    {
        mLeftChild = child;
    }
    else
    {
        child->mRightSibling = mLeftChild;
        mLeftChild = child;
    }
}

Node::Node(SGObject* sgObject) : mSGObject(sgObject)
{
    Q_ASSERT_X(sgObject, "Node::Node", "Konnte Node nicht erstellen. SGObject ist ein nullptr.");
}

Node::Node() : mSGObject(new SGObject)
{
}

Node::~Node()
{
    delete mLeftChild;
    mLeftChild = nullptr;
    delete mRightSibling;
    mRightSibling = nullptr;
}
