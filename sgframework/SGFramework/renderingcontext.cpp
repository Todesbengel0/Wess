#include "renderingcontext.h"
#include "shadermanager.h"
#include "sunlight.h"
#include "pointlight.h"
#include "spotlight.h"
#include <QtMath>

const int RenderingContext::sMaxLights = 8;
const int RenderingContext::sUniformBufferBinding = 1;

RenderingContext::~RenderingContext()
{
}

void RenderingContext::setRenderConfig(RenderConfiguration *pConfig)
{
//    if (mRenderConfig != NULL)
//        delete mRenderConfig;
    mRenderConfig = pConfig;
}

void RenderingContext::setCamera(Camera *pCamera)
{
//    if (mCam != NULL)
//        delete mCam;
    mCam = pCamera;
}

void RenderingContext::addActiveLight(SunLight* light)
{
    if (mActiveSunLights.size() >= sMaxLights)
    {
        qDebug() << "RenderingContext::addActiveLight: Konnte Licht nicht hinzufügen. Maximale Anzahl an möglichen Sonnenlichtern überschritten.";
    }
    else
    {
        mActiveSunLights.append(light);
    }
}

void RenderingContext::resetContext()
{
    mModelMatrix.setToIdentity();
    mActiveSunLights.clear();
    mActivePointLights.clear();
    mActiveSpotLights.clear();
}

void RenderingContext::addActiveLight(PointLight *light)
{
    if (mActivePointLights.size() >= sMaxLights)
    {
        qDebug() << "RenderingContext::addActiveLight: Konnte Licht nicht hinzufügen. Maximale Anzahl an möglichen Punktlichtern überschritten.";
    }
    else
    {
        mActivePointLights.append(light);
    }
}

void RenderingContext::addActiveLight(SpotLight *light)
{
    if (mActiveSpotLights.size() >= sMaxLights)
    {
        qDebug() << "RenderingContext::addActiveLight: Konnte Licht nicht hinzufügen. Maximale Anzahl an möglichen Spotlichtern überschritten.";
    }
    else
    {
        mActiveSpotLights.append(light);
    }
}

void RenderingContext::setUniforms(QOpenGLShaderProgram* s)
{
    QOpenGLShaderProgram& shaderProgram = *s;
    if (mUniformBuffer.isCreated())
    {
        // Uniform buffer existiert bereits => Aktualisiere Lichtdaten
        mUniformBuffer.addShaderProgram(shaderProgram);
        bool haveSunLightsChanged = haveSunLightsChanges();
        bool havePointLightsChanged = havePointLightsChanges();
        bool haveSpotLightsChanged = haveSpotLightsChanges();
        bool haveLightsAnyChanges = haveSunLightsChanged | havePointLightsChanged | haveSpotLightsChanged;
        if (haveLightsAnyChanges && mUniformBuffer.bind())
        {
            const int sunLightsUniformCount = (Light::ChangeFlagCount + SunLight::ChangeFlagCount) * sMaxLights;
            const int pointLightsUniformCount = (Light::ChangeFlagCount + PointLight::ChangeFlagCount) * sMaxLights;
            if (haveSunLightsChanged)
            {
                updateSunLightsBufferData(0);
            }

            if (havePointLightsChanged)
            {
                updatePointLightsBufferData(sunLightsUniformCount);
            }

            if (haveSpotLightsChanged)
            {
                updateSpotLightsBufferData(sunLightsUniformCount + pointLightsUniformCount);
            }

            mUniformBuffer.release();
        }
    }
    else
    {
        // Uniform buffer existiert noch nicht => Erzeuge uniform buffer und setze Daten.
        createUniformBuffer(shaderProgram);
        if (mUniformBuffer.bind())
        {
            const int sunLightsUniformCount = (Light::ChangeFlagCount + SunLight::ChangeFlagCount) * sMaxLights;
            const int pointLightsUniformCount = (Light::ChangeFlagCount + PointLight::ChangeFlagCount) * sMaxLights;
            updateSunLightsBufferData(0);
            updatePointLightsBufferData(sunLightsUniformCount);
            updateSpotLightsBufferData(sunLightsUniformCount + pointLightsUniformCount);

            mUniformBuffer.release();
        }
    }

    mUniformBuffer.bindBase();
    shaderProgram.setUniformValue("sunLightCount", mActiveSunLights.size());
    shaderProgram.setUniformValue("pointLightCount", mActivePointLights.size());
    shaderProgram.setUniformValue("spotLightCount", mActiveSpotLights.size());
}

