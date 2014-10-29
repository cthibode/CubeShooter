#ifndef MAIN_H_
#define MAIN_H_

#ifdef __APPLE__
#include "GLFW/glfw3.h"
#include <OPENGL/gl.h>
#include <unistd.h>
#endif

#ifdef __unix__
#include "GLFW/glfw3.h"
#include <unistd.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <Windows.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

#define PI 3.14159
#define RAD_TO_DEG(r) ((r) * 180/PI)
#define DEG_TO_RAD(d) ((d) * PI/180)

enum Color {GRAY, DARK_GRAY, WHITE, BROWN, GLOW_YELLOW, GLOW_RED, GLOW_TEAL, ATTRIB};

#endif