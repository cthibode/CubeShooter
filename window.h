#include "main.h"

/* Manages the GLFW window, including keyboard and mouse input */
class Window {
   public:
      Window();
      Window(int w, int h);
      ~Window();
      int initialize();
      void destroy();
      void updateWindow();
      void setShaderProjMatrix(GLint projMatHandle);
      bool isKeyPressed(int key);
      bool isMousePressed(int button);
      void getMouseDiff(int *dx, int *dy);
      void getDimensions(int *w, int *h);
      void setShouldClose(bool shouldClose);
      int getShouldClose();

   private:
      GLFWwindow *window;
      int width, height;
};