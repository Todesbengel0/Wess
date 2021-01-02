#include "stdafx.h"
#include "King.h"

King::King() = default;

King::~King() = default;

QString King::GetMeshFilePath() const
{
    return ":/models/king.obj";
}

void King::OnInit()
{
	GetRotationTrafo().rotate(90.0f, 0.0f, 1.0f, 0.0f);
}
