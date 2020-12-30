#include "stdafx.h"
#include "Chessboard.h"
#include "Selection.h"

// figures
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"

Chessboard::Chessboard(float size, float height)
	: mSize(size)
	, mHeight(height)
	, mgBaseFrame(nullptr)
	, mdBaseFrame(nullptr)
	, mgSideFrame(nullptr)
	, mdSideFrame(nullptr)
	, mgField(nullptr)
	, mSelection(nullptr)
{
	memset(mFigures, 0, sizeof mFigures);
}

Chessboard::~Chessboard()
{
	// figures
	Figure::FreeMeshes();
	for (auto& line : mFigures)
		for (auto figure : line)
			delete figure;

	// fields
	delete mgField;

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
	//mtTopFrameCenter.rotate(90.0f, 0.0f, 1.0f, 0.0f);
	//mtTopFrameCenter.scale(1.0f, 1.0f, -1.0f);
	mtTopFrameCenter.translate(0.0f, mHeight / 2.0f, 0.0f);	// pos relative to base frame
	auto nTopFrameCenter = new Node(&mtTopFrameCenter);
	MakeSideFrame(nTopFrameCenter, side_frame_thickness * mSize, field_height * mHeight);
	ntBaseFrame->addChild(nTopFrameCenter);

	// translated origin for fields, so (0,0) is bottom left
	// making subsequent positioning easier
	// not that in a right handed system a higher positive Z is closer (inverted)
	mtFieldRoot.translate(-3.5f * (field_width * mSize), 0.0f, 3.5f * (field_width * mSize));
	auto nFieldRoot = new Node(&mtFieldRoot);
	nTopFrameCenter->addChild(nFieldRoot);

	// fields
	MakeFields(nFieldRoot, field_width * mSize, field_height * mHeight);

	// figures
	mtFigureRoot.translate(0.0f, 0.5f * (field_height * mHeight), 0.0f);
	auto nFigureRoot = new Node(&mtFigureRoot);
	nFieldRoot->addChild(nFigureRoot);
	MakeFigures(nFigureRoot, field_width * mSize);

	//Selection
	mSelection = new Selection(float(field_width * mSize), this);
	nRoot->addChild(mSelection->Init());
	
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

void Chessboard::MakeFields(Node* nFigureRoot, float field_size, float frame_height)
{
	// geometry
	mgField = new SimpleCube(field_size, frame_height, field_size);

	float zPos = 0.0f;

	for (int z = 0; z < _countof(mFields); ++z)
	{
		float xPos = 0.0f;

		for (int x = 0; x < _countof(mFields[z]); ++x)
		{
			// black / white pattern
			ChessColor col = ((x + z) % 2) ? ChessColor::White : ChessColor::Black;

			auto node = mFields[z][x].Init(mgField, col, xPos, zPos);
			nFigureRoot->addChild(node);

			xPos += field_size;
		}

		zPos -= field_size;
	}
}

void Chessboard::MakeFigures(Node* nFigureRoot, float field_size)
{
	// pawns
	for (int x = 0; x < 8; ++x)
	{
		MakeFigure<Pawn>(x, 1, nFigureRoot, ChessColor::White, field_size);
		MakeFigure<Pawn>(x, 6, nFigureRoot, ChessColor::Black, field_size);
	}

	// rooks
	MakeFigure<Rook>(0, 0, nFigureRoot, ChessColor::White, field_size);
	MakeFigure<Rook>(7, 0, nFigureRoot, ChessColor::White, field_size);
	MakeFigure<Rook>(0, 7, nFigureRoot, ChessColor::Black, field_size);
	MakeFigure<Rook>(7, 7, nFigureRoot, ChessColor::Black, field_size);

	// knight
	MakeFigure<Knight>(1, 0, nFigureRoot, ChessColor::White, field_size);
	MakeFigure<Knight>(6, 0, nFigureRoot, ChessColor::White, field_size);
	MakeFigure<Knight>(1, 7, nFigureRoot, ChessColor::Black, field_size);
	MakeFigure<Knight>(6, 7, nFigureRoot, ChessColor::Black, field_size);

	// bishop
	MakeFigure<Bishop>(2, 0, nFigureRoot, ChessColor::White, field_size);
	MakeFigure<Bishop>(5, 0, nFigureRoot, ChessColor::White, field_size);
	MakeFigure<Bishop>(2, 7, nFigureRoot, ChessColor::Black, field_size);
	MakeFigure<Bishop>(5, 7, nFigureRoot, ChessColor::Black, field_size);

	// queen
	MakeFigure<Queen>(3, 0, nFigureRoot, ChessColor::White, field_size);
	MakeFigure<Queen>(3, 7, nFigureRoot, ChessColor::Black, field_size);

	// king
	MakeFigure<King>(4, 0, nFigureRoot, ChessColor::White, field_size);
	MakeFigure<King>(4, 7, nFigureRoot, ChessColor::Black, field_size);
}

const Transformation& Chessboard::GetRootTrafo() const
{
	return mtRoot;
}

ChessField* Chessboard::GetField(int x, int z)
{
	return &mFields[z][x];
}

Figure* Chessboard::GetFigure(int x, int z)
{
	if (mFigures[z][x] != nullptr) {
		return mFigures[z][x];
	}
	else return nullptr;
}
