#include "geom.h"

vector<float> Square::squareVerts = vector<float>();
vector<float> Square::squareNorms = vector<float>();
vector<unsigned short> Square::squareIndexes = vector<unsigned short>();
GLuint Square::squareVbo = 0;
GLuint Square::squareNbo = 0;
GLuint Square::squareIbo = 0;
unsigned int Square::squareIboLen = 0;

Square::Square() : Geom() {
   if (squareIboLen == 0)
      squareInit();

   vertBuffObj = squareVbo;
   normBuffObj = squareNbo;
   indexBuffObj = squareIbo;
   iboLen = squareIboLen;
}

Square::~Square() {

}

/* Create the buffer objects for a square */
void Square::squareInit() {
   float const verts[] = {
      -0.5, 0.5, 0,
      0.5, 0.5, 0,
      -0.5, -0.5, 0, 
      0.5, -0.5, 0
   };

   float const norms[] = {
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,
      0, 0, 1
   };

   unsigned short const indexes[] {
      0, 2, 1,
      1, 2, 3
   };

   squareVerts = vector<float>(verts, verts + sizeof(verts) / sizeof(float));
   squareNorms = vector<float>(norms, norms + sizeof(norms) / sizeof(float));
   squareIndexes = vector<unsigned short>(indexes, indexes + sizeof(indexes) / sizeof(unsigned short));
   squareIboLen = squareIndexes.size();

   geomInit(&squareVbo, &squareVerts, &squareNbo, &squareNorms, &squareIbo, &squareIndexes);
}