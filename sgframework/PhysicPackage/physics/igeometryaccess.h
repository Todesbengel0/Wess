#ifndef IGEOMETRYACCESS_H
#define IGEOMETRYACCESS_H

#include <QMatrix4x4>
#include <QList>
#include <QVector3D>

class PhysicObject;

/*!
 * \brief The IGeometryAccess class
 * the interface which must be implemented to set and get information to objects which will be drawn to the scene and
 * animated by physics
 */
class IGeometryAccess
{
public:
    explicit IGeometryAccess()
    {
    }
    virtual ~IGeometryAccess()
    {
    }

    /*!
     * \brief setPhysicObject
     * should set the physic object to the implemented interface
     * \param physicObject
     */
    virtual void setPhysicObject(PhysicObject* physicObject) = 0;
    /*!
     * \brief setModelMatrix
     * should set the model matrix to the implemented interface
     * \param modelMatrix
     */
    virtual void setModelMatrix(const QMatrix4x4& modelMatrix) = 0;
    /*!
     * \brief setStaticGeometry
     * set if the object should be static or dynamic in his movement in the scene
     * \param isStatic
     */
    virtual void setStaticGeometry(bool isStatic) = 0;
    /*!
     * \brief getModelMatrix
     * \return returns the model matrix used by the object in the scene
     */
    virtual QMatrix4x4& getModelMatrix() = 0;
    /*!
     * \brief getModelMatrix
     * \return returns the model matrix used by the object in the scene
     */
    virtual const QMatrix4x4& getModelMatrix() const = 0;
    /*!
     * \brief getPhysicObject
     * \return returns the physic object used by the implemented interface
     */
    virtual PhysicObject* getPhysicObject() = 0;
    /*!
     * \brief isStaticGeometry
     * \return returns if the object should be static ord dynamic in the scene
     */
    virtual bool isStaticGeometry() = 0;
    /*!
     * \brief getTriangleList
     * \return returns the triangle list of the object which is needed to calculate automatic bounding volumes
     */
    virtual QVector<QVector4D> getTriangleList() = 0;
};

#endif // IGEOMETRYACCESS_H
