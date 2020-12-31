#-------------------------------------------------
#
# Project created by QtCreator 2011-03-04T17:34:20
#
#-------------------------------------------------

include(../sgframework/Common.pri)

TARGET = App
SOURCES += \
    Chessboard.cpp \
    Game.cpp \
    Graveyard.cpp \
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
    Chessboard.h \
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
    Selection.h
    stdafx.h

PRECOMPILED_HEADER = stdafx.h
