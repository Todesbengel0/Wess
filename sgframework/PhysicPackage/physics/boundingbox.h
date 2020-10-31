#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <QList>
#include <QVector3D>
#include <QMatrix4x4>

/*!
 * \author Sebastian Vesenmayer
 * \brief The BoundingBox class
 *
 * has methods to generate a bounding boxes from vertices
 */
class BoundingBox
{
protected:
    BoundingBox();
public:
    /*!
     * \brief getBoxHalfExtends
     *
     * returns an axis aligned bounding box in model space from vertices
     *
     * \param a_Vertices input vertices for bounding box calculation
     * \param a_HalfExtends output 3d vector which hold the half extends from the midpoint of the box
     * \param a_Midpoint output midpoint of the box in model space
     */
    static void getBoxHalfExtends(const QVector<QVector4D> &a_Vertices, QVector3D &a_HalfExtends, QVector3D &a_Midpoint);

    /*!
     * \brief getCovarianceOptimizedBoxFromPoints
     *
     * returns a optimized bounding box, calculated from the distribution of the input vertices
     *
     * \param a_VertexList input vertices for bounding box calculation
     * \param a_HalfExtends output 3d vector which holds the half extends from the transformed midpoint
     * \param a_Transformation output transformation of the box to fit the object
     */
    static void getCovarianceOptimizedBoxFromPoints(const QVector<QVector4D> &a_VertexList, QVector3D &a_HalfExtends, QMatrix4x4 &a_Transformation);
};

#endif // BOUNDINGBOX_H
