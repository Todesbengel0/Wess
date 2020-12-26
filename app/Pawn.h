#pragma once

#include "Figure.h"

class Pawn : public Figure
{
public:
	Pawn();
	~Pawn();

protected:
	QString GetMeshFilePath() const override;

};
