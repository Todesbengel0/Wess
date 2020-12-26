#ifndef SAPEBUFFER_H
#define SAPEBUFFER_H

#include "shape.h"

//! Managed die erstellung von Shapes die intern als Ringbuffer gespeichert werden. \see Shape
class ShapeBuffer
{
public:
    //! Löscht alle Shapes aus dem ShapeBuffer
    ~ShapeBuffer();

    //! Macht den nachfolger des aktuellen Shapes zum aktuellen Shape. \see getCurrentShape()
    void moveToNext() {Q_ASSERT(mCurrentShape);mCurrentShape = &mCurrentShape->getNextShape();}

    //! Fügt ein neues Shape hinter dem aktuellen Shape an. Wenn der Buffer noch leer ist wird das Shape einfach
    //! erstellt und als aktuelles Shape gewählt. Das aktuelle Shape wird nicht verändert (zeig also anschliesend nicht
    //! auf das neu hinzugefügte Shape).
    void addAfterCurrent(Drawable& drawable);

    //! Getter für die anzahl an Shapes inerhalb des Buffers.
    int getSize() const {return mSize;}

    //! Getter für das aktuelle Shape. Das aktuelle Shape gann mit moveToNext() geändert werden.
    Shape& getCurrentShape();

private:
    //! Pointer auf das aktuelle Shape. \see getCurrentShape()
    Shape* mCurrentShape = nullptr;

    //! Die anzahl an Shapes inerhalb des Buffers.
    int mSize = 0;
};

//inline void ShapeBuffer::moveToNext()
//{
//    Q_ASSERT(mCurrentShape);
//    mCurrentShape = &mCurrentShape->getNextShape();
//}

//inline int ShapeBuffer::getSize() const
//{
//    return mSize;
//}
#endif // SAPEBUFFER_H
