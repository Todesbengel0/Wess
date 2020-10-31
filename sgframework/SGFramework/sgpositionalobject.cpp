#include "sgpositionalobject.h"
#include "sgobjectvisitor.h"
#include "scenemanager.h"

SGPositionalObject::SGPositionalObject()
{

}

void SGPositionalObject::beforeTraverseChilds(SGObjectVisitor& visitor)
{
    mWorldMatrix = SceneManager::instance()->getActiveContext()->getModelMatrix();
}
