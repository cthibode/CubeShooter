#include "main.h"

/* Manages the GLFW window */
class Window {
   public:
      Window();
      Window(int w, int h);
      ~Window();
      int initialize();
      void destroy();
      void updateWindow();
      bool isKeyPressed(int key);
      void setShouldClose(bool shouldClose);
      int getShouldClose();

   private:
      GLFWwindow *window;
      int width, height;
};