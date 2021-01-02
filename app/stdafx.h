#pragma once

// common STL headers
#include <iostream>
#include <string>
#include <array>

// common OpenGL headers
#include <opengl.h>

// common framework includes
#include <node.h>
#include <transformation.h>
#include <drawable.h>
#include <simplecube.h>
#include <color.h>
#include <trianglemesh.h>
#include <camera.h>
#include <listener.h>
#include <inputregistry.h>
#include <shader.h>
#include <shadermanager.h>
#include <pointlight.h>
#include <material.h>

// common project includes


// macros
#define ColClamp(x)	((x) / 255.0f)
#define Lerp(a,b,t) ((a)+((b)-(a))*t)
