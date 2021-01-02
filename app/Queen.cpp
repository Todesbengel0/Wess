#include "stdafx.h"
#include "Queen.h"

Queen::Queen() = default;

Queen::~Queen() = default;

bool Queen::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition)
{
    if(xStartPosition == xTargetPosition || zStartPosition == zTargetPosition || abs(xStartPosition-xTargetPosition) == abs(zStartPosition-zTargetPosition))
        return true;
    return false;
}

QString Queen::GetMeshFilePath() const
{
    return ":/models/queen.obj";
}
