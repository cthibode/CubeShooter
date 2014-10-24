#include "geom.h"

Enemy::Enemy() : Cube() {
   tScale = vec3(1);
   radius = 0.7;
   health = 1;
   jumpHeight = 0.5;
   jumpSpeed = 0.1;
   moveSpeed = 0.02;

   /* Common for all enemies */
   color = WHITE;
   age = 0;
   state = LIVE; // change later
}

Enemy::~Enemy() {

}

void Enemy::update(vec3 destination) {
   vec3 moveVec = normalize(destination - tPosition);

   if (state == LIVE) {
      if (moveVec.z == 0)
         moveVec.z = 0.001;
      tRotYaw = RAD_TO_DEG(atan(moveVec.x / moveVec.z));
      tPosition.x += moveVec.x * moveSpeed;
      tPosition.y = tScale.y / 2.0 + abs(sin(age)) * jumpHeight;
      tPosition.z += moveVec.z * moveSpeed;
      age += jumpSpeed;
   }
   else if (state == SPAWN) {

   }
   else if (state == DIE) {

   }

}