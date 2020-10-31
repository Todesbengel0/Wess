#ifndef VERTEXFORMATMANAGER_H
#define VERTEXFORMATMANAGER_H

#include <QtGlobal>
#include <QVector>
#include "vertexformat.h"

//! Die VertexFormatManager Klasse verwaltet Vertexformate und sorgt dafür, dass ein gegebenes Vertexformat stets derselben Adresse zugeordnet wird.
//! Dadurch wird ein Zeigervergleich zwischen zwei Vertexformaten möglich.
class VertexFormatManager
{
public:
    //! Die VertexFormatManager Klasse darf nicht instanziiert werden, da sie statisch ist.
    VertexFormatManager() = delete;

    //! Gibt Adresse abhängig von gesetzten Formaten zurück.
    //! Zwei unterschiedliche vertexFormate, in denen dieselben Formate gesetzt sind, bekommen dieselbe Adresse zugewiesen.
    static VertexFormat* getVertexFormat(VertexFormat vertexFormat);

private:
    static QVector<VertexFormat> sVertexFormats;
};

#endif // VERTEXFORMATMANAGER_H
