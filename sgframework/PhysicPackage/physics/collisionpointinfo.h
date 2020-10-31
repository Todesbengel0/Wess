#ifndef COLLISIONPOINTINFO_H
#define COLLISIONPOINTINFO_H
#include <QVector3D>

/*!
 * \author Sebastian Vesenmayer
 * \brief The CollisionPointInfo struct
 *
 * holds infos about a collision point which can be used to spawn particles, sounds etc
 */
struct CollisionPointInfo
{
    QVector3D m_PositionA;
    QVector3D m_PositionB;
    QVector3D m_NormalOnB;
    float m_Distance;
};

#endif // COLLISIONPOINTINFO_H
