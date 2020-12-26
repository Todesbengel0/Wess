#pragma once

#include "Figure.h"

class Queen : public Figure
{
public:
	Queen();
	~Queen();

protected:
	QString GetMeshFilePath() const override;
};
