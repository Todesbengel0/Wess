#-------------------------------------------------
#
# Project created by QtCreator 2011-03-04T17:34:20
#
#-------------------------------------------------

include(../sgframework/Common.pri)

TARGET = App
SOURCES += \
    ActorHighlight.cpp \
    BoardFrame.cpp \
    ChessBoard.cpp \
    EnvSphere.cpp \
    EnvTable.cpp \
    Game.cpp \
    Graveyard.cpp \
    WoodenBumpMap.cpp \
    WoodenPatternMap.cpp \
    WoodenPiece.cpp \
    WoodenShader.cpp \
    myscene.cpp \
    Bishop.cpp \
    ChessField.cpp \
    ChessFieldActor.cpp \
    Figure.cpp \
    HelperAxes.cpp \
    King.cpp \
    Knight.cpp \
    Pawn.cpp \
    Queen.cpp \
    Rook.cpp \
    Selection.cpp

FORMS += \
    dockwidget.ui

HEADERS += \
    ActorHighlight.h \
    BoardFrame.h \
    ChessBoard.h \
    EnvSphere.h \
    EnvTable.h \
    FigureType.h \
    Game.h \
    Bishop.h \
    ChessColor.h \
    ChessField.h \
    ChessFieldActor.h \
    Figure.h \
    Graveyard.h \
    HelperAxes.h \
    King.h \
    Knight.h \
    Pawn.h \
    Queen.h \
    Rook.h \
    Selection.h \
    WoodenBumpMap.h \
    WoodenPatternMap.h \
    WoodenPiece.h \
    WoodenShader.h
    stdafx.h

PRECOMPILED_HEADER = stdafx.h

RESOURCES += \
    App.qrc
