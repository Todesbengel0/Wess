#include "canvas.h"

Canvas::Canvas()
{

}

void Canvas::setCanvas(QVector<CanvasData> canvasList)
{
    m_canvasList = canvasList;
}

CanvasData Canvas::getLeftCanvas(CanvasData canvas)
{
    QString leftCanvas = canvas.leftCanvasId;
    if(leftCanvas=="")
    {
        return canvas;
    }
    for(int i = 0; i < m_canvasList.length(); ++i)
    {
        if(leftCanvas == m_canvasList[i].id)
        {
            return m_canvasList[i];
        }
    }
}

CanvasData Canvas::getRightCanvas(CanvasData canvas)
{
    for(int i = 0; i < m_canvasList.length(); ++i)
    {
        QString id1 = canvas.id;
        QString id2 = m_canvasList[i].leftCanvasId;
        if(id1 == id2) //direkter vergleich canvas.id == m_canvasList[i].leftCanvasId funktioniert nicht
        {
            return m_canvasList[i];
        }
    }
    return canvas;
}

void Canvas::setCalculateCorners(CanvasData canvas)
{
    QString id = canvas.id;
    for(int i = 0; i < m_canvasList.length(); ++i) //suche die berechneten Leinwand in der Liste
    {
        if(id == m_canvasList[i].id)
        {
            m_canvasList[i].hasCoordinates=true;
            m_canvasList[i].leftBottom = canvas.leftBottom;
            m_canvasList[i].leftTop = canvas.leftTop;
            m_canvasList[i].rightBottom = canvas.rightBottom;
            return;
        }
    }
}

QVector<QMatrix4x4> Canvas::getProjectionMatrices(Camera* camera, const QVector3D& deltaEyePos, bool stereo)
{
    QVector<QMatrix4x4> projectionMatrices;
    for(int i = m_canvasList.length()-1; i >= 0;--i)
    {
        if(stereo)
        {
            QVector3D eyePositionLeft = camera->getPosition() - deltaEyePos; //linkes Auge
            projectionMatrices.push_back(calculateProjection(i, eyePositionLeft, camera->getNearPlane(), camera->getFarPlane()));

            QVector3D eyePositionRight = camera->getPosition() + deltaEyePos; //rechtes Auge
            projectionMatrices.push_back(calculateProjection(i, eyePositionRight, camera->getNearPlane(), camera->getFarPlane()));
        }
        else
        {
            projectionMatrices.push_back(calculateProjection(i, camera->getPosition(), camera->getNearPlane(), camera->getFarPlane()));
        }
    }
    return projectionMatrices;
}

/*!
 * \brief Canvas::calculateProjection
 * liefert eine Projektionsmatrix zurück, welche in Abhängigkeit von der Augposition und den Display-/Leinwandeckpunkten berechnet wird.
 * Dadurch entsteht der Eindruck eines fließenden Übergangs aus der realen Welt, als würde man durch ein Fenster sehen.
 * \param eyePosition Augposition des Spielers von einer gewählten Raummitte in Meter
 * \param near Entfernung der near-clipping-plane
 * \param far Entfernung der far-clipping-plane
 * \return 4x4 Projektionsmatrix
 */

QMatrix4x4 Canvas::calculateProjection(int index, QVector3D eyePosition, float near, float far)
{
    QVector3D bottomLeft, bottomRight, topLeft;
    QVector3D toRight, toUp, normal;
    float left, right, bottom, top, distance;
    QMatrix4x4 m, projectionsMatrix;
    projectionsMatrix.setToIdentity();

    toRight = m_canvasList[index].rightBottom - m_canvasList[index].leftBottom;
    toUp = m_canvasList[index].leftTop - m_canvasList[index].leftBottom;

    toRight.normalize();
    toUp.normalize();

    normal = QVector3D::crossProduct(toRight,toUp);
    normal.normalize();

    bottomLeft = m_canvasList[index].leftBottom - eyePosition;
    bottomRight = m_canvasList[index].rightBottom - eyePosition;
    topLeft = m_canvasList[index].leftTop - eyePosition;

    distance = -QVector3D::dotProduct(bottomLeft,normal);

    left = QVector3D::dotProduct(toRight,bottomLeft) * near / distance;
    right = QVector3D::dotProduct(toRight,bottomRight) * near / distance;
    bottom = QVector3D::dotProduct(toUp,bottomLeft) * near / distance;
    top = QVector3D::dotProduct(toUp,topLeft) * near / distance;

    projectionsMatrix.frustum(left,right,bottom,top,near,far);

    m.setToIdentity();
    m.setColumn(0,QVector4D(toRight.x(),toUp.x(),normal.x(),0.f));
    m.setColumn(1,QVector4D(toRight.y(),toUp.y(),normal.y(),0.f));
    m.setColumn(2,QVector4D(toRight.z(),toUp.z(),normal.z(),0.f));

   projectionsMatrix = projectionsMatrix * m;
   projectionsMatrix.translate(-eyePosition.x(),-eyePosition.y(),-eyePosition.z());
   return projectionsMatrix;
}
