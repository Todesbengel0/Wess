#include "renderingstate.h"
#include "modeltransformation.h"

RenderingState RenderingState::sInstance;

RenderingState& RenderingState::getInstance()
{
    return sInstance;
}

void RenderingState::bindDrawable(Drawable& drawable, Shader& shader, Geometry& geometry)
{
    if (mCurrentShader != &shader)
    {
        // auch wenn der shader gewechselt wird müssen die werte der uniforms wieder zurück gesetzt werden da sie beim
        // erneuten binden sonst falsche werte enthalten können
        if (mCurrentShader)
        {
            deactivateProperties(mCurrentShader);
        }

        bindShader(shader);
        activateProperties(drawable);

        // Shaderspezifische Uniforms setzen
        mCurrentShader->setShaderUniforms();

        // Geometrie muss neu gebunden werden, wenn sich der shader verändert (aufgrund potentiell unterschiedlichen
        // attribute locations)
        bindDrawableGeometry(geometry, shader);
    }
    else
    {
        activateProperties(drawable);

        if (mCurrentGeometry != &geometry)
        {
            // Geometrie hat sich verändert => binde neue geometrie
            bindDrawableGeometry(geometry, shader);
        }
    }

    setPolygonFillMode(drawable);
}

void RenderingState::resetState()
{
    if (mCurrentGeometry)
    {
        // zuvor gebundene buffer wieder freigeben
        mCurrentGeometry->unbindBuffer();
    }

    if (mCurrentShader)
    {
        deactivateProperties(mCurrentShader);
    }

    mCurrentGeometry = nullptr;
    mCurrentShader = nullptr;
}

void RenderingState::bindShader(Shader& shader)
{
    mCurrentShader = &shader;
    mCurrentShader->enableShader();
}

void RenderingState::bindDrawableGeometry(Geometry& geometry, Shader& shader)
{
    if (mCurrentGeometry)
    {
        // zuvor gebundene buffer wieder freigeben
        mCurrentGeometry->unbindBuffer();
    }

    mCurrentGeometry = &geometry;
    mCurrentGeometry->bindBuffer(shader);
}

void RenderingState::activateProperties(Drawable& drawable)
{
    const PropListT& newProperties = drawable.getPropertyList();
    PropListT::const_iterator newPropertiesIterator = newProperties.cbegin();
    if (mCurrentProperties) // wenn aktuell properties aktiv sind
    {
        // iteriere über beide maps und nutze aus dass sie sortiert sind um herauszufeinden ob eine Property neu
        // hinzugekommen ist, sich verändert hat, nicht mehr vorhanden ist oder noch benötigt wird.
        PropListT::const_iterator currentPropertiesIterator = mCurrentProperties->cbegin();
        while (newPropertiesIterator != newProperties.cend() && currentPropertiesIterator != mCurrentProperties->cend())
        {
            DrawablePropertyId newPropertyId = newPropertiesIterator->first;
            if (needsActivation(newPropertyId))
            {
                DrawablePropertyId currentPropertyId = currentPropertiesIterator->first;
                if (newPropertyId < currentPropertyId)
                {
                    // eine propertie mit dem gleichen type wie das in 'newPropertiesIterator' enthaltene property
                    // ist nicht in 'mCurrentProperties' enthalten, und wird deshalb aktiviert
                    DrawableProperty* newProperty = newPropertiesIterator->second;
                    mTempPropertiesToActivate.append(newProperty);
                    ++newPropertiesIterator;
                }
                else if (newPropertyId > currentPropertyId)
                {
                    // ein propertie mit einem anderen type wie das in 'newPropertiesIterator' enthaltene property ist
                    // noch in der liste, und wird deshalb deaktiviert
                    DrawableProperty* currentProperty = currentPropertiesIterator->second;
                    currentProperty->turnOff(mCurrentShader);
                    ++currentPropertiesIterator;
                }
                else
                {
                    // beide properties haben den selben type
                    DrawableProperty* newProperty = newPropertiesIterator->second;
                    DrawableProperty* currentProperty = currentPropertiesIterator->second;
                    if (newProperty != currentProperty)
                    {
                        // beide properties sind unterschiedlich also wird die aktuelle deaktiviert und die neuer
                        // aktiviert
                        currentProperty->turnOff(mCurrentShader);
                        mTempPropertiesToActivate.append(newProperty);
                    }
                    ++currentPropertiesIterator;
                    ++newPropertiesIterator;
                }
            }
            else
            {
                ++newPropertiesIterator;
            }
        }

        // alle übrigen alten properties deaktivieren
        while (currentPropertiesIterator != mCurrentProperties->cend())
        {
            DrawableProperty* currentProperty = currentPropertiesIterator->second;
            currentProperty->turnOff(mCurrentShader);
            ++currentPropertiesIterator;
        }

        // alle neuen Properties aktivieren
        for (int i = 0; i < mTempPropertiesToActivate.size(); ++i)
        {
            mTempPropertiesToActivate[i]->turnOn(mCurrentShader);
        }

        // liste leeren ohne mTempPropertiesToActivate.capacity() zu verändern um unötige allokationen zu vermeiden
        mTempPropertiesToActivate.erase(mTempPropertiesToActivate.begin(), mTempPropertiesToActivate.end());
    }

    // alle übrigen neuen properties aktivieren
    for (; newPropertiesIterator != newProperties.end(); ++newPropertiesIterator)
    {
        DrawableProperty* newProperty = newPropertiesIterator->second;
        DrawablePropertyId id = newPropertiesIterator->first;
        if (needsActivation(id))
        {
            newProperty->turnOn(mCurrentShader);
        }
    }

    // da nun alle properties aktiviert bzw. deaktivirt wurden die im neuen drawable nicht bzw. nicht mehr vorhanden
    // waren werden die properites zu den aktuellen properties.
    mCurrentProperties = &newProperties;
}

void RenderingState::deactivateProperties(Shader* shader)
{
    if (mCurrentProperties)
    {
        // Texturen müssten eigentlich nicht neu gebunden werden, wenn das shaderprogramm wechselt. Jedoch müssen die
        // uniforms neu gesetzt werden was ebenfalls in der turnOn metode passiert
        for (PropListT::const_iterator it = mCurrentProperties->cbegin(); it != mCurrentProperties->cend(); ++it)
        {
            DrawableProperty* currentProperty = it->second;
            currentProperty->turnOff(shader);
        }
    }

    // da alle deaktivirt wurden auf null setzen. somit werden beim aufrufen von aktivateProperties keine deaktiviert
    mCurrentProperties = nullptr;
}

void RenderingState::setPolygonFillMode(const Drawable& drawable)
{
#ifndef QT_OPENGL_ES // nicht verfügbra in von OpenGL ES
    if (drawable.isPolyFilled() == mCurrentPolygonIsFilled)
    {
        // polygon fill mode hat sich nicht geändert
        return;
    }

    // polygon fill mode verändern
    mCurrentPolygonIsFilled = drawable.isPolyFilled();
    if (mCurrentPolygonIsFilled)
    {
        glFunctions->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glFunctions->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
#else
    Q_UNUSED(drawable);
#endif
}

bool RenderingState::needsActivation(DrawablePropertyId id)
{
    return id != Shader::getPropertyId() && id != ModelTransformation::getPropertyId() && id != Geometry::getPropertyId();
}
