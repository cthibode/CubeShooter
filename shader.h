#include "main.h"

enum ShaderType { VERT, FRAG, GEOM };
enum ShadeMode { PHONG, AMBIENT };

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
      GLint getModelMatHandle();
      GLint getViewMatHandle();
      GLint getProjMatHandle();

   private:
      int shadeProg;

      GLint h_aPosition;
      GLint h_aNormal;
      
      GLint h_uModelMatrix;
      GLint h_uViewMatrix;
      GLint h_uProjMatrix;

      GLint h_uMatDif;
      GLint h_uMatSpec;
      GLint h_uMatAmb;
      GLint h_uMatShine;
      GLint h_uShadeMode;
};