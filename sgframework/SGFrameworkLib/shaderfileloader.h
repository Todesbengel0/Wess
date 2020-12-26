#ifndef CGLSHADERFILELOADER_H
#define CGLSHADERFILELOADER_H

#include <string>
#include <QString>

/*!
    *	\brief Loader for OpenGL Shader source Files
    *
    *	This class loads source code for shaders and recursively resolves includes in the shader code
    *
    *	\author Sebastian Vesenmayer
    */
class ShaderFileLoader
{
public:
    static std::string loadShaderFromFile(std::string& filePath);
    static std::string loadShaderFromFile(const char* filePath);
    static QString loadShaderFromFileQt(const char* filePath);
    static QString loadShaderFromFileQt(const QString& filePath);
};
#endif // CGLSHADERFILELOADER_H
