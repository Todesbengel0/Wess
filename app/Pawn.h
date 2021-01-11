#pragma once

#include "Figure.h"

class Pawn : public Figure
{
public:
    Pawn();
    ~Pawn();

    bool ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board) override;

protected:
	QString GetMeshFilePath() const override;

};
