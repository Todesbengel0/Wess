#include "shadermanager.h"
#include "shader.h"

QHash<QString, Shader*> ShaderManager::sShaders;
QDir ShaderManager::sDir;
