#include "stdafx.h"
#include "Chessboard.h"

Chessboard::Chessboard(float size, float height)
	: mSize(size)
	, mHeight(height)
	, mtRoot(nullptr)
	, mnRoot(nullptr)
{
}

Chessboard::~Chessboard()
{
	delete mdBaseFrame;
	delete mgBaseFrame;

	//delete mnRoot;	// free'd by root node!
	delete mtRoot;
}

Node* Chessboard::Init()
{
	mtRoot = new Transformation;
	mnRoot = new Node(mtRoot);

	// bottom base frame of the board
	mgBaseFrame = new SimpleCube(mSize, mHeight, mSize);
	mdBaseFrame = new Drawable(mgBaseFrame);
	mnBaseFrame = new Node(mdBaseFrame);
	mnRoot->addChild(mnBaseFrame);

	return mnRoot;
}

Node* Chessboard::GetRoot() const
{
	return mnRoot;
}

Transformation* Chessboard::GetRootTrafo() const
{
	return mtRoot;
}
