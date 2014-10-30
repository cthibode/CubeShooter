#include <vector>
#include "main.h"

/* Manages the light positions and colors */
class Light {
   public:
      Light();
      ~Light();
      void initLights(vec3 crateLightPos, float stageSize, float stageHeight);
      void setShaderLights(GLint lightPosHandle, GLint lightColorHandle, GLint numLightsHandle);
      void resetLights();
      int addLight(vec3 pos, Color color);
      void updateCrateLight(vec3 cratePos);
      void updateStageLights();
      void updateStageLightsLose();
      int getMinLights();
      int getMaxLights();

   private:
      vector<float> lightPos;
      vector<float> lightColor;
      int const minLights, maxLights;
      float stageSize, stageHeight;
};