#include "shader.h"
#include <stdio.h>

Shader::Shader() {
   shadeProg = 0;
}

Shader::~Shader() {
   if (shadeProg != 0)
      glDeleteProgram(shadeProg);
}

void Shader::initialize() {
   shadeProg = glCreateProgram();
}

/* Compile and link the given shader data to the program */
int Shader::installShader(const GLchar *shaderData, ShaderType type) {
   GLint shader;
   GLint compiled, linked;

   switch (type) {
      case VERT:
         shader = glCreateShader(GL_VERTEX_SHADER);
         break;
      case FRAG:
         shader = glCreateShader(GL_FRAGMENT_SHADER);
         break;
      case GEOM:
         shader = glCreateShader(GL_GEOMETRY_SHADER);
         break;
   }

   /* Compile the shader */
   glShaderSource(shader, 1, &shaderData, NULL);
   glCompileShader(shader);
   glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
   if (!compiled) {
      printf("COMPILE\n");
   
      return 0;
   }
   /* Link the shader to the program */
   glAttachShader(shadeProg, shader);
   glLinkProgram(shadeProg);
   glGetProgramiv(shadeProg, GL_LINK_STATUS, &linked);
   //if (!linked)
   //   return 0;

   glUseProgram(shadeProg);
   return 1;
}

/* Get the handles for the shader attributes and uniforms */
void Shader::initHandles() {
   h_aPosition = glGetAttribLocation(shadeProg, "aPosition");
   h_aNormal = glGetAttribLocation(shadeProg, "aNormal");

   h_uModelMatrix = glGetUniformLocation(shadeProg, "uModelMatrix");
   h_uViewMatrix = glGetUniformLocation(shadeProg, "uViewMatrix");
   h_uProjMatrix = glGetUniformLocation(shadeProg, "uProjMatrix");
}