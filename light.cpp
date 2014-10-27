#include "light.h"

Light::Light() : minLights(6), maxLights(15) {
   lightPos = vector<float>();
   lightColor = vector<float>();
}

Light::~Light() {

}

/* Initialize the position and color vectors with the stage and crate lights */
void Light::initLights(vec3 crateLightPos, float stageSize, float stageHeight) {
   float lp[] = {
      0, stageHeight / 2.0, -stageSize / 2.0,
      0, stageHeight / 2.0, stageSize / 2.0,
      -stageSize / 2.0, stageHeight / 2.0, 0,
      stageSize / 2.0, stageHeight / 2.0, 0,
      0, stageHeight, 0,
      crateLightPos.x, crateLightPos.y, crateLightPos.z
   };

   //float lp[] = {
   //   STAGE_SIZE / 4.0, STAGE_HEIGHT / 2.0, -STAGE_SIZE / 4.0,
   //   -STAGE_SIZE / 4.0, STAGE_HEIGHT / 2.0, STAGE_SIZE / 4.0,
   //   -STAGE_SIZE / 4.0, STAGE_HEIGHT / 2.0, -STAGE_SIZE / 4.0,
   //   STAGE_SIZE / 4.0, STAGE_HEIGHT / 2.0, STAGE_SIZE / 4.0,
   //   0, STAGE_HEIGHT / 2.0, 0,
   //   crateLightPos.x, crateLightPos.y, crateLightPos.z
   //};

   float lc[] = {
      0, 1, 0,
      0, 1, 0,
      1, 0, 1,
      1, 0, 1,
      0, 0, 1,
      1, 1, 1
   };

   lightPos = vector<float>(lp, lp + sizeof(lp) / sizeof(float));
   lightColor = vector<float>(lc, lc + sizeof(lc) / sizeof(float));
}

/* Send the light data to the shader */
void Light::setShaderLights(GLint lightPosHandle, GLint lightColorHandle, GLint numLightsHandle) {
   glUniform3fv(lightPosHandle, lightPos.size() / 3, lightPos.data());
   glUniform3fv(lightColorHandle, lightColor.size() / 3, lightColor.data());
   glUniform1i(numLightsHandle, lightPos.size() / 3);
}

/* Reinitializes the position and color vectors so that only the stage and crate lights remain */
void Light::resetLights() {
   lightPos.erase(lightPos.begin() + minLights * 3, lightPos.end());
   lightColor.erase(lightColor.begin() + minLights * 3, lightColor.end());
}

/* Add a point light of the given color at the given location 
 * return: 1 if the light was successfully added, 0 otherwise 
 */
int Light::addLight(vec3 pos, Color color) {
   vec3 bulletColor;

   /* Make sure not to exceed the maximum number of lights */
   if (lightPos.size() >= maxLights * 3)
      return 0;

   if (color == GLOW_YELLOW)
      bulletColor = vec3(1, 1, 0);
   else if (color == GLOW_RED)
      bulletColor = vec3(1, 0, 0);
   else if (color == GLOW_TEAL)
      bulletColor = vec3(0, 1, 1);
   else
      bulletColor = vec3(1, 1, 1);

   lightPos.push_back(pos.x);
   lightPos.push_back(pos.y);
   lightPos.push_back(pos.z);
   lightColor.push_back(bulletColor.r);
   lightColor.push_back(bulletColor.g);
   lightColor.push_back(bulletColor.b);

   return 1;
}

/* Update the crate light location */
void Light::updateCrateLight(vec3 newPos) {
   lightPos[minLights * 3 - 3] = newPos.x;
   lightPos[minLights * 3 - 2] = newPos.y + 0.5;
   lightPos[minLights * 3 - 1] = newPos.z;
}

/* Move the stage lights */
void Light::updateStageLights() {

}

int Light::getMinLights() {
   return minLights;
}

int Light::getMaxLights() {
   return maxLights;
}