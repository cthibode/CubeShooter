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
      stageSize / 3.0, stageHeight, 0,
      -stageSize / 3.0, stageHeight, 0,
      0, stageHeight, stageSize / 3.0,
      0, stageHeight, -stageSize / 3.0,
      0, stageHeight, 0,
      crateLightPos.x, crateLightPos.y, crateLightPos.z
   };

   float lc[] {
      1, 0, 1,
      1, 0, 1,
      0, 0, 1, 
      0, 0, 1,
      0, 0, 0.5,
      1, 1, 1
   };

   lightPos = vector<float>(lp, lp + sizeof(lp) / sizeof(float));
   lightColor = vector<float>(lc, lc + sizeof(lc) / sizeof(float));
   this->stageSize = stageSize;
   this->stageHeight = stageHeight;
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
   vec3 newColor;

   /* Make sure not to exceed the maximum number of lights */
   if (lightPos.size() >= maxLights * 3)
      return 0;

   if (color == GLOW_YELLOW)
      newColor = vec3(1, 1, 0);
   else if (color == GLOW_RED)
      newColor = vec3(1, 0, 0);
   else if (color == GLOW_TEAL)
      newColor = vec3(0, 1, 1);
   else
      newColor = vec3(1, 1, 1);

   lightPos.push_back(pos.x);
   lightPos.push_back(pos.y);
   lightPos.push_back(pos.z);
   lightColor.push_back(newColor.r);
   lightColor.push_back(newColor.g);
   lightColor.push_back(newColor.b);

   return 1;
}

/* Update the crate light location */
void Light::updateCrateLight(vec3 newPos) {
   lightPos[minLights * 3 - 3] = newPos.x;
   lightPos[minLights * 3 - 2] = newPos.y + 0.5;
   lightPos[minLights * 3 - 1] = newPos.z;
}

/* Move and update the color of the stage lights */
void Light::updateStageLights() {
   static float angle = 0;
   static float radius = stageSize / 3.0;
   static bool angDecr = false;
   static bool radDecr = true;
   static int colorIndex = 0;

   static vec3 const colors[] = {
      vec3(1, 0, 1), vec3(0, 0, 1), vec3(0, 0, .5),
      vec3(1, 1, 0), vec3(1, 0, 1), vec3(0.5, 0, 0.5),
      vec3(0, 1, 1), vec3(0, 0, 1), vec3(0, 0.5, 0.5),
      vec3(1, 0, 0), vec3(0.5, 0, 1), vec3(0.5, 0, 0),
      vec3(0, 1, 1), vec3(0, 1, 0), vec3(0, 0.5, 0),
      vec3(1, 1, 0), vec3(1, 0.5, 0), vec3(0.5, 0.5, 0),
   };

   lightPos[0] = radius * cos(DEG_TO_RAD(angle));
   lightPos[2] = radius * sin(DEG_TO_RAD(angle));
   lightPos[1*3 + 0] = radius * cos(DEG_TO_RAD(angle + 180));
   lightPos[1*3 + 2] = radius * sin(DEG_TO_RAD(angle + 180));
   lightPos[2*3 + 0] = radius * cos(DEG_TO_RAD(angle + 90));
   lightPos[2*3 + 2] = radius * sin(DEG_TO_RAD(angle + 90));
   lightPos[3*3 + 0] = radius * cos(DEG_TO_RAD(angle - 90));
   lightPos[3*3 + 2] = radius * sin(DEG_TO_RAD(angle - 90));

   angle = angDecr ? angle - 1 : angle + 1;
   if (angle >= 500 || angle <= -500)
      angDecr = !angDecr;

   radius = radDecr ? radius - 0.1 : radius + 0.1;
   if (radius >= stageSize / 3.0 || radius <= 0) {
      radDecr = !radDecr;
      
      colorIndex += 3;
      if (colorIndex >= sizeof(colors) / sizeof(vec3))
         colorIndex = 0;
      lightColor[0] = lightColor[3 + 0] = colors[colorIndex].r;
      lightColor[1] = lightColor[3 + 1] = colors[colorIndex].g;
      lightColor[2] = lightColor[3 + 2] = colors[colorIndex].b;
      lightColor[2*3 + 0] = lightColor[3*3 + 0] = colors[colorIndex + 1].r;
      lightColor[2*3 + 1] = lightColor[3*3 + 1] = colors[colorIndex + 1].g;
      lightColor[2*3 + 2] = lightColor[3*3 + 2] = colors[colorIndex + 1].b;
      lightColor[4*3 + 0] = colors[colorIndex + 2].r;
      lightColor[4*3 + 1] = colors[colorIndex + 2].g;
      lightColor[4*3 + 2] = colors[colorIndex + 2].b;
   }
   else if (abs(radius - (stageSize / 3.0) / 2.0) < 0.1) {
      lightColor[0] = lightColor[3 + 0] = colors[colorIndex + 1].r;
      lightColor[1] = lightColor[3 + 1] = colors[colorIndex + 1].g;
      lightColor[2] = lightColor[3 + 2] = colors[colorIndex + 1].b;
      lightColor[2*3 + 0] = lightColor[3*3 + 0] = colors[colorIndex].r;
      lightColor[2*3 + 1] = lightColor[3*3 + 1] = colors[colorIndex].g;
      lightColor[2*3 + 2] = lightColor[3*3 + 2] = colors[colorIndex].b;
   }
}

/* Set the stage lights when the player has lost */
void Light::updateStageLightsLose() {
   int count;

   float const lp[] = {
      stageSize / 4.0, stageHeight, stageSize / 4.0,
      -stageSize / 4.0, stageHeight, stageSize / 4.0,
      -stageSize / 4.0, stageHeight, -stageSize / 4.0,
      stageSize / 4.0, stageHeight, -stageSize / 4.0,
      0, stageHeight, 0
   };

   for (count = 0; count < (minLights - 1) * 3; count++) {
      lightPos[count] = lp[count];
      lightColor[count] = 1;
   }
   lightColor[minLights * 3 - 3] = 0;
   lightColor[minLights * 3 - 2] = 0;
   lightColor[minLights * 3 - 1] = 0;
}

int Light::getMinLights() {
   return minLights;
}

int Light::getMaxLights() {
   return maxLights;
}