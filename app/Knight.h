#pragma once

#include "Figure.h"

class Knight : public Figure
{
public:
	Knight();
	~Knight();

    FigureType GetType() const override;

    bool ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board) override;

protected:
	QString GetMeshFilePath() const override;
	void OnInit() override;
};
