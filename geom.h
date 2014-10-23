#include <vector>
#include "main.h" 

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

/* A Square that acts as a wall for the stage */
class Wall : public Square {
   public:
      Wall();
      ~Wall();
};