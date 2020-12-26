#pragma once

#include "Figure.h"

class Rook : public Figure
{
public:
	Rook();
	~Rook();

protected:
	QString GetMeshFilePath() const override;
};
