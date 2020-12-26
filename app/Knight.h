#pragma once

#include "Figure.h"

class Knight : public Figure
{
public:
	Knight();
	~Knight();

protected:
	QString GetMeshFilePath() const override;
};
