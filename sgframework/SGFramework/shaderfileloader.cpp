#include "shaderfileloader.h"
#include <fstream>
#include <vector>
#include <QFile>
#include <QDebug>

/*!
    *	this function compares two sequences of strings and returns true if tag is in the sequence of codeIterator
    *	\param tagIterator iterator of the sequence to search
    *	\param tag string of the sequence to search
    *	\param codeIterator iterator of the complete text
    */
bool compareTagWithCode(std::string::iterator& tagIterator, std::string& tag, std::string::iterator& codeIterator)
{
    bool parsingOk = true;
    int iteratorIterations = 0;
    while (parsingOk)
    {
        if (*tagIterator == *codeIterator)
        {
            tagIterator++;
            codeIterator++;
            iteratorIterations++;
        }
        else
        {
            parsingOk = false;
            for (int i = 0; i < iteratorIterations; i++)
            {
                codeIterator--;
            }
            tagIterator = tag.begin();
        }
        if (tagIterator == (--(tag.end())))
        {
            if (*tagIterator == *codeIterator)
            {
                codeIterator++;
            }
            else
            {
                parsingOk = false;
                for (int i = 0; i < iteratorIterations; i++)
                {
                    codeIterator--;
                }
            }
            tagIterator = tag.begin();
            break;
        }
    }
    return parsingOk;
}

/*!
 * this function parses a source string for include files
 * \param sourceString the reference to the source code which should be parsed
 * \param extraFilesToLoad reference to a vector of strings, which will hold the paths for the included shadercode
 * \param insertionPoints reference to a vector of integers, which will hold the insertion points for the include shader
 * code
 * \param defines reference to the defines already set by other shader source files, determines if the code in the file
 * will be loaded to the shader code to compile
*/
void parseForIncludeFiles(std::string& sourceString, std::vector<std::string>& extraFilesToLoad,
                          std::vector<int>& insertionPoints, std::vector<std::string>& defines)
{
    std::string includeTag("sginclude");
    std::string defineTag("sgdefine");
    std::string::iterator includeTagIterator = includeTag.begin();
    std::string::iterator defineTagIterator = defineTag.begin();
    std::string::iterator shaderCodeIterator = sourceString.begin();
    std::string::iterator endOfString = sourceString.end();
    const char* start = &*shaderCodeIterator;
    enum STATES
    {
        SEARCH_HASH,
        PARSE_FILE_NAME,
        GOTO_NEW_LINE,
        QUIT,
        PARSE_DEFINE
    };
    STATES state = SEARCH_HASH;
    while (state != QUIT)
    {
        switch (state)
        {
        case SEARCH_HASH:
            if (*shaderCodeIterator == '#')
            {
                shaderCodeIterator++;
                bool parsingOk;
                // Parse for include
                parsingOk = compareTagWithCode(includeTagIterator, includeTag, shaderCodeIterator);
                if (parsingOk)
                {
                    state = PARSE_FILE_NAME;
                    break;
                }
                // end parse include

                if (!parsingOk)
                {
                    // parse define
                    parsingOk = compareTagWithCode(defineTagIterator, defineTag, shaderCodeIterator);
                }
                if (parsingOk)
                {
                    state = PARSE_DEFINE;
                    break;
                    // TODO parse define name, compare with list, and add it if not there if allready defined break and
                    // clear files to load
                }
                // end parse define
                // Goto new line if no includes or defines
                if (!parsingOk)
                {
                    state = GOTO_NEW_LINE;
                }
            }
            else if (*shaderCodeIterator == ' ' || *shaderCodeIterator == '\n')
            {
                shaderCodeIterator++;
            }
            else
            {
                state = QUIT;
            }
            break;
        case PARSE_FILE_NAME:
            while (*shaderCodeIterator == ' ')
            {
                shaderCodeIterator++;
            }
            if (*shaderCodeIterator == '\"')
            {
                shaderCodeIterator++;
                std::string vewFileName;
                // read maximum of 250 characters for shader path
                for (int i = 0; i < 250; i++)
                {
                    if (*shaderCodeIterator != '\"')
                    {
                        vewFileName.push_back(*shaderCodeIterator);
                        shaderCodeIterator++;
                    }
                    else
                    {
                        break;
                    }
                }
                // delete chars till hashtag
                while (*shaderCodeIterator != '#')
                {
                    *shaderCodeIterator = ' ';
                    shaderCodeIterator--;
                }
                *shaderCodeIterator = ' ';

                unsigned long long insertionPoint = (unsigned long long)(&*shaderCodeIterator - start);

                extraFilesToLoad.push_back(vewFileName);
                insertionPoints.push_back((int)insertionPoint);

                state = GOTO_NEW_LINE;
            }
            else
            {
                state = QUIT;
            }
            break;
        case PARSE_DEFINE:
        {
            // string for new define
            std::string newDefine("");
            // go to right till define begins
            while (*shaderCodeIterator == ' ')
            {
                shaderCodeIterator++;
            }
            // read maximum of 250 characters for shader define
            for (int i = 0; i < 250; i++)
            {
                if (*shaderCodeIterator != ' ' && *shaderCodeIterator != '\n' && *shaderCodeIterator != '#')
                {
                    // if it begins with numbers cancel
                    if (i == 0 && *shaderCodeIterator >= '0' && *shaderCodeIterator <= '9')
                    {
                        break;
                    }
                    newDefine.push_back(*shaderCodeIterator);
                    shaderCodeIterator++;
                }
                else
                {
                    break;
                }
            }
            // Print error message if empty
            if (newDefine.empty())
            {
                // TODO print error message could not parse define in shader
            }
            // delete chars till hashtag
            shaderCodeIterator--; // decrement else the next character is deleted
            while (*shaderCodeIterator != '#')
            {
                *shaderCodeIterator = ' ';
                shaderCodeIterator--;
            }
            *shaderCodeIterator = ' ';

            // look if define is already in list
            bool defineNotSet = true;
            if (!newDefine.empty())
            {
                for (int i = 0, j = (int)defines.size(); i < j; i++)
                {
                    if (defines.at(i).compare(newDefine) == 0)
                        defineNotSet = false;
                }
            }
            // add it to defined list if not in list else clear includes and code because already defined
            if (defineNotSet)
            {
                defines.push_back(newDefine);
                state = GOTO_NEW_LINE;
            }
            else
            {
                sourceString.clear();
                extraFilesToLoad.clear();
                insertionPoints.clear();
                state = QUIT;
            }
            break;
        }
        case GOTO_NEW_LINE:
            while (*shaderCodeIterator != '\n')
            {
                shaderCodeIterator++;
                if (shaderCodeIterator == endOfString)
                {
                    state = QUIT;
                    break;
                }
            }
            if (state != QUIT)
            {
                state = SEARCH_HASH;
            }
            break;
        default:
            break;
        }
    }
}

