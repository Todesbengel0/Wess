#include "stdafx.h"
#include "Bishop.h"
#include "Chessboard.h"

Bishop::Bishop() = default;

Bishop::~Bishop() = default;

bool Bishop::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if (abs(xStartPosition-xTargetPosition) == abs(zStartPosition-zTargetPosition))
        return true;
    return false;
}

QString Bishop::GetMeshFilePath() const
{
    return ":/models/bishop.obj";
}
