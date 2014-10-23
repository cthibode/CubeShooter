#include "main.h"

class Window {
   public:
      Window();
      Window(int w, int h);
      ~Window();
      int initialize();
      void destroy();
      void updateWindow();
      int getShouldClose();
      GLFWwindow* getWindow();

   private:
      GLFWwindow *window;
      int width, height;
};