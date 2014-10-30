#include <vector>
#include "main.h" 

enum EnemyType {NORMAL, BIG, HIGH_JUMP, ET_END};
enum EnemyState {SPAWN, LIVE, DIE, DEAD};
enum Weapon {PISTOL, MACHINE, SHOTGUN, W_END};

/* Parent class for all shapes */
class Geom {
   public:
      virtual ~Geom();
      void draw(GLint positionHandle, GLint normalHandle, GLint modelMatHandle);
      void setPosition(vec3 pos);
      void setScale(vec3 sca);
      void setRotation(float pitch, float roll, float yaw);
      void setColor(Color col);
      vec3 getPosition();
      Color getColor();
      
   protected:
      Geom();
      void geomInit(GLuint *vbo, vector<float> *verts, GLuint *nbo, vector<float> *norms, GLuint *ibo, vector<unsigned short> *indexes);
     
      GLuint vertBuffObj, normBuffObj, indexBuffObj;
      unsigned int iboLen;
      vec3 tPosition, tScale;
      float tRotPitch, tRotRoll, tRotYaw;
      Color color;

   private:
      void transform(GLuint modelMatHandle);
};

/* A Geom object that is a square on the xy plane*/
class Square : public Geom {
   public:
      virtual ~Square();

   protected:
      Square();

   private:
      void squareInit();
      static vector<float> squareVerts;
      static vector<float> squareNorms;
      static vector<unsigned short> squareIndexes;
      static GLuint squareVbo, squareNbo, squareIbo;
      static unsigned int squareIboLen;
};

/* A Geom object that is a cube */
class Cube : public Geom {
   public:
      virtual ~Cube();
      bool isColliding(vec3 point);
      bool isColliding(Cube *cube);

   protected:
      Cube();
      float radius;

   private:
      void cubeInit();
      static vector<float> cubeVerts;
      static vector<float> cubeNorms;
      static vector<unsigned short> cubeIndexes;
      static GLuint cubeVbo, cubeNbo, cubeIbo;
      static unsigned int cubeIboLen;
};

/* A Square that acts as a wall for the stage */
class Wall : public Square {
   public:
      Wall();
      ~Wall();
};

/* A Cube that tries to attack the player */
class Enemy : public Cube {
   public:
      Enemy(EnemyType enemyType);
      ~Enemy();
      void update(vec3 destination);
      bool reduceHealth(int amt);
      EnemyState getState();

   private:
      float age;
      EnemyType type;
      EnemyState state;
      int health;
      float size;
      float jumpHeight;
      float jumpSpeed;
      float moveSpeed;
};

/* A Cube that hits and eliminates enemies */
class Bullet : public Cube {
   public:
      Bullet(Weapon weapon);
      ~Bullet();
      void align(vec3 target);
      void setBounds(float xPos, float xNeg, float yPos, float yNeg, float zPos, float zNeg);
      void update();
      bool shouldRemove();
      int getDamage();
      int getCooldown();
      Weapon getType();

   private:
      Weapon type;
      vec3 direction;
      float xPosBound, xNegBound, yPosBound, yNegBound, zPosBound, zNegBound;
      float speed;
      int damage;
      int cooldown;
      int life;
};

/* A Cube that gives the player a new weapon and increases score when picked up */
class Crate : public Cube {
   public:
      Crate();
      ~Crate();
      void update();
};