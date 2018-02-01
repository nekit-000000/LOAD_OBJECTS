/* START OF 'scene.cpp' FILE */

#include <glm/gtx/transform.hpp>
#include <stack>

#include "scene.h"


SCENE_NODE::SCENE_NODE (void) : 
   link(NULL), next(NULL)
{
}


TRANSFORM_NODE::TRANSFORM_NODE (void)
{
   type = SCENE_NODE::NODE_TYPE::TRANSFORM;
}


TRANSFORM_NODE::TRANSFORM_NODE (const glm::mat4 & newTransform) :
   transform(newTransform)
{
   type = SCENE_NODE::NODE_TYPE::TRANSFORM;
}


OBJECT_NODE::OBJECT_NODE (void)
{
   type = SCENE_NODE::NODE_TYPE::OBJECT;
}


OBJECT_NODE::OBJECT_NODE (const objMODEL & newObj) :
   object(newObj)
{
   type = SCENE_NODE::NODE_TYPE::OBJECT;
}


OBJECT_NODE::~OBJECT_NODE (void)
{
}


TRANSFORM_NODE::~TRANSFORM_NODE (void)
{
}


SCENE_NODE::~SCENE_NODE (void)
{
   SCENE_NODE * node = this;
   std::stack<SCENE_NODE *> arr;

   arr.push(node);
   while (!arr.empty()) {
      node = arr.top();
      arr.pop();

      while (node->link != NULL || node->next != NULL) {
         while (node->next != NULL) {
            arr.push(node);
            node = node->next;
         }

         if (node->link != NULL) {
            arr.push(node);
            node = node->link;
         }
      }

      if (!arr.empty()) {
         if (arr.top()->link == node) {
            arr.top()->link = NULL;
         } else {
            arr.top()->next = NULL;
         }
      }

      if (node != this) {
         delete node;
      } else {
         break;
      }
   }
}


void TRANSFORM_NODE::AddChild (SCENE_NODE * node)
{
   if (link == NULL) {
      link = node;
   } else {
      SCENE_NODE * itNode = link;
      while (itNode->next != NULL) {
         itNode = itNode->next;
      }

      itNode->next = node;
   }
}


void TRANSFORM_NODE::SetTransform (const glm::mat4 & matrix)
{ 
   transform = matrix; 
}


void TRANSFORM_NODE::Translate (const glm::vec3 & translateVec)
{
   glm::mat4 translateMatr = glm::translate(translateVec);
   transform = translateMatr * transform;
}


void TRANSFORM_NODE::Rotate (const glm::vec3 & vec, const float phi)
{
   glm::mat4 rotateMatr = glm::rotate(phi, vec);
   transform = rotateMatr * transform;
}


void TRANSFORM_NODE::Scale (const glm::vec3 & scaleVec)
{
   glm::mat4 scaleMatr = glm::scale(scaleVec);
   transform = scaleMatr * transform;
}


void TRANSFORM_NODE::Scale (const float coeff)
{
   glm::mat4 scaleMatr = glm::scale(glm::vec3(coeff, coeff, coeff));
   transform = scaleMatr * transform;
}


void OBJECT_NODE::SetObject (const objMODEL & newObj)
{
   object = newObj; 
}


/* END OF 'scene.cpp' FILE */
