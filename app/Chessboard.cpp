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

ChessBoard::ChessBoard(float size, float height)
	: mSize(size)
	, mHeight(height)
    , mfield_size(1)
    , mIsWhiteTurn(true)
    , mSpeedyPawn(nullptr)
	, mgBaseFrame(nullptr)
	, mdBaseFrame(nullptr)
	, mgSideFrame(nullptr)
	, mdSideFrame(nullptr)
	, mgField(nullptr)
	, mSelection(nullptr)
{
	memset(mFigures, 0, sizeof mFigures);
}

ChessBoard::~ChessBoard()
{
	// figures
	Figure::FreeMeshes();
	for (auto& line : mFigures)
		for (auto figure : line)
			delete figure;

    for (auto& plane: mPromotionFigures)
        for (auto& line : plane)
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

    // graveyards
    delete mBlackGraveyard;
    delete mWhiteGraveyard;
}

Node* ChessBoard::Init()
{
	// size ratios
	constexpr float field_height = 0.8f;
	constexpr float field_width = 0.11f;
	constexpr float graveyard_field_dist = 3.0f; // in #fields

	// calculated size ratios (do not change)
	constexpr float base_frame_height = 1.0 - field_height;
	constexpr float side_frame_thickness = (1.0f - 8.0f * field_width) / 2.0f;

	// root of the chessboard
	auto nRoot = new Node(&mtRoot);

	// bottom base frame of the board
	mgBaseFrame = new SimpleCube(mSize, base_frame_height * mHeight, mSize);
	mdBaseFrame = new Drawable(mgBaseFrame);
	QVector3D base_col(ColClamp(158), ColClamp(115), ColClamp(83));
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
    mfield_size = field_width * mSize;
    mtFieldRoot.translate(-3.5f * mfield_size, 0.0f, 3.5f * mfield_size);
	auto nFieldRoot = new Node(&mtFieldRoot);
	nTopFrameCenter->addChild(nFieldRoot);

	// fields
    MakeFields(nFieldRoot, mfield_size, field_height * mHeight);

	// figures
	mtFigureRoot.translate(0.0f, 0.5f * (field_height * mHeight), 0.0f);
	auto nFigureRoot = new Node(&mtFigureRoot);
	nFieldRoot->addChild(nFigureRoot);
    MakeFigures(nFigureRoot, mfield_size);

	//Selection
    mSelection = new Selection(mfield_size, this);
    nFieldRoot->addChild(mSelection->Init());

    // Graveyards
	// positions are relative to A1 at (0,0)
    mWhiteGraveyard = new Graveyard(-(7.0f + graveyard_field_dist) * mfield_size, 7.0f * mfield_size, -mfield_size);
    mBlackGraveyard = new Graveyard(graveyard_field_dist * mfield_size, 0.0f, mfield_size);
    nRoot->addChild(mWhiteGraveyard->Init());
    nRoot->addChild(mBlackGraveyard->Init());
	
	return nRoot;
}

void ChessBoard::MakeSideFrame(Node* nTopFrameCenter, float frame_thickness, float frame_height)
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

void ChessBoard::MakeFields(Node* nFigureRoot, float field_size, float frame_height)
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

void ChessBoard::MakeFigures(Node* nFigureRoot, float field_size)
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

    // Promotion presets
    for (int i = 0; i < 8; ++i)
    {
        Figure* figure;
        Node* node;
        figure = new Queen;
        node = figure->Init(ChessColor::White, 4, 4);
        figure->SetPosition(-500, -500);	// Z inverted on RHS
        nFigureRoot->addChild(node);
        mPromotionFigures[0][3][i] = figure;

        figure = new Queen;
        node = figure->Init(ChessColor::Black, 4, 4);
        figure->SetPosition(-500, -500);	// Z inverted on RHS
        nFigureRoot->addChild(node);
        mPromotionFigures[1][3][i] = figure;

        figure = new Rook;
        node = figure->Init(ChessColor::White, 4, 4);
        figure->SetPosition(-500, -500);	// Z inverted on RHS
        nFigureRoot->addChild(node);
        mPromotionFigures[0][2][i] = figure;

        figure = new Rook;
        node = figure->Init(ChessColor::Black, 4, 4);
        figure->SetPosition(-500, -500);	// Z inverted on RHS
        nFigureRoot->addChild(node);
        mPromotionFigures[1][2][i] = figure;

        figure = new Bishop;
        node = figure->Init(ChessColor::White, 4, 4);
        figure->SetPosition(-500, -500);	// Z inverted on RHS
        nFigureRoot->addChild(node);
        mPromotionFigures[0][1][i] = figure;

        figure = new Bishop;
        node = figure->Init(ChessColor::Black, 4, 4);
        figure->SetPosition(-500, -500);	// Z inverted on RHS
        nFigureRoot->addChild(node);
        mPromotionFigures[1][1][i] = figure;

        figure = new Knight;
        node = figure->Init(ChessColor::White, 4, 4);
        figure->SetPosition(-500, -500);	// Z inverted on RHS
        nFigureRoot->addChild(node);
        mPromotionFigures[0][0][i] = figure;

        figure = new Knight;
        node = figure->Init(ChessColor::Black, 4, 4);
        figure->SetPosition(-500, -500);	// Z inverted on RHS
        nFigureRoot->addChild(node);
        mPromotionFigures[1][0][i] = figure;
    }
}

