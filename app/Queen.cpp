#include "stdafx.h"
#include "Queen.h"
#include "Chessboard.h"

Queen::Queen() = default;

Queen::~Queen() = default;

bool Queen::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if (abs(xStartPosition-xTargetPosition) == abs(zStartPosition-zTargetPosition))
    {
        for (int i = 1; i < abs(xStartPosition-xTargetPosition); ++i)
            if(board->GetFigure(xStartPosition+i*((xStartPosition-xTargetPosition<0)?1:-1), zStartPosition+i*((zStartPosition-zTargetPosition<0)?1:-1)))
                return false;
        return true;
    }
    else if (xStartPosition == xTargetPosition)
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

QString Queen::GetMeshFilePath() const
{
    return ":/models/queen.obj";
}
