
attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec4 aColor;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;
uniform int uShadeMode;

varying vec3 vPos, vNorm;
varying vec4 vColor;

void main() {
   vec4 transPos, transNorm;

   /* Model transforms */
   gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);

   /* Calculate the transformed position and normals */
   transPos = uModelMatrix * vec4(aPosition, 1);
   transNorm = uModelMatrix * vec4(aNormal, 0);

   /* Interpolate the position and normal and send to the fragment shader */
   vPos = vec3(transPos.xyz);
   vNorm = vec3(transNorm.xyz);
   vColor = aColor;
}