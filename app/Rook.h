#pragma once

#include "Figure.h"

class Rook : public Figure
{
public:
	Rook();
	~Rook();

    bool ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board) override;

protected:
	QString GetMeshFilePath() const override;
};
