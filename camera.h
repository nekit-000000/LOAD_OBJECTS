/* START OF 'camera.h' FILE */

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>


class CAMERA {
   friend class DISPLAY;
public:
   CAMERA (void);

   ~CAMERA (void);

   glm::mat4 GetWorldToViewMatrix (void) const;

private:
   glm::vec3 position;
   glm::vec3 viewDirection;
   glm::vec3 up;
};


#endif // CAMERA_H

/* END OF 'camera.h' FILE */
