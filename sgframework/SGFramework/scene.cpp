#include "scene.h"
#include "scenemanager.h"

Scene::Scene(Node* pRoot) : mRoot(pRoot), mShapeList(SceneManager::instance()->getShapeListForRoot(pRoot))
{
    Q_ASSERT_X(mRoot, "Scene::Scene", "Root-Node ist null!");
}
