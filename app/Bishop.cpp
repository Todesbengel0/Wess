#include "stdafx.h"
#include "Bishop.h"
#include "Chessboard.h"

Bishop::Bishop() = default;

Bishop::~Bishop() = default;

bool Bishop::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if (abs(xStartPosition-xTargetPosition) != abs(zStartPosition-zTargetPosition))
        return false;
    for (int i = 1; i < abs(xStartPosition-xTargetPosition); ++i)
        if(board->GetFigure(xStartPosition+i*((xStartPosition-xTargetPosition<0)?1:-1), zStartPosition+i*((zStartPosition-zTargetPosition<0)?1:-1)))
            return false;
    return true;
}

QString Bishop::GetMeshFilePath() const
{
    return ":/models/bishop.obj";
}
