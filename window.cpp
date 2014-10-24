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

   glfwSetCursorPos(window, width / 2.0, height / 2.0);
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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

/* Stores the pixel difference between the cursor's position and the center of the screen into the given addresses */
void Window::getMouseDiff(int *dx, int *dy) {
   double xCur, yCur;

   glfwGetWindowSize(window, &width, &height);
   glfwGetCursorPos(window, &xCur, &yCur);

   *dx = (int)(xCur - width/2.0);
   *dy = (int)(height/2.0 - yCur);

   glfwSetCursorPos(window, width/2.0, height/2.0);
}

/* Store the dimensions of the window at the given addresses */
void Window::getDimensions(int *w, int *h) {
   glfwGetWindowSize(window, &width, &height);
   *w = width;
   *h = height;
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