void RenderingContext::setBlending()
{
    glFunctions->glEnable(GL_BLEND);
    glFunctions->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderingContext::init()
{
    mCam = new Camera();
    mRenderConfig = new RenderConfiguration();
    mShadowLight = NULL;
    Q_INIT_RESOURCE(Shader);

    mDefaultShader = ShaderManager::getShader(QString(":/shader/basic.vert"), QString(":/shader/basic.frag"));
}

void RenderingContext::createUniformBuffer(QOpenGLShaderProgram& shaderProgram)
{
    const int sunLightsUniformCount = (Light::ChangeFlagCount + SunLight::ChangeFlagCount) * sMaxLights;
    const int pointLightsUniformCount = (Light::ChangeFlagCount + PointLight::ChangeFlagCount) * sMaxLights;
    const int spotLightsUniformCount = (Light::ChangeFlagCount + SpotLight::ChangeFlagCount) * sMaxLights;
    const int uniformCount = sunLightsUniformCount + pointLightsUniformCount + spotLightsUniformCount;
    int index = -1;
    const GLchar* uniformNames[uniformCount];
    QVector<QByteArray> names;
    names.reserve(uniformCount);

    // Uniformnamen von Sonnenlichtern
    for (int i = 0; i < sMaxLights; ++i)
    {
        names.push_back(QString("sunLights[%1].ambient").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("sunLights[%1].diffuse").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("sunLights[%1].specular").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("sunLights[%1].viewDirection").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
    }

    // Uniformnamen von Punktlichtern
    for (int i = 0; i < sMaxLights; ++i)
    {
        names.push_back(QString("pointLights[%1].ambient").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("pointLights[%1].diffuse").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("pointLights[%1].specular").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("pointLights[%1].viewPosition").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("pointLights[%1].attenuations").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
    }

    // Uniformnamen von Spotlichtern
    for (int i = 0; i < sMaxLights; ++i)
    {
        names.push_back(QString("spotLights[%1].ambient").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("spotLights[%1].diffuse").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("spotLights[%1].specular").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("spotLights[%1].viewPosition").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("spotLights[%1].viewDirection").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("spotLights[%1].attenuations").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("spotLights[%1].falloffAngle").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
        names.push_back(QString("spotLights[%1].falloffExponent").arg(i).toLatin1());
        uniformNames[++index] = names.last().constData();
    }

    mUniformBuffer.create(shaderProgram, "LightUniforms", uniformNames);
}

void RenderingContext::updateSunLightsBufferData(int indexOffset)
{
    int index = indexOffset - 1;
    for (int i = 0; i < mActiveSunLights.size(); ++i)
    {
        const SunLight* light = mActiveSunLights[i];
        mUniformBuffer.write(++index, &light->getAmbient(), sizeof(light->getAmbient()));
        mUniformBuffer.write(++index, &light->getDiffuse(), sizeof(light->getDiffuse()));
        mUniformBuffer.write(++index, &light->getSpecular(), sizeof(light->getSpecular()));
        mUniformBuffer.write(++index, &light->getViewDirection(), sizeof(light->getViewDirection()));
    }
}

void RenderingContext::updatePointLightsBufferData(int indexOffset)
{
    int index = indexOffset - 1;
    for (int i = 0; i < mActivePointLights.size(); ++i)
    {
        const PointLight* light = mActivePointLights[i];
        mUniformBuffer.write(++index, &light->getAmbient(), sizeof(light->getAmbient()));
        mUniformBuffer.write(++index, &light->getDiffuse(), sizeof(light->getDiffuse()));
        mUniformBuffer.write(++index, &light->getSpecular(), sizeof(light->getSpecular()));
        mUniformBuffer.write(++index, &light->getViewPosition(), sizeof(light->getViewPosition()));
        mUniformBuffer.write(++index, &light->getAttenuations(), sizeof(light->getAttenuations()));
    }
}

void RenderingContext::updateSpotLightsBufferData(int indexOffset)
{
    int index = indexOffset - 1;
    for (int i = 0; i < mActiveSpotLights.size(); ++i)
    {
        const SpotLight* light = mActiveSpotLights[i];

        // Konvertiere zu cos vom Winkel, um Berechnung im Shader zu vermeiden.
        const float falloffAngle = std::cos(light->getFalloffAngle() * static_cast<float>(M_PI) / 180.0f);

        const float falloffExponent = light->getFalloffExponent();
        mUniformBuffer.write(++index, &light->getAmbient(), sizeof(light->getAmbient()));
        mUniformBuffer.write(++index, &light->getDiffuse(), sizeof(light->getDiffuse()));
        mUniformBuffer.write(++index, &light->getSpecular(), sizeof(light->getSpecular()));
        mUniformBuffer.write(++index, &light->getViewPosition(), sizeof(light->getViewPosition()));
        mUniformBuffer.write(++index, &light->getViewDirection(), sizeof(light->getViewDirection()));
        mUniformBuffer.write(++index, &light->getAttenuations(), sizeof(light->getAttenuations()));
        mUniformBuffer.write(++index, &falloffAngle, sizeof(falloffAngle));
        mUniformBuffer.write(++index, &falloffExponent, sizeof(falloffExponent));
    }
}

bool RenderingContext::haveSunLightsChanges() const
{
    bool hasChanges = false;
    for (int i = 0; i < mActiveSunLights.size(); ++i)
    {
        SunLight* light = mActiveSunLights[i];
        if (light->hasChanges())
        {
            hasChanges = true;
            light->acceptChanges();
        }
    }

    return hasChanges;
}

bool RenderingContext::havePointLightsChanges() const
{
    bool hasChanges = false;
    for (int i = 0; i < mActivePointLights.size(); ++i)
    {
        PointLight* light = mActivePointLights[i];
        if (light->hasChanges())
        {
            hasChanges = true;
            light->acceptChanges();
        }
    }

    return hasChanges;
}

bool RenderingContext::haveSpotLightsChanges() const
{
    bool hasChanges = false;
    for (int i = 0; i < mActiveSpotLights.size(); ++i)
    {
        SpotLight* light = mActiveSpotLights[i];
        if (light->hasChanges())
        {
            hasChanges = true;
            light->acceptChanges();
        }
    }

    return hasChanges;
}

void RenderingContext::update()
{
    mRenderConfig->update(mCam);
    mCam->setNoMatrixCalcNeeded();
}
