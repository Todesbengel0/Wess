#ifndef DRAWABLEPROPERTYID
#define DRAWABLEPROPERTYID

//! ein typesicherer integer
enum class DrawablePropertyId : int
{
    Shader,
    Texture,
    Geometry,

    //! die anzahl der vordefinierten variablen mehr werden mithilfe von DrawablePropertyIdGeter generiert
    Size
};

class Shader;
class Texture;
class Geometry;

//! Die DrawablePropertyIdGeter Klasse dient als DrawablePropertyId generator.
//! Sie weist einem gegebenen Templatetyp über die Methode getIdOf eine eindeutige DrawablePropertyId zu.
//! Die generierte DrawablePropertyId ist dabei abhängig von der Aufrufreihenfolge der Methode getIdOf. \n
//! Beispiel 1:
//! \code
//! DrawablePropertyIdGeter::getIdof<Geometry>(); // Weist Klasse Geometry die DrawablePropertyId 1 zu und gibt
//!                                               // DrawablePropertyId 1 zurück.
//! DrawablePropertyIdGeter::getIdof<Shader>(); // Weist Klasse Shader die DrawablePropertyId 2 zu und gibt
//!                                             // DrawablePropertyId 2 zurück.
//! \endcode
//! Beispiel 2:
//! \code
//! DrawablePropertyIdGeter::getIdof<Shader>(); // Weist Klasse Shader die DrawablePropertyId 1 zu und gibt
//!                                             // DrawablePropertyId 1 zurück.
//! DrawablePropertyIdGeter::getIdof<Shader>(); // Gibt DrawablePropertyId 1 zurück.
//! DrawablePropertyIdGeter::getIdof<Geometry>(); // Weist Klasse Geometry die DrawablePropertyId 2 zu und gibt
//!                                               // DrawablePropertyId 2 zurück.
//! \endcode
class DrawablePropertyIdGeter
{
private:
    // löschen sowie instazieren verbieten
    DrawablePropertyIdGeter() = delete;
    ~DrawablePropertyIdGeter() = delete;

public:
    //! Weist gegebenem Templatetyp eine eindeutige DrawablePropertyId zu und gibt sie zurück.
    //! Die Funktion ist teilweise spezialisiert sodass beispielsweise der Geometry die id DrawablePropertyId::Geometry
    //! zugewiesen wird. Dies wird benötigt, um einen der Sortieralgorithmen zu implementieren, der es erfordert dass die
    //! Properties anhand ihres Aufwands beim Wechseln sortiert sind.
    template <typename T>
    static inline DrawablePropertyId getIdOf();

private:
    //! Gibt bei jedem aufruf eine um 1 erhöte id zurück. Die ersten DrawablePropertyId::Size werden übersprungen.
    static inline DrawablePropertyId getNextId();
};

inline DrawablePropertyId DrawablePropertyIdGeter::getNextId()
{
    static int id(static_cast<int>(DrawablePropertyId::Size)); // die ersten DrawablePropertyId::Size sind reserviert
    return static_cast<DrawablePropertyId>(++id);
}

template <typename T>
inline DrawablePropertyId DrawablePropertyIdGeter::getIdOf()
{
    static const DrawablePropertyId id(getNextId());
    return id;
}

template <>
inline DrawablePropertyId DrawablePropertyIdGeter::getIdOf<Shader>()
{
    static const DrawablePropertyId id(DrawablePropertyId::Shader);
    return id;
}

template <>
inline DrawablePropertyId DrawablePropertyIdGeter::getIdOf<Texture>()
{
    static const DrawablePropertyId id(DrawablePropertyId::Texture);
    return id;
}

template <>
inline DrawablePropertyId DrawablePropertyIdGeter::getIdOf<Geometry>()
{
    static const DrawablePropertyId id(DrawablePropertyId::Geometry);
    return id;
}

#endif // DRAWABLEPROPERTYID
