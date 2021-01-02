#pragma once

#include "Figure.h"

class Queen : public Figure
{
public:
	Queen();
	~Queen();

    bool ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition) override;

protected:
	QString GetMeshFilePath() const override;
};
