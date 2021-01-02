#pragma once

class WoodenShader;
class WoodenPatternMap;
class WoodenBumpMap;
class WoodenPiece
{
public:
	WoodenPiece();
	virtual ~WoodenPiece();

protected:
	void Init(Drawable* draw, QVector3D base_color, float shininess);

public:
	WoodenShader* GetShader();

private:
	Material* mMat;
	WoodenShader* mShader;

	static WoodenPatternMap* msPatternMap;
	static WoodenBumpMap* msBumpMap;
};
