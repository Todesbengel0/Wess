#pragma once

#include "Figure.h"


class Graveyard
{
//ctor / dtor
public:
    Graveyard(float zKoord, float firstPosition, float steps);
    ~Graveyard();

// initialization
public:
    Node* Init();

// public methods
public:
    void AddFigure(Figure* deadFigure);

//private methods
private:
    void ShuffleFigures(int startShuffle, int endShuffle);
    void MoveFigure(Figure* deadFigure, int xPosition, int zPosition);

//members
private:
    Figure* mFigures[6][8]; //Figuren auf dem Friedhof
    const float mZKoord;
    const float mFirstPosition;
    const float mSteps;

//scene objects
private:
    Transformation mtRoot;
};
