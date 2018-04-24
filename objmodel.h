/* START OF 'objmodel.h' FILE */

#pragma once
#ifndef OBJMODEL_H
#define OBJMODEL_H


#include "material.h"
#include "intrusiveptr.h"


// Forward declaration
class OBJECT_NODE;


class objMODEL {
   friend class DISPLAY;
   friend class RENDER;
   friend class SHADER_MODEL;
   friend INTRUSIVE_PTR<OBJECT_NODE> LoadObj (const std::string & fileName);

public:
   objMODEL (void);
   objMODEL (const objMODEL & prim);

   ~objMODEL (void);

   objMODEL & operator= (const objMODEL & prim);

private:
   MATERIAL mat;
   std::vector<glm::vec3> vertices;
   std::vector<glm::vec3> normals;
   std::vector<size_t> vertexIndices;
   std::vector<size_t> normalIndices;
   std::vector<COLORREF> vertexColors;
   std::vector<glm::vec2> uvCoords;
   std::vector<size_t> uvIndices;
};


#endif // OBJMODEL_H

/* END OF 'objmodel.h' FILE */
