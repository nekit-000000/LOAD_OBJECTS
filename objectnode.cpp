/* START OF 'objectnode.cpp' FILE */


#include "nodevisitor.h"
#include "objectnode.h"


OBJECT_NODE::OBJECT_NODE (void)
{
}


OBJECT_NODE::OBJECT_NODE (const objMODEL & newObj) :
   object(newObj)
{
}


OBJECT_NODE::~OBJECT_NODE (void)
{
}


void OBJECT_NODE::SetObject (const objMODEL & newObj)
{
   object = newObj;
}


void OBJECT_NODE::Accept (NODE_VISITOR & nv)
{
   nv.Apply(*this);
}


/* END OF 'objectnode.cpp' FILE */
