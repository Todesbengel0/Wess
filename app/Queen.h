#pragma once

#include "Figure.h"

class Queen : public Figure
{
public:
	Queen();
	~Queen();

    FigureType GetType() const override;

    bool ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board) override;

protected:
	QString GetMeshFilePath() const override;
};
