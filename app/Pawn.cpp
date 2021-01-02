#include "stdafx.h"
#include "Pawn.h"

Pawn::Pawn() = default;

Pawn::~Pawn() = default;

bool Pawn::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition)
{
    if ((abs(zTargetPosition-zStartPosition)==1 && abs(xTargetPosition-xStartPosition)<=1)
            || ((zStartPosition == 1 || zStartPosition == 6) && abs(zStartPosition-zTargetPosition)==2 && xStartPosition-xTargetPosition==0))
        return true;
    return false;
}

QString Pawn::GetMeshFilePath() const
{
    return ":/models/pawn.obj";
}
