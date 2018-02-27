/* START OF 'camera.h' FILE */

#pragma once
#ifndef CAMERA_H
#define CAMERA_H


class CAMERA {
   friend class DISPLAY;
   friend class RENDER;

public:
   CAMERA (void);
   CAMERA (const CAMERA & newCamera);

   ~CAMERA (void);

   CAMERA & operator= (const CAMERA & newCamera);

   glm::mat4 GetWorldToViewMatrix (void) const;

private:
   void MouseUpdate (const POINT & newPos, const float rotationSpeed);

private:
   glm::vec3 position;
   glm::vec3 viewDirection;
   glm::vec3 up;
};


#endif // CAMERA_H

/* END OF 'camera.h' FILE */
