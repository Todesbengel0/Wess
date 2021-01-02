#include "stdafx.h"
#include "Rook.h"
#include "Chessboard.h"

Rook::Rook() = default;

Rook::~Rook() = default;

bool Rook::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if (xStartPosition == xTargetPosition || zStartPosition == zTargetPosition)
        return true;
    return false;
}

QString Rook::GetMeshFilePath() const
{
    return ":/models/rook.obj";
}
