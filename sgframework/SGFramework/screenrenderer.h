#ifndef SCREENRENDERER_H
#define SCREENRENDERER_H

#include "renderer.h"
#include "reshapelistener.h"

//! Einfachster aller Renderer, der in den Default-Buffer rendert und via Reshape die Skalierung anpasst.
class ScreenRenderer : public Renderer, public ReshapeListener
{
public:
    ScreenRenderer(unsigned short contextNr, unsigned short sceneNr, bool sortDrawables = true);

    //! Überschreiben der Methode der Basisklasse: Vorbereiten des Default-Framebuffers und dann Aufruf der Methode der
    //! Basisklasse
    void renderScene() override;

    //! Bei Änderung des Seitenverhältnisses wird der Viewport angepasst. Redefinition aus ReshapeListener
    void reshape(int w, int h) override;
};

#endif // SCREENRENDERER_H
