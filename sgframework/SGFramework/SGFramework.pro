#-------------------------------------------------
#
# Project created by QtCreator 2011-03-04T17:34:20
#
#-------------------------------------------------

QT       += gui core widgets opengl multimedia xml

TARGET = SGFramework
TEMPLATE = lib
CONFIG += static
CONFIG += c++11

VRPN = false #Ohne VRPN übersetzen

macx {
#    message("* Using settings for Mac.")
#Benötigt für die Übersetzung der Bullet
QMAKE_CXXFLAGS += -Wno-argument-outside-range
}

#Um GL ES3 Header zu finden
android {
    message("* Using settings for Android.")
    message($$ANDROID_PLATFORM_PATH)
    INCLUDEPATH += /Users/bdr1/AndroidDev/android-ndk-r11c/platforms/android-23/arch-arm/usr/include #$$ANDROID_PLATFORM_PATH/include
}

INCLUDEPATH += $$PWD
include($$PWD/../PhysicPackage/PhysicPackage.pri)
include($$PWD/../AudioPackage/AudioPackage.pri)

if (!equals(VRPN, "true")) {
    message("Running without VRPN")
    DEFINES += SG_NO_VRPN
} else {
    include($$PWD/../VRPNClient/VRPNClient.pri)
}

DESTDIR = $$PWD/../SGFrameworkLib

