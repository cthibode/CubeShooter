#include "particleSystem.h"

ParticleSystem::ParticleSystem() : maxParticles(10000) {
   numParticles = 0;
   vertBuffObj = colorBuffObj = sizeBuffObj = 0;
   container = new Particle[maxParticles];
   activePos = new float[maxParticles * 3];
   activeColor = new float[maxParticles * 4];
   activeSize = new float[maxParticles];
}

ParticleSystem::~ParticleSystem() {
   delete[] container;
   delete[] activePos;
   delete[] activeColor;
   delete[] activeSize;
}

/* Create the buffer objects for the particle vertices, colors, and sizes */
void ParticleSystem::initialize() {
   glGenBuffers(1, &vertBuffObj);
   glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
   glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(float), NULL, GL_STREAM_DRAW);

   glGenBuffers(1, &colorBuffObj);
   glBindBuffer(GL_ARRAY_BUFFER, colorBuffObj);
   glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

   glGenBuffers(1, &sizeBuffObj);
   glBindBuffer(GL_ARRAY_BUFFER, sizeBuffObj);
   glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float), NULL, GL_STREAM_DRAW);
}

/* Add the given amount of particles of the given behavior to the container */
void ParticleSystem::createParticles(vec3 position, int amount, PartBehavior type) {
   Particle tmpPart;
   float tmpAng, tmpDist;
   int count;

   for (count = 0; count < amount; count++) {
      tmpPart.type = type;

      switch (type) {
         case CONFETTI:
            tmpAng = DEG_TO_RAD(rand() % 360);
            tmpDist = rand() % 100 / 1000.0;
            tmpPart.position = position;
            tmpPart.color = vec4(rand() % 100 / 100.0, rand() % 100 / 100.0, rand() % 100 / 100.0, 0.7);
            tmpPart.velocity = vec3(cos(tmpAng) * tmpDist, tmpDist, sin(tmpAng) * tmpDist);
            tmpPart.life = 90 + rand() % 20;
            tmpPart.size = 5.0;
            break;
         case FLOAT_UP:
            tmpAng = DEG_TO_RAD(rand() % 360);
            tmpDist = rand() % 100 / 400.0;
            tmpPart.position = position + vec3(cos(tmpAng) * tmpDist, 0, sin(tmpAng) * tmpDist);
            tmpPart.color = vec4(1, 1, 1, 1);
            tmpPart.velocity = vec3(0, 0.01, 0);
            tmpPart.life = 500;
            tmpPart.size = 3.0;
            break;
      }
      
      container[getNextIndex()] = tmpPart;
   }
}

/* Update active particles' life, position, velocity, color, size, etc. and add to the shader data array */
void ParticleSystem::update() {
   Particle *part;
   int numActive = 0;
   int count;

   for (count = 0; count < maxParticles; count++) {
      part = &container[count];

      if (part->life > 0) {
         /* Update the particle's movement */
         switch (part->type) {
            case CONFETTI:
               part->position += part->velocity;
               if (part->position.y < 0) {
                  part->position.y = 0;
                  part->velocity.x *= 0.7;
                  part->velocity.y = -part->velocity.y * 0.5;
                  part->velocity.z *= 0.7;
               }
               else
                  part->velocity.y -= 0.01;
               break;
            case FLOAT_UP:
               part->position += part->velocity;
               part->color.a -= 0.005;
         }
         part->life--;

         /* Add this particle's position, color, and size to the shader data array */
         activePos[3 * numActive + 0] = part->position.x;
         activePos[3 * numActive + 1] = part->position.y;
         activePos[3 * numActive + 2] = part->position.z;

         activeColor[4 * numActive + 0] = part->color.r;
         activeColor[4 * numActive + 1] = part->color.g;
         activeColor[4 * numActive + 2] = part->color.b;
         activeColor[4 * numActive + 3] = part->color.a;

         activeSize[numActive] = part->size;

         numActive++;
      }
   }

   numParticles = numActive;
}

/* Draw the particles */
void ParticleSystem::draw(GLint modelMatHandle, GLint positionHandle, GLint colorHandle, GLint ptSizeHandle) {
   /* Undo any model matrix transformations from previous geometry */
   glUniformMatrix4fv(modelMatHandle, 1, GL_FALSE, value_ptr(mat4(1.0)));

   /* Update buffer data and draw */
   glEnableVertexAttribArray(positionHandle);
   glEnableVertexAttribArray(colorHandle);
   glEnableVertexAttribArray(ptSizeHandle);

   glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
   glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(float), NULL, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, numParticles * 3 * sizeof(float), activePos);
   glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, colorBuffObj);
   glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, numParticles * 4 * sizeof(float), activeColor);
   glVertexAttribPointer(colorHandle, 4, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, sizeBuffObj);
   glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(float), NULL, GL_STREAM_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, numParticles * sizeof(float), activeSize);
   glVertexAttribPointer(ptSizeHandle, 1, GL_FLOAT, GL_FALSE, 0, 0);

   glDrawArrays(GL_POINTS, 0, numParticles);
   
   glDisableVertexAttribArray(positionHandle);
   glDisableVertexAttribArray(colorHandle);
   glDisableVertexAttribArray(ptSizeHandle);
}

/* (Private) Return the next available index in container for a new particle */
int ParticleSystem::getNextIndex() {
   static bool firstRound = true;
   static int lastUsed = -1;
   int count;

   for (count = lastUsed + 1; count < maxParticles; count++) {
      if (firstRound || container[count].life <= 0) {
         lastUsed = count;
         return count;
      }
   }
   firstRound = false;
   for (count = 0; count <= lastUsed; count++) {
      if (container[count].life <= 0) {
         lastUsed = count;
         return count;
      }
   }

   return 0;
}
