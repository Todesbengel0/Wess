#ifndef SHAPE_H
#define SHAPE_H

#include <QMatrix4x4>

class Drawable;
class ShapeList;

//! Die Klasse Shape wird benötigt da ein Drawable mehrmals inerhalb einer Szene vorhanden sein kann und die
//! Model-/ViewMatrix, die nur während des Durchlaufens des Baumes berechnet werden kann, für das sortierte Rendern
//! inerhalb der ShapeList benötigt wird.
class Shape
{
public:
    //! Aktuallisiert die ModelViewMatrix anhand der WorldMatrix des Drawables sowie der ViewMatrix der aktuellen
    //! Kammera.
    void updateModelViewMatrix();

    //! Getter für die ModelViewMatrix
    const QMatrix4x4 getModelViewMatrix() const {return (mViewMatrix*mModelMatrix);}
    //! Getter für die ModelViewMatrix
    const QMatrix4x4& getViewMatrix() const {return mViewMatrix;}
    //! Getter für die ModelViewMatrix
    const QMatrix4x4& getModelMatrix() const {return mModelMatrix;}

    //! Gibt das nächste Shape des drawables zurück. Die Shapes werden als verketteter Ringpuffer gespeichert (der letzte
    //! pointer zeigt wieder auf das erste element). ACHTUNG! Wenn das Drawable nur einen Shape hat dann gibt diese
    //! methode sich selbst zurück.
    Shape& getNextShape() const {return *mNextShape;}

    //! Gibt true zurück wenn sich das Shape in der ShapeList befindet. Wird benötigt um neue Shapes einmal zur
    //! ShapeList hinzuzufügen.
    bool isInShapeList(ShapeList& shapeList) const;

    //! Setze ob sich das Shape in der ShapeList befindet. Wird benötigt um neue Shapes einmal zur ShapeList
    //! hinzuzufügen.
    void setIsInShapeList(ShapeList& shapeList, const bool isIn = true);

    //! Getter für das Drawable das diesen Shape besitzt. Wird benötigt da inerhalb der ShapeList Shapes sortiert werden
    //! und zum anschließenden render das Drawable benötigt wird.
    Drawable& getDrawable() {return mDrawable;}

    //! Getter für die Map die angibt in welchen ShapeListen sich dieses Shape befindet.
    const QMap<ShapeList*, bool>& getIsInShapeList() const {return mIsInShapeList;}

private:
    //! Modelmatrix
    QMatrix4x4 mModelMatrix;
    //! Viewmatrix
    QMatrix4x4 mViewMatrix;

    //! \see getNextShape()
    Shape* mNextShape;

    //! Pro ShapeList in der sich dieses Shape befindet ein eintrag mit true. Wenn sich das Shape nich inerhalb der
    //! ShapeListe befindet kann entweder false gesetzt sein oder kein eintrag inerhalb der liste vorhanden sein. Dies
    //! könnte auch über ein QSet implementiert werden, was jedoch wegen des zu grosen overhads von QSet/QHash bei
    //! wenigen elementen über QMap implementiert wurde.
    QMap<ShapeList*, bool> mIsInShapeList;

    //! \see getDrawable()
    Drawable& mDrawable;

    //! privater Konstruktor für ShapeBuffer
    Shape(Drawable& drawable);

    //! privater Konstruktor für ShapeBuffer
    Shape(Shape* nextShape);

    //! privater setter für ShapeBuffer
    void setNextShape(Shape* nextShape) {mNextShape = nextShape;}

    //! für den zugriff auf die Konstruktoren sowei setNextShape().
    friend class ShapeBuffer;
};

#endif // SHAPE_H
