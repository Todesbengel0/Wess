#include "shapelist.h"
#include "drawable.h"
#include "geometry.h"
#include "scenemanager.h"
#include "shader.h"
#include "modeltransformation.h"
#include "texture.h"
#include "bumpmap.h"
#include "shapesorter.h"
#include "shadowmapproperty.h"

void ShapeList::updateDrawableShape(Drawable& drawable)
{

    Shape& shape = drawable.getShapes().getCurrentShape();
    shape.updateModelViewMatrix();

    if (!shape.isInShapeList(*this))
    {
        // Shape noch nicht in einer der listen mTransparentShapes oder mOpakeShapes dieser Shapelist => hinzufügen
        addShape(shape);
    }
}

void ShapeList::renderOpakeShapes()
{
    renderShapes(mOpakeShapes);
}

void ShapeList::renderTransparentShapes()
{
    renderShapes(mTransparentShapes);
}

void ShapeList::onTransparencyChanged(Drawable& drawable)
{
    if (drawable.isTransparent())
    {
        // Wenn das Drawable nun Transparent ist müssen alle Shapes des Drawables aus der opaken liste entfernt werden
        // und zur transparenten liste hinzugefügt werden.
        Shape* firstShape = &drawable.getShapes().getCurrentShape();
        Shape* shape = firstShape;
        do
        {
            mOpakeShapes.removeOne(shape);    // removeOn() reicht da es nur einmal enthalten sein kann
            mTransparentShapes.append(shape); // wird in afterTraverse() sotiert

            shape = &shape->getNextShape();
        } while (shape != firstShape);
    }
    else
    {
        // Wenn das Drawable nun Opake ist müssen alle Shapes des Drawables aus der transparenten liste entfernt werden
        // und zur opaken liste hinzugefügt werden.
        Shape* firstShape = &drawable.getShapes().getCurrentShape();
        Shape* shape = firstShape;
        do
        {
            mTransparentShapes.removeOne(shape); // removeOn() reicht da es nur einmal enthalten sein kann
            mPreSortedSorter.sortShapes(mOpakeShapes, shape);

            shape = &shape->getNextShape();
        } while (shape != firstShape);
    }
}

void ShapeList::onDrawablePropertyChanged(Drawable& /*drawable*/)
{
    // TODO: die shapes des drawables neu einsotieren
}

void ShapeList::renderOpakeGeometries(Shader& shader)
{
    renderShapeGeometries(mOpakeShapes, shader);
}

void ShapeList::renderTransparentGeometries(Shader& shader)
{
    renderShapeGeometries(mTransparentShapes, shader);
}

void ShapeList::afterTraverse()
{
    // transparente shapes anhand ihrer entfernung zur kamera sortieren. weiter entfernteste müssen zuerst gerendert
    // werden.


    mTransparentShapeSorter.sortShapes(mTransparentShapes);
}

void ShapeList::renderShape(Shape& shape, Geometry& geometry, Shader& shader)
{
    bindShapeUniforms(shape.getModelMatrix(), shape.getViewMatrix(), shader);
    geometry.render(&shape.getDrawable());
}

void ShapeList::renderShapes(QVector<Shape*>& shapes)
{
    Drawable* currentDrawable = nullptr;
    for (int i = 0; i < shapes.size(); ++i)
    {
        Shape& shape = *shapes[i];
        Drawable& drawable = shape.getDrawable();
        drawable.resetTraverseIndex();
        Shader* shader = drawable.getShader();
        Geometry* geometry = drawable.getGeometry();
        if (shader && shader->isOK() && drawable.isEnabled() && geometry)
        {
            if (&drawable != currentDrawable)
            {
                RenderingState::getInstance().bindDrawable(drawable, *shader, *geometry);
                currentDrawable = &drawable;
            }
            renderShape(shape, *geometry, *shader);
        }
    }
}

void ShapeList::renderShapeGeometries(QVector<Shape*>& shapes, Shader& shader)
{
    // ACHTUNG! shader muss bereits aktiv, gebunden, und dessen uniforms gesetzt sein
    for (int i = 0; i < shapes.size(); ++i)
    {
        Shape& shape = *shapes[i];
        Drawable& drawable = shape.getDrawable();
        Geometry* geometry = drawable.getGeometry();
        if (drawable.isEnabled() && geometry)
        {
            PropListT::const_iterator iter = drawable.getPropertyList().find(ShadowMapProperty::getPropertyId());
            if (iter != drawable.getPropertyList().end())
            {
                iter->second->turnOn(&shader);
            }

            geometry->bindBuffer(shader);
            renderShape(shape, *geometry, shader);
            if (iter != drawable.getPropertyList().end())
            {
                iter->second->turnOff(&shader);
            }
        }
    }
}

void ShapeList::addShape(Shape& shape)
{
    if (shape.getDrawable().isTransparent())
    {
        mTransparentShapes.append(&shape);
    }
    else
    {
        mPreSortedSorter.sortShapes(mOpakeShapes, &shape);
    }

    shape.setIsInShapeList(*this);
}

void ShapeList::bindShapeUniforms(const QMatrix4x4& modelMatrix, const QMatrix4x4& viewMatrix, Shader& shader)
{
    // Set the uniform variable on shader program
    QOpenGLShaderProgram* shaderProgram = shader.shaderProgram();
    shaderProgram->setUniformValue("projectionMatrix", SceneManager::instance()->getActiveContext()->getDisplayConfiguration()->getProjectionMatrix());
    shaderProgram->setUniformValue("modelMatrix", modelMatrix);
    shaderProgram->setUniformValue("viewMatrix", viewMatrix);
    shaderProgram->setUniformValue("normalMatrix", (viewMatrix*modelMatrix).normalMatrix());
}

int ShapeList::calcCostOfOpakeShapeList()
{
    if (mOpakeShapes.isEmpty())
    {
        return 0;
    }

    QVector<Shape *>::iterator iter, lastIter;
    iter = mOpakeShapes.begin();
    lastIter = iter;
    int cost = ShapeSorter::calcCostsBetweenShapes(nullptr, *iter);
    for (iter++; iter != mOpakeShapes.end(); iter++)
    {
        cost += ShapeSorter::calcCostsBetweenShapes(*lastIter, *iter);
        lastIter = iter;
    }
    return cost;
}
