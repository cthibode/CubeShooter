attribute vec3 aPosition;
attribute vec3 aNormal;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

void main() {
   gl_Position = vec4(aPosition.xyz, 1.0);
   gl_FrontColor = vec4(0.5, 0.5, 0.5, 1.0);
}