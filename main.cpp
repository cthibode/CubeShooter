#include <stdio.h>
#include <vector>
#include "main.h"
#include "window.h"
#include "shader.h"
#include "glslHelper.h"
#include "geom.h"

#define STAGE_SIZE 30
#define STAGE_HEIGHT 4

void handleKeyboardInput(Window *window);
void createStage(vector<Wall*> *walls);

int main() {
   Window *window = new Window(1920, 1080);
   Shader *shader = new Shader();
   vector<Wall*> walls;
   vector<Enemy*> enemies;
   unsigned int count;

   GLint hPos, hNorm;
   GLint hModelMat, hViewMat, hProjMat;

   /* Initialize the window */
   if (!window->initialize()) {
      fprintf(stderr, "ERROR: window initialize\n");
      exit(EXIT_FAILURE);
   }
   glewInit();

   /* Install and initialize the shaders */
   shader->initialize();
   if (!shader->installShader(textFileRead("shaderVert.glsl"), VERT)) {
      fprintf(stderr, "ERROR: install vertex shader\n");
      exit(EXIT_FAILURE);
   }
   if (!shader->installShader(textFileRead("shaderFrag.glsl"), FRAG)) {
      fprintf(stderr, "ERROR: install fragment shader\n");
      exit(EXIT_FAILURE);
   }
   shader->initHandles();

   /* Retrieving handles to save some function calls in the main loop */
   hPos = shader->getPositionHandle();
   hNorm = shader->getNormalHandle();
   hModelMat = shader->getModelMatHandle();
   hViewMat = shader->getViewMatHandle();
   hProjMat = shader->getProjMatHandle();

   /* Initialize buffers */
   glClearColor(1.0, 1.0, 1.0, 1.0);
   glClearDepth(1.0);
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);

   /* Initialize the stage*/
   createStage(&walls);

   // temp==========
   mat4 Projection = perspective(80.0f, (float)1920/1080, 0.1f, 50.f);
   glUniformMatrix4fv(hProjMat, 1, GL_FALSE, value_ptr(Projection));
   mat4 View = lookAt(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));
   glUniformMatrix4fv(hViewMat, 1, GL_FALSE, value_ptr(View));
   Enemy *temp = new Enemy();
   temp->setPosition(vec3(1, 1, -5));
   enemies.push_back(temp);
   //===============

   /* Game loop */
   while (!window->getShouldClose()) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      handleKeyboardInput(window);

      for (count = 0; count < walls.size(); count++) {
         shader->setMaterial(walls[count]->getColor());
         walls[count]->draw(hPos, hNorm, hModelMat);
      }
      for (count = 0; count < enemies.size(); count++) {
         shader->setMaterial(enemies[count]->getColor());
         enemies[count]->draw(hPos, hNorm, hModelMat);
      }
      
      window->updateWindow();
   }
   
   window->destroy();
   delete window;

   return 0;
}

/* Check for key presses and respond accordingly */
void handleKeyboardInput(Window *window) {
   if (window->isKeyPressed(GLFW_KEY_ESCAPE))
      window->setShouldClose(true);
}

/* Initialize the stage by adding the walls */
void createStage(vector<Wall*> *walls) {
   Wall *temp;

   temp = new Wall();
   temp->setPosition(vec3(0, 0, -STAGE_SIZE/2.0));
   temp->setScale(vec3(STAGE_SIZE, STAGE_HEIGHT, 1));
   walls->push_back(temp);
}