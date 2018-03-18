/* START OF 'nodevisitor.cpp' FILE */

#include "nodevisitor.h"


void NODE_VISITOR::Apply (SCENE_NODE & node)
{
   node.Ascend(*this);
}


void NODE_VISITOR::Apply (OBJECT_NODE & node)
{
   Apply(static_cast<SCENE_NODE &>(node));
}


void NODE_VISITOR::Apply (TRANSFORM_NODE & node)
{
   Apply(static_cast<SCENE_NODE &>(node));
}


/* END OF 'nodevisitor.cpp' FILE */
