#ifndef SHAPELIST_H
#define SHAPELIST_H

#include "shape.h"
#include "drawable.h"
#include "transparentshapesorter.h"
#include "presortedsorter.h"

#include <QtGlobal>

class Shader;
class Geometry;
class Texture;

//! Die ShapeList Klasse sortiert und rendert shapes. Beim Durchlaufen des Baumes (wenn sortiertes Rendering verwendet
//! wird) werden die Shapes der Shapeliste hinzugefügt/sortiert und erst nach dem der Baum Durchlaufen wurde gerendert.
class ShapeList
{
public:
    //! Wird beim Durchlaufen des Baumes (wenn sortiertes Rendering verwendet wird) aufgerufen und fügt das Shape der
    //! liste hinzu wenn es neu ist und aktualisiert die ModelViewMatrix des Shapes.
    void updateDrawableShape(Drawable& drawable);

    //! Rendert alle Opaken Shapes in der Sortierten reihenfolge unter verwendung von RenderingState sodass nur
    //! Properties gebunden werden die nicht schon gebunden sind. Opake Shapes werden so sortiert um die kosten
    //! (Priority) die das ändern einer DrawableProperty verursacht möglichst gering zu halten.
    void renderOpakeShapes();

    //! Rendert alle Transparenten Shapes in der Sortierten reihenfolge unter verwendung von RenderingState sodass nur
    //! Properties gebunden werden die nicht schon gebunden sind. Transparente Shapes werden anhand ihrer tiefenwerte
    //! inerhalb des Kammera-Koordinatensystems sortiert.
    void renderTransparentShapes();

    //! wird aufgerufen nach dem durchlaufen der Szenengraph zur aktualisierung der Matrizen/Objekte
    //! (getRoot()->traverse()) aufgerufen. Hier werden alle Transparenten Shapes neu sortiert da sich die Tiefenwerte
    //! anhand der Transparente Shapes sortiert werden \see TransparentShapeSorter immer ändern wenn sich die Kammera
    //! bewegt was fast jedes Frame geschiet.
    void afterTraverse();

    //! Wird aufgerufen wenn sich die transparenz eines Drawables inerhalb dieser ShapeList geändert hat und verschiebt
    //! die shapes von einer der beiden listen mOpakeShapes oder mTransparentShapes in die jeweils andere liste.
    void onTransparencyChanged(Drawable& drawable);

    //! Wird aufgerufen wenn sich eine Property eines Drawables inerhalb dieser ShapeList geändert hat. Die Shapes des
    //! Drawables müssen neu sortiert werden.
    void onDrawablePropertyChanged(Drawable& drawable);

    //! Rendert alle Geometrien von Opaken Shapes unter verwendung von \param shader der normale Shader sowie andere
    //! DrawableProperties werden ignoriert.
    void renderOpakeGeometries(Shader& shader);

    //! Rendert alle Geometrien von Transparenten Shapes unter verwendung von \param shader der normale Shader sowie
    //! andere DrawableProperties werden ignoriert.
    void renderTransparentGeometries(Shader& shader);

    //! Bindet die pro Shape unterschiedlchen Uniforms wie Projektions- und Model-/ViewMatrix.
    static void bindShapeUniforms(const QMatrix4x4& modelMatrix, const QMatrix4x4& viewMatrix, Shader& shader);

private:
    //! Rendert ein Shape (Properties müssen schon aktiv sein)
    void renderShape(Shape& shape, Geometry& geometry, Shader& shader);

    //! Rendert eine Liste von Shapes. Properties werden über RenderingState gebunden.
    void renderShapes(QVector<Shape*>& shapes);

    //! Rendert eine Liste von Shapes ohne Properties zu aktivieren unter verwendung von \param shader
    void renderShapeGeometries(QVector<Shape*>& shapes, Shader& shader);

    //! Fügt das Shape entweder zu mOpakeShapes oder zu mTransparentShapes hinzu.
    void addShape(Shape& shape);

    //! Berechnet die gesamt Kosten die die für das Rendern der in mOpakeShapes gespeicherten Shapes benötigten
    //! Statusferänderungen. Wird verwendet um die Qualität der Sortieralgorithmen zu überprüfen.
    int calcCostOfOpakeShapeList();

    //! Eine sortierte liste mit Opaken Shapes. Opake Shapes werden so sortiert um die kosten (Priority) die das ändern
    //! einer DrawableProperty verursacht möglichst gering zu halten.
    QVector<Shape*> mOpakeShapes;

    //! Eine sortierte liste mit transparenten Shapes. \see mTransparentShapeSorter
    QVector<Shape*> mTransparentShapes;

    //! Sortierer um Transparente Shapes anhand ihrer tiefenwerte inerhalb des Kammera-Koordinatensystems zu sortieren.
    //! In afterTraverse() werden alle Transparenten Shapes neu sortiert da sich die Tiefenwerte immer ändern wenn sich
    //! die Kammera bewegt was fast jedes Frame geschiet.
    TransparentShapeSorter mTransparentShapeSorter;

    //! Ein sortierer für Obake Shapes.
    PreSortedSorter mPreSortedSorter;
};

#endif // SHAPELIST_H
