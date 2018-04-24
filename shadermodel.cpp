/* START OF 'shadermodel.cpp' FILE */


#include "shadermodel.h"


SHADER_MODEL::SHADER_MODEL (void) :
   model(NULL)
{

}


glm::vec4 SHADER_MODEL::Vertex (int index, int vert)
{
   uvs[vert] = model->uvCoords[model->uvIndices[index]];

   glm::vec4 vertex = glm::vec4(model->vertices[model->vertexIndices[index]], 1);

   return transformMatrix * vertex;
}


bool SHADER_MODEL::Fragment (glm::vec3 bar, COLORREF & color)
{
   float intensityModified = glm::dot(bar, intensity);

   if (model->mat.map.GetWidth() > 0) {
      glm::vec2 uv = uvs * bar;
      int y = (int)(uv.y * (model->mat.map.GetHeight() - 1));
      int x = (int)(uv.x * (model->mat.map.GetWidth() - 1));
      color = model->mat.map.GetColor(y * model->mat.map.GetWidth() + x);
   } else {
      color = WireFrameColor;
   }

   return false;
}


void SHADER_MODEL::SetModel (const objMODEL * model)
{
   this->model = const_cast<objMODEL *>(model);
}


void SHADER_MODEL::SetTransformMatrix(const glm::mat4x4 & matr)
{
   transformMatrix = matr;
}


/* END OF 'shadermodel.cpp' FILE */
