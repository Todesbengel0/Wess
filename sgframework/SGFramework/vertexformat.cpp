#include "vertexformat.h"

DrawablePropertyId VertexFormat::getId() const
{
    return getPropertyId();
}

VertexFormat::VertexFormat()
{
    //setze Prio für das Sortieren der Shapelist
    setPriority(60);

    mFormats.resetFlags();
}

