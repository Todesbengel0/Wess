#ifndef TRANSFORMATIONBASE_H
#define TRANSFORMATIONBASE_H

#include <QMatrix4x4>

//! Basisklasse für Transformations-Klassen implementiert die eigendlichen transformations-methoden.
//! \sa Transformation, ModelTransformation
class TransformationBase
{
public:
    TransformationBase();

    //! Verschieben um Vektor, der durch die 3 Parameter gebildet wird
    void translate(float tx, float ty, float tz);

    //! Rotieren um die Achse \param axis um den Winkel \param angle
    void rotate(float angle, const QVector3D& axis);

    //! Rotieren
    void rotate(float angle, float x, float y, float z);

    //! Skalieren
    void scale(const QVector3D& scale);

    //! Skalieren
    void scale(float sx, float sy, float sz);

    //! Transformationen zurücksetzen (LoadIdentity)
    void resetTrafo();

    //! Transformation auf globale matrix anwenden
    void activate();

    //! Transformation von globaler matrix zurücksetzen
    void deActivate();

    //! Setter für die ModelMatirx.
    void setModelMatrix(const QMatrix4x4& modelMatrix);

    //! Mutable getter für die ModelMatrix.
    inline QMatrix4x4& getModelMatrix() {return mModelMatrix;}

    //! Const getter für die ModelMatrix.
    inline const QMatrix4x4& getModelMatrix() const {return mModelMatrix;}

private:
    //! Die Transformationen werden in dieser ModelMatrix gespeichert.
    QMatrix4x4 mModelMatrix;

    //! Speichert die aktuelle ModelMatrix des RenderingContext's beim anwenden der Transformation über activate()
    //! sodass diese in deActivate() wieder zurückgesetzt werden kann.
    QMatrix4x4 mMTemp;

    //! Speichert die aktuelle ModelMatrix des ShadowLight's beim anwenden der Transformation über activate() sodass
    //! diese in deActivate() wieder zurückgesetzt werden kann.
    QMatrix4x4 mMLTemp;
};

//inline QMatrix4x4& TransformationBase::getModelMatrix()
//{
//    return mModelMatrix;
//}

//inline const QMatrix4x4& TransformationBase::getModelMatrix() const
//{
//    return mModelMatrix;
//}

#endif // TRANSFORMATIONBASE_H
