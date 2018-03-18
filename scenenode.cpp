/* START OF 'scenenode.cpp' FILE */


#include "nodevisitor.h"
#include "scenenode.h"
#include "transformnode.h"


// Visitor that collects parent's paths
class COLLECT_PARENT_PATHS : public NODE_VISITOR {
public:
   COLLECT_PARENT_PATHS (const SCENE_NODE * stopNode = NULL) :
      stopNode(stopNode)
   {
   }

   void Apply (SCENE_NODE & node)
   {
      if (node.GetNumParents() == 0 || &node == stopNode) {
         nodePathList.push_back(GetNodePath());
      } else {
         Apply(node);
      }
   }


public:
   const SCENE_NODE * stopNode;
   NODE_PATH       nodePath;
   NODE_PATH_LIST  nodePathList;
};


SCENE_NODE::SCENE_NODE (void)
{
}


void SCENE_NODE::Accept (NODE_VISITOR & nv)
{
   nv.nodePath.push_back(this);
   nv.Apply(*this);
   nv.nodePath.pop_back();
}


void SCENE_NODE::Ascend (NODE_VISITOR & nv)
{
   for (auto it : parents) {
      it->Accept(nv);
   }
}


NODE_PATH_LIST SCENE_NODE::GetParentalNodePaths (SCENE_NODE * stopNode) const
{
   COLLECT_PARENT_PATHS cpp(stopNode);
   const_cast<SCENE_NODE *>(this)->Accept(cpp);
   return cpp.nodePathList;
}


void  SCENE_NODE::AddParent (TRANSFORM_NODE * parent)
{
   parents.push_back(parent);
}


SCENE_NODE::~SCENE_NODE (void)
{
}


/* END OF 'scenenode.cpp' FILE */
