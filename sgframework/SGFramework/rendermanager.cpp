#include "rendermanager.h"
#include "renderingstate.h"

RenderManager::RenderManager()
{
}

void RenderManager::registerPreRenderer(Renderer* renderer)
{
    mPreRendererList.push_back(renderer);
}

void RenderManager::registerRenderer(Renderer* renderer)
{
    mRendererList.push_back(renderer);
}

void RenderManager::registerPostRenderer(Renderer* renderer)
{
    mPostRendererList.push_back(renderer);
}

void RenderManager::renderScene()
{
    RenderingState::getInstance().resetState();

    // pre rendering
    for (int i = 0; i < mPreRendererList.size(); i++)
    {
        if (mPreRendererList.at(i)->isEnabled())
        {
            mPreRendererList.at(i)->renderScene();
        }
    }

    // rendering
    for (int x = 0; x < mRendererList.size(); x++)
    {
        if (mRendererList.at(x)->isEnabled())
        {
            mRendererList.at(x)->renderScene();
        }
    }

    // post rendering
    for (int z = 0; z < mPostRendererList.size(); z++)
    {
        if (mPostRendererList.at(z)->isEnabled())
        {
            mPostRendererList.at(z)->renderScene();
        }
    }
}
