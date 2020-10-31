#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include <QList>
#include <QVector3D>

/*!
 * \author Sebastian Vesenmayer
 * \brief The BoundingSphere class
 *
 * has methods to generate a bounding sphere from vertices
 */
class BoundingSphere
{
protected:
    QVector3D m_Center;
    float m_Radius;

    /*!
     * \brief WelzlSphere
     *
     * recursive method to generate a welzl sphere
     * \param a_Vertices vertices which should lie in the sphere
     * \param a_NrVertices number of vertices in a_Vertices
     * \param a_SetOfSupport outer points
     * \param a_NrSetOfSupport number of outer points in a_SetOfSupport
     * \return
     */
    static BoundingSphere WelzlSphere(const QVector<QVector4D> &a_Vertices, int a_NrVertices, QVector3D a_SetOfSupport [], int a_NrSetOfSupport);
public:
    /*!
     * \brief BoundingSphere
     *
     * generates a bounding sphere with 0,0,0 center and 0 radius
     */
    BoundingSphere();

    /*!
     * \brief BoundingSphere
     *
     * generates a bounding sphere with midpoint a_Point and a stability radius
     * \param a_Point
     */
    BoundingSphere(QVector3D &a_Point);

    /*!
     * \brief BoundingSphere
     *
     * generates a bounding sphere from two points
     * \param a_PointA
     * \param a_PointB
     */
    BoundingSphere(QVector3D &a_PointA,QVector3D &a_PointB);

    /*!
     * \brief BoundingSphere
     *
     * generates a bounding sphere from three points
     * \param a_PointA
     * \param a_PointB
     * \param a_PointC
     */
    BoundingSphere(QVector3D &a_PointA,QVector3D &a_PointB,QVector3D &a_PointC);

    /*!
     * \brief BoundingSphere
     *
     * generates a bounding sphere from four points
     * \param a_PointA
     * \param a_PointB
     * \param a_PointC
     * \param a_PointD
     */
    BoundingSphere(QVector3D &a_PointA, QVector3D &a_PointB, QVector3D &a_PointC, QVector3D &a_PointD);

    /*!
     * \brief getWelzlSphere
     *
     * \param a_Vertices vertices from which a bounding sphere should be generated
     * \return a minimum bounding sphere from vertices
     */
    static BoundingSphere getWelzlSphere(const QVector<QVector4D> &a_Vertices);
    QVector3D getCenter();
    float getRadius();
};

#endif // BOUNDINGSPHERE_H
