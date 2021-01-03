#include "stdafx.h"
#include "King.h"
#include "Chessboard.h"

King::King() : Figure(5){}

King::~King() = default;

bool King::ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board)
{
    if (abs(xStartPosition-xTargetPosition) <= 1 && abs(zStartPosition-zTargetPosition) <= 1)
        return true;
    bool isWhite = this->GetFieldColor()==White;
    if ((xStartPosition==4 && zStartPosition==(isWhite?0:7)) && abs(xStartPosition-xTargetPosition) == 2 && zStartPosition-zTargetPosition == 0)
    {
        if(xStartPosition-xTargetPosition == -2)
        {
            if (board->GetFigure(5, zStartPosition) || board->GetFigure(6, zStartPosition) || !board->GetFigure(7, zStartPosition))
                return false;
            Figure* figure = board->GetFigure(7, zStartPosition);
            if (figure->GetTypeID()!=3 || (figure->GetFieldColor()==White)!=isWhite)
                return false;
            board->SetFigureOnFieldNoLogic(7, zStartPosition, 5, zStartPosition);
            figure->MovePosition(-board->GetFieldSize()*2, 0);
            return true;
        }
        else
        {
            if (board->GetFigure(3, zStartPosition) || board->GetFigure(2, zStartPosition) || board->GetFigure(1, zStartPosition) || !board->GetFigure(0, zStartPosition))
                return false;
            Figure* figure = board->GetFigure(0, zStartPosition);
            if (figure->GetTypeID()!=3 || (figure->GetFieldColor()==White)!=isWhite)
                return false;
            board->SetFigureOnFieldNoLogic(0, zStartPosition, 3, zStartPosition);
            figure->MovePosition(board->GetFieldSize()*3, 0);
            return true;
        }
    }
    return false;
}

QString King::GetMeshFilePath() const
{
    return ":/models/king.obj";
}

void King::OnInit()
{
	GetRotationTrafo().rotate(90.0f, 0.0f, 1.0f, 0.0f);
}
