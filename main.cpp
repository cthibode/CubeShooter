#include <stdio.h>
#include <time.h>
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
#define ENEMY_COOLDOWN 200
#define SPAWN_BUFFER 2
#define SPAWN_PTS 8
#define MAX_LIGHTS 15
#define MIN_LIGHTS 5

void handleKeyboardInput(Window *window, Camera *camera);
void handleMouseInput(Window *window, Camera *camera, vector<Bullet*> *bullets);
void createStage(vector<Wall*> *walls);
void initLights(vector<float> *lightPos, vector<float> *lightColor);
void keepFPS(double fps);

int main() {
   Window *window = new Window(1920, 1080);
   Shader *shader = new Shader();
   Camera *camera = new Camera();
   vector<Wall*> walls;
   vector<Enemy*> enemies;
   vector<Bullet*> bullets;
   vector<float> lightPos;
   vector<float> lightColor;
   Enemy *tmpEnemy;
   int enemyCooldown = ENEMY_COOLDOWN;
   unsigned int count, count2;

   GLint hPos, hNorm;
   GLint hModelMat, hViewMat, hProjMat;
   GLint hLightPos, hLightColor, hNumLights;
   GLint hCamPos;

   vec3 enemySpawnPts[] = {
      vec3(-STAGE_SIZE / 2.0 + SPAWN_BUFFER, 0, -STAGE_SIZE / 2.0 + SPAWN_BUFFER),
      vec3(0, 0, -STAGE_SIZE / 2.0 + SPAWN_BUFFER),
      vec3(STAGE_SIZE / 2.0 - SPAWN_BUFFER, 0, -STAGE_SIZE / 2.0 + SPAWN_BUFFER),
      vec3(STAGE_SIZE / 2.0 - SPAWN_BUFFER, 0, 0),
      vec3(STAGE_SIZE / 2.0 - SPAWN_BUFFER, 0, STAGE_SIZE / 2.0 - SPAWN_BUFFER),
      vec3(0, 0, STAGE_SIZE / 2.0 - SPAWN_BUFFER),
      vec3(-STAGE_SIZE / 2.0 + SPAWN_BUFFER, 0, STAGE_SIZE / 2.0 - SPAWN_BUFFER),
      vec3(-STAGE_SIZE / 2.0 + SPAWN_BUFFER, 0, 0),
   };

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
   srand(time(NULL));
   createStage(&walls);
   initLights(&lightPos, &lightColor);
   camera->setBounds(STAGE_SIZE/2.0 - CAM_BUFFER, -STAGE_SIZE/2.0 + CAM_BUFFER, STAGE_SIZE/2.0 - CAM_BUFFER, -STAGE_SIZE/2.0 + CAM_BUFFER);

   /* Game loop */
   while (!window->getShouldClose()) {
      keepFPS(60);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      handleKeyboardInput(window, camera);
      handleMouseInput(window, camera, &bullets);
      camera->setView(hViewMat);
      camera->setCamPos(hCamPos);
      window->setProjMatrix(hProjMat);

      /* Add enemies */
      if (--enemyCooldown <= 0) {
         tmpEnemy = new Enemy();
         tmpEnemy->setPosition(enemySpawnPts[rand() % SPAWN_PTS]);
         enemies.push_back(tmpEnemy);
         enemyCooldown = ENEMY_COOLDOWN;
      }

      /* Draw the stage */
      for (count = 0; count < walls.size(); count++) {
         shader->setMaterial(walls[count]->getColor());
         walls[count]->draw(hPos, hNorm, hModelMat);
      }
      /* Draw the enemies */
      for (count = 0; count < enemies.size(); count++) {
         /* Check for collisions with bullets */
         if (enemies[count]->getState() == LIVE) {
            for (count2 = 0; count2 < bullets.size(); count2++) {
               if (enemies[count]->isColliding(bullets[count2]->getPosition())) {
                  delete bullets[count2];
                  bullets.erase(bullets.begin() + count2);
                  count2--;
                  if (enemies[count]->reduceHealth())
                     break;
               }
            }
         }

         enemies[count]->update(camera->getEye());
         if (enemies[count]->getState() == DEAD) {
            delete enemies[count];
            enemies.erase(enemies.begin() + count);
            count--;
            continue;
         }
         shader->setMaterial(enemies[count]->getColor());
         enemies[count]->draw(hPos, hNorm, hModelMat);
      }
      /* Draw the bullets */
      for (count = 0; count < bullets.size(); count++) {
         if (bullets[count]->shouldRemove()) {
            delete bullets[count];
            bullets.erase(bullets.begin() + count);
            count--;
            continue;
         }

         shader->setMaterial(bullets[count]->getColor());
         bullets[count]->update();
         bullets[count]->draw(hPos, hNorm, hModelMat);

         lightPos.push_back(bullets[count]->getPosition().x);
         lightPos.push_back(bullets[count]->getPosition().y);
         lightPos.push_back(bullets[count]->getPosition().z);
         lightColor.push_back(1);
         lightColor.push_back(1);
         lightColor.push_back(0);
      }
      
      /* Send the light data to the shader */
      glUniform3fv(hLightPos, lightPos.size() / 3, lightPos.data());
      glUniform3fv(hLightColor, lightColor.size() / 3, lightColor.data());
      glUniform1i(hNumLights, lightPos.size() / 3);
      lightPos.erase(lightPos.begin() + MIN_LIGHTS * 3, lightPos.end());
      lightColor.erase(lightColor.begin() + MIN_LIGHTS * 3, lightColor.end());

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

/* Check for mouse movement and clicks and respond accordingly */
void handleMouseInput(Window *window, Camera *camera, vector<Bullet*> *bullets) {
   static int bulletCooldown = 0;
   int dx, dy, width, height;
   Bullet *newBullet;
   vec3 camPos, lookAtPos;

   window->getMouseDiff(&dx, &dy);
   window->getDimensions(&width, &height);
   camera->moveLookAt(dx, dy, width, height);

   if (bulletCooldown > 0)
      bulletCooldown--;
   if (bulletCooldown <= 0 && window->isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
      // Add options for other weapon types here =================
      camPos = camera->getEye();
      lookAtPos = camera->getLookAt();
      newBullet = new Bullet();
      newBullet->setPosition(vec3(camPos.x, camPos.y - 0.2, camPos.z));
      newBullet->align(vec3(lookAtPos.x, lookAtPos.y - 0.2, lookAtPos.z));
      newBullet->setBounds(STAGE_SIZE/2.0, -STAGE_SIZE/2.0, STAGE_HEIGHT, 0, STAGE_SIZE/2.0, -STAGE_SIZE/2.0);
      bullets->push_back(newBullet);
      bulletCooldown = 30;//Change this later
   }
      
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

/* Sets an FPS cap*/
void keepFPS(double fps) {
   static double prevTime = 0;
   static double curTime = 0;
   double diff;

   curTime = glfwGetTime();
   diff = curTime - prevTime;
   prevTime = curTime;

   if (diff < 1.0 / fps) {
      #ifdef _WIN32
      Sleep((1.0 / fps - diff) * 1000);
      #else
      usleep((1.0 / fps - diff) * 1000000);
      #endif
   }
}