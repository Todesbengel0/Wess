#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "opengl.h"
#include "renderer.h"

//! Diese Klasse hält die Instanzen der registrierten Renderer und arbeitet diese nacheinander ab.
class RenderManager
{
public:
    RenderManager();
    void registerPreRenderer(Renderer* renderer);
    void registerRenderer(Renderer* renderer);
    void registerPostRenderer(Renderer* renderer);

    //! Durchläuft die Listen der Renderer und stößt deren Renderingprozess an
    void renderScene();

private:
    QVector<Renderer*> mPreRendererList;
    QVector<Renderer*> mRendererList;
    QVector<Renderer*> mPostRendererList;    
};

#endif // RENDERMANAGER_H
