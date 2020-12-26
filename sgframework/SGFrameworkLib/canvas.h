#ifndef CANVAS_H
#define CANVAS_H
#include "structure.h"
#include "QMatrix4x4"
#include <QDebug>
#include "camera.h"

//! Diese Klasse beinhält alle Displays und dessen Positionierung im Raum.
//! Dazu entsprechend hält sie für jeden Display dessen entsprechende
//! Projektionsmatrix. Weitergehend berechnet diese Klasse auch
//! die Projektionsmatrix sowie die Viewmatrix

class Canvas
{
public:
    Canvas();

    //!Setzen aller vorhandenen Leinwände/Displays
    void setCanvas(QVector <CanvasData> canvasList);

    //!Rückgabe der linken Leinwand/Display
    CanvasData getLeftCanvas(CanvasData canvas);

    //!Rückgabe der rechten Leinwand/Display
    CanvasData getRightCanvas(CanvasData canvas);

    //!Setzen der Koordinaten für eine bestimmte Leinwand/Display
    void setCalculateCorners(CanvasData canvas);

    //!Berechnung und Rückgabe der Projektionsmatrizen
    QVector<QMatrix4x4> getProjectionMatrices(Camera* camera, const QVector3D& deltaEyePos, bool stereo);

private:
    //!Berechnung der Projektionsmatrix
    QMatrix4x4 calculateProjection(int index, QVector3D eyePosition, float near, float far);
    QVector<CanvasData> m_canvasList;
};

#endif // CANVAS_H
