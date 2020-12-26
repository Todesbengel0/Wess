#ifndef RENDERINGSTATE_H
#define RENDERINGSTATE_H

#include "shader.h"
#include "geometry.h"
#include "drawable.h"

//! Diese Klasse managed den status der aktuell gebundenen Buffer, Shader sowie Uniforms die über DrawableProperties
//! gesetzt werden und aktiviert bzw. deaktiviert benötigte bzw. nicht mehr benötigte anhand des Status.
class RenderingState
{
public:
    //! gibt die statische instanz dieser Klasse zurück
    static RenderingState& getInstance();

    //! bindet das Drawable sowie desen Properties, Shader und die Geometry. Sodass anschließend lediglich noch die
    //! algemeinen Uniforms gebunden werden müssen befor die Geometry gerendert werden kann.
    void bindDrawable(Drawable& drawable, Shader& shader, Geometry& geometry);

    //! setzt den status zurück sodass alle Uniforms/Shader/VertexArrayObjects/DrawableProperty deaktiviert werden und
    //! fals benötigt beim rendern des nächsten Drawables wieder aktiviert werden. Die Methode wird einmal pro frame
    //! befor irgend ein Renderer anfängt zu rendern aufgerufen damit eventuell geänderte Uniforms aktualieisert werden.
    //! So würde sich in in sehr einfachen Szenen z.B. wenn man nur Drawables die den selben ShaderTimed verwenden,
    //! verwendet, dessen uniform Time nicht aktualiesert werden.
    void resetState();

private:
    //! privater standard konstruktor da die instanz statisch erzeugt wird. um zu verhindern das eine instanz auserhalb
    //! von dieser Klasse erstellt wird.
    RenderingState() = default;

    //! bindent den shader und macht ihn zum aktuellen Shader (mCurrentShader).
    void bindShader(Shader& shader);

    //! bindet den buffer der Geometry und macht sie zur aktuellen Gemoetry (mCurrentGeometry). falls es bereits eine
    //! aktuelle geometry gibt wird dessen buffer deaktiviert (ist in den meisten fällen eigendlich nicht nötig).
    void bindDrawableGeometry(Geometry& geometry, Shader& shader);

    //! alle properties die neu dazukommen oder sich verändern im vergleich zum zuvorigen Drawable werden aktiviert bzw.
    //! deaktiviert wenn sie im neuen nicht mehr benötigt werden. Das erkennen ob eine Propertie neu dazu gekomen ist und
    //! somit aktiviert werden muss bzw. nicht mehr vorhanden ist und deshalb deaktiviert werden muss wird durch
    //! sortieren der porperties anhand der PropertyId erledigt. So muss der algorithmus keine liniare suche
    //! durchführen. Die properties des Drawables liegen immer soriert vor da sie inerhalb eine std::map abgeleght sind.
    //! Um zu verhindern dass beim deaktivieren einer Property eine Uniform-Variable die durch eine neu hinzugekommene
    //! Property gesetzt wurde wieder umgesetzt wird werden zuerst alle nicht mehr benötigten Properties deaktivirt und
    //! anschlißend alle neuen Properties aktiviert.
    void activateProperties(Drawable& drawable);

    //! alle properies müssen neu gebunden werden wenn der shader sich ändert dabmit die uniforms gesetzt werden. hir
    //! werden die aktuellen properties nur deaktivirt da sie dann später (in activateProperties) wieder aktivirt werden
    //! falls sie für dieses drawabele benötigt werden. Es wird für jede aktuelle Propertie turnOff mit 'shader'
    //! aufgerufen.
    void deactivateProperties(Shader* shader);

    //! setzt den polygon fill mode (glPolygonMode) wenn sich dieser vom aktuellen Modus unterscheidet
    void setPolygonFillMode(const Drawable& drawable);

    //! bestimmt ob eine DrawableProperty vom type id inerhalb von activateProperties aktiviert werden soll oder nicht.
    //! Aktuell werden Shader, ModelTransformationen sowie Geometrien seperat aktiviert.
    //! Shader müssen als erstes gebunden werden weshalb diese direkt in bindDrawable gebunden werden.
    //! ModelTransformationen werden ignoriert da diese schon während des traversierens angewendet wurden.
    //! Geometrien benötigen beim binden das drawable um an die property Color dieser zu kommen. und somit die
    //! VertexCollors auf den entsperchenden wert zu setzen. Die turnOn methode bekommt jedoch nur den shader mit da
    //! properties nur pro shader gebunden werden. Somit werden Geometrien inerhalb von bindDrawable bzw.
    //! bindDrawableGeometry gebunden. Auch wird ein Pointer auf die Geometry zum rendern benotigt sie ist also schon
    //! bekannt und kann einfach gebunden werden.
    bool needsActivation(DrawablePropertyId id);

    //! der aktuell gebundene Shader um eine veränderung des Shaders zu erkennen.
    Shader* mCurrentShader = nullptr;

    //! die aktuell gebundene Geometry um eine veränderung der Geometry zu erkennen.
    Geometry* mCurrentGeometry = nullptr;

    //! ein pointer auf die properties des zuletzt gerenderten drawables um zu bestimmen welche properties neu
    //! hinzugekommen sind und welche schon vorhanden sind.
    const PropListT* mCurrentProperties = nullptr;

    //! der aktuelle polygon fill mode (glPolygonMode)
    bool mCurrentPolygonIsFilled = true;

    //! Temporäres Array zur verwendung in activateProperties(). Als Membervariable implementiert um unötige
    //! allokationen zu vermeiden.
    QVector<DrawableProperty*> mTempPropertiesToActivate;

    //! eine statische instanz des rendering states da dieser nur einma existieren darf da der Status was gerade auf der
    //! Grafikkarte gebunden ist ansonsten ohne das wissen einer anderen instance wissen verändert werden würde.
    static RenderingState sInstance;
};

#endif // RENDERINGSTATE_H
