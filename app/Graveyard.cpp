#include "stdafx.h"
#include "Graveyard.h"
#include "Pawn.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Knight.h"

Graveyard::Graveyard(float zKoord, float firstPosition, float steps)
    : mZKoord(zKoord),
      mFirstPosition(firstPosition),
      mSteps(steps)
{
    memset(mFigures, 0, sizeof(mFigures));
}

Graveyard::~Graveyard()
{
    // figures
    Figure::FreeMeshes();
    for (auto& line : mFigures)
        for (auto figure : line)
            delete figure;
}

Node* Graveyard::Init()
{
    auto nRoot = new Node(&mtRoot);
    return nRoot;
}

void Graveyard::AddFigure(Figure* deadFigure)
{
    short figureValue = 0;
    if (dynamic_cast<Pawn*>(deadFigure))
        figureValue = 0;
    else if (dynamic_cast<Knight*>(deadFigure))
        figureValue = 1;
    else if (dynamic_cast<Bishop*>(deadFigure))
        figureValue = 2;
    else if (dynamic_cast<Rook*>(deadFigure))
        figureValue = 3;
    else if (dynamic_cast<Queen*>(deadFigure))
        figureValue = 4;
    else if (dynamic_cast<King*>(deadFigure))
        figureValue = 5;
    for (int j = 0; j < 6; ++j)
    {
        if (!mFigures[j][0])
        {
            mFigures[j][0] = deadFigure;
            MoveFigure(deadFigure, j, 0);
            return;
        }
        short graveyardFValue = 0;
        if (dynamic_cast<Pawn*>(mFigures[j][0]))
            graveyardFValue = 0;
        else if (dynamic_cast<Knight*>(mFigures[j][0]))
            graveyardFValue = 1;
        else if (dynamic_cast<Bishop*>(mFigures[j][0]))
            graveyardFValue = 2;
        else if (dynamic_cast<Rook*>(mFigures[j][0]))
            graveyardFValue = 3;
        else if (dynamic_cast<Queen*>(mFigures[j][0]))
            graveyardFValue = 4;
        else if (dynamic_cast<King*>(mFigures[j][0]))
            graveyardFValue = 5;
        if (graveyardFValue < figureValue)
            continue;
        if (graveyardFValue == figureValue)
           for (int i = 0; i < 8; ++i)
               if (!mFigures[j][i])
               {
                   mFigures[j][i] = deadFigure;
                   MoveFigure(deadFigure, j, i);
                   return;
               }
        ShuffleFigures(j, 5);
        mFigures[j][0] = deadFigure;
        MoveFigure(deadFigure, j, 0);
        return;
    }
}

void Graveyard::ShuffleFigures(int startShuffle, int endShuffle)
{
    for (; endShuffle > startShuffle; --endShuffle)
        for (int i = 0; i < 8; ++i)
        {
            if(!mFigures[endShuffle-1][i])
                break;
            mFigures[endShuffle][i] = mFigures[endShuffle-1][i];
            MoveFigure(mFigures[endShuffle][i], endShuffle, i);
        }
    for (int i = 0; i < 8; ++i)
        mFigures[startShuffle][i] = nullptr;
}

void Graveyard::MoveFigure(Figure *deadFigure, int xPosition, int zPosition)
{
    deadFigure->SetPosition(mFirstPosition + mSteps * xPosition, mZKoord + mSteps * zPosition);
}
