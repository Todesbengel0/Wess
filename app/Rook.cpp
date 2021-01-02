#include "stdafx.h"
#include "Rook.h"

Rook::Rook() = default;

Rook::~Rook() = default;

QString Rook::GetMeshFilePath() const
{
    return ":/models/rook.obj";
}
