#include "shader.h"

Shader::Shader() {
   shadeProg = 0;
}

Shader::~Shader() {
   if (shadeProg != 0)
      glDeleteProgram(shadeProg);
}

/* Create the shader program */
void Shader::initialize() {
   shadeProg = glCreateProgram();
}

/* Compile and link the given shader data to the program */
int Shader::installShader(const GLchar *shaderData, ShaderType type) {
   GLint shader = 0;
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
   if (!compiled)
      return 0;

   /* Link the shader to the program */
   glAttachShader(shadeProg, shader);
   glLinkProgram(shadeProg);
   glGetProgramiv(shadeProg, GL_LINK_STATUS, &linked);
   if (!linked)
      return 0;

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

   h_uMatDif = glGetUniformLocation(shadeProg, "uMat.dColor");
   h_uMatSpec = glGetUniformLocation(shadeProg, "uMat.sColor");
   h_uMatAmb = glGetUniformLocation(shadeProg, "uMat.aColor");
   h_uMatShine = glGetUniformLocation(shadeProg, "uMat.shine");
   h_uShadeMode = glGetUniformLocation(shadeProg, "uShadeMode");

   h_uLightPos = glGetUniformLocation(shadeProg, "uLightPos");
   h_uLightColor = glGetUniformLocation(shadeProg, "uLightColor");
   h_uNumLights = glGetUniformLocation(shadeProg, "uNumLights");

   h_uCamPos = glGetUniformLocation(shadeProg, "uCamPos");
}

/* Set the material type to the given color */
void Shader::setMaterial(Color color) {
   switch (color) {
      case GRAY:
         glUniform3f(h_uMatDif, 0.5f, 0.5f, 0.5f);
         glUniform3f(h_uMatSpec, 0.5f, 0.5f, 0.5f);
         glUniform3f(h_uMatAmb, 0.2f, 0.2f, 0.2f);
         glUniform1f(h_uMatShine, 1.0);
         glUniform1i(h_uShadeMode, PHONG);
         break;
      case DARK_GRAY:
         glUniform3f(h_uMatDif, 0.3f, 0.3f, 0.3f);
         glUniform3f(h_uMatSpec, 0.3f, 0.3f, 0.3f);
         glUniform3f(h_uMatAmb, 0.12f, 0.12f, 0.12f);
         glUniform1f(h_uMatShine, 1.0f);
         glUniform1i(h_uShadeMode, PHONG);
         break;
      case WHITE:
         glUniform3f(h_uMatDif, 1.0f, 1.0f, 1.0f);
         glUniform3f(h_uMatSpec, 1.0f, 1.0f, 1.0f);
         glUniform3f(h_uMatAmb, 0.5f, 0.5f, 0.5f);
         glUniform1f(h_uMatShine, 1.0f);
         glUniform1i(h_uShadeMode, PHONG);
         break;
      case BROWN:
         glUniform3f(h_uMatDif, 0.6f, 0.4f, 0.0f);
         glUniform3f(h_uMatSpec, 1.0f, 1.0f, 0.5f);
         glUniform3f(h_uMatAmb, 0.3f, 0.2f, 0.0f);
         glUniform1f(h_uMatShine, 100.0);
         glUniform1i(h_uShadeMode, PHONG);
         break;
      case GLOW_YELLOW:
         glUniform3f(h_uMatDif, 1.0f, 1.0f, 0.0f);
         glUniform3f(h_uMatSpec, 1.0f, 1.0f, 0.0f);
         glUniform3f(h_uMatAmb, 1.0f, 1.0f, 0.0f);
         glUniform1f(h_uMatShine, 1.0f);
         glUniform1i(h_uShadeMode, AMBIENT);
         break;
   }
}

GLint Shader::getPositionHandle() {
   return h_aPosition;
}

GLint Shader::getNormalHandle() {
   return h_aNormal;
}

GLint Shader::getModelMatHandle() {
   return h_uModelMatrix;
}

GLint Shader::getViewMatHandle() {
   return h_uViewMatrix;
}

GLint Shader::getProjMatHandle() {
   return h_uProjMatrix;
}

GLint Shader::getLightPosHandle() {
   return h_uLightPos;
}

GLint Shader::getLightColorHandle() {
   return h_uLightColor;
}

GLint Shader::getNumLightsHandle() {
   return h_uNumLights;
}

GLint Shader::getCamPosHandle() {
   return h_uCamPos;
}