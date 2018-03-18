/* START OF 'transformnode.cpp' FILE */


#include "nodevisitor.h"
#include "transformnode.h"


TRANSFORM_NODE::TRANSFORM_NODE (void)
{
}


TRANSFORM_NODE::TRANSFORM_NODE (const glm::mat4 & newTransform) :
   transform(newTransform)
{
}


TRANSFORM_NODE::~TRANSFORM_NODE (void)
{
}


void TRANSFORM_NODE::AddChild (SCENE_NODE * node)
{
   if (!node) {
      return;
   }

   node->AddParent(this);
   children.push_back(node);
}


void TRANSFORM_NODE::Descend (NODE_VISITOR & nv)
{
   for (auto it : children) {
      it->Accept(nv);
   }
}


void TRANSFORM_NODE::Accept (NODE_VISITOR & nv)
{
   nv.Apply(*this);
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


/* END OF 'transformnode.cpp' FILE */