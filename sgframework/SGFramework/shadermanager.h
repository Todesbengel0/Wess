#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QHash>
#include <QDir>
#include "shader.h"
#include "drawablepropertyid.h"

//! Die ShaderManager Klasse erzeugt und verwaltet Shader.\n
//! Sie sorgt dafür, dass maximal nur eine Instanz eines Shaders existiert.\n
//! Beispiel:
//! \code
//! Shader* shader = ShaderManager::getShader("basic.vert", "basic.frag"); // Erzeugt und gibt einen Shader vom
//!                                                                        // Typ "Shader" zurück
//! shader = ShaderManager::getShader("basic.vert", "basic.frag"); // Gibt Shader vom Typ "Shader" zurück. Da dieser
//!                          // bereits existiert wird dieselbe Adresse wie bei obigem Funktionsaufruf zurückgegeben
//! Shader* shader2 = ShaderManager::getShader("phong.vert", "phong.frag"); // Erzeugt und gibt einen Shader vom
//!                                                                         // Typ "Shader" zurück
//! MyShader* shader = ShaderManager::getShader<MyShader>("basic.vert", "basic.frag") // Erzeugt und gibt einen Shader
//!                                                                                   // vom Typ "MyShader" zurück
//! \endcode
class ShaderManager
{
public:
    //! Gibt Shader von gegebenem Templatetyp zurück.
    //! Falls noch kein Shader für den gebenenen Templatetyp sowie für die gegebenen Parameter existiert, wird ein
    //! Shader erzeugt.
    template <typename T = Shader>
    inline static T* getShader(const QString& vertexShaderPath, const QString& fragmentShaderPath,
                               const QString& geometryShaderPath = QString(),
                               const QString& tesselationControlShaderPath = QString(),
                               const QString& tesselationEvaluationShaderPath = QString());

private:
    ShaderManager() = delete;

    //! Ein Shader pro kombination von Shader-Dateien sowie Typen.
    static QHash<QString, Shader*> sShaders;

    //! Der aktuelle Ordner (Working-Directory) von dem aus die Pfade der Shader bestimmt werden. So sind die Pfade
    //! eindeutig und nicht zu lang.
    static QDir sDir;
};

template <typename T>
inline T* ShaderManager::getShader(const QString& vertexShaderPath, const QString& fragmentShaderPath,
                                   const QString& geometryShaderPath, const QString& tesselationControlShaderPath,
                                   const QString& tesselationEvaluationShaderPath)
{
    // Durch id wird erreicht dass wenn zwei Shader mit den gleichen Dateien jedoch unterschiedlichen Klassen erstellt
    // werden diese unterschiden werden können sihe Beispiel in ShaderManager dokumentation. Die id ist eindeutig pro
    // von Shader abgeleitete Klasse.
    const int id = static_cast<int>(DrawablePropertyIdGeter::getIdOf<T>());
    const QString key = QString::number(id) + sDir.relativeFilePath(vertexShaderPath) + sDir.relativeFilePath(fragmentShaderPath) +
                        sDir.relativeFilePath(geometryShaderPath) + sDir.relativeFilePath(tesselationControlShaderPath) +
                        sDir.relativeFilePath(tesselationEvaluationShaderPath);
    Shader*& shader = sShaders[key];
    if (!shader)
    {
        shader = new T;
        shader->initialize(vertexShaderPath, fragmentShaderPath, geometryShaderPath, tesselationControlShaderPath,
                           tesselationEvaluationShaderPath);
    }

    return static_cast<T*>(shader);
}

#endif // SHADERMANAGER_H
