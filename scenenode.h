/* START OF 'scenenode.h' FILE */


#pragma once
#ifndef SCENE_NODE_H
#define SCENE_NODE_H


#include "intrusiveptr.h"
#include "referenced.h"


// Forward declarations
class TRANSFORM_NODE;
class SCENE_NODE;
class NODE_VISITOR;


// Type declaration
using NODE_PATH = std::vector<SCENE_NODE *>;
using NODE_PATH_LIST = std::vector<NODE_PATH>;


class SCENE_NODE : public REFERENCED {
   friend class RENDER;
   friend class TRANSFORM_NODE;
   friend class DISPLAY;

public:
   using PARENT_LIST = std::vector<TRANSFORM_NODE *>;

   SCENE_NODE          (void);

   virtual ~SCENE_NODE (void);

   // Visitor pattern functionality
   virtual void Accept                     (NODE_VISITOR & nv);
   virtual void Ascend                     (NODE_VISITOR & nv);

   inline const PARENT_LIST & GetParents   (void) const     { return parents; }
   inline PARENT_LIST GetParents           (void)           { return parents; }
   inline TRANSFORM_NODE * GetParent       (size_t i)       { return parents[i]; }
   inline const TRANSFORM_NODE * GetParent (size_t i) const { return parents[i]; }
   inline size_t GetNumParents             (void) const     { return parents.size(); }
   void  AddParent                         (TRANSFORM_NODE * parent);
   NODE_PATH_LIST GetParentalNodePaths     (SCENE_NODE * stopNode = NULL) const;

protected:
   PARENT_LIST parents;
};


#endif // SCENE_NODE_H


/* END OF 'scenenode.h' FILE */
