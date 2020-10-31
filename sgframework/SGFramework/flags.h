#ifndef FLAGS_H
#define FLAGS_H

#include <QtGlobal>

//! Die Flags Klasse enthält Informationen über Flagwerte vom Typ EnumType.
//! Sie bietet zusätzlich Funktionen zum Hinzufügen, Entfernen und überprüfen von Flags an. \n
//! Beispiel:
//! \code
//! Flags<int> flags; // Erstellt Instanz von Flags vom Typ int
//! flags.resetFlags(); // Setzt alle gesetzten Flags zurück
//! flags.addFlag(0); // Setzt das erste Bit von Flags auf 1
//! flags.addFlag(3); // Setzt das vierte Bit von Flags auf 1
//! flags.hasFlagSet(0); // Gibt true zurück
//! flags.hasFlagSet(3); // Gibt true zurück
//! flags.hasFlagSet(1); // Gibt false zurück
//! \endcode
template <typename EnumType>
class Flags
{
public:
    //! Erzeugt eine Instanz von Flags.
    //!
    //! Hinweis: Initial wird jedes Flag auf 1 gesetzt.
    Flags() {}

    //! Setzt alle gesetzten Flags auf 0.
    void resetFlags() {mFlags = 0;}

    //! Fügt \param flag den bereits gesetzten Flags hinzu.
    inline void addFlag(EnumType flag);

    //! Entfernt \param flag von gesetzten Flags.
    inline void removeFlag(EnumType flag);

    //! Gibt true zurück, falls \param flag gesetzt ist, sonst false.
    inline bool hasFlagSet(EnumType flag) const;

    //! Gibt true zurück, falls ein Bit gesetzt ist, sonst false.
    bool hasAnyFlagSet() const {return mFlags != 0;}

    //! Gibt den integralen Wert der Flags zurück.
    int getFlags() const {return mFlags;}

private:
    int mFlags = -1; // Aktiviere initial jedes Flag
};

template <typename EnumType>
inline void Flags<EnumType>::addFlag(EnumType flag)
{
    Q_ASSERT_X(flag < sizeof(mFlags) * 8, "Flags<EnumType>::addFlag", "Konnte flag nicht hinzufügen. Wert vom Flag überschreitet maximal erlaubten Wert. Max flag wert = 31");
    mFlags |= (1 << flag);
}

template <typename EnumType>
inline void Flags<EnumType>::removeFlag(EnumType flag)
{
    Q_ASSERT_X(flag < sizeof(mFlags) * 8, "Flags<EnumType>::removeFlag", "Konnte flag nicht entfernen. Wert vom Flag überschreitet maximal erlaubten Wert. Max flag wert = 31");
    mFlags &= ~(1 << flag);
}

template <typename EnumType>
inline bool Flags<EnumType>::hasFlagSet(EnumType flag) const
{
    Q_ASSERT_X(flag < sizeof(mFlags) * 8, "Flags<EnumType>::hasFlagSet", "Konnte flag nicht überprüfen. Wert vom Flag überschreitet maximal erlaubten Wert. Max flag wert = 31");
    const int f = (1 << flag);
    return (mFlags & f) == f;
}

//template <typename EnumType>
//inline Flags<EnumType>::Flags()
//{
//}

//template <typename EnumType>
//inline void Flags<EnumType>::resetFlags()
//{
//    mFlags = 0;
//}



//template <typename EnumType>
//inline bool Flags<EnumType>::hasAnyFlagSet() const
//{
//    return mFlags != 0;
//}

//template <typename EnumType>
//inline int Flags<EnumType>::getFlags() const
//{
//    return mFlags;
//}

#endif // FLAGS_H
