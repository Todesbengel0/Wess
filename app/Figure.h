#pragma once

#include "ChessFieldActor.h"
#include "WoodenPiece.h"

class ChessBoard;

class Figure : public ChessFieldActor, public WoodenPiece
{
// ctor / dtor
public: 
	Figure();
    Figure(int typeID);
	~Figure();

// initialization
public:
	Node* Init(ChessColor color, int initialX = -1, int initialZ = -1);
	static void FreeMeshes();
protected:
	virtual QString GetMeshFilePath() const = 0;
	virtual void OnInit();
private:
	static TriangleMesh* GetLoadMesh(const QString& file_path);

// getters / setters
public:
    void SetPosition(float tx, float tz);
    QVector2D GetPosition();
    int GetTypeID() const;
protected:
	Transformation& GetRotationTrafo();
	int GetInitialX() const;
	int GetInitialZ() const;

// methods
public:
    //! X-Position sind Felder A-H, Z-Position sind Felder 1-8
    virtual bool ValidMovement(int xStartPosition, int zStartPosition, int xTargetPosition, int zTargetPosition, ChessBoard* board) = 0;
    //! Move Figure in absolute Coordinates (handle with Caution!)
    void MovePosition(float tx, float tz);

// properties
private:
	float mSize;
	int mInitialX;
	int mInitialZ;
    QVector2D mPosition;
    int mTypeID;

// scene objects
private:
	Transformation mtRotation;
	Transformation mtPosition;
	TriangleMesh* mgMesh;

	static QMap<QString, TriangleMesh*> msgMeshes;
};

