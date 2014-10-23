#include "main.h"

enum ShaderType { VERT, FRAG, GEOM };

class Shader {
   public:
      Shader();
      ~Shader();
      void initialize();
      int installShader(const GLchar *shaderData, ShaderType type);
      void initHandles();

   private:
      int shadeProg;

      GLint h_aPosition;
      GLint h_aNormal;
      
      GLint h_uModelMatrix;
      GLint h_uViewMatrix;
      GLint h_uProjMatrix;


};