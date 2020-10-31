#include "scenemanager.h"
#include "renderer.h"
#include "rendermanager.h"
#include <QDebug>
Renderer::Renderer(SGObjectVisitor* sgObjectVisitor) : mSGObjectVisitor(sgObjectVisitor)
{
    Q_ASSERT_X(mSGObjectVisitor, "Renderer::Renderer", "sgObjectVisitor ist null!");
}

void Renderer::renderScene()
{
    mActive = true;
    SceneManager::instance()->setActiveContext(mContextNr);
    SceneManager::instance()->setActiveScene(mSceneNr);
    RenderingContext* activeContext = SceneManager::instance()->getActiveContext();
    Scene* activeScene = SceneManager::instance()->getActiveScene();

    activeContext->update();
    activeContext->setBlending();

    // Erster Durchlauf: Objekte aktualisieren
    activeContext->resetContext();
    do
    {
        activeContext->getDisplayConfiguration()->setViewport();
        activeScene->getRoot()->traverse(*mSGObjectVisitor);
        mSGObjectVisitor->afterTraverseScene();
    }
    while(activeContext->getDisplayConfiguration()->nextViewport());
    activeContext->getDisplayConfiguration()->cleanUp();
    mActive = false;
}

bool Renderer::isActive() const
{
    return mActive;
}

void Renderer::setAsPreRenderer()
{
    SceneManager::instance()->getRenderManager()->registerPreRenderer(this);
}

void Renderer::setAsMainRenderer()
{
    SceneManager::instance()->getRenderManager()->registerRenderer(this);
}

void Renderer::setAsPostRenderer()
{
    SceneManager::instance()->getRenderManager()->registerPostRenderer(this);
}
