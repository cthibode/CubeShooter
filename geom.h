#include <vector>
#include "main.h" 

enum EnemyState {SPAWN, LIVE, DIE};

/* Parent class for all shapes */
class Geom {
   public:
      virtual ~Geom();
      void draw(GLint positionHandle, GLint normalHandle, GLint modelMatHandle);
      void setPosition(vec3 pos);
      void setScale(vec3 sca);
      void setRotation(float pitch, float roll, float yaw);
      void setColor(Color col);
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

   protected:
      Cube();

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
      Enemy();
      ~Enemy();
      void update(vec3 destination);

   private:
      float age;
      EnemyState state;
      
      int health;
      float radius;
      float jumpHeight;
      float jumpSpeed;
      float moveSpeed;
};

class Bullet : public Cube {
   public:
      Bullet();
      ~Bullet();
      void align(vec3 target);
      void update();

   private:
      vec3 direction;
      float speed;
};