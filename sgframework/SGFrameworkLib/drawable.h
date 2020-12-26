#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "sgpositionalobject.h"
#include "drawableproperty.h"
#include "shader.h"
#include "physics.h"
#include "shapebuffer.h"
#include "geometry.h"

typedef std::pair<DrawablePropertyId, DrawableProperty*> PropEleT;
typedef std::map<DrawablePropertyId, DrawableProperty*> PropListT;

//! Basisklasse für alle darstellbaren Objekte.
//! Die geometrischen Informationen des zu zeichnenden Objektes
//! hält die Klasse Geometry. Damit ist es möglich dieselben geometrischen
//! Objekte mit verschiedenem Erscheinungsbild in einer Szene zu verwenden
//! ohne die geometrischen Information zu duplizieren.
//! \sa Transformation, Geometry, Color und Material
class Drawable : public SGPositionalObject, public IGeometryAccess
{
public:
    //! Konstruktor ohne geometry. Die Geometry kann über setGeometry() gesetzt werden.
    Drawable();

    //! Kopykonstruktor sollte mit bedacht verwendet werden da keine Deep-Copy erstellt wird und somit viele Parameter
    //! zwischen beiden instanzen geshared.
    Drawable(const Drawable& other);

    //! Konstruktor der gleich die geometry setzt.
    Drawable(Geometry* geometry);

    //! Funktion um eine Property des Drawables zu erhalten. Wenn es noch keine DrawableProperty mit dem angegebenen
    //! Datentyp existiert wir ein neue DrawableProperty erstellt und zurückgegeben.
    template <class T>
    inline T* getProperty();

    //! Setzt bzw. überschreibt die DrawableProperty mit dem gegebenen Type.
    template <class T>
    inline void setProperty(T* param);

    //! Gibt true zurück wenn dieses Drawable eine DrawableProperty vom angegebenen Datentype besitzt.
    template <class T>
    inline bool isPropertyValid();

    //! Entfernt die DrawableProperty mit dem angegebenen Datentype aus der Property liste des Drawables. Die
    //! DrawableProperty selbst wird nicht gelöscht da diese zwischen mehreren Drawables geshared werden kann uns somit
    //! mehrmals gelöscht werden würde.
    template <class T>
    inline bool delProperty();

    //! Füllen deaktivieren sodass die Geometry des Drawables im Wireframe-Modus gerendert wird.
    //! Standardmäsig wird die Gemetry gefüllt gerendert.
    void deactivateFill(){mPolyFill = false;}

    //! Gibt an ob das Poligon gefüllt (nicht im Wireframe-Modus) gerendert wird. \see deactivateFill()
    bool isPolyFilled() const {return mPolyFill;}

    //! Setzt die Shader Property des Drawables. Dies ist equivalent zu setProperty<Shader>(s)
    void setShader(Shader* s);

    //! Gibt die Shader Property des Drawable zurück. Dies ist equivalent zu getProperty<Shader>()
    Shader* getShader(){return getProperty<Shader>();}

    //! Setzt die Geometry Property des Drawables. Dies ist equivalent zu setProperty<Geometry>(g)
    void setGeometry(Geometry* g);

    //! Gibt die Geometry Property des Drawable zurück. Dies ist equivalent zu getProperty<Geometry>()
    Geometry* getGeometry(){return getProperty<Geometry>();}

    //! Setzt die Transparent Eigenschaft des Drawables. Transparente Drawables werden nachdem alle Opake Drawables
    //! gerendert wurden gerendert sodass diese korrekt die Opaken Objekte überblenden anstatt den Schwarzen
    //! (glClearColor) Hintergrund. Wenn sortiert gerendert wird (Standard) werden Transparente Objekte zusätzlich
    //! anhand ihrer tiefe sortiert sodass sich Transparente Objekte untereinander korrekt überlappen.
    void setTransparent(bool transparent = true);

    //! Gibt an ob das Drawable als Transparentes Drawable gehandhabt wird. \see setTransparent();
    bool isTransparent() const {return mIsTransparent;}

