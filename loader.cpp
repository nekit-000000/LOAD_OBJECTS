/* START OF "loader.cpp" FILE */


#include "loader.h"


INTRUSIVE_PTR<OBJECT_NODE> LoadObj (const std::string & fileName)
{
   objMODEL prim;
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
         prim.vertices.push_back(vertex);

         //
         if ((int)rand() % 2 == 0) {
            prim.vertexColors.push_back(RGB(0, 0, 255));
         }
         else {
            prim.vertexColors.push_back(RGB(255, 0, 0));
         }
         //
      } else if (line == "vn") {
         loadNormals = true;
         glm::vec3 normal;
         ifs >> normal.x;
         ifs >> normal.y;
         ifs >> normal.z;
         prim.normals.push_back(normal);
      } else if (line == "vt") {
         loadTextureCoord = true;
         glm::vec3 uv;
         ifs >> uv.x;
         ifs >> uv.y;
         ifs >> uv.z;
      } else if (line == "f") {
         size_t vertexIndex, uvIndex, normalIndex;
         char c;

         for (int i = 0; i < 3; i++) {
            ifs >> vertexIndex;
            prim.vertexIndices.push_back(vertexIndex - 1);

            if (loadTextureCoord == true) {
               // Skip slashes
               ifs.get(c);
               while (c == '/') {
                  ifs.get(c);
               }
               ifs.unget();

               ifs >> uvIndex;
            }

            if (loadNormals == true) {
               // Skip slashes
               ifs.get(c);
               while (c == '/') {
                  ifs.get(c);
               }
               ifs.unget();

               ifs >> normalIndex;
               prim.normalIndices.push_back(normalIndex - 1);
            }
         }
      }
   }

   ifs.close();

   return INTRUSIVE_PTR<OBJECT_NODE>(new OBJECT_NODE(prim));
}


/* END OF "loader.cpp" FILE */
