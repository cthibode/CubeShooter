#include <stdio.h>
#include <vector>
#include "main.h"
#include "window.h"
#include "shader.h"
#include "camera.h"
#include "glslHelper.h"
#include "geom.h"

#define STAGE_SIZE 30.0f
#define STAGE_HEIGHT 4.0f
#define CAM_SPEED 0.05f
#define CAM_BUFFER 0.4f
#define MAX_LIGHTS 15

void handleKeyboardInput(Window *window, Camera *camera);
void handleMouseInput(Window *window, Camera *camera);
void createStage(vector<Wall*> *walls);
void initLights(vector<float> *lightPos, vector<float> *lightColor);

int main() {
   Window *window = new Window(1920, 1080);
   Shader *shader = new Shader();
   Camera *camera = new Camera();
   vector<Wall*> walls;
   vector<Enemy*> enemies;
   vector<float> lightPos;
   vector<float> lightColor;
   unsigned int count;

   GLint hPos, hNorm;
   GLint hModelMat, hViewMat, hProjMat;
   GLint hLightPos, hLightColor, hNumLights;
   GLint hCamPos;

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
   hLightPos = shader->getLightPosHandle();
   hLightColor = shader->getLightColorHandle();
   hNumLights = shader->getNumLightsHandle();
   hCamPos = shader->getCamPosHandle();

   /* Initialize buffers */
   glClearColor(1.0, 1.0, 1.0, 1.0);
   glClearDepth(1.0);
   glDepthFunc(GL_LEQUAL);
   glEnable(GL_DEPTH_TEST);

   /* Enable backface culling */
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

   /* Initialize the stage*/
   createStage(&walls);
   initLights(&lightPos, &lightColor);
   camera->setBounds(STAGE_SIZE/2.0 - CAM_BUFFER, -STAGE_SIZE/2.0 + CAM_BUFFER, STAGE_SIZE/2.0 - CAM_BUFFER, -STAGE_SIZE/2.0 + CAM_BUFFER);

   //temp
   Enemy *temp = new Enemy();
   temp->setPosition(vec3(0, 0, -5));
   enemies.push_back(temp);

   /* Game loop */
   while (!window->getShouldClose()) {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      handleKeyboardInput(window, camera);
      handleMouseInput(window, camera);
      camera->setView(hViewMat);
      camera->setCamPos(hCamPos);
      window->setProjMatrix(hProjMat);

      for (count = 0; count < walls.size(); count++) {
         shader->setMaterial(walls[count]->getColor());
         walls[count]->draw(hPos, hNorm, hModelMat);
      }
      for (count = 0; count < enemies.size(); count++) {
         shader->setMaterial(enemies[count]->getColor());
         enemies[count]->update(camera->getEye());
         enemies[count]->draw(hPos, hNorm, hModelMat);
      }
      
      glUniform3fv(hLightPos, lightPos.size() / 3, lightPos.data());
      glUniform3fv(hLightColor, lightColor.size() / 3, lightColor.data());
      glUniform1i(hNumLights, lightPos.size() / 3);

      window->updateWindow();
   }
   
   window->destroy();
   delete window;

   return 0;
}

/* Check for key presses and respond accordingly */
void handleKeyboardInput(Window *window, Camera *camera) {
   if (window->isKeyPressed(GLFW_KEY_W))
      camera->moveFB(CAM_SPEED);
   if (window->isKeyPressed(GLFW_KEY_S))
      camera->moveFB(-CAM_SPEED);
   if (window->isKeyPressed(GLFW_KEY_D))
      camera->moveLR(CAM_SPEED);
   if (window->isKeyPressed(GLFW_KEY_A))
      camera->moveLR(-CAM_SPEED);
   if (window->isKeyPressed(GLFW_KEY_ESCAPE))
      window->setShouldClose(true);
}

void handleMouseInput(Window *window, Camera *camera) {
   int dx, dy, width, height;

   window->getMouseDiff(&dx, &dy);
   window->getDimensions(&width, &height);
   camera->moveLookAt(dx, dy, width, height);
}

/* Initialize the stage by adding the walls */
void createStage(vector<Wall*> *walls) {
   Wall *temp;

   /* Floor */
   temp = new Wall();
   temp->setRotation(-90, 0, 0);
   temp->setScale(vec3(STAGE_SIZE, STAGE_SIZE, 1));
   temp->setColor(DARK_GRAY);
   walls->push_back(temp);

   /* Ceiling */
   temp = new Wall();
   temp->setPosition(vec3(0, STAGE_HEIGHT, 0));
   temp->setRotation(90, 0, 0);
   temp->setScale(vec3(STAGE_SIZE, STAGE_SIZE, 1));
   temp->setColor(DARK_GRAY);
   walls->push_back(temp);

   /* Walls */
   temp = new Wall();
   temp->setPosition(vec3(0, STAGE_HEIGHT / 2.0, -STAGE_SIZE / 2.0));
   temp->setScale(vec3(STAGE_SIZE, STAGE_HEIGHT, 1));
   walls->push_back(temp);

   temp = new Wall();
   temp->setPosition(vec3(0, STAGE_HEIGHT / 2.0, STAGE_SIZE / 2.0));
   temp->setRotation(0, 0, 180);
   temp->setScale(vec3(STAGE_SIZE, STAGE_HEIGHT, 1));
   walls->push_back(temp);

   temp = new Wall();
   temp->setPosition(vec3(-STAGE_SIZE / 2.0, STAGE_HEIGHT / 2.0, 0));
   temp->setRotation(0, 0, 90);
   temp->setScale(vec3(STAGE_SIZE, STAGE_HEIGHT, 1));
   walls->push_back(temp);

   temp = new Wall();
   temp->setPosition(vec3(STAGE_SIZE / 2.0, STAGE_HEIGHT / 2.0, 0));
   temp->setRotation(0, 0, -90);
   temp->setScale(vec3(STAGE_SIZE, STAGE_HEIGHT, 1));
   walls->push_back(temp);
}

void initLights(vector<float> *lightPos, vector<float> *lightColor) {
   float lp[] = {
      0, STAGE_HEIGHT/2.0, -STAGE_SIZE/2.0,
      0, STAGE_HEIGHT/2.0, STAGE_SIZE/2.0,
      -STAGE_SIZE/2.0, STAGE_HEIGHT/2.0, 0,
      STAGE_SIZE/2.0, STAGE_HEIGHT/2.0, 0,
      0, STAGE_HEIGHT, 0
   };

   float lc[] = {
      0, 1, 0,
      0, 1, 0,
      1, 0, 1,
      1, 0, 1,
      0, 0, 1
   };

   *lightPos = vector<float>(lp, lp + sizeof(lp) / sizeof(float));
   *lightColor = vector<float>(lc, lc + sizeof(lc) / sizeof(float));
}
