#include "stdafx.h"
#include "Rook.h"
#include "Chessboard.h"

Rook::Rook() : Figure(3){}

Rook::~Rook() = default;

bool Rook::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if (xStartPosition == xTargetPosition)
    {
        for (int i = 1; i < abs(zStartPosition-zTargetPosition); ++i)
            if (board->GetFigure(xStartPosition, zStartPosition + i*((zStartPosition-zTargetPosition<0)?1:-1)))
                return false;
        return true;
    }
    else if (zStartPosition == zTargetPosition)
    {
        for (int i = 1; i < abs(xStartPosition-xTargetPosition); ++i)
            if (board->GetFigure(xStartPosition + i*((xStartPosition-xTargetPosition<0)?1:-1), zStartPosition))
                return false;
        return true;
    }
    return false;
}

QString Rook::GetMeshFilePath() const
{
    return ":/models/rook.obj";
}
