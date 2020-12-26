#ifndef CANVASRENDERCONFIGURATION_H
#define CANVASRENDERCONFIGURATION_H

#include "structure.h"
#include "canvas.h"
#include "renderconfiguration.h"

class CanvasRenderConfiguration : public RenderConfiguration
{
public:
    CanvasRenderConfiguration(bool pDefaultConfig=false, QString pPath=QString());

    //! Setzen des Pfades in dem das XML-File sich befindet
    void setPath(const QString path = "../SGFrameworkLib/cave.xml", const QString caveID = "");

    //!Setzt die gelesen Daten aus dem XML-File
    void setParserData(QVector<CanvasData> canvas);

    CanvasData getOldCanvas() const;

    void setOldCanvas(const CanvasData &oldCanvas);

    //! 3D-Rendering setzen
    void setStereo(const bool stereo = true);

    //!Setzen des Viewports
    virtual void setViewport() override;

    //!Updaten der Projektions- und Viewmatrix
    virtual void update(Camera* camera) override;

    virtual void cleanUp() override;

    void setDeltaEyePosition(const QVector3D& deltaEyePosition);
    QVector3D getDeltaEyePosition() const;

protected:
    //!Fügt alle Leinwände/Monitore hinzu
    void addCanvas(QVector <CanvasData> canvasList);

    //!Berechnet automatisch die benachbarten Leinwände, wenn vorhanden
    void calculateCorners(QVector <CanvasData> canvasList);

    //!Setzt die Anzahl der benötigten Viewports
    void calculateviewportSizeFromCanvasCount();

    Canvas mCanvas;
    CanvasData mOldCanvas;
    QVector <CanvasData> mCanvasData;

    //! Nötig für 3D bei derUnterscheidung ob untere oder oberer Viewport gesetzt wird
    unsigned short mPos;

    //! Angabe ob 3D gerendert werden soll
    bool mStereo;

    QVector3D mDeltaEyePosition;
};

#endif // CANVASRENDERCONFIGURATION_H
