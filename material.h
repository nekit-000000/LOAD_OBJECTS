/* START OF 'material.h' FILE */


#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H


#include "texture.h"


class MATERIAL {
   friend class RENDER;
   friend class SHADER_MODEL;

public:
   MATERIAL (void);
   MATERIAL (const TEXTURE & tex, const glm::vec3 & ka = glm::vec3(1, 1, 1), const glm::vec3 & kd = glm::vec3(1, 1, 1),
      const glm::vec3 & ks = glm::vec3(1, 1, 1));

   void SetMap            (HBITMAP & hBmp);
   const TEXTURE & GetMap (void) const { return map; };

public:
   glm::vec3 ka;
   glm::vec3 kd;
   glm::vec3 ks;

private:
   TEXTURE map;
};


#endif // MATERIAL_H


/* END OF 'material.h' FILE */
