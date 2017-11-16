/* START OF 'primitive.cpp' FILE */

#include <vector>
#include <iostream>
#include <fstream>
#include "primitive.h"


VERTEX::VERTEX (const glm::vec3 & position) : 
   position(position), color(1, 1, 1), normal(0, 0,0)
{

}


VERTEX::VERTEX (void) :
   position(0, 0, 0), color(1, 1, 1), normal(0, 0, 0)
{

}


VERTEX::VERTEX (const glm::vec3 & position, const glm::vec3 & normal, const glm::vec3 & color) :
   position(position), color(color), normal(normal)
{

}


VERTEX::VERTEX (const glm::vec3 & position, const glm::vec3 & normal) :
   position(position), color(1, 1, 1), normal(normal)
{

}

VERTEX::~VERTEX (void)
{

}


PRIMITIVE::PRIMITIVE (void)
{
}


PRIMITIVE::PRIMITIVE (VERTEX * vert, size_t numOfVert)
{
   for (size_t i = 0; i < numOfVert; i++) {
      vertices.push_back(vert[i]);
   }
}


PRIMITIVE::PRIMITIVE (std::vector<VERTEX> & vertices) : vertices(vertices)
{

}


PRIMITIVE::~PRIMITIVE (void)
{
}


glm::vec4 VERTEX::Transform (const glm::mat4 & matr) const
{
   glm::vec4 vec(position.x, position.y, position.z, 1);
   return matr * vec;
}


void PRIMITIVE::Draw (int width, int height) const
{
   glm::mat4 matrix = projectionMatrix * viewMatrix;
   for (auto it = vertices.begin(); it != vertices.end(); ++it) {
      glm::vec4 temp_vec(it->position.x, it->position.y, it->position.z, 1);
      glm::vec4 vec = matrix * temp_vec;
      vec.x *= ((float)height) / width;
      glColor3d(it->color.x, it->color.y, it->color.z);
      glVertex2d(vec.x / vec.w, vec.y / vec.w);
   }
}


bool PRIMITIVE::Load (const std::string & fileName, double scale)
{
   std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
   std::vector<glm::vec3> temp_vertices;
   std::vector<glm::vec3> temp_uvs;
   std::vector<glm::vec3> temp_normals;
   bool loadTextureCoord = false;
   bool loadNormals = false;
   

   std::fstream ifs(fileName);

   while (ifs) {
      std::string line;

      ifs >> line;
      if (ifs.eof()) { 
         break; 
      }

      if (line == "v") {
         glm::vec3 vertex;
         ifs >> vertex.x;
         ifs >> vertex.y; 
         ifs >> vertex.z; 
         vertex *= scale;
         temp_vertices.push_back(vertex);
      } else if (line == "vn") {
         loadNormals = true;
         glm::vec3 normal;
         ifs >> normal.x;
         ifs >> normal.y;
         ifs >> normal.z;
         temp_normals.push_back(normal);
      } else if (line == "vt") {
         loadTextureCoord = true;
         glm::vec3 uv;
         ifs >> uv.x;
         ifs >> uv.y;
         ifs >> uv.z;
         temp_uvs.push_back(uv);
      } else if (line == "f") {
         size_t vertexIndex, uvIndex, normalIndex;
         char c;
         
         for (int i = 0; i < 3; i++) {
            ifs >> vertexIndex;
            vertexIndices.push_back(vertexIndex);

            if (loadTextureCoord == true) {
               // Skip slashes
               ifs.get(c);
               while (c == '/') {
                  ifs.get(c);
               }
               ifs.unget();

               ifs >> uvIndex;
               uvIndices.push_back(uvIndex);
            }
            
            if (loadNormals == true) {
               // Skip slashes
               ifs.get(c);
               while (c == '/') {
                  ifs.get(c);
               }
               ifs.unget();

               ifs >> normalIndex;
               normalIndices.push_back(normalIndex);
            }
         }
      }
   }


   for (size_t i = 0; i < vertexIndices.size(); i++) {
      vertices.push_back(temp_vertices[vertexIndices[i] - 1]); // OBJ indices start with 1
   }

   if (loadNormals == true) {
      for (size_t i = 0; i < normalIndices.size(); i++) {
         vertices[i].normal = temp_normals[normalIndices[i] - 1];
      }
   }
   
   ifs.close();

   return true;
}


void PRIMITIVE::SetView (const glm::mat4 & matr)
{
   viewMatrix = matr;
}


void PRIMITIVE::SetProjection(const glm::mat4 & matr)
{
   projectionMatrix = matr;
}


/* END OF 'primitive.cpp' FILE */
