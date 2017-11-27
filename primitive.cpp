/* START OF 'primitive.cpp' FILE */

#include <vector>
#include <fstream>
#include <windows.h>

#include "primitive.h"


VERTEX::VERTEX (const glm::vec3 & position) :
   position(position), color(1, 1, 1), normal(0, 0, 0)
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


POLYGON::POLYGON (VERTEX vert[3])
{
   for (int i = 0; i < 3; i++) {
      vertices[i] = vert[i];
   }
}


POLYGON::~POLYGON (void)
{

}


PRIMITIVE::PRIMITIVE (void)
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


bool PRIMITIVE::Load (const std::string & fileName, double scale)
{
   //std::vector<unsigned int> vertexIndices, normalIndices;
   std::vector<unsigned int> uvIndices;
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
         VERTEX polygon[3];
         char c;

         for (int i = 0; i < 3; i++) {
            ifs >> vertexIndex;
            //vertexIndices.push_back(vertexIndex);
            polygon[i].position = temp_vertices[vertexIndex - 1];

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

               //normalIndices.push_back(normalIndex);

               polygon[i].normal = temp_normals[normalIndex - 1];
            }
         }

         polygons.push_back(polygon);
      }
   }

   ifs.close();

   return true;
}


/* END OF 'primitive.cpp' FILE */
