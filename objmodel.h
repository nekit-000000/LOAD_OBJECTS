/* START OF 'objmodel.h' FILE */

#pragma once
#ifndef OBJMODEL_H
#define OBJMODEL_H

class objMODEL {
   friend class DISPLAY;
   friend class RENDER;
   friend class OBJECT_NODE * LoadObj (const std::string & fileName);

public:
   objMODEL (void);
   objMODEL (const objMODEL & prim);

   ~objMODEL (void);

   objMODEL & operator= (const objMODEL & prim);

private:
   std::vector<glm::vec3> vertices;
   std::vector<glm::vec3> normals;
   std::vector<size_t> vertexIndices;
   std::vector<size_t> normalIndices;
   std::vector<COLORREF> vertexColors;
};


#endif // OBJMODEL_H

/* END OF 'objmodel.h' FILE */
