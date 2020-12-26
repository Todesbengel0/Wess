#pragma once

#include "Figure.h"

class Bishop : public Figure
{
public:
	Bishop();
	~Bishop();

protected:
	QString GetMeshFilePath() const override;
};
