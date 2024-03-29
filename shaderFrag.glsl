#define MAX_LIGHTS 15
#define MIN_LIGHTS 6

struct Material {
   vec3 dColor;
   vec3 sColor;
   vec3 aColor;
   float shine;
};

uniform vec3 uCamPos;
uniform vec3 uLightPos[MAX_LIGHTS];
uniform vec3 uLightColor[MAX_LIGHTS];
uniform int uNumLights;
uniform int uShadeMode;
uniform Material uMat;

varying vec3 vPos, vNorm;
varying vec4 vColor;

void main() {
   vec4 totalColor = vec4(0, 0, 0, 1);
   vec3 pos, norm, light, view, halfVec;
   float angleNL, angleNH, attenuation;

   /* Blinn Phong shading */
   if (uShadeMode == 0) {
      pos = normalize(vPos);
      norm = normalize(vNorm);
      view = normalize(uCamPos - vPos);

      for (int count = 0; count < uNumLights; count++) {
         light = normalize(uLightPos[count]);
         
         angleNL = clamp(dot(norm, light), 0.0, 1.0);

         halfVec = (light + view) / length(light + view);
         angleNH = clamp(dot(norm, halfVec), 0.0, 1.0);
         angleNH = pow(angleNH, uMat.shine);

         if (count < MIN_LIGHTS)
            attenuation = 0.7 * distance(vPos, uLightPos[count]);
         else
            attenuation = 2 * distance(vPos, uLightPos[count]);

         totalColor.rgb += (uLightColor[count] * uMat.dColor * angleNL + uLightColor[count] * uMat.sColor * angleNH + 
                           uLightColor[count] * uMat.aColor) / attenuation;
      }
   }
   /* Ambient only */
   else if (uShadeMode == 1) {
      totalColor.rgb = uMat.aColor;
   }
   /* Attribute color (for particles) */
   else if (uShadeMode == 2) {
      totalColor = vColor;
   }

   gl_FragColor = totalColor;
}