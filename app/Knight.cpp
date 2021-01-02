#include "stdafx.h"
#include "Knight.h"

Knight::Knight() = default;

Knight::~Knight() = default;

QString Knight::GetMeshFilePath() const
{
    return ":/models/knight.obj";
}

void Knight::OnInit()
{
	// fix rotation
	float rot = 90.0f;
	if (GetInitialX() > 4)
		rot = -rot;
	GetRotationTrafo().rotate(rot, 0.0f, 1.0f, 0.0f);
}
