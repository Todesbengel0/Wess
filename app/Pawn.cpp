#include "stdafx.h"
#include "Pawn.h"

Pawn::Pawn() = default;

Pawn::~Pawn() = default;

QString Pawn::GetMeshFilePath() const
{
    return ":/models/pawn.obj";
}
