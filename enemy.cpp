#include "geom.h"

Enemy::Enemy() : Cube() {
   /* Normal enemy */
   size = 1.0;
   radius = 0.7;
   health = 2;
   jumpHeight = 0.5;
   jumpSpeed = 0.1;
   moveSpeed = 0.02;

   /* Common for all enemies */
   tScale = vec3(0.1);
   color = WHITE;
   age = 0;
   state = SPAWN;
}

Enemy::~Enemy() {

}

/* Move and rotate the enemy according to its state */
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
      tScale *= 1.05;
      tPosition.y = size / 2.0;
      tRotPitch = tRotRoll = tRotYaw = age;
      age += 20;

      if (tScale.x >= size) {
         tScale = vec3(size);
         tRotPitch = tRotRoll = tRotYaw = 0;
         state = LIVE;
      }
   }
   else if (state == DIE) {
      tScale *= 0.95;
      tRotPitch = tRotRoll = tRotYaw = age;
      age += 20;

      if (tScale.x <= 0.01)
         state = DEAD;
   }
}

/* Reduces health by 1 and changes the enemy's state if necessary 
 * return: true if the enemy's state is DIE, otherwise false
 */
bool Enemy::reduceHealth() {
   bool ret = false;
   if (--health <= 0) {
      state = DIE;
      ret = true;
   }
   return ret;
}

EnemyState Enemy::getState() {
   return state;
}