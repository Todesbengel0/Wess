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
    short figureValue = deadFigure->GetTypeID();
    for (int j = 0; j < 6; ++j)
    {
        if (!mFigures[j][0])
        {
            mFigures[j][0] = deadFigure;
            MoveFigure(deadFigure, j, 0);
            return;
        }
        short graveyardFValue = mFigures[j][0]->GetTypeID();
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
