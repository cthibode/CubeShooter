#include "main.h"

class Camera {
   public:
      Camera();
      ~Camera();
      void setBounds(float xPos, float xNeg, float zPos, float zNeg);
      void setShaderView(GLint viewMatHandle);
      void setShaderCamPos(GLint camPosHandle);
      void moveFB(float speed);
      void moveLR(float speed);
      void moveLookAt(int dx, int dy, int width, int height);
      void updateJump();
      void startJump();
      vec3 getEye();
      vec3 getLookAt();

   private:
      vec3 eye;
      vec3 lookAt;
      vec3 const upVector;
      float xPosBound, xNegBound, zPosBound, zNegBound;
      bool isJumping;
};