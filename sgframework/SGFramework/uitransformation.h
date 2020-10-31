#ifndef UITRANSFORMATION_H
#define UITRANSFORMATION_H

#include <QObject>
#include "opengl.h"

#include "transformation.h"

//! Eine Transformation, die Slots zur Verfügung stellt, in die die jeweiligen Transformationswerte
//! über entsprechende Signals, z.B. aus GUI-Elementen eingespeist werden können.
class UITransformation : public QObject, public Transformation
{
    Q_OBJECT
private:
    GLfloat mLastXrot, mLastYrot, mLastZrot, mLastXTrans, mLastYTrans, mLastZTrans;
public:
    UITransformation() {mLastXrot=mLastYrot=mLastZrot=mLastXTrans=mLastYTrans=mLastZTrans=0;}

public slots:
    void setXrot(int degrees) {this->rotate(degrees-mLastXrot, 1.0,.0,.0); mLastXrot = degrees;}
    void setYrot(int degrees) {this->rotate(degrees-mLastYrot, .0,1.0,.0); mLastYrot = degrees;}
    void setZrot(int degrees) {this->rotate(degrees-mLastZrot, .0,.0,1.0); mLastZrot = degrees;}
    //Parameter bei Translation /10, da Slider immer ints liefern - Zehntel sind idR genau genug!
    void setXtra(int units) {this->translate(units/10.-mLastXTrans/10., .0,.0); mLastXTrans=units;}
    void setYtra(int units) {this->translate(0., units/10.0-mLastYTrans/10.,.0); mLastYTrans=units;}
    void setZtra(int units) {this->translate(0., .0,units/10.0-mLastZTrans/10.); mLastZTrans=units;}
};

#endif // UITRANSFORMATION_H
