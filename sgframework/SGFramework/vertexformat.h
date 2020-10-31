#ifndef VERTEXFORMAT_H
#define VERTEXFORMAT_H

#include <QVector>
#include "drawableproperty.h"
#include "flags.h"

//! Die VertexFormat Klasse enthält Informationen über genutzte Vertexformate einer Geometrie.
class VertexFormat : public DrawableProperty
{
public:
    //! Dieses enum bestimmt die Vertexformate.
    enum Format
    {
        // Enums keine Werte zuweisen. Werden als Bitmaske verwendet
        FormatPosition,
        FormatNormal,
        FormatTextureCoodinate,
        FormatColor,
        FormatIndex,

        FormatCount // Muss immer an letzter Stelle stehen. Bezeichnet die Anzahl vorhandener Vertex Formate
    };

    //! Erzeugt ein neues VertexFormat.
    VertexFormat();

    //! Fügt das Vertexformat \param format hinzu.
    void addVertexFormat(Format format) {mFormats.addFlag(format);}

    //! Entfernt das Vertexformat \param format.
    void removeVertexFormat(Format format) {mFormats.removeFlag(format);}

    //! Gibt true zurück, falls das Vertexformat \param format gesetzt ist. Gibt sonst false zurück
    bool hasVertexFormat(Format format) const {return mFormats.hasFlagSet(format);}

    //! Gibt Vertexformate als Flags zurück.
    int getFormats() const {return mFormats.getFlags();}

    //! Gibt DrawablePropertyId zurück.
    virtual DrawablePropertyId getId() const override;

    //! Gibt DrawablePropertyId zurück.
    static DrawablePropertyId getPropertyId() {return DrawablePropertyIdGeter::getIdOf<VertexFormat>();}

private:
    Flags<Format> mFormats;
};

//inline void VertexFormat::addVertexFormat(Format format)
//{
//    mFormats.addFlag(format);
//}

//inline void VertexFormat::removeVertexFormat(Format format)
//{
//    mFormats.removeFlag(format);
//}

//inline bool VertexFormat::hasVertexFormat(Format format) const
//{
//    return mFormats.hasFlagSet(format);
//}

//inline int VertexFormat::getFormats() const
//{
//    return mFormats.getFlags();
//}

//inline DrawablePropertyId VertexFormat::getPropertyId()
//{
//    return DrawablePropertyIdGeter::getIdOf<VertexFormat>();
//}

#endif // VERTEXFORMAT_H
