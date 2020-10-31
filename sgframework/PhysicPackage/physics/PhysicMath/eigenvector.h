#ifndef EIGENVECTOR_H
#define EIGENVECTOR_H
#include <QMatrix3x3>
#include <QVector3D>
#include <math.h>
/*!
 * \brief The Eigenvector class has Helping Functions to Calculate the Eigenvectors of a Covariancematrix.
 */
class Eigenvector
{
private:
    static QVector3D solveEigenvectors(QMatrix3x3 aMatrix, double aEigenvalue);
public:
    /*!
     * \brief getEigenvectors
     * \param aCovariance
     * \param aFirst
     * \param aSecond
     * \param aThird
     */
    static void getEigenvectors(QMatrix3x3 aCovariance,QVector3D &aFirst,QVector3D &aSecond,QVector3D &aThird);
    /*!
     * \brief getEigenValues
     * \param aCovariance
     * \param aFirst
     * \param aSecond
     * \param aThird
     */
    static void getEigenvalues(QMatrix3x3 aCovariance,double &aFirst,double &aSecond,double &aThird);
    /*!
     * \brief getCoeff returns the Eigenvalues to the corresponding Covariancematrix.
     *The Covariancematrix is in Column Major Order
     * \param aCovariance Covariancematrix to calculate
     * \param aA First Polynomcoeeficient
     * \param aB Second Polynomcoeeficient
     * \param aC Third Polynomcoeeficient
     * \param aD Fourth Polynomcoeeficient
     */
    static void getCoeff(QMatrix3x3 aCovariance, double &aA, double &aB, double &aC, double &aD);
    static double sqrt3(double aX);
    static void cubicEquationSolver(double aA,double aB,double aC,double aD,double &aResult1,double &aResult2, double &aResult3);
    static double ruleOfSarrus(QMatrix3x3 a3x3Matrix);
};

#endif // EIGENVECTOR_H
