#include "stdafx.h"
#include "Chessboard.h"

Chessboard::Chessboard(float size, float height)
	: mSize(size)
	, mHeight(height)
	, mtRoot(nullptr)
	, mgBaseFrame(nullptr)
	, mdBaseFrame(nullptr)
	, mtBaseFrame(nullptr)
	, mgSideFrame(nullptr)
	, mdSideFrame(nullptr)
	, mtSideFrameCenter(nullptr)
{
}

Chessboard::~Chessboard()
{
	// side frame
	delete[] mtsSideFrame;
	delete mdSideFrame;
	delete mgSideFrame;
	delete mtSideFrameCenter;

	// bottom base frame of the board
	delete mtBaseFrame;
	delete mdBaseFrame;
	delete mgBaseFrame;

	// root of the chessboard
	delete mtRoot;
}

Node* Chessboard::Init()
{
	// size ratios
	constexpr float field_height = 0.8f;
	constexpr float field_width = 0.11f;

	// calculated size ratios (do not change)
	constexpr float base_frame_height = 1.0 - field_height;
	constexpr float side_frame_thickness = 1.0f - 8.0f * field_width;

	// root of the chessboard
	mtRoot = new Transformation;
	auto nRoot = new Node(mtRoot);

	// bottom base frame of the board
	mgBaseFrame = new SimpleCube(mSize, base_frame_height * mHeight, mSize);
	mdBaseFrame = new Drawable(mgBaseFrame);
	mtBaseFrame = new Transformation;
	mtBaseFrame->translate(0.0f, -(mHeight - base_frame_height) / 2.0f, 0.0f);	// pos relative to obj center
	auto ntBaseFrame = new Node(mtBaseFrame);
	ntBaseFrame->addChild(new Node(mdBaseFrame));
	nRoot->addChild(ntBaseFrame);

	// surrounding side frames
	mtSideFrameCenter = new Transformation;
	mtSideFrameCenter->translate(0.0f, mHeight / 2.0f, 0.0f);	// pos relative to base frame
	auto nSideFrameCenter = new Node(mtSideFrameCenter);
	MakeSideFrame(nSideFrameCenter, side_frame_thickness * mSize, field_height * mHeight);
	ntBaseFrame->addChild(nSideFrameCenter);

	return nRoot;
}

void Chessboard::MakeSideFrame(Node* nSideFrameCenter, float frame_thickness, float frame_height)
{
	// sizes
	const float frame_width = mSize - frame_thickness;

	// geometry / drawable
	mgSideFrame = new SimpleCube(frame_width, frame_height, frame_thickness);
	mdSideFrame = new Drawable(mgSideFrame);

	// transformations for positioning
	// pos relative to side frame center
	mtsSideFrame = new Transformation[4];

	for (int i = 0; i < 4; ++i)
	{
		// rotate each side frame around the center with same offset
		mtsSideFrame[i].rotate(i * 90.0f, 0.0f, 1.0f, 0.0f);
		mtsSideFrame[i].translate(frame_thickness / 2.0f, 0.0f, frame_width / 2.0f);

		// connect to center
		auto nSideFrame = new Node(&mtsSideFrame[i]);
		nSideFrame->addChild(new Node(mdSideFrame));
		nSideFrameCenter->addChild(nSideFrame);
	}
}

Transformation* Chessboard::GetRootTrafo() const
{
	return mtRoot;
}
