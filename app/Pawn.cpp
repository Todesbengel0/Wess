#include "stdafx.h"
#include "Pawn.h"
#include "Chessboard.h"

Pawn::Pawn() = default;

Pawn::~Pawn() = default;

FigureType Pawn::GetType() const
{
    return tPawn;
}

bool Pawn::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if (abs(xStartPosition-xTargetPosition)==0 && abs(zStartPosition-zTargetPosition)==0)
        return true;
    if (zStartPosition-zTargetPosition==((this->GetFieldColor()==White)?-1:1) && ((abs(xTargetPosition-xStartPosition)==0 && !board->GetFigure(xTargetPosition, zTargetPosition))))
        return true;
    if (zStartPosition-zTargetPosition==((this->GetFieldColor()==White)?-1:1) && abs(xTargetPosition-xStartPosition)==1)
    {
        if (board->GetFigure(xTargetPosition, zTargetPosition))
            return true;
        Figure* figure = board->GetFigure(xTargetPosition, zStartPosition);
        if (!figure || figure != board->GetSpeedyPawn())
            return false;
        board->SetFigureOnFieldNoLogic(xTargetPosition, zStartPosition, xTargetPosition, zTargetPosition);
        //figure->MovePosition(0, board->GetFieldSize()*(zStartPosition-zTargetPosition));
        return true;
    }
    if (!((zStartPosition == 1 || zStartPosition == 6) && zStartPosition-zTargetPosition==((this->GetFieldColor()==White)?-2:2) && xStartPosition-xTargetPosition==0))
        return false;
    if (board->GetFigure(xTargetPosition, zStartPosition + (zTargetPosition-zStartPosition)*0.5f))
        return false;
    return true;
}

QString Pawn::GetMeshFilePath() const
{
    return ":/models/pawn.obj";
}
