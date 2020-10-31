#include "vertexformatmanager.h"

VertexFormat* VertexFormatManager::getVertexFormat(VertexFormat vertexFormat)
{
    Q_ASSERT_X(vertexFormat.getFormats() < sVertexFormats.size(),
               "VertexFormatManager::getVertexFormat", "Konnte Adresse von VertexFormat nicht ermitteln. VertexFormat außerhalb der gültigen Vertexformate");
    VertexFormat& v = sVertexFormats[vertexFormat.getFormats()];
    v = vertexFormat;
    return &v;
}

QVector<VertexFormat> VertexFormatManager::sVertexFormats((1 << VertexFormat::FormatCount));


