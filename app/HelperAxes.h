#pragma once

class HelperAxes
{
public:
	HelperAxes();
	~HelperAxes();

public:
	Node* Init();

public:
	Transformation* GetTrafo();

private:
	Transformation mtRoot;
	SimpleCube* mgArrow;
	Drawable* mdAxes[3];
	Transformation mtAxes[3];
};
