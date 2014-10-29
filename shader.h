#include "main.h"

enum ShaderType { VERT, FRAG, GEOM };
enum ShadeMode { PHONG, AMBIENT, PARTICLE };

/* Holds the shader program and shader variable handles */
class Shader {
   public:
      Shader();
      ~Shader();
      void initialize();
      int installShader(const GLchar *shaderData, ShaderType type);
      void initHandles();
      void setMaterial(Color color);
      GLint getPositionHandle();
      GLint getNormalHandle();
      GLint getColorHandle();
      GLint getPtSizeHandle(); // needed?
      GLint getModelMatHandle();
      GLint getViewMatHandle();
      GLint getProjMatHandle();
      GLint getLightPosHandle();
      GLint getLightColorHandle();
      GLint getNumLightsHandle();
      GLint getCamPosHandle();

   private:
      int shadeProg;

      GLint h_aPosition;
      GLint h_aNormal;
      GLint h_aColor;
      GLint h_aPtSize; /// needed?
      
      GLint h_uModelMatrix;
      GLint h_uViewMatrix;
      GLint h_uProjMatrix;

      GLint h_uMatDif;
      GLint h_uMatSpec;
      GLint h_uMatAmb;
      GLint h_uMatShine;
      GLint h_uShadeMode;

      GLint h_uLightPos;
      GLint h_uLightColor;
      GLint h_uNumLights;

      GLint h_uCamPos;
};