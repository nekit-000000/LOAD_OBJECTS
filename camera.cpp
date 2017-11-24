/* START OF 'camera.cpp' FILE */

#include <glm/gtx/transform.hpp>

#include "camera.h"


CAMERA::CAMERA (void) : 
   viewDirection(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), position(0.0f, 0.0f, -10.0f)
{
}


CAMERA::~CAMERA(void)
{
}


void CAMERA::MouseUpdate (const POINT & mouseDelta, const float rotationSpeed)
{
   float angleX = mouseDelta.x * rotationSpeed;
   float angleY = mouseDelta.y * rotationSpeed;

   // Horizontal rotation
   viewDirection = glm::mat3(glm::rotate(angleX, up)) * viewDirection;

   // Vertical rotation
   glm::vec3 rotateAround = glm::cross(viewDirection, up);
   viewDirection = glm::mat3(glm::rotate(angleY, rotateAround)) * viewDirection;
}


glm::mat4 CAMERA::GetWorldToViewMatrix (void) const
{
   return glm::lookAt(position, position + viewDirection, up);
}


/* END OF 'camera.cpp' FILE */
