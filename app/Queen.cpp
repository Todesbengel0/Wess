#include "stdafx.h"
#include "Queen.h"

Queen::Queen() = default;

Queen::~Queen() = default;

QString Queen::GetMeshFilePath() const
{
    return ":/models/queen.obj";
}
