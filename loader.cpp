/* START OF "loader.cpp" FILE */


#include "loader.h"


static std::wstring SToWS (const std::string & s)
{
   int len;
   int slength = (int)s.length() + 1;

   len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);

   wchar_t* buf = new wchar_t[len];
   MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
   std::wstring r(buf);

   delete[] buf;

   return r;
}


static MATERIAL ParseMtlLib (const std::string & fileName)
{
   std::fstream ifs(fileName);
   MATERIAL mat;
   float x;
   float y;
   float z;

   while (ifs) {
      std::string line;
      ifs >> line;

      if (line == "Ka" || line == "Kd" || line == "Ks") {
         ifs >> x;
         ifs >> y;
         ifs >> z;
         
         glm::vec3 vec(x, y, z);
         if (line == "Ka") {
            mat.ka = vec;
         } else if (line == "Kd") {
            mat.kd = vec;
         } else {
            mat.ks = vec;
         }
      } else if (line == "map") {
         ifs >> line;
         std::wstring stemp = SToWS(line);
         HBITMAP hBmp = (HBITMAP)LoadImage(NULL, stemp.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
         mat.SetMap(hBmp);
      }
   }

   return mat;
}


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

      if (line == "usemtl")
         int i = 0;
      if (line == "v") {
         glm::vec3 vertex;
         ifs >> vertex.x;
         ifs >> vertex.y;
         ifs >> vertex.z;
         prim.vertices.push_back(vertex);

         if ((int)rand() % 2 == 0) {
            prim.vertexColors.push_back(RGB(0, 0, 255));
         }
         else {
            prim.vertexColors.push_back(RGB(255, 0, 0));
         }
         
      } else if (line == "vn") {
         loadNormals = true;
         glm::vec3 normal;
         ifs >> normal.x;
         ifs >> normal.y;
         ifs >> normal.z;
         prim.normals.push_back(normal);
      } else if (line == "vt") {
         loadTextureCoord = true;
         glm::vec2 uv;
         ifs >> uv.x;
         ifs >> uv.y;

         prim.uvCoords.push_back(uv);
      } else if (line == "mtllib") {
         ifs >> line;
         MATERIAL mat = ParseMtlLib(line);
         prim.mat = mat;
      } else if (line == "f") {
         size_t vertexIndex, uvIndex, normalIndex;
         char c;

         for (int i = 0; i < 3; i++) {
            ifs >> vertexIndex;
            prim.vertexIndices.push_back(vertexIndex - 1);

            if (loadTextureCoord) {
               // Skip slashes
               ifs.get(c);
               while (c == '/') {
                  ifs.get(c);
               }
               ifs.unget();

               ifs >> uvIndex;
               prim.uvIndices.push_back(uvIndex - 1);
            }

            if (loadNormals) {
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

         ifs.get(c);
         while (c == ' ') {
            ifs.get(c);
         }

         ifs.unget();
         if (isdigit(c)) {
            prim.vertexIndices.push_back(prim.vertexIndices[prim.vertexIndices.size() - 3]);
            prim.vertexIndices.push_back(prim.vertexIndices[prim.vertexIndices.size() - 2]);
            prim.uvIndices.push_back(prim.uvIndices[prim.uvIndices.size() - 3]);
            prim.uvIndices.push_back(prim.uvIndices[prim.uvIndices.size() - 2]);
            ifs >> vertexIndex;
            prim.vertexIndices.push_back(vertexIndex - 1);

            if (loadTextureCoord) {
               // Skip slashes
               ifs.get(c);
               while (c == '/') {
                  ifs.get(c);
               }
               ifs.unget();

               ifs >> uvIndex;
               prim.uvIndices.push_back(uvIndex - 1);
            }

            if (loadNormals) {
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
