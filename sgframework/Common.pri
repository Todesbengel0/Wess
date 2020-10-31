QT       += gui core widgets opengl multimedia xml
TEMPLATE = app
CONFIG += c++11

win32 {
    CONFIG += console   #Notwendig, da sonst qMain gesucht wird und nicht gefunden wird... eher ein Bug in Qt5.5?
}
#macx: QMAKE_MAC_SDK = macosx10.13

SGFRAMEWORKINCLUDEPATH = $$PWD/SGFrameworkLib
INCLUDEPATH += $${SGFRAMEWORKINCLUDEPATH}
DEPENDPATH += $${SGFRAMEWORKINCLUDEPATH}

SGFRAMEWORKPATH = $${SGFRAMEWORKINCLUDEPATH}
LIBS += -L$${SGFRAMEWORKPATH} -lSGFramework

win32:!win32-g++: PRE_TARGETDEPS += $${SGFRAMEWORKPATH}/SGFramework.lib
else:unix|win32-g++: PRE_TARGETDEPS += $${SGFRAMEWORKPATH}/libSGFramework.a

#Für OpenAL 3D-Sound
win32-g++: LIBS += -L$$PWD/AudioPackage/OpenALSoft/libs/Win32 -lOpenAL32
#Wenn msvc dann nehmen wir der Einfachheit halber 64 - "win64" gibt es NICHT
win32-msvc*: LIBS += -L$$PWD/AudioPackage/OpenALSoft/libs/Win64 -lOpenAL32
#macx: LIBS += -L$$PWD/AudioPackage/OpenALSoft/libs/OSXDebug -l openal

#Für VRPN-Tracking -> CAVE
unix:macx: VRPN_LIB_PATH=$$PWD/VRPNClient/lib/mac
unix:!macx: VRPN_LIB_PATH=$$PWD/VRPNClient/lib/linux
win32-g++: VRPN_LIB_PATH=$$PWD/VRPNClient/lib/mingw
win32-msvc*{
    CONFIG(debug, debug|release) {
        VRPN_LIB_PATH=$$PWD/VRPNClient/lib/msvc/debug
    }
    else {
        VRPN_LIB_PATH=$$PWD/VRPNClient/lib/msvc/release
    }
}

libraryFiles.files = $${VRPN_LIB_PATH}/*.a $${VRPN_LIB_PATH}/*.lib
#mingw 32
#win32-g++: LIBS += -L$$VRPN_LIB_PATH -lvrpn -lgpsnmea -lquat -lvrpn_timecode_generator -lvrpnserver -lws2_32
#win32-msvc: LIBS += -L$$VRPN_LIB_PATH -lvrpn -lgpsnmea -lquat -lvrpn_timecode_generator -lvrpnserver
unix: LIBS += -L$$VRPN_LIB_PATH #-lvrpn -lgpsnmea -lquat -lvrpn_timecode_generator

DEFINES += SRCDIR=\\\"$${_PRO_FILE_PWD_}\\\"
