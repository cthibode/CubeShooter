#include <stdio.h>
#include "window.h"

static void errorCallback(int error, const char* description) {
   fprintf(stderr, "%s\n", description);
}

Window::Window() {
   window = 0;
   width = 1920;
   height = 1080;
}

Window::Window(int w, int h) {
   window = 0;
   width = w;
   height = h;
}

Window::~Window() {
      
}

/* Initialize the GLFW window 
 * return: 1 if successful, 0 otherwise
 */
int Window::initialize() {
   glfwSetErrorCallback(errorCallback);

   if (!glfwInit())
      return 0;

   window = glfwCreateWindow(width, height, "Cube Shooter", /*glfwGetPrimaryMonitor()*/ NULL, NULL);
   if (!window) {
      glfwTerminate();
      return 0;
   }

   glfwMakeContextCurrent(window);
   return 1;
}

/* Destroy the GLFW window */
void Window::destroy() {
   glfwDestroyWindow(window);
   glfwTerminate();
}

/* Update the window with the buffer contents */
void Window::updateWindow() {
   glfwSwapBuffers(window);
   glfwPollEvents();
}

/* Returns true if the given key is pressed, otherwise false */
bool Window::isKeyPressed(int key) {
   return (glfwGetKey(window, key) ==  GLFW_PRESS);
}

/* Set if the window should close */
void Window::setShouldClose(bool shouldClose) {
   int close = shouldClose ? GL_TRUE : GL_FALSE;
   glfwSetWindowShouldClose(window, close);
}

/* Returns non-zero if the window should close, otherwise 0 */
int Window::getShouldClose() {
   return glfwWindowShouldClose(window);
}
