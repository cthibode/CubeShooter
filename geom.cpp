#include "geom.h"

Geom::Geom() {
   tPosition = vec3(0, 0, 0);
   tRotPitch = tRotRoll = tRotYaw = 0;
}

Geom::~Geom() {

}

/* Create the buffer objects for a shape */
void Geom::geomInit(GLuint *vbo, vector<float> *verts, GLuint *nbo, vector<float> *norms, GLuint *ibo, vector<unsigned short> *indexes) {
   glGenBuffers(1, vbo);
   glBindBuffer(GL_ARRAY_BUFFER, *vbo);
   glBufferData(GL_ARRAY_BUFFER, verts->size() * sizeof(float), verts->data(), GL_STATIC_DRAW);

   glGenBuffers(1, nbo);
   glBindBuffer(GL_ARRAY_BUFFER, *nbo);
   glBufferData(GL_ARRAY_BUFFER, norms->size() * sizeof(float), norms->data(), GL_STATIC_DRAW);

   glGenBuffers(1, ibo);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ibo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes->size() * sizeof(unsigned short), indexes->data(), GL_STATIC_DRAW);
}

/* Draw the shape */
void Geom::draw(GLint positionHandle, GLint normalHandle, GLint modelMatHandle) {
   transform(modelMatHandle);

   glEnableVertexAttribArray(positionHandle);
   glEnableVertexAttribArray(normalHandle);
   
   glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
   glVertexAttribPointer(positionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ARRAY_BUFFER, normBuffObj);
   glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffObj);
   glDrawElements(GL_TRIANGLES, iboLen, GL_UNSIGNED_SHORT, 0);

   glDisableVertexAttribArray(positionHandle);
   glDisableVertexAttribArray(normalHandle);
}

/* (Private) Translate, rotate, and scale the shape */
void Geom::transform(GLuint modelMatHandle) {
   mat4 scaleMat = scale(mat4(1.0), tScale);
   mat4 rotateMat = rotate(mat4(1.0), tRotYaw, vec3(0, 1, 0)) * rotate(mat4(1.0), tRotPitch, vec3(1, 0, 0)) * 
      rotate(mat4(1.0), tRotRoll, vec3(0, 0, 1));
   mat4 transMat = translate(mat4(1.0), tPosition);
   mat4 transform = transMat * rotateMat * scaleMat;
   glUniformMatrix4fv(modelMatHandle, 1, GL_FALSE, value_ptr(transform));
}

void Geom::setPosition(vec3 pos) {
   tPosition = pos;
}

void Geom::setScale(vec3 sca) {
   tScale = sca;
}

void Geom::setRotation(float pitch, float roll, float yaw) {
   tRotPitch = pitch;
   tRotRoll = roll;
   tRotYaw = yaw;
}

void Geom::setColor(Color col) {
   color = col;
}

vec3 Geom::getPosition() {
   return tPosition;
}

Color Geom::getColor() {
   return color;
}