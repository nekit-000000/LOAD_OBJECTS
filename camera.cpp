/* START OF 'camera.cpp' FILE */

#include "camera.h"
#include <glm/gtx/transform.hpp>


CAMERA::CAMERA (void) : 
   viewDirection(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), position(0.0f, 0.0f, -10.0f)
{
}


CAMERA::~CAMERA(void)
{
}


glm::mat4 CAMERA::GetWorldToViewMatrix (void) const
{
   return glm::lookAt(position, position + viewDirection, up);
}


/* END OF 'camera.cpp' FILE */
