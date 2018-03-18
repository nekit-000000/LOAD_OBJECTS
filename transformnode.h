/* START OF 'transformnode.h' FILE */


#pragma once
#ifndef TRANSFORM_NODE_H
#define TRANSFORM_NODE_H


#include "scenenode.h"


class TRANSFORM_NODE : public SCENE_NODE {
   friend class RENDER;

public:
   using NODE_LIST = std::vector<INTRUSIVE_PTR<SCENE_NODE>>;

   TRANSFORM_NODE  (void);
   TRANSFORM_NODE  (const glm::mat4 & newTransform);

   ~TRANSFORM_NODE (void);

   void AddChild                  (SCENE_NODE * node);
   void Rotate                    (const glm::vec3 & rotateVec, const float phi);
   void Scale                     (const glm::vec3 & scaleVec);
   void Scale                     (const float coeff);
   void Translate                 (const glm::vec3 & translateVec);
   void SetTransform              (const glm::mat4 & matrix);
   const glm::mat4 & GetTransform (void) const { return transform; }

   template <typename VALUE_TYPE> 
   void AddChild                  (const INTRUSIVE_PTR<VALUE_TYPE> & child) { AddChild(child.Get()); };

   void Descend                   (NODE_VISITOR & nv);
   void Accept                    (NODE_VISITOR & nv);

private:
   glm::mat4 transform;
   NODE_LIST children;
};


#endif // TRANSFORM_NODE_H


/* END OF 'transformnode.h' FILE */
