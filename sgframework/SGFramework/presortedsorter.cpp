#include "presortedsorter.h"
#include "QVector"

void PreSortedSorter::sortShapes(QVector<Shape*>& sorted, Shape* newShape)
{
    // wenn die liste leer ist, einfach einfügen
    if (sorted.isEmpty())
    {
        sorted.append(newShape);
        return;
    }

    // Initialisierung und behandeln des ersten Falles: Einfügen an die erste Position der Liste
    QVector<Shape *>::iterator bestPos, lastIter, iter;
    iter = sorted.begin();
    int currentCost = calcCostsBetweenShapes(nullptr, *iter);
    int potentialCost = calcCostsBetweenShapes(nullptr, newShape) + calcCostsBetweenShapes(newShape, *iter);
    int lowestCost = potentialCost - currentCost;
    bestPos = iter;
    lastIter = iter;
    if (lowestCost == 0)
    {
        // Da es keine bessere Position geben kann muss nicht weiter nach einer besseren position gesucht werden
        // -> Einfügen des neuen Elements an die Beste Position
        sorted.insert(bestPos, newShape);
        return;
    }
    iter++;

    // Durchlaufen der liste und finden der Günstigsten Position
    for (; iter != sorted.end(); iter++)
    {
        currentCost = calcCostsBetweenShapes(*lastIter, *iter);
        potentialCost = calcCostsBetweenShapes(*lastIter, newShape) + calcCostsBetweenShapes(newShape, *iter);
        int cost = potentialCost - currentCost;
        if (lowestCost > cost)
        {
            lowestCost = cost;
            bestPos = iter;
            if (lowestCost == 0)
            {
                // Da es keine bessere Position geben kann muss nicht weiter nach einer besseren position gesucht werden
                // -> Einfügen des neuen Elements an die Beste Position
                sorted.insert(bestPos, newShape);
                return;
            }
        }
        lastIter = iter;
    }

    // Behandeln des letzten Falles: Einfügen an das Ende der Liste)
    potentialCost = calcCostsBetweenShapes(*lastIter, newShape);
    if (lowestCost > potentialCost)
    {
        bestPos = iter;
    }

    // Einfügen des neuen Elements an die Beste Position
    sorted.insert(bestPos, newShape);
    return;
}

void PreSortedSorter::sortShapes(QVector<Shape*>& shapes)
{
    QVector<Shape*> unsorted;
    unsorted = shapes;
    shapes.clear();
    sortShapes(shapes, unsorted);
}

void PreSortedSorter::sortShapes(QVector<Shape*>& sorted, QVector<Shape*>& unsorted)
{
    for (QVector<Shape*>::iterator iter = unsorted.begin(); iter != unsorted.end(); iter++)
    {
        sortShapes(sorted, *iter);
    }
}
