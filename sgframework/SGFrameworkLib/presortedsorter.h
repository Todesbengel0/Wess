#ifndef PRESORTEDSORTER_H
#define PRESORTEDSORTER_H
#include "shapesorter.h"

class PreSortedSorter : public ShapeSorter
{
public:
    void sortShapes(QVector<Shape*>& shapes) override;
    void sortShapes(QVector<Shape*>& sorted, QVector<Shape*>& unsorted) override;
    void sortShapes(QVector<Shape*>& sorted, Shape* newShape) override;
};

#endif // PRESORTEDSORTER_H
