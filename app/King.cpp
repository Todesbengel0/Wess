#include "stdafx.h"
#include "King.h"

King::King() = default;

King::~King() = default;

bool King::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition)
{
    if (abs(xStartPosition-xTargetPosition) <= 1 && abs(zStartPosition-zTargetPosition) <= 1)
        return true;
    return false;
}

QString King::GetMeshFilePath() const
{
    return ":/models/king.obj";
}

void King::OnInit()
{
	GetRotationTrafo().rotate(90.0f, 0.0f, 1.0f, 0.0f);
}
