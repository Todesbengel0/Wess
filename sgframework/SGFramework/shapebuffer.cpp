#include "shapebuffer.h"

ShapeBuffer::~ShapeBuffer()
{
    Shape* firstShape = mCurrentShape;
    do
    {
        Shape* currentShape = mCurrentShape;
        if (currentShape)
        {
            moveToNext();
            delete currentShape;
        }
    } while (mCurrentShape != firstShape);

    mCurrentShape = nullptr;
    mSize = 0;
}


void ShapeBuffer::addAfterCurrent(Drawable& drawable)
{
    Shape* newShape;
    if (mCurrentShape)
    {
        // der buffer enhält bereits ein ellement -> an das kurrent shape anhängen
        newShape = new Shape(&mCurrentShape->getNextShape());
        mCurrentShape->setNextShape(newShape);
    }
    else
    {
        // der buffer ist lehr -> einfach erstellen
        mCurrentShape = new Shape(drawable);
    }

    ++mSize;
}

Shape& ShapeBuffer::getCurrentShape()
{
    Q_ASSERT_X(mCurrentShape, "ShapeBuffer::getCurrentShape()", "currentShape ist null bzw. der ShapeBuffer ist lehr");
    return *mCurrentShape;
}