/*!
    *	gets the path of the file to solve recursive calls when loading shader code includes
    *	\param filePath path to file of the source code
    *	\return relative path from execution folder
    */
std::string getPrePath(std::string filePath)
{
    while (*(--(filePath.end())) != '/')
    {
        filePath.erase(--(filePath.end()));
    }
    return filePath;
}

/*!
    *	recursive function to load all included shader files. inserts the included files automatically and build the
  *source for later compilation
    *	\param filePath path to file to load
    *	\param defines defines which are already loaded to determine if file is already included
    */
std::string loadShaderFromDefinedFiles(std::string& filePath, std::vector<std::string>& defines)
{
    // Optimized for Qt Resource files
    std::string shaderCode;

    QFile* inputFile = new QFile(filePath.c_str());

    // If path is not in the execution folder move up and try again for max 10 times
    for (int i = 0; i < 10; i++)
    {
        if (!inputFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            filePath.insert(0, "../");
            delete inputFile;
            inputFile = new QFile(filePath.c_str());
        }
        else
            break;
    }

    if (!inputFile->isOpen())
    {
        qDebug() << "could not open" << filePath.c_str() << "shader source file. returned empty string.";
        return shaderCode;
    }
    std::string prePath = getPrePath(filePath);

    shaderCode = QString(inputFile->readAll()).toStdString();
    inputFile->close();

    std::vector<std::string> extraFilesToLoad;
    std::vector<int> insertionPoints;

    parseForIncludeFiles(shaderCode, extraFilesToLoad, insertionPoints, defines);

    for (int i = 0, offsetChange = 0; i < (int)extraFilesToLoad.size(); i++)
    {
        std::string vewFilePath;
        // if shaders are in resources, includepaths must be absolute, because there are no unix paths
        if (*(prePath.begin()) == ':')
        {
            vewFilePath = extraFilesToLoad.at(i);
        }
        else
        {
            vewFilePath = prePath + extraFilesToLoad.at(i);
        }
        std::string codeToInsert = loadShaderFromDefinedFiles(vewFilePath, defines);
        shaderCode.insert(offsetChange + insertionPoints.at(i), codeToInsert);
        offsetChange += (int)codeToInsert.length();
    }
    return shaderCode;
}

/*!
    *	this function takes a the file path of a shader source file and returns it as a string object
    *	\param filePath location of the file as string
    *	\return shader source code
    */
std::string ShaderFileLoader::loadShaderFromFile(std::string& filePath)
{
    std::vector<std::string> defines;
    return loadShaderFromDefinedFiles(filePath, defines);
}

/*!
    *	this function calls loadShaderFromFile(std::string &filePath), but takes a const char* as argument
    *	\sa loadShaderFromFile(std::string &filePath)
    */
std::string ShaderFileLoader::loadShaderFromFile(const char* filePath)
{
    std::string stringFilePath(filePath);
    return loadShaderFromFile(stringFilePath);
}

/*!
 * \brief CGLShaderFileLoader::loadShaderFromFileQt
 * QString wrapper of loadShaderFromFile
 * \param filePath
 * \return
 */
QString ShaderFileLoader::loadShaderFromFileQt(const char* filePath)
{
    QString gtString = QString(loadShaderFromFile(filePath).c_str());
    return gtString;
}

/*!
 * \brief CGLShaderFileLoader::loadShaderFromFileQt
 * QString wrapper of loadShaderFromFile
 * \param filePath
 * \return
 */
QString ShaderFileLoader::loadShaderFromFileQt(const QString& filePath)
{
    QString gtString = QString(loadShaderFromFile(filePath.toStdString().c_str()).c_str());
    return gtString;
}
