#include "camera.h"

Camera::Camera() : upVector(vec3(0, 1, 0)) {
   eye = vec3(0, 1, 0);
   lookAt = vec3(0, 1, -1);
   xPosBound = xNegBound = zPosBound = zNegBound = NULL;
}

Camera::~Camera() {

}

/* Set the boundaries of the camera's location. Set to NULL for no boundaries. */
void Camera::setBounds(float xPos, float xNeg, float zPos, float zNeg) {
   xPosBound = xPos;
   xNegBound = xNeg;
   zPosBound = zPos;
   zNegBound = zNeg;
}

/* Calculate the view matrix and send to the shader */
void Camera::setView(GLint viewMatHandle) {
   mat4 View = glm::lookAt(eye, lookAt, upVector);
   glUniformMatrix4fv(viewMatHandle, 1, GL_FALSE, value_ptr(View));
}

void Camera::setCamPos(GLint camPosHandle) {
   glUniform3f(camPosHandle, eye.x, eye.y, eye.z);
}

/* Move the camera forward/back at the given speed. Positive speed moves forward, negative speed moves back. */
void Camera::moveFB(float speed) {
   vec3 wAxis = normalize(eye - lookAt);
   wAxis = normalize(vec3(wAxis.x, 0, wAxis.z));

   eye.x += -wAxis.x * speed;
   eye.z += -wAxis.z * speed;
   lookAt.x += -wAxis.x * speed;
   lookAt.z += -wAxis.z * speed;

   if (xPosBound != NULL && eye.x > xPosBound)
      eye.x = xPosBound;
   else if (xNegBound != NULL && eye.x < xNegBound)
      eye.x = xNegBound;
   if (zPosBound != NULL && eye.z > zPosBound)
      eye.z = zPosBound;
   else if (zNegBound != NULL && eye.z < zNegBound)
      eye.z = zNegBound;
}

/* Move the camera left/right at the given speed. Positive speed moves right, negative speed moves left. */
void Camera::moveLR(float speed) {
   vec3 wAxis = normalize(eye - lookAt);
   vec3 uAxis = normalize(cross(upVector, wAxis));

   eye += uAxis * speed;
   lookAt += uAxis * speed;

   if (xPosBound != NULL && eye.x > xPosBound)
      eye.x = xPosBound;
   else if (xNegBound != NULL && eye.x < xNegBound)
      eye.x = xNegBound;
   if (zPosBound != NULL && eye.z > zPosBound)
      eye.z = zPosBound;
   else if (zNegBound != NULL && eye.z < zNegBound)
      eye.z = zNegBound;
}

/* Change the direction the camera points in 
 * params: dx, dy - the displacement of the cursor from the center of the screen 
 *         width, height - the dimensions of the window
 */
void Camera::moveLookAt(int dx, int dy, int width, int height) {
   static float pitchAng = 0;
   static float yawAng = -PI/2.0;

   pitchAng += dy * PI / height;
   yawAng += dx * PI / width;

   if (pitchAng >= DEG_TO_RAD(80))
      pitchAng = DEG_TO_RAD(80);
   else if (pitchAng <= DEG_TO_RAD(-80))
      pitchAng = DEG_TO_RAD(-80);

   lookAt.x = eye.x + cos(pitchAng) * cos(yawAng);
   lookAt.y = eye.y + sin(pitchAng);
   lookAt.z = eye.z + cos(pitchAng) * cos(PI/2 - yawAng);
}