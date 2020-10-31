#include "eigenvector.h"

QVector3D Eigenvector::solveEigenvectors(QMatrix3x3 aMatrix, double aEigenvalue)
{
    QVector3D vEigenvector;
    float* vData=aMatrix.data();
    vData[0]-=(float)aEigenvalue;
    vData[4]-=(float)aEigenvalue;
    vData[8]-=(float)aEigenvalue;

    double m=vData[1]/vData[0];
    double a=m*vData[3]-vData[4];
    double b=m*vData[6]-vData[7];

    double vY=-b/a;
    double vX=(-vData[6]-vData[3]*vY)/vData[0];

    vEigenvector.setX(vX);
    vEigenvector.setY(vY);
    vEigenvector.setZ(1);
    vEigenvector.normalize();
    return vEigenvector;
}

void Eigenvector::getEigenvectors(QMatrix3x3 aCovariance, QVector3D &aFirst, QVector3D &aSecond, QVector3D &aThird)
{
    double vFirstEigenVal,vSecondEigenVal,vThirdEigenVal;
    getEigenvalues(aCovariance,vFirstEigenVal,vSecondEigenVal,vThirdEigenVal);
    aFirst=solveEigenvectors(aCovariance,vFirstEigenVal);
    aSecond=solveEigenvectors(aCovariance,vSecondEigenVal);
    aThird=solveEigenvectors(aCovariance,vThirdEigenVal);
}

void Eigenvector::getEigenvalues(QMatrix3x3 aCovariance, double &aFirst, double &aSecond, double &aThird)
{
    double vA,vB,vC,vD;
    double vResult1,vResult2,vResult3;
    getCoeff(aCovariance,vA,vB,vC,vD);
    cubicEquationSolver(vA,vB,vC,vD,vResult1,vResult2,vResult3);
    if(vResult1>=vResult2 && vResult1>=vResult3)
    {
        aFirst=vResult1;
        if(vResult2>=vResult3)
        {
            aSecond=vResult2;
            aThird=vResult3;
        }
        else
        {
            aSecond=vResult3;
            aThird=vResult2;
        }
    }
    if((vResult1<=vResult2 && vResult1>=vResult3) || (vResult1>=vResult2 && vResult1<=vResult3))
    {
        aSecond=vResult2;
        if(vResult2>=vResult3)
        {
            aFirst=vResult2;
            aThird=vResult3;
        }
        else
        {
            aFirst=vResult3;
            aThird=vResult2;
        }
    }
    if(vResult1<=vResult2 && vResult1<=vResult3)
    {
        aThird=vResult1;
        if(vResult2>=vResult3)
        {
            aFirst=vResult2;
            aSecond=vResult3;
        }
        else
        {
            aFirst=vResult3;
            aSecond=vResult2;
        }
    }
}

void Eigenvector::getCoeff(QMatrix3x3 aCovariance, double &aA, double &aB, double &aC, double &aD)
{
    float *vMatData=aCovariance.data();
    aA=-1;
    aB=vMatData[0]+vMatData[4]+vMatData[8];
    aC=-(vMatData[0]*vMatData[4])
            -(vMatData[0]*vMatData[8])
            -(vMatData[4]*vMatData[8])
            +(vMatData[5]*vMatData[7])
            +(vMatData[1]*vMatData[3])
            +(vMatData[2]*vMatData[6]);
    aD=+(vMatData[0]*vMatData[4]*vMatData[8])
            -(vMatData[0]*vMatData[5]*vMatData[7])
            -(vMatData[1]*vMatData[3]*vMatData[8])
            +(vMatData[1]*vMatData[5]*vMatData[6])
            +(vMatData[2]*vMatData[3]*vMatData[7])
            -(vMatData[2]*vMatData[4]*vMatData[6]);
}

double Eigenvector::sqrt3(double aX)
{
    if(aX==0)
        return 0.0;
    else if(aX>0.0)
        return pow(aX,1.0/3.0);
    else
        return -pow(fabs(aX),1.0/3.0);
}

void Eigenvector::cubicEquationSolver(double aA, double aB, double aC, double aD, double &aResult1, double &aResult2, double &aResult3)
{
    double a,b,c;
    double p,q;
    double diskr;
    double u,v;

    a=aB/aA;
    b=aC/aA;
    c=aD/aA;

    p=b-1.0/3.0*a*a;
    q=2.0/27.0*a*a*a-1.0/3.0*a*b+c;
    diskr=p*p*p/27.0+q*q/4.0;

    if(diskr>0)
    {
        u=sqrt3(-q/2.0+sqrt(diskr));
        v=sqrt3(-q/2.0-sqrt(diskr));
        aResult1=(u+v)-a/3.0;
        aResult2=(-(u+v)/2.0)-a/3.0;
        aResult3=aResult2;
        //y2im=(u-v)/2.0*sqrt(3.0); y3im=-y2im
    }
    else
    {
        double spabs3;
        double phi;
        const double pi=3.14159265358979323846;

        spabs3=sqrt(fabs(p)/3.0);
        phi= acos((-q/2.0)/sqrt(fabs(p)*fabs(p)*fabs(p)/27.0));

        aResult1=(2.0*spabs3*cos(phi/3.0))-a/3.0;
        aResult2=(-2.0*spabs3*cos(phi/3.0-pi/3.0))-a/3.0;
        aResult3=(-2.0*spabs3*cos(phi/3.0+pi/3.0))-a/3.0;
        //y2im & y3im =0.0
    }
}

double Eigenvector::ruleOfSarrus(QMatrix3x3 a3x3Matrix)
{
    float* vData=a3x3Matrix.data();
    return ((vData[0]*vData[4]*vData[8])
            +(vData[3]*vData[7]*vData[2])
            +(vData[6]*vData[1]*vData[5])
            -(vData[2]*vData[4]*vData[6])
            -(vData[5]*vData[7]*vData[0])
            -(vData[8]*vData[1]*vData[3]));
}
