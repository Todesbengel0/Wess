#include "shapesorter.h"
#include "drawable.h"

int ShapeSorter::calcCostsBetweenShapes(Shape* s1, Shape* s2)
{
    // Besondere Fälle: s1 hat keinen Nachfolger
    if (s2 == nullptr)
    {
        return 0;
    }

    PropListT::const_iterator iter2 = s2->getDrawable().getPropertyList().begin();
    PropListT::const_iterator end2 = s2->getDrawable().getPropertyList().end();
    int cost = 0;

    // Besondere Fälle: s2 hat keinen Vorgänger
    if (s1 == nullptr)
    {
        for (; iter2 != end2; ++iter2)
        {
            cost += iter2->second->getPriority();
        }
        return cost;
    }

    PropListT::const_iterator iter1 = s1->getDrawable().getPropertyList().begin();
    PropListT::const_iterator end1 = s1->getDrawable().getPropertyList().end();

    // finde alle Properties die in s2 aber nicht in s1 sind, und summire ihre Prioritäten
    while (iter1 != end1 && iter2 != end2)
    {
        if (iter1->first > iter2->first)
        {
            cost += iter2->second->getPriority();
            ++iter2;
        }
        else if (iter1->first < iter2->first)
        {
            ++iter1;
        }
        else
        {
            if (iter1->second != iter2->second)
            {
                cost += iter2->second->getPriority();
            }
            ++iter1;
            ++iter2;
        }
    }

    for (; iter2 != end2; ++iter2) // wenn die while schleife wegen s1 abgebrochen ist, sind noch elemente in s2
    {
        cost += iter2->second->getPriority();
    }
    return cost;
}
