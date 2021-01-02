#include "stdafx.h"
#include "Queen.h"
#include "Chessboard.h"

Queen::Queen() = default;

Queen::~Queen() = default;

bool Queen::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if(xStartPosition == xTargetPosition || zStartPosition == zTargetPosition || abs(xStartPosition-xTargetPosition) == abs(zStartPosition-zTargetPosition))
        return true;
    return false;
}

QString Queen::GetMeshFilePath() const
{
    return ":/models/queen.obj";
}
