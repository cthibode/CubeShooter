struct Material {
   vec3 dColor;
   vec3 sColor;
   vec3 aColor;
   float shine;
};

uniform int uShadeMode;
uniform Material uMat;

varying vec3 vPos, vNorm;

void main() {
   gl_FragColor = vec4(uMat.aColor, 1.0);
}