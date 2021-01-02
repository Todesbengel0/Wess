#pragma once

#include "Figure.h"

class Bishop : public Figure
{
public:
	Bishop();
	~Bishop();

    bool ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition) override;

protected:
	QString GetMeshFilePath() const override;
};
