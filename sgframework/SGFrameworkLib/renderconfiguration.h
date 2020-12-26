#ifndef RENDERCONFIGURATION_H
#define RENDERCONFIGURATION_H

#include "reshapelistener.h"
#include "camera.h"
#include "opengl.h"

//! Diese Klasse berechnet die Positionierung
//! und Konfiguration (stereo...) der Ziele für das Rendering
//! Das können Displays im Raum sein. Oder im Prerendering Texturen (FBOs).
//! Dazu beinhält die Klasse die Viewports, sowie zu den einzelnen
//! Viewports die Projektions- und Viewmatrizen

class RenderConfiguration : public ReshapeListener
{
public:
    RenderConfiguration();

    //!Berechnung und Rückgabe der ViewMatrix
    QMatrix4x4 getViewMatrix();

    //!Rückgabe der Projektionsmatrix für den aktuellen Viewport
    QMatrix4x4 getProjectionMatrix();

    //!Geht alle Viewports durch
    bool nextViewport();

    //!Setzen des Viewports
    virtual void setViewport();

    //!Updaten der Projektions- und Viewmatrix
    virtual void update(Camera* camera);

    //! Änderung der Größe des Ausgabefensters
    virtual void reshape(int w, int h);

    virtual void cleanUp();

protected:
    //! Aktueller Viewport
    unsigned short mCurrentViewport;
    //! Anzahl der Viewports
    unsigned short mViewportCount;

    QVector<QMatrix4x4> mProjectionsMatrices;
    QMatrix4x4 mViewMatrix;
    bool mWindowSizeChanged;
};

#endif // RENDERCONFIGURATION_H
