#ifndef INTERPOLATEDROTATION_H
#define INTERPOLATEDROTATION_H

#include "idleobserver.h"
#include "transformation.h"

//! Eine Klasse, die zwischen zwei Orienterungen eines Objektes animiert hin- und herwechselt
//! InterpolatedRotation kann die Orientierungen zu Demo-Zwecken als Matrizen oder als Quaternionen enthalten, um
//! Vor- und Nachteile zu visualisieren
class InterpolatedRotation : public Transformation, public IdleObserver
{
public:
    InterpolatedRotation();
    void rotateStart(float angle, QVector3D axis);
    void rotateEnd(float angle, QVector3D axis);
    virtual void doIt() override;
    void setSpeed(float s) {mSpeed = s;}
    void setBackandForth(bool b) {mBackandForth = b;}
    void setRepresentAsQuat(bool b) {mRepresentasQuat = b;}

private:
    QMatrix4x4 mStartMatrix, mEndMatrix;
    QQuaternion mStartQuat, mEndQuat;
    float mT, mSpeed;
    bool mBackandForth;
    bool mRepresentasQuat;
};

//inline void InterpolatedRotation::setSpeed(float s)
//{
//    mSpeed = s;
//}

//inline void InterpolatedRotation::setBackandForth(bool b)
//{
//    mBackandForth = b;
//}

//inline void InterpolatedRotation::setRepresentAsQuat(bool b)
//{
//    mRepresentasQuat = b;
//}

#endif // INTERPOLATEDTRANSFORMATION_H
