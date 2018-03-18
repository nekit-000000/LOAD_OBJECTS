/* START OF 'objectnode.h' FILE */


#pragma once
#ifndef OBJECT_NODE_H
#define OBJECT_NODE_H


#include "scenenode.h"
#include "objmodel.h"


class OBJECT_NODE : public SCENE_NODE {
   friend class RENDER;

public:
   OBJECT_NODE  (void);
   OBJECT_NODE  (const objMODEL & newObj);

   ~OBJECT_NODE (void);

   void SetObject (const objMODEL & newObj);
   void Accept    (NODE_VISITOR & nv);

private:
   objMODEL object;
};


#endif // OBJECT_NODE_H


/* END OF 'objectnode.h' FILE */
