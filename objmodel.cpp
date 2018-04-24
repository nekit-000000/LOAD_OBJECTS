/* START OF 'objmodel.cpp' FILE */


#include "objmodel.h"


objMODEL::objMODEL (void)
{
}


objMODEL::~objMODEL (void)
{
}


objMODEL::objMODEL (const objMODEL & prim)
{
   vertices = prim.vertices;
   normals = prim.normals;
   vertexIndices = prim.vertexIndices;
   normalIndices = prim.normalIndices;
   vertexColors = prim.vertexColors;
   uvIndices = prim.uvIndices;
   uvCoords = prim.uvCoords;
   mat = prim.mat;
}


objMODEL & objMODEL::operator= (const objMODEL & prim)
{
   vertices = prim.vertices;
   normals = prim.normals;
   vertexIndices = prim.vertexIndices;
   normalIndices = prim.normalIndices;
   vertexColors = prim.vertexColors;
   mat = prim.mat;

   return *this;
}


/* END OF 'objmodel.cpp' FILE */
