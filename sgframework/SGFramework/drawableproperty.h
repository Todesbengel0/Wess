#ifndef _DRAWABLEPROPERTY_H_
#define _DRAWABLEPROPERTY_H_

#include <string>
#include <iostream>
#include "node.h"
#include "drawablepropertyid.h"

class Shader;

//! Basisklasse für alle Eigenschaften eines Drawable, die sein Erscheinungsbild beeinflussen.
class DrawableProperty
{
public:
    DrawableProperty() : mPriority(0){}
    virtual ~DrawableProperty(){}

    /*! TurnOn aktiviert alle Einstellungen, die für das Rendern der jeweiligen Property
    * notwendig sind. Die Methodik ist:
    * -# Aktivieren der Property
    * -# Rendern von Drawablen mit gleicher Property sowie gleichem Shader (da uniforms pro Shader gesetzt werden)
    * -# Deaktiveren der Property
    */
    virtual void turnOn(Shader* /*shader*/){}

    //! TurnOff deaktiviert alle Einstellungen, die für das Rendern der jeweiligen Property notwendig sind.
    virtual void turnOff(Shader* /*shader*/){}

    //! getId() gibt den selben wert wie PropertyClass::getPropertyId() zurück jedoch ist sie nicht statisch und kann
    //! somit verwendet werden um werden der laufzeit den datentype zu überprüfen, also z.B. ob eine property eine
    //! transformation oder eine textur ist.
    virtual DrawablePropertyId getId() const = 0;

    //! Getter für die Priorität beim sortieren der Drawables zu rendern \see ShapeSorter::calcCostsBetweenShapes().
    int getPriority() const{return mPriority;}

    //! Setter für die Priorität beim sortieren der Drawables zu rendern.
    void setPriority(int priority) {mPriority = priority;}

private:
    //! Priorität zum sortieren der Drawables beim sortierten rendern.
    int mPriority;
};

//inline DrawableProperty::DrawableProperty() : mPriority(0)
//{
//}

//inline DrawableProperty::~DrawableProperty()
//{
//}

//inline void DrawableProperty::turnOn(Shader* /*shader*/)
//{
//}

//inline void DrawableProperty::turnOff(Shader* /*shader*/)
//{
//}

//inline int DrawableProperty::getPriority() const
//{
//    return mPriority;
//}

//inline void DrawableProperty::setPriority(int priority)
//{
//    mPriority = priority;
//}

#endif
