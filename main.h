#ifndef MAIN_H_
#define MAIN_H_

#ifdef __APPLE__
#include "GLFW/glfw3.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include "GLFW/glfw3.h"
#endif

#ifdef _WIN32
#include <GL/glew.h>
#include "GLFW/glfw3.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3dll.lib")
#endif

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



#endif