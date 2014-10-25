#include "geom.h"

vector<float> Cube::cubeVerts = vector<float>();
vector<float> Cube::cubeNorms = vector<float>();
vector<unsigned short> Cube::cubeIndexes = vector<unsigned short>();
GLuint Cube::cubeVbo = 0;
GLuint Cube::cubeNbo = 0;
GLuint Cube::cubeIbo = 0;
unsigned int Cube::cubeIboLen = 0;

Cube::Cube() : Geom() {
   if (cubeIboLen == 0)
      cubeInit();

   vertBuffObj = cubeVbo;
   normBuffObj = cubeNbo;
   indexBuffObj = cubeIbo;
   iboLen = cubeIboLen;
}

Cube::~Cube() {

}

/* Create the buffer objects for a square */
void Cube::cubeInit() {
   float const verts[] = {
      -0.5, -0.5, -0.5, // back face verts [0-3]
      -0.5, 0.5, -0.5,
      0.5, 0.5, -0.5,
      0.5, -0.5, -0.5,

      -0.5, -0.5, 0.5, // front face verts [4-7]
      -0.5, 0.5, 0.5,
      0.5, 0.5, 0.5,
      0.5, -0.5, 0.5,

      -0.5, -0.5, 0.5, // left face verts [8-11]
      -0.5, -0.5, -0.5,
      -0.5, 0.5, -0.5,
      -0.5, 0.5, 0.5,

      0.5, -0.5, 0.5, // right face verts [12-15]
      0.5, -0.5, -0.5,
      0.5, 0.5, -0.5,
      0.5, 0.5, 0.5,

      -0.5, 0.5, 0.5, // top face verts [16-19]
      -0.5, 0.5, -0.5,
      0.5, 0.5, -0.5,
      0.5, 0.5, 0.5,

      -0.5, -0.5, 0.5, // bottom face verts [20-23]
      -0.5, -0.5, -0.5,
      0.5, -0.5, -0.5,
      0.5, -0.5, 0.5
   };

   float const norms[] = {
      0, 0, -1, // back face verts [0-3]
      0, 0, -1,
      0, 0, -1,
      0, 0, -1,

      0, 0, 1, // front face verts [4-7]
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,

      -1, 0, 0, // left face verts [8-11]
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0,

      1, 0, 0, // right face verts [12-15]
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,

      0, 1, 0, // top face verts [16-19]
      0, 1, 0,
      0, 1, 0,
      0, 1, 0,

      0, -1, 0, // bottom face verts [20-23]
      0, -1, 0,
      0, -1, 0,
      0, -1, 0
   };

   unsigned short const indexes[] {
      0, 1, 2, // back face verts [0-3]
      2, 3, 0,

      4, 7, 6, // front face verts [4-7]
      6, 5, 4,

      8, 11, 10, // left face verts [8-11]
      10, 9, 8,

      12, 13, 14, // right face verts [12-15]
      14, 15, 12,

      16, 19, 18, // top face verts [16-19]
      18, 17, 16,

      20, 21, 22, // bottom face verts [20-23]
      22, 23, 20
   };

   cubeVerts = vector<float>(verts, verts + sizeof(verts) / sizeof(float));
   cubeNorms = vector<float>(norms, norms + sizeof(norms) / sizeof(float));
   cubeIndexes = vector<unsigned short>(indexes, indexes + sizeof(indexes) / sizeof(unsigned short));
   cubeIboLen = cubeIndexes.size();

   geomInit(&cubeVbo, &cubeVerts, &cubeNbo, &cubeNorms, &cubeIbo, &cubeIndexes);
}

/* Returns true if the given point is within the cube's bounding sphere, otherwise false */
bool Cube::isColliding(vec3 point) {
   return (glm::distance(tPosition, point) < radius);
}

/* Returns true if the two cubes are intersecting, otherwise false */
bool Cube::isColliding(Cube *cube) {
   return (glm::distance(tPosition, cube->tPosition) < (radius + cube->radius));
}
