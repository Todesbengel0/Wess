#include "transparentshapesorter.h"
#include <QtMath>

void TransparentShapeSorter::sortShapes(QVector<Shape*>& shapes)
{
    // std::stable_sort sowie qFuzzyCompare wird benötigt da ansonsten objekte anhand von floating point Fehlern jedes
    // Frame unterschiedlich sortiert werden können und somit Flakern würden.
    std::stable_sort(shapes.begin(), shapes.end(), [](Shape* left, Shape* right)
                     {
                         float first = left->getModelViewMatrix().constData()[14];
                         float second = right->getModelViewMatrix().constData()[14];

                         return first < second && !qFuzzyCompare(first, second);
                     });
}

void TransparentShapeSorter::sortShapes(QVector<Shape*>& sorted, QVector<Shape*>& unsorted)
{
    sorted.append(unsorted);
    sortShapes(sorted);
}

void TransparentShapeSorter::sortShapes(QVector<Shape*>& sorted, Shape* unsorted)
{
    sorted.append(unsorted);
    sortShapes(sorted);
}
