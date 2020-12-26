#ifndef RENDERER_H
#define RENDERER_H

#include "opengl.h"
#include "renderingcontext.h"
#include "sgobjectvisitor.h"

//! Basisklasse für alle On- und Offscreen-Renderer
//! Es sind Renderer vorgesehen, die vor dem eigentlichen Rendering laufen (z.B. Shadowmaps, Environtment-Maps u.ä.),
//! Hauptrenderer (Screen) sowie Postrenderer, die nach dem eigentlichen Rendering laufen (z.B. Postprocessing-Effekte
//! wie Blur, etc.). Das eigendliche Rendern wird durch einen SGObjectVisitor durchgeführt und kann somit einfach
//! konfiguriert und ausgetauscht werden.
class Renderer
{
public:
    virtual ~Renderer() = default;

    //! Implementiert Defaultrendering mit Szene
    //! Rendering ablauf:
    //! -# Aktiven Renderingkontext und Szene setzen
    //! -# Kamera, Blending setzen und Renderingkontext zurücksetzen
    //! -# Szenengraph Durchlaufen wobei SGObjectVisitor::beforeTraverseChilds() aufgerufen wird
    //! -# SGObjectVisitor::afterTraverseScene() aufrufen.
    virtual void renderScene();

    //! Aktiviert oder Deaktiviert den Renderer. Standardmäsig ist der Renderer Aktiviert.
    void setEnabled(bool value = true) {this->mEnabled = value;}

    //! Gibt an ob der Renderer Aktiviert ist.
    bool isEnabled() {return mEnabled;}

    //! Gibt den RenderingContext des Renderers zurück.
    unsigned short getRendingContext() const {return mContextNr;}

    //! Setzt den RenderingContext.
    void setRendingContext(unsigned short contextNr) {mContextNr = contextNr;}

    //! Setzt die zu Rendernde Szene.
    void setScene(unsigned short sceneNr) {mSceneNr = sceneNr;}

    //! Gibt an ob der Renderer sich aktuell im Renderingvorgan befinden.
    bool isActive() const;

protected:
    //! Gibt an ob der Renderer Aktiviert ist.
    bool mEnabled = true;

    //! Gibt an ob der Renderer sich aktuell im Renderingvorgan befinden.
    bool mActive = false;

    //! RenderingContext des Renderers.
    unsigned short mContextNr;

    //! Die zu Rendernde Szene.
    unsigned short mSceneNr;

    //! Der SGObjectVisitor der beim traversieren des Baumes verwendet wird.
    SGObjectVisitor* mSGObjectVisitor;

    //! Protected Konstrukotr da Renderer eine Basisklasse ist.
    Renderer(SGObjectVisitor* sgObjectVisitor);

    //! Fügt den Renderer als Pre Renderer dem RenderingManager hinzu.
    void setAsPreRenderer();

    //! Fügt den Renderer als Post Renderer dem RenderingManager hinzu.
    void setAsPostRenderer();

    //! Fügt den Renderer als Haupt Renderer dem RenderingManager hinzu.
    void setAsMainRenderer();
};

//inline void Renderer::setEnabled(bool value)
//{
//    this->mEnabled = value;
//}

//inline bool Renderer::isEnabled()
//{
//    return mEnabled;
//}

//inline unsigned short Renderer::getRendingContext() const
//{
//    return mContextNr;
//}

//inline void Renderer::setRendingContext(unsigned short contextNr)
//{
//    mContextNr = contextNr;
//}

//inline void Renderer::setScene(unsigned short sceneNr)
//{
//    mSceneNr = sceneNr;
//}

#endif // RENDERER_H
