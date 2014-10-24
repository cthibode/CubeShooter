#include "geom.h"

Bullet::Bullet() : Cube() {
   tScale = vec3(0.02, 0.02, 0.2);
   speed = 0.3;
   color = GLOW_YELLOW;

   direction = vec3(0);
}

Bullet::~Bullet() {

}

void Bullet::align(vec3 target) {
   direction = normalize(target - tPosition);
   tRotPitch = RAD_TO_DEG(atan(direction.y / glm::sqrt(direction.x * direction.x + direction.z * direction.z)));
   tRotYaw = RAD_TO_DEG(atan(direction.x / direction.z));
}

void Bullet::update() {
   tPosition += direction * speed;
}