    //! Erlaubt das setzen, ob das Drawable im aktuellen Render-Durchlauf gezeichnet werden soll.
    //! Wird z.B. deaktiviert, wenn Env.- oder Shadow-Maps gebaut werden sollen.
    //! Beziht sich nur auf das Drawable selbst nicht auf seine Kinder.
    void setEnabled(bool aBool) {mEnabled = aBool;}

    //! Gibt an ob das Drawable gezeichnet werden soll. \see setEnabled()
    bool isEnabled() const {return mEnabled;}

    //! Gibt die interne Propertyliste welche die eigenschaften des Drawables beinhaltet zurück.
    const PropListT& getPropertyList() const {return mPropList;}

    //! Gibt einen ShapeBuffer zurück der eine pointer auf das aktuelle Shape enhält. Jedes Shape entehlt einen
    //! pointer auf das jeweils nächste Shape wobei das letzte Shape wiederum einen poiter auf das erste Shape enthält.
    //! Während des traversieren des Szenengraphs wird der Buffer jeweils auf das nächste shape inerhalb der liste
    //! gesetzt. Auserhalb der Traversierung/Renderings ist das aktuelle Shape das erste Shape der liste. Die Shapes
    //! inerhalb des Buffers haben keine feste reihenfolge so werden neue sapes einfach an das aktuelle Shape angehängt.
    //! Eine konstante reihenfolge wird nicht benötigt da inerhalb der ShapeList ein pointer auf das jeweilige Shape
    //! gespeichert wird und die Shapes hauptzächlich als temporärer speicher für die ModelViewMatrizen verwendet
    //! werden.
    ShapeBuffer& getShapes() {return mShapes;}

    //! Den TraverseIndex wieder auf 0 setzen, sodass bei der nächsten Traversierung des Baumes korrekt gezählt wird.
    //! Dies wird während des sortierten renderns aufgerufen. \see ShapeList::renderShapes()
    void resetTraverseIndex() {mTraverseIndex = 0;}

protected:
    //! Wird aufgerufen wenn das Node traversiert wird bevor die kinder des Nodes traversiert werden.
    //! Ruft unter anderem SGObjectVisitor::beforeTraverseChilds() auf. \see SGObjectVisitor::beforeTraverseChilds()
    virtual void beforeTraverseChilds(SGObjectVisitor& visitor) override;

private:
    //! Die Property-List implementiert eine Art von Delegate. Hierfür gäbe es performantere Wege. Dieser
    //! Weg bietet den Vorteil, dass die Properties dieser Klasse zum Übersetzungszeitpunkt nicht bekannt sein müssen.
    //! Im Verlauf der Übungen werden über Color und Transformation hinaus weitere Properties eingeführt werden.
    //! Dies kann dann erledigt werden ohne die Implementierung dieser Klasse zu ändern. Daher diese Konstruktion.
    //! Notiz am Rande: In der STL gibt es leider keine Hashmap, die die Reihenfolge des Einfügens von Elementen später
    //! weiter verwenden/erhalten kann. Die Properties werden in der Map nach Ihrer id sortiert. Man muss daher
    //! aufpassen bei Properties, bei denen es auf die Reihenfolge des Renderings ankommt.
    PropListT mPropList;

    //! Flag für gefülltes Zeichnen.
    bool mPolyFill;

    //! Flag für transparente Drawables. \see setTransparent()
    bool mIsTransparent;

    //! Zeigt an, ob das Drawable im aktuellen Render-Durchlauf gezeichnet werden soll.
    //! Wird z.B. deaktiviert, wenn Env.- oder Shadow-Maps gebaut werden sollen
    bool mEnabled;

    //! \see Shape::getShapes()
    ShapeBuffer mShapes;

    //! anhand des Traverse indexes wird bestimmen wie oft dieses Drawable inerhalb des baumes vorhanden ist um die
    //! entsprechende anzahl an Shapes zu erstellen. Er wird beim traversieren hochgezählt und beim render dann wieder
    //! auf 0 gesetzt.
    int mTraverseIndex;

