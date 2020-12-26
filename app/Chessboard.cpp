#include "stdafx.h"
#include "Chessboard.h"

Chessboard::Chessboard(float size, float height)
	: mSize(size)
	, mHeight(height)
	, mgBaseFrame(nullptr)
	, mdBaseFrame(nullptr)
	, mgSideFrame(nullptr)
	, mdSideFrame(nullptr)
{
}

Chessboard::~Chessboard()
{
	// side frame
	delete mdSideFrame;
	delete mgSideFrame;

	// bottom base frame of the board
	delete mdBaseFrame;
	delete mgBaseFrame;
}

Node* Chessboard::Init()
{
	// size ratios
	constexpr float field_height = 0.8f;
	constexpr float field_width = 0.11f;

	// calculated size ratios (do not change)
	constexpr float base_frame_height = 1.0 - field_height;
	constexpr float side_frame_thickness = (1.0f - 8.0f * field_width) / 2.0f;

	// root of the chessboard
	auto nRoot = new Node(&mtRoot);

	// bottom base frame of the board
	mgBaseFrame = new SimpleCube(mSize, base_frame_height * mHeight, mSize);
	mdBaseFrame = new Drawable(mgBaseFrame);
	mtBaseFrame.translate(0.0f, -(mHeight - base_frame_height) / 2.0f, 0.0f);	// pos relative to obj center
	auto ntBaseFrame = new Node(&mtBaseFrame);
	ntBaseFrame->addChild(new Node(mdBaseFrame));
	nRoot->addChild(ntBaseFrame);

	// surrounding side frames
	mtTopFrameCenter.translate(0.0f, mHeight / 2.0f, 0.0f);	// pos relative to base frame
	auto nTopFrameCenter = new Node(&mtTopFrameCenter);
	MakeSideFrame(nTopFrameCenter, side_frame_thickness * mSize, field_height * mHeight);
	ntBaseFrame->addChild(nTopFrameCenter);

	// fields
	MakeFields(nTopFrameCenter, field_width * mSize, field_height * mHeight);
	
	return nRoot;
}

void Chessboard::MakeSideFrame(Node* nTopFrameCenter, float frame_thickness, float frame_height)
{
	// sizes
	const float frame_width = mSize - frame_thickness;

	// geometry / drawable
	mgSideFrame = new SimpleCube(frame_width, frame_height, frame_thickness);
	mdSideFrame = new Drawable(mgSideFrame);

	// transformations for positioning
	// pos relative to side frame center
	for (int i = 0; i < 4; ++i)
	{
		// rotate each side frame around the center with same offset
		mtsSideFrame[i].rotate(i * 90.0f, 0.0f, 1.0f, 0.0f);
		mtsSideFrame[i].translate(frame_thickness / 2.0f, 0.0f, frame_width / 2.0f);

		// connect to center
		auto nSideFrame = new Node(&mtsSideFrame[i]);
		nSideFrame->addChild(new Node(mdSideFrame));
		nTopFrameCenter->addChild(nSideFrame);
	}
}

void Chessboard::MakeFields(Node* nTopFrameCenter, float field_size, float frame_height)
{
	// geometry
	mgField = new SimpleCube(field_size, frame_height, field_size);

	float zPos = -3.5f * field_size;

	for (int z = 0; z < _countof(mFields); ++z)
	{
		float xPos = -3.5f * field_size;

		for (int x = 0; x < _countof(mFields[z]); ++x)
		{
			ChessColor col = ((x + z) % 2) ? ChessColor::White : ChessColor::Black;

			auto node = mFields[z][x].Init(mgField, col, xPos, zPos);
			nTopFrameCenter->addChild(node);

			xPos += field_size;
		}

		zPos += field_size;
	}
}

const Transformation& Chessboard::GetRootTrafo() const
{
	return mtRoot;
}
