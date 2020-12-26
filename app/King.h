#pragma once

#include "Figure.h"

class King : public Figure
{
public:
	King();
	~King();

protected:
	QString GetMeshFilePath() const override;
};
