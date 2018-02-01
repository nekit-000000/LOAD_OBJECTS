/* START OF 'scene.h' FILE */

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "objmodel.h"


class SCENE_NODE {
   friend class RENDER;
   friend class TRANSFORM_NODE;

public:
   enum class  NODE_TYPE {
      TRANSFORM,
      OBJECT
   };

public:
   SCENE_NODE (void);

   virtual ~SCENE_NODE (void);

protected:
   NODE_TYPE type;
   SCENE_NODE * link;
   SCENE_NODE * next;
};


class TRANSFORM_NODE : public SCENE_NODE {
   friend class RENDER;

public:
   TRANSFORM_NODE (void);
   TRANSFORM_NODE (const glm::mat4 & newTransform);

   ~TRANSFORM_NODE (void);

   void AddChild                  (SCENE_NODE * node);
   void Rotate                    (const glm::vec3 & rotateVec, const float phi);
   void Scale                     (const glm::vec3 & scaleVec);
   void Scale                     (const float coeff);
   void Translate                 (const glm::vec3 & translateVec);
   void SetTransform              (const glm::mat4 & matrix);
   const glm::mat4 & GetTransform (void) const { return transform; }

private:
   glm::mat4 transform;
};


class OBJECT_NODE : public SCENE_NODE {
   friend class RENDER;

public:
   OBJECT_NODE (void);
   OBJECT_NODE (const objMODEL & newObj);

   ~OBJECT_NODE (void);

   void SetObject (const objMODEL & newObj);

private:
   objMODEL object;
};


#endif // SCENE_H

/* END OF 'scene.h' FILE */
