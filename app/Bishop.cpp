#include "stdafx.h"
#include "Bishop.h"

Bishop::Bishop() = default;

Bishop::~Bishop() = default;

QString Bishop::GetMeshFilePath() const
{
    return ":/models/bishop.obj";
}
