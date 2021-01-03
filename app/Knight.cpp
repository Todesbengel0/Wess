#include "stdafx.h"
#include "Knight.h"
#include "Chessboard.h"

Knight::Knight() = default;

Knight::~Knight() = default;

bool Knight::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if ((xStartPosition-xTargetPosition == 0 && zStartPosition-zTargetPosition == 0)
            || (abs(xStartPosition-xTargetPosition) == 1 && abs(zStartPosition-zTargetPosition) == 2)
            || (abs(xStartPosition-xTargetPosition) == 2 && abs(zStartPosition-zTargetPosition) == 1))
        return true;
    return false;
}

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
