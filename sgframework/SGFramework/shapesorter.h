#ifndef SHAPESORTER_H
#define SHAPESORTER_H

#include "shape.h"
#include "qmap.h"

//! Basisklasse für alle Sortieralgorithmen welche die inerhalb von ShapeList gesamelten Shapes sortieren.
class ShapeSorter
{
public:
    //! Sortiert die gesamte liste \param shapes neu.
    virtual void sortShapes(QVector<Shape*>& shapes) = 0;

    //! Sortiert alle \param unsorted Shapes in die schon sortierte liste \param sorted ein. Alle in \param unsorted
    //! enhaltenen Shapes dürfen nicht schon in \param sorted enthalten sein.
    virtual void sortShapes(QVector<Shape*>& sorted, QVector<Shape*>& unsorted) = 0;

    //! Sortiert das \param unsorted Shape in die schon sortierte liste \param sorted ein. Das \param unsorted Shape
    //! darf noch nicht in \param sorted enthalten sein.
    virtual void sortShapes(QVector<Shape*>& sorted, Shape* unsorted) = 0;

    //! Berechnet die für das wechseln der Properties wenn dafon ausgegengen wird das die in \param s1 enthaltentn
    //! Properties bereits gebunden sind und die in \param s2 enthaltentn Properties gebunden werden sollen anhand der
    //! Priority der DrawableProperties. Wenn \param s1 null ist wir davon ausgegangen dass noch keine Properties
    //! gebunden sind.
    static int calcCostsBetweenShapes(Shape* s1, Shape* s2);
};

#endif // SHAPESORTER_H
