# Additional include path
INCLUDEPATH += $$PWD/../AudioPackage/OpenALSoft/include/AL \
INCLUDEPATH += $$PWD/../AudioPackage/audio \

DEFINES += LIBDIR=\\\"$${_PRO_FILE_PWD_}/../AudioPackage/OpenALSoft/dll/Win64/\\\"

SOURCES += $$PWD/audio/audioengine.cpp \
    $$PWD/audio/audiolistener.cpp \
    $$PWD/audio/isoundapi.cpp \
    $$PWD/audio/openalsoundapi.cpp \
    $$PWD/audio/qtstereosoundapi.cpp \
    $$PWD/audio/soundfile.cpp \
    $$PWD/audio/soundfileloader.cpp \
    $$PWD/audio/soundsource.cpp \
    $$PWD/audio/openalhelper.cpp \
    $$PWD/audio/efxdirectfilter.cpp \
    $$PWD/audio/efxeffect.cpp \
    $$PWD/audio/efxfilter.cpp \
    $$PWD/audio/efxreverbeffect.cpp

HEADERS += $$PWD/audio/audioengine.h \
    $$PWD/audio/soundsource.h \
    $$PWD/audio/soundfile.h \
    $$PWD/audio/audiolistener.h \
    $$PWD/audio/soundfileloader.h \
    $$PWD/audio/isoundapi.h \
    $$PWD/audio/openalsoundapi.h \
    $$PWD/audio/qtstereosoundapi.h \
    $$PWD/audio/openalhelper.h \
    $$PWD/audio/efxdirectfilter.h \
    $$PWD/audio/efxeffect.h \
    $$PWD/audio/efxfilter.h \
    $$PWD/audio/efxreverbeffect.h

