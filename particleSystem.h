#include "main.h"

enum PartBehavior { CONFETTI, FLOAT_UP };

/* Holds the variables needed for each individual particle */
struct Particle {
   PartBehavior type;
   vec3 position, velocity;
   vec4 color;
   float size;
   int life;
};

/* Maintains the creation and behavior of particles */
class ParticleSystem {
   public:
      ParticleSystem();
      ~ParticleSystem();
      void initialize();
      void createParticles(vec3 position, int amount, PartBehavior type);
      void update();
      void draw(GLint modelMatHandle, GLint positionHandle, GLint colorHandle, GLint ptSizeHandle);

   private:
      int getNextIndex();
      int const maxParticles;
      int numParticles;
      Particle *container;
      float *activePos;
      float *activeColor;
      float *activeSize;
      GLuint vertBuffObj, colorBuffObj, sizeBuffObj;
};