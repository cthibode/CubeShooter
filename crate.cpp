#include "geom.h"

Crate::Crate() : Cube() {
   tScale = vec3(0.3);
   color = BROWN;
   radius = 0.6;
}

Crate::~Crate() {

}

/* Rotate the crate */
void Crate::update() {
   static float age = 0;
   tRotYaw = age;
   age++;
}