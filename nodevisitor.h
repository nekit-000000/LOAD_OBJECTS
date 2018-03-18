/* START OF 'nodevisitor.h' FILE */


#pragma once
#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H


#include "transformnode.h"
#include "objectnode.h"


class NODE_VISITOR {
protected:
   NODE_PATH GetNodePath (void) { return nodePath; };

public:
   virtual void Apply    (SCENE_NODE & node);
   virtual void Apply    (OBJECT_NODE & node);
   virtual void Apply    (TRANSFORM_NODE & node);

public:
   NODE_PATH nodePath;
};


#endif // NODE_VISITOR_H


/* END OF 'nodevisitor.h' FILE */
