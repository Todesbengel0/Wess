#pragma once

#include "Figure.h"

class King : public Figure
{
public:
	King();
	~King();

    bool ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board) override;

protected:
	QString GetMeshFilePath() const override;
	void OnInit() override;
};
