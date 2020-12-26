#ifndef TRANSPARENTSHAPESORTER_H
#define TRANSPARENTSHAPESORTER_H

#include "shapesorter.h"

//! Implementiert Sortieralgorithmus zur sortierung von Transparenten Shapes anhand ihrer Tiefenwerte im
//! Kammera-Koordinatensystem. Als erstes müssen die am weitesten entfertesten Shapes gerendert werden sodass die näher
//! liegenden Shapes dies überbecken können. Auch würden sich Shapes fälschlicherweise überdecken wenn diese nicht
//! sortiert werden würden da Transparente Objekte ohne aktiven Tiefenbuffer gerendert werden.
class TransparentShapeSorter : public ShapeSorter
{
public:
    //! \see ShapeSorter::sortShapes(QVector<Shape*>& shapes)
    virtual void sortShapes(QVector<Shape*>& shapes) override;

    //! \see ShapeSorter::sortShapes(QVector<Shape*>& shapes, QVector<Shape*>& unsorted)
    virtual void sortShapes(QVector<Shape*>& sorted, QVector<Shape*>& unsorted) override;

    //! \see ShapeSorter::sortShapes(QVector<Shape*>& shapes, Shape* unsorted)
    virtual void sortShapes(QVector<Shape*>& sorted, Shape* unsorted) override;
};

#endif // TRANSPARENTSHAPESORTER_H
