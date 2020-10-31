#include "modeltransformation.h"

void ModelTransformation::turnOn(Shader* /*shader*/)
{
    activate();
}

void ModelTransformation::turnOff(Shader* /*shader*/)
{
    deActivate();
}

DrawablePropertyId ModelTransformation::getId() const
{
    return getPropertyId();
}
