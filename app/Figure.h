#pragma once


enum FigureColor {
	white, black
};

class Figure
{
public: 
	Figure(FigureColor color);
	~Figure();

	void SetPosition(int x, int y);

private:
	float mSize;
	FigureColor mColor;

	Transformation mtPosition;
	Drawable* mdFigure;
	Geometry* mgMesh;
};

