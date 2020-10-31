#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

#include <QVector3D>
#include <QString>

//! Diese Klasse generiert zwei Datenstrukturen um eine kompaktere
//! Übergabe von Parametern zu gewährleisten

struct CanvasData
{
        QString id;
        QString leftCanvasId;
        QVector3D leftBottom;
        QVector3D rightBottom;
        QVector3D leftTop;
        float width;
        float height;
        float angle;
        bool hasCoordinates;
};

#endif // STRUCTURE_H
