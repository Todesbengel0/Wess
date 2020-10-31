#include "boundingsphere.h"
#include <QMatrix3x3>
#include <QVector4D>

#define RADIUS_STABILITY 0.0001f

BoundingSphere BoundingSphere::WelzlSphere(const QVector<QVector4D> &a_Vertices, int a_NrVertices, QVector3D a_SetOfSupport[], int a_NrSetOfSupport)
{
    if(a_NrVertices==0 || a_NrSetOfSupport==4)
    {
        switch(a_NrSetOfSupport)
        {
        case 0:
            return BoundingSphere();
        case 1:
            return BoundingSphere(a_SetOfSupport[0]);
        case 2:
            return BoundingSphere(a_SetOfSupport[0],a_SetOfSupport[1]);
        case 3:
            return BoundingSphere(a_SetOfSupport[0],a_SetOfSupport[1],a_SetOfSupport[2]);
        case 4:
            return BoundingSphere(a_SetOfSupport[0],a_SetOfSupport[1],a_SetOfSupport[2],a_SetOfSupport[3]);
        }
    }
    int v_Index=a_NrVertices-1;

    BoundingSphere v_SmallestSphere=WelzlSphere(a_Vertices,v_Index,a_SetOfSupport,a_NrSetOfSupport);

    const QVector3D vertex = a_Vertices[v_Index].toVector3D();
    if((vertex-v_SmallestSphere.m_Center).length()<v_SmallestSphere.m_Radius)
        return v_SmallestSphere;

    a_SetOfSupport[a_NrSetOfSupport]=vertex;
    return WelzlSphere(a_Vertices,v_Index,a_SetOfSupport,a_NrSetOfSupport+1);
}

BoundingSphere::BoundingSphere()
{
    m_Radius=0.f;
    m_Center=QVector3D(0.f,0.f,0.f);
}

BoundingSphere::BoundingSphere(QVector3D &a_Point)
{
    m_Radius=RADIUS_STABILITY;
    m_Center=a_Point;
}

BoundingSphere::BoundingSphere(QVector3D &a_PointA, QVector3D &a_PointB)
{
    QVector3D v_Segment=(a_PointB-a_PointA)/2.f;
    m_Radius=v_Segment.length()+RADIUS_STABILITY;
    m_Center=a_PointA+v_Segment;
}

BoundingSphere::BoundingSphere(QVector3D &a_PointA, QVector3D &a_PointB, QVector3D &a_PointC)
{
    QVector3D v_VecA=a_PointB-a_PointA;
    QVector3D v_VecB=a_PointC-a_PointA;

    QVector3D v_Temp=QVector3D::crossProduct(v_VecA,v_VecB);

    float v_Denominator=2.f*(QVector3D::dotProduct(v_Temp,v_Temp));
    QVector3D v_Radius=(QVector3D::dotProduct(v_VecB,v_VecB)
               *QVector3D::crossProduct(v_Temp,v_VecA)
               +QVector3D::dotProduct(v_VecA,v_VecA)
               *QVector3D::crossProduct(v_VecB,v_Temp))
            /v_Denominator;
    m_Radius=v_Radius.length()+RADIUS_STABILITY;
    m_Center=a_PointA+v_Radius;
}

BoundingSphere::BoundingSphere(QVector3D &a_PointA, QVector3D &a_PointB, QVector3D &a_PointC, QVector3D &a_PointD)
{
    QVector3D v_VecA=a_PointB-a_PointA;
    QVector3D v_VecB=a_PointC-a_PointA;
    QVector3D v_VecC=a_PointD-a_PointA;

    float m11=v_VecA.x();
    float m12=v_VecA.y();
    float m13=v_VecA.z();
    float m21=v_VecB.x();
    float m22=v_VecB.y();
    float m23=v_VecB.z();
    float m31=v_VecC.x();
    float m32=v_VecC.y();
    float m33=v_VecC.z();

    float v_Denominator=2.f*(m11*(m22*m33-m32*m23)
                             -m21*(m12*m33-m32*m13)
                             +m31*(m12*m23-m22*m13));
    QVector3D v_Radius=(QVector3D::dotProduct(v_VecC,v_VecC)
                        *QVector3D::crossProduct(v_VecA,v_VecB)
                        +QVector3D::dotProduct(v_VecB,v_VecB)
                        *QVector3D::crossProduct(v_VecC,v_VecA)
                        +QVector3D::dotProduct(v_VecA,v_VecA)
                        *QVector3D::crossProduct(v_VecB,v_VecC))
            /v_Denominator;
    m_Radius=v_Radius.length()+RADIUS_STABILITY;
    m_Center=a_PointA+v_Radius;
}

BoundingSphere BoundingSphere::getWelzlSphere(const QVector<QVector4D>& a_Vertices)
{
    int v_NrPoints=a_Vertices.size();
    QVector3D v_SetOfSupport [4];
    int v_NrPointsInSetOfSupport=0;
    return WelzlSphere(a_Vertices,v_NrPoints,v_SetOfSupport,v_NrPointsInSetOfSupport);
}

QVector3D BoundingSphere::getCenter()
{
    return m_Center;
}


float BoundingSphere::getRadius()
{
    return m_Radius;
}

