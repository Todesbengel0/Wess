#ifndef OPENGL_H
#define OPENGL_H

#include <QtGlobal>

//! wenn SG_USE_OPENGL_3_3_FUNCTIONS definiert ist wird ein kompatibilitätsmodus verwendet in dem nurnoch OpenGL 3.3
//! funktionen verwendet werden können und eine instanz von QOpenGLFunctions_3_3_Core erstellt. Aktuell werden keine
//! funktionen durch die verwendung von OpenGL 3.3 deaktiviert.
//!
//! wenn QT_OPENGL_ES definiert ist (wird von QT deffiniert) wird ein kompatibilitätsmodus verwendet in dem nur noch OpenGL ES 2.0
//! funktionen verwendet werden können und eine instanz von QOpenGLFunctions erstellt. Aktuell wird zur
//! compilierung der Shader mindestens OpenGL 3.0 bzw. OpenGL ES 3.0 benötigt da diese neuere funktionalitäten
//! (layount/in/out) verwenden. Aktuell wird in diesem kompatibilitätsmodus lediglich glPolygonMode nicht unterstützt
//! wodurch Geometrien nicht mehr im Wireframe Modus gerendert werden konnen. Somit werden alle Geometrien gefült
//! dargestellt.
//!
//! wenn keins der beiden defines definiert ist (standard), wird aktuell OpenGL 4.0 verwendet und somit eine instanz von
//! QOpenGLFunctions_4_0_Core erstellt.
//#define SG_USE_OPENGL_3_3_FUNCTIONS

//! fügt einen QOpenGLDebugLogger hinzu der oppenGL debug massages ausgibt. er wird nur im debug modus verwendet.
#if defined(QT_DEBUG) && !defined(QT_OPENGL_ES)
#define SG_LOG_OPENGL_DEBUG_MASSAGES
#endif // QT_DEBUG

#include <QOpenGLFunctions>

#ifdef QT_OPENGL_ES
extern QOpenGLFunctions* glFunctions;
#define SGFGLFUNCTIONS QOpenGLFunctions
#define SGFSHADERVERSIONSTRING "#version 300 es \n precision mediump int; \n precision mediump float; \n"

#elif defined(SG_USE_OPENGL_3_3_FUNCTIONS)
#include <QOpenGLFunctions_3_3_Core>
extern QOpenGLFunctions_3_3_Core* glFunctions;
#define SGFGLFUNCTIONS QOpenGLFunctions_3_3_Core
#define SGFSHADERVERSIONSTRING "#version 330 \n"

#else // use QOpenGLFunctions_4_0_Core
#include <qopenglfunctions_4_0_core.h>
extern QOpenGLFunctions_4_0_Core* glFunctions;
#define SGFGLFUNCTIONS QOpenGLFunctions_4_0_Core
#define SGFSHADERVERSIONSTRING "#version 400 \n"
#endif

#endif // OPENGL_H