INSTALL_ROOT = $$PWD/../SGFrameworkLib
headersDataFiles.path = $${INSTALL_ROOT}
headersDataFiles.files = $$PWD/*.h

headersDataFiles.files += $$PWD/../PhysicPackage/physics/*.h

headersDataFiles.files += $$PWD/../AudioPackage/OpenALSoft/include/AL/*.h
headersDataFiles.files += $$PWD/../AudioPackage/audio/*.h

INSTALLS += headersDataFiles

libraryFiles.path = $${INSTALL_ROOT}
INSTALLS += libraryFiles

win32-g++: LIBS += -lws2_32

SOURCES += $$PWD/transformation.cpp \
    $$PWD/reshapelistener.cpp \
    $$PWD/node.cpp \
    $$PWD/listener.cpp \
    $$PWD/keyboardtransformation.cpp \
    $$PWD/idleobserver.cpp \
    $$PWD/geometry.cpp \
    $$PWD/color.cpp \
    $$PWD/simpleellipse.cpp \
    $$PWD/simpleplane.cpp \
    $$PWD/main.cpp \
    $$PWD/simplesphere.cpp \
    $$PWD/animatedtransformation.cpp \
    $$PWD/camera.cpp \
    $$PWD/simplecube.cpp \
    $$PWD/openglwidget.cpp \
    $$PWD/objfilereader.cpp \
    $$PWD/trianglemesh.cpp \
    $$PWD/shader.cpp \
    $$PWD/renderingcontext.cpp \
    $$PWD/material.cpp \
    $$PWD/light.cpp \
    $$PWD/texture.cpp \
    $$PWD/tessplane.cpp \
    $$PWD/drawable.cpp \
    $$PWD/shadertimed.cpp \
    $$PWD/overlaywidget.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/interpolatedrotation.cpp \
    $$PWD/shaderfileloader.cpp \
    $$PWD//physicengineticker.cpp \
    $$PWD/rendermanager.cpp \
    $$PWD/screenrenderer.cpp \
    $$PWD/renderer.cpp \
    $$PWD/shadowlight.cpp \
    $$PWD/shadowmapproperty.cpp \
    $$PWD/keyboardcameracontroller.cpp \
    $$PWD/scenemanager.cpp \
    $$PWD/mousekeyboardcameracontroller.cpp \
    $$PWD/fboproperty.cpp \
    $$PWD/fborenderer.cpp \
    shape.cpp \
    shapelist.cpp \
    modeltransformation.cpp \
    scene.cpp \
    renderingstate.cpp \
    transparentshapesorter.cpp \
    shadermanager.cpp \
    shapebuffer.cpp \
    shapesorter.cpp \
    presortedsorter.cpp \
    sgobject.cpp \
    transformationbase.cpp \
    sgobjectvisitor.cpp \
    sortedrenderingsgobjectvisitor.cpp \
    shadowmaprenderingsgobjectvisitor.cpp \
    preorderrenderingsgobjectvisitor.cpp \
    vertexformat.cpp \
    vertexformatmanager.cpp \
    indexedgeometry.cpp \
    uniformbuffer.cpp \
    flags.cpp \
    pointlight.cpp \
    spotlight.cpp \
    sunlight.cpp \
    soundmanager.cpp \
    inputregistry.cpp \
    mouseinput.cpp \
    keyboardinput.cpp \
    canvas.cpp \
    xmlparser.cpp \
    physicaccessablecamera.cpp \
    renderconfiguration.cpp \
    sgpositionalobject.cpp \
    canvasrenderconfiguration.cpp \
    cameracontroller.cpp \

if (equals(VRPN, "true")) {
    SOURCES += trackinginput.cpp \
        flystickinput.cpp \
        dtrackcameracontroller.cpp \
}

HEADERS  += $$PWD/transformation.h \
    $$PWD/reshapelistener.h \
    $$PWD/node.h \
    $$PWD/listener.h \
    $$PWD/keyboardtransformation.h \
    $$PWD/idleobserver.h \
    $$PWD/geometry.h \
    $$PWD/color.h \
    $$PWD/simpleplane.h \
    $$PWD/simpleellipse.h \
    $$PWD/simplecube.h \
    $$PWD/simplesphere.h \
    $$PWD/camera.h \
    $$PWD/uitransformation.h \
    $$PWD/light.h \
    $$PWD/openglwidget.h\
    $$PWD/simplecube.h \
    $$PWD/renderingcontext.h \
    $$PWD/opengl.h \
    $$PWD/drawable.h \
    $$PWD/drawableproperty.h \
    $$PWD/overlaywidget.h \
    $$PWD/mainwindow.h \
    $$PWD/interpolatedrotation.h \
    $$PWD/shaderfileloader.h \
    $$PWD/scene.h \
    $$PWD/shader.h \
    $$PWD/trianglemesh.h \
    $$PWD/objfilereader.h \
    $$PWD/physicengineticker.h \
    $$PWD/bumpmap.h \
    $$PWD/rendermanager.h \
    $$PWD/screenrenderer.h \
    $$PWD/renderer.h \
    $$PWD/shadowlight.h \
    $$PWD/shadowmapproperty.h \
    $$PWD/keyboardcameracontroller.h \
    $$PWD/scenemanager.h \
    $$PWD/mousekeyboardcameracontroller.h \
   #$$PWD/fboproperty.h \
    #$$PWD/fborenderer.h \
    $$PWD/animatedtransformation.h \
    $$PWD/material.h \
    $$PWD/shadertimed.h \
    $$PWD/tessplane.h \
    $$PWD/texture.h \
    fboproperty.h \
    fborenderer.h \
    shape.h \
    shapelist.h \
    drawablepropertyid.h \
    modeltransformation.h \
    shapesorter.h \
    renderingstate.h \
    transparentshapesorter.h \
    shadermanager.h \
    shapebuffer.h \
    presortedsorter.h \
    sgobject.h \
    transformationbase.h \
    sgobjectvisitor.h \
    sortedrenderingsgobjectvisitor.h \
    shadowmaprenderingsgobjectvisitor.h \
    preorderrenderingsgobjectvisitor.h \
    vertexformat.h \
    vertexformatmanager.h \
    indexedgeometry.h \
    uniformbuffer.h \
    flags.h \
    pointlight.h \
    spotlight.h \
    sunlight.h \
    soundmanager.h \
    inputregistry.h \
    mouseinput.h \
    keyboardinput.h \
    trackinginput.h \
    flystickinput.h \
    canvas.h \
    structure.h \
    xmlparser.h \
    physicaccessablecamera.h \
    renderconfiguration.h \
    sgpositionalobject.h \
    canvasrenderconfiguration.h \
    cameracontroller.h \
    dtrackcameracontroller.h

OTHER_FILES +=

RESOURCES += \
    $$PWD/Shader.qrc \
    displayconfig.qrc

FORMS += \
    $$PWD/glwithwidgets.ui

DISTFILES +=
