#include "boundingbox.h"
#include "PhysicMath/eigenvector.h"

BoundingBox::BoundingBox()
{
}

void BoundingBox::getBoxHalfExtends(const QVector<QVector4D>& a_Vertices, QVector3D &a_HalfExtends, QVector3D &a_Midpoint)
{
    QVector3D vMax(a_Vertices[0].x(),a_Vertices[0].y(),a_Vertices[0].z());
    QVector3D vMin(a_Vertices[0].x(),a_Vertices[0].y(),a_Vertices[0].z());
    for(int i=1,j=a_Vertices.size();i<j;i++)
    {
        if(a_Vertices[i].x()>vMax.x())
            vMax.setX(a_Vertices[i].x());
        else if(a_Vertices[i].x()<vMin.x())
            vMin.setX(a_Vertices[i].x());
        if(a_Vertices[i].y()>vMax.y())
            vMax.setY(a_Vertices[i].y());
        else if(a_Vertices[i].y()<vMin.y())
            vMin.setY(a_Vertices[i].y());
        if(a_Vertices[i].z()>vMax.z())
            vMax.setZ(a_Vertices[i].z());
        else if(a_Vertices[i].z()<vMin.z())
            vMin.setZ(a_Vertices[i].z());
    }
    a_Midpoint=QVector3D((vMax.x()+vMin.x())/2.f,
                        (vMax.y()+vMin.y())/2.f,
                        (vMax.z()+vMin.z())/2.f);
    a_HalfExtends=QVector3D((vMax.x()-vMin.x())/2.f,
                           (vMax.y()-vMin.y())/2.f,
                           (vMax.z()-vMin.z())/2.f);
}

void BoundingBox::getCovarianceOptimizedBoxFromPoints(const QVector<QVector4D>& a_VertexList, QVector3D &a_HalfExtends, QMatrix4x4 &a_Transformation)
{
    if(a_VertexList.isEmpty())
    {
        return;
    }
    QMatrix3x3 v_CovarianceMatrix;
    v_CovarianceMatrix.setToIdentity();
    QVector<QVector4D> v_VertexList;
    int v_NumberOfPoints=0;
    float v_XYZHat[]={0,0,0};
    float v_Exyz[9];
    for(int i=0;i<9;i++)
    {
        v_Exyz[i]=0;
    }

    //Remove all Duplicated Points
    for(int i=0;i<a_VertexList.size();i++)
    {
        if(!v_VertexList.contains(a_VertexList[i]))
        {
            v_VertexList.append(a_VertexList[i]);
        }
    }

    //Calculate ^x ^y ^z
    v_NumberOfPoints=v_VertexList.size();
    for(int i=0;i<v_NumberOfPoints;i++)
    {
        v_XYZHat[0]+=v_VertexList[i].x();
        v_XYZHat[1]+=v_VertexList[i].y();
        v_XYZHat[2]+=v_VertexList[i].z();
    }
    v_XYZHat[0]=v_XYZHat[0]/v_NumberOfPoints;
    v_XYZHat[1]=v_XYZHat[1]/v_NumberOfPoints;
    v_XYZHat[2]=v_XYZHat[2]/v_NumberOfPoints;

    //Calculate E[xx],E[yx],E[zx],E[xy]...
    for(int i=0;i<v_NumberOfPoints;i++)
    {
        v_Exyz[0]+=(v_VertexList[i].x()*v_VertexList[i].x());
        v_Exyz[1]+=(v_VertexList[i].y()*v_VertexList[i].x());
        v_Exyz[2]+=(v_VertexList[i].z()*v_VertexList[i].x());
        v_Exyz[3]+=(v_VertexList[i].x()*v_VertexList[i].y());
        v_Exyz[4]+=(v_VertexList[i].y()*v_VertexList[i].y());
        v_Exyz[5]+=(v_VertexList[i].z()*v_VertexList[i].y());
        v_Exyz[6]+=(v_VertexList[i].x()*v_VertexList[i].z());
        v_Exyz[7]+=(v_VertexList[i].y()*v_VertexList[i].z());
        v_Exyz[8]+=(v_VertexList[i].z()*v_VertexList[i].z());
    }
    for (int i=0;i<9;i++)
    {
        v_Exyz[i]=v_Exyz[i]/(float)v_NumberOfPoints;
    }

    //Calculate Covariance Matrix
    float* v_MatrixPointer=v_CovarianceMatrix.data();
    v_MatrixPointer[0]=v_Exyz[0]-(v_XYZHat[0]*v_XYZHat[0]);
    v_MatrixPointer[1]=v_Exyz[1]-(v_XYZHat[1]*v_XYZHat[0]);
    v_MatrixPointer[2]=v_Exyz[2]-(v_XYZHat[2]*v_XYZHat[0]);
    v_MatrixPointer[3]=v_Exyz[3]-(v_XYZHat[0]*v_XYZHat[1]);
    v_MatrixPointer[4]=v_Exyz[4]-(v_XYZHat[1]*v_XYZHat[1]);
    v_MatrixPointer[5]=v_Exyz[5]-(v_XYZHat[2]*v_XYZHat[1]);
    v_MatrixPointer[6]=v_Exyz[6]-(v_XYZHat[0]*v_XYZHat[2]);
    v_MatrixPointer[7]=v_Exyz[7]-(v_XYZHat[1]*v_XYZHat[2]);
    v_MatrixPointer[8]=v_Exyz[8]-(v_XYZHat[2]*v_XYZHat[2]);

    //Get EigenVectors from CovarianceMatrix
    QVector3D v_EigenVektorRUF[3];
    Eigenvector::getEigenvectors(v_CovarianceMatrix,v_EigenVektorRUF[0],v_EigenVektorRUF[1],v_EigenVektorRUF[2]);

    //Compute Transformation Matrix
    a_Transformation.setToIdentity();
    float* v_TransformPointer=a_Transformation.data();
    v_TransformPointer[0]=v_EigenVektorRUF[0].x();
    v_TransformPointer[1]=v_EigenVektorRUF[0].y();
    v_TransformPointer[2]=v_EigenVektorRUF[0].z();
    v_TransformPointer[4]=v_EigenVektorRUF[1].x();
    v_TransformPointer[5]=v_EigenVektorRUF[1].y();
    v_TransformPointer[6]=v_EigenVektorRUF[1].z();
    v_TransformPointer[8]=v_EigenVektorRUF[2].x();
    v_TransformPointer[9]=v_EigenVektorRUF[2].y();
    v_TransformPointer[10]=v_EigenVektorRUF[2].z();

    //Rotate Points and get the half extends + Midpoint
    QVector<QVector4D> v_RotatedVertices;
    int i;
    for(i=0;i<v_NumberOfPoints;i++)
    {
        v_RotatedVertices.append(a_Transformation.inverted()*v_VertexList[i]);
    }
    QVector3D v_Midpoint;
    getBoxHalfExtends(v_RotatedVertices,a_HalfExtends,v_Midpoint);

    //Update Transformation Matrix and return
    v_Midpoint=a_Transformation*v_Midpoint;
    v_TransformPointer[12]=v_Midpoint.x();
    v_TransformPointer[13]=v_Midpoint.y();
    v_TransformPointer[14]=v_Midpoint.z();
}