const Transformation& ChessBoard::GetRootTrafo() const
{
	return mtRoot;
}

ChessField* ChessBoard::GetField(int x, int z)
{
	return &mFields[z][x];
}

Figure* ChessBoard::GetFigure(int x, int z)
{
	if (mFigures[z][x] != nullptr) {
		return mFigures[z][x];
	}
	else return nullptr;
}

bool ChessBoard::SetFigureOnField(int x, int z, int tox, int toz)
{
    // check if x, z, tox and toz are valid koordinates
    if (x < 0 || x > 7 || z < 0 || z > 7 || tox < 0 || tox > 7 || toz < 0 || toz > 7)
        return false;

    // check if figure only wants to be deselected (no movement)
    if (x==tox && z==toz)
        return true;

    // check if figure has correct color
    if (mFigures[z][x]->GetFieldColor()!=(mIsWhiteTurn?White:Black))
        return false;

    // check if figure can actually move that way
    if (!mFigures[z][x]->ValidMovement(x, z, tox, toz, this))
        return false;

    // place, if space not occupied already
    if (mFigures[toz][tox] == nullptr)
	{
		mFigures[toz][tox] = mFigures[z][x];
        mFigures[z][x] = nullptr;
        mIsWhiteTurn = !mIsWhiteTurn;

        if(mFigures[toz][tox]->GetTypeID()==0 && abs(toz-z)==2)
            mSpeedyPawn = mFigures[toz][tox];
        else
            mSpeedyPawn = nullptr;
		return true;
	}

	// check figure can't slain one of their own
	if (mFigures[toz][tox]->GetFieldColor() == mFigures[z][x]->GetFieldColor())
		return false;

	// teleports slain piece of the board, can later be replaced by a death funktion
    if (mFigures[toz][tox]->GetFieldColor() == ChessColor::White)
        mWhiteGraveyard->AddFigure(mFigures[toz][tox]);
    else if (mFigures[toz][tox]->GetFieldColor() == ChessColor::Black)
		mBlackGraveyard->AddFigure(mFigures[toz][tox]);

	mFigures[toz][tox] = mFigures[z][x];
    mFigures[z][x] = nullptr;
    mIsWhiteTurn = !mIsWhiteTurn;
    mSpeedyPawn = nullptr;
    return true;
}

bool ChessBoard::SetFigureOnFieldNoLogic(int x, int z, int tox, int toz)
{
    // check if x, z, tox and toz are valid koordinates
    if (x < 0 || x > 7 || z < 0 || z > 7 || tox < 0 || tox > 7 || toz < 0 || toz > 7)
        return false;
    mFigures[toz][tox] = mFigures[z][x];
    mFigures[z][x] = nullptr;
    return true;
}

float ChessBoard::GetFieldSize()
{
    return mfield_size;
}

Figure* ChessBoard::GetSpeedyPawn()
{
    return mSpeedyPawn;
}

void ChessBoard::Promote(int x, int z, int type)
{
    if(!mFigures[z][x])
        return;
    auto color = mFigures[z][x]->GetFieldColor();
    if(mFigures[z][x]->GetTypeID()!=0 || z!=((color==White)?7:0))
        return;
    Figure* pawn = mFigures[z][x];
    Figure* promotedFigure = nullptr;
    for (int i = 0; i < 8; ++i)
        if ((promotedFigure = mPromotionFigures[(color==White)?0:1][type-1][i])->GetTypeID()!=0)
        {
            mFigures[z][x] = promotedFigure;
            mPromotionFigures[(color==White)?0:1][type-1][i] = pawn;
            break;
        }
    pawn->SetPosition(-500, -500);
    promotedFigure->SetPosition(x*mfield_size, -z*mfield_size);
}
