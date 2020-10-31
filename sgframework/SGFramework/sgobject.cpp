#include "sgobject.h"
#include "sgobjectvisitor.h"

SGObject::SGObject()
{
}

SGObject::~SGObject()
{
}

void SGObject::beforeTraverseChilds(SGObjectVisitor& /*visitor*/)
{
}

void SGObject::afterTraverseChilds()
{
}
