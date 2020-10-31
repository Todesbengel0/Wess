#include "drawable.h"
#include "geometry.h"
#include "scenemanager.h"
#include "modeltransformation.h"
#include "sgobjectvisitor.h"
#include "color.h"

Drawable::Drawable()
{
    init();
}

Drawable::Drawable(const Drawable& other)
    : mPropList(other.mPropList)
    , mPolyFill(other.mPolyFill)
    , mIsTransparent(other.mIsTransparent)
    , mEnabled(other.mEnabled)
    , mShapes()         // nicht kopieren damit das neue Drawable eigene Shapes erhält
    , mTraverseIndex(0) // nicht setzen damit da neue Drawable eigene Shapes erhält
    , mPhysicObject(other.mPhysicObject)
    , mIsStatic(other.mIsStatic)
    , mTempMatrix(other.mTempMatrix)
{
}

Drawable::Drawable(Geometry* geometry)
{
    init();
    setGeometry(geometry);
}

void Drawable::init()
{
    mPolyFill = true;
    mIsTransparent = false;
    setShader(SceneManager::instance()->getActiveContext()->getDefaultShader());
    setProperty(new Color(0.0f, 0.0f, 0.0f, 0.0f)); // jedes Drawable braucht eine eigene color da ansonsten
                                                    // getProperty<Color>().setValue(...) alle colors verändern würde
    mEnabled = true;
    mTraverseIndex = 0;

    initPhysic();
}

void Drawable::drawablePropertyChanged(DrawableProperty& /*property*/)
{
    if (mShapes.getSize() > 0)
    {
        // das erste (currentShape) ist ausreichend da die methode nicht während des renderns aufgerufen wird und somit
        // alle shapes in den gleichen ShapeListen enthalten sind.
        const QMap<ShapeList*, bool> isInShapeList = mShapes.getCurrentShape().getIsInShapeList();
        for (QMap<ShapeList*, bool>::const_iterator i = isInShapeList.constBegin(); i != isInShapeList.constEnd(); ++i)
        {
            i.key()->onDrawablePropertyChanged(*this);
        }
    }
}

void Drawable::drawablePropertyChanged(Geometry& geometry)
{
    geometry.initializeGeometry(*this);
    drawablePropertyChanged(static_cast<DrawableProperty&>(geometry));
}

void Drawable::transparencyChanged()
{
    if (mShapes.getSize() > 0)
    {
        const QMap<ShapeList*, bool> isInShapeList = mShapes.getCurrentShape().getIsInShapeList();
        for (QMap<ShapeList*, bool>::const_iterator i = isInShapeList.constBegin(); i != isInShapeList.constEnd(); ++i)
        {
            i.key()->onTransparencyChanged(*this);
        }
    }
}

void Drawable::setShader(Shader* s)
{
    setProperty(s);
}

void Drawable::setGeometry(Geometry* g)
{
    setProperty(g);
}

void Drawable::beforeTraverseChilds(SGObjectVisitor& visitor)
{
    ModelTransformation* transformation = nullptr;
    // Model Transformation wenn vorhanden anwenden
    PropListT::iterator it = mPropList.find(ModelTransformation::getPropertyId());
    if (it != mPropList.end())
    {
        transformation = static_cast<ModelTransformation*>(it->second);
        transformation->activate();
    }

    // bewegung durch Physik und Transformationen anwenden
    if (!manipulatePhysic())
    {
        SGPositionalObject::beforeTraverseChilds(visitor);
    }

    visitor.beforeTraverseChilds(*this);

    // Model Transformation wenn vorhanden wieder deaktivieren
    if (transformation)
    {
        transformation->deActivate();
    }
}

void Drawable::initPhysic()
{
    mIsStatic = false;
    mPhysicObject = nullptr;
}

bool Drawable::manipulatePhysic()
{
    bool retVal = true;
    if (mPhysicObject)
    {
        QMatrix4x4 tempMatrix = SceneManager::instance()->getActiveContext()->getModelMatrix();
        if (!qFuzzyCompare(mTempMatrix, tempMatrix))
        {
            mWorldMatrix = mPhysicObject->getEngineModelMatrix();
            QMatrix4x4 newMatrix = tempMatrix;
            QVector4D newPosition = (mWorldMatrix.column(3) + (tempMatrix.column(3) - mTempMatrix.column(3)));
            newPosition.setW(1);
            newMatrix.setColumn(3, newPosition);
            mWorldMatrix = newMatrix;
            mPhysicObject->setEngineModelMatrix(mWorldMatrix);
            mTempMatrix = tempMatrix;
        }
    }
    else
    {
        retVal = false;
    }
    return retVal;
}

void Drawable::setPhysicObject(PhysicObject* physicObject)
{
    mPhysicObject = physicObject;
}

void Drawable::setModelMatrix(const QMatrix4x4& modelMatrix)
{
    mWorldMatrix = modelMatrix;
}

void Drawable::setStaticGeometry(bool isStatic)
{
    mIsStatic = isStatic;
}

QMatrix4x4& Drawable::getModelMatrix()
{
    return mWorldMatrix;
}

const QMatrix4x4& Drawable::getModelMatrix() const
{
    return mWorldMatrix;
}

PhysicObject* Drawable::getPhysicObject()
{
    return mPhysicObject;
}

bool Drawable::isStaticGeometry()
{
    return mIsStatic;
}

QVector<QVector4D> Drawable::getTriangleList()
{
    Geometry* geometry = getGeometry();
    Q_ASSERT_X(geometry, "Drawable::getTriangleList",
               "Drawable hat keine Geometrie. Bitte vorher Drawable::setGeometry aufrufen.");

    geometry->initializeGeometry(*this);
    return geometry->getTriangleList();
}

void Drawable::setTransparent(bool transparent)
{
    if (mIsTransparent != transparent)
    {
        mIsTransparent = transparent;
        transparencyChanged();
    }
}
