#pragma once

//////////////////////////////////////////////////////////////////////////
// Klasse: Chessboard
// Beschreibung: Hauptklasse des Schachbretts
// Autor: Vinzenz Funk
//////////////////////////////////////////////////////////////////////////

class Chessboard
{
// ctor / dtor
public:
	Chessboard(float size, float height);
	~Chessboard();

// initialization
public:
	Node* Init();
private:
	void MakeSideFrame(Node* nSideFrameCenter, float frame_thickness, float frame_height);

// getters / setters
public:
	Transformation* GetRootTrafo() const;

// members
private:
	const float mSize;
	const float mHeight;

// scene objects
private:
	// root of the chessboard
	Transformation* mtRoot;

	// bottom base frame of the board
	SimpleCube* mgBaseFrame;
	Drawable* mdBaseFrame;
	Transformation* mtBaseFrame;

	// side frame
	Transformation* mtSideFrameCenter;
	SimpleCube* mgSideFrame;
	Drawable* mdSideFrame;
	Transformation* mtsSideFrame;
};
