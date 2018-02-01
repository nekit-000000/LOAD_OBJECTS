/* START OF 'objmodel.cpp' FILE */

#include <vector>
#include <fstream>
#include <windows.h>

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
}


objMODEL & objMODEL::operator= (const objMODEL & prim)
{
   vertices = prim.vertices;
   normals = prim.normals;
   vertexIndices = prim.vertexIndices;
   normalIndices = prim.normalIndices;
   vertexColors = prim.vertexColors;

   return *this;
}


/* END OF 'objmodel.cpp' FILE */
