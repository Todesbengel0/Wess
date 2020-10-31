#include "shape.h"
#include "scenemanager.h"
#include "transformation.h"
#include "texture.h"

#include <QtGlobal>

Shape::Shape(Drawable& drawable) : mModelMatrix(), mViewMatrix(), mNextShape(this), mIsInShapeList(), mDrawable(drawable)
{
    Q_ASSERT_X(&drawable, "Shape::Shape", "Fehler im Konstruktor drawable ist null.");
}

Shape::Shape(Shape* nextShape)
    : mModelMatrix(), mViewMatrix(), mNextShape(nextShape), mIsInShapeList(), mDrawable(nextShape->mDrawable)
{
    Q_ASSERT_X(nextShape, "Shape::Shape", "Fehler im Konstruktor nextShape ist null.");
}

void Shape::updateModelViewMatrix()
{
    mViewMatrix = SceneManager::instance()->getActiveContext()->getDisplayConfiguration()->getViewMatrix();
    mModelMatrix = mDrawable.getWorldMatrix();
}

bool Shape::isInShapeList(ShapeList& shapeList) const
{
    QMap<ShapeList*, bool>::const_iterator find = mIsInShapeList.constFind(&shapeList);
    if (find == mIsInShapeList.constEnd())
    {
        return false;
    }

    return *find;
}

void Shape::setIsInShapeList(ShapeList& shapeList, const bool isIn)
{
    mIsInShapeList[&shapeList] = isIn;
}
