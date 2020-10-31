#include "screenrenderer.h"
#include "scenemanager.h"
#include "openglwidget.h"
#include "sortedrenderingsgobjectvisitor.h"
#include "preorderrenderingsgobjectvisitor.h"
#include <QDebug>
ScreenRenderer::ScreenRenderer(unsigned short contextNr, unsigned short sceneNr, bool sortDrawables)
    : Renderer(sortDrawables ? static_cast<SGObjectVisitor*>(new SortedRenderingSGObjectVisitor())
                             : static_cast<SGObjectVisitor*>(new PreOrderRenderingSGObjectVisitor()))
{
    mEnabled = true;
    mContextNr = contextNr;
    mSceneNr = sceneNr;

    setAsMainRenderer();
}

void ScreenRenderer::reshape(int w, int h)
{
    ReshapeListener::reshape(w, h);
}


void ScreenRenderer::renderScene()
{
    glFunctions->glBindFramebuffer(GL_FRAMEBUFFER, OpenGLWidget::getInstance()->defaultFramebufferObject());
    glFunctions->glEnable(GL_DEPTH_TEST);
    glFunctions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    Renderer::renderScene();
}

