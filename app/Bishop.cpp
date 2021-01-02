#include "stdafx.h"
#include "Bishop.h"

Bishop::Bishop() = default;

Bishop::~Bishop() = default;

bool Bishop::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition)
{
    if (abs(xStartPosition-xTargetPosition) == abs(zStartPosition-zTargetPosition))
        return true;
    return false;
}

QString Bishop::GetMeshFilePath() const
{
    return ":/models/bishop.obj";
}
