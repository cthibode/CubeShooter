#include "geom.h"

Bullet::Bullet(Weapon weapon) : Cube() {
   assert(weapon != W_END);
   switch (weapon) {
      case PISTOL:
         tScale = vec3(0.02, 0.02, 0.2);
         color = GLOW_YELLOW;
         radius = 0.2;
         speed = 0.3;
         cooldown = 30;
         life = 120;
         break;
      case MACHINE:
         tScale = vec3(0.02, 0.02, 0.2);
         color = GLOW_RED;
         radius = 0.2;
         speed = 0.3;
         cooldown = 5;
         life = 90;
         break;
      case SHOTGUN:
         tScale = vec3(0.02, 0.02, 0.2);
         color = GLOW_TEAL;
         radius = 0.2;
         speed = 0.5;
         cooldown = 50;
         life = 20;
         break;
   }

   /* Common for all bullets */
   type = weapon;
   direction = vec3(0);
   xPosBound = xNegBound = yPosBound = yNegBound = zPosBound = zNegBound = 100;
}

Bullet::~Bullet() {

}

/* Rotate the bullet to point towards the target */
void Bullet::align(vec3 target) {
   direction = normalize(target - tPosition);
   tRotPitch = RAD_TO_DEG(atan(direction.y / glm::sqrt(direction.x * direction.x + direction.z * direction.z)));
   tRotYaw = RAD_TO_DEG(atan(direction.x / direction.z));
   if (direction.z > 0)
      tRotYaw += 180;
}

/* Set the boundaries that bullet is allowed to be in */
void Bullet::setBounds(float xPos, float xNeg, float yPos, float yNeg, float zPos, float zNeg) {
   assert(xPos >= xNeg && yPos >= yNeg && zPos >= zNeg);
   xPosBound = xPos;
   xNegBound = xNeg;
   yPosBound = yPos;
   yNegBound = yNeg;
   zPosBound = zPos;
   zNegBound = zNeg;
}

/* Update the bullet's position */
void Bullet::update() {
   tPosition += direction * speed;
   life--;
}

/* Returns true if the bullet's life has run out our the bullet went out of bounds, otherwise false */
bool Bullet::shouldRemove() {
   bool ret = false;
   
   if (tPosition.x > xPosBound || tPosition.x < xNegBound || tPosition.y > yPosBound || tPosition.y < yNegBound ||
       tPosition.z > zPosBound || tPosition.z < zNegBound)
      ret = true;
   else if (life <= 0)
      ret = true;

   return ret;
}

int Bullet::getCooldown() {
   return cooldown;
}

Weapon Bullet::getType() {
   return type;
}