    //! Seperate Funktion zur Initialisierung des Drawables um keinen dubliziertern Code in den Konstruktoren zu
    //! benötigen.
    void init();

    //! Wird intern aufgerufen wenn eine DrawableProperty ersetzt oder hinzugefügt wird. Ruft
    //! ShapeList::onDrawablePropertyChanged() von allen ShapeListen auf in denen das Drawable enthalten ist.
    void drawablePropertyChanged(DrawableProperty& property = *static_cast<DrawableProperty*>(nullptr));

    ///! Überladung von drawablePropertyChanged(DrawableProperty* property = nullptr) für Geometrien
    void drawablePropertyChanged(Geometry& geometry);

    //! Wird intern aufgerufen wenn sich mIsTransparent ändert. Ruft ShapeList::onTransparencyChanged() von allen
    //! ShapeListen auf in denen das Drawable enthalten ist.
    void transparencyChanged();

    //! Funktion nur zur verwendung in SortedRenderingSGObjectVisitor erhöt den Traverse index um 1 und gibt den neuen
    //! wert zurück.
    //! \see mTraverseIndex
    int incrementTraverseIndex() {return ++mTraverseIndex;}

    //! SortedRenderingSGObjectVisitor erlauben incrementTraverseIndex() aufzurufen.
    friend class SortedRenderingSGObjectVisitor;

private:
    //! Die representation des Drawables in der Physik Engine. \see PhysicObject
    PhysicObject* mPhysicObject;

    //! Gibt an ob das PhysicObject Statisch (nicht durch die Physik bewegt wird) ist.
    bool mIsStatic;

    //! Temporäre matrix. Wird für manipulatePhysic() benötigt.
    QMatrix4x4 mTempMatrix;

    //! Initialisierungsmethode der Physik spezivischen variablen.
    void initPhysic();

    //! Aktualisiert die Modelmatrix des Drawable aus der Matrix, die die Physik-Engine berechnet hat
    bool manipulatePhysic();

public:
    //! Interfaceimplementierung für Physik
    virtual void setPhysicObject(PhysicObject* physicObject) override;
    virtual void setModelMatrix(const QMatrix4x4& modelMatrix) override;
    virtual void setStaticGeometry(bool isStatic) override;
    virtual QMatrix4x4& getModelMatrix() override;
    virtual const QMatrix4x4& getModelMatrix() const override;
    virtual PhysicObject* getPhysicObject() override;
    virtual bool isStaticGeometry() override;
    virtual QVector<QVector4D> getTriangleList() override;
};

template <class T>
inline T* Drawable::getProperty()
{
    DrawablePropertyId id = T::getPropertyId();
    DrawableProperty*& property = mPropList[id];
    if (!property)
    {
        // map::operator[] fügt einen nullptr zur map hinzu wenn die id nicht gefunden wurde. Somit kann der pointer
        // direkt (über die referenz) verändert werden, um eine neue Property einzufügen, ohne das ein weiterer zugriff
        // auf die map benötigt wird.
        property = new T();
        drawablePropertyChanged(*static_cast<T*>(property));
    }

    return static_cast<T*>(property);
}

template <class T>
inline void Drawable::setProperty(T* param)
{
    DrawablePropertyId id = T::getPropertyId();
    DrawableProperty*& currentProperty = mPropList[id];
    if (currentProperty != param)
    {
        // map::operator[] fügt einen nullptr zur map hinzu wenn die id nicht gefunden wurde. Somit kann der pointer
        // direkt (über die referenz) verändert werden, um eine neue Property einzufügen, ohne das ein weiterer zugriff
        // auf die map benötigt wird.
        currentProperty = param;
        drawablePropertyChanged(*param);
    }
}

template <class T>
inline bool Drawable::isPropertyValid()
{
    PropListT::iterator ret;
    DrawablePropertyId id = T::getPropertyId();
    ret = mPropList.find(id);
    if (ret == mPropList.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}

template <class T>
inline bool Drawable::delProperty()
{
    DrawablePropertyId id = T::getPropertyId();
    drawablePropertyChanged();
    return ((bool)mPropList.erase(id));
}

#endif // DRAWABLE_H
