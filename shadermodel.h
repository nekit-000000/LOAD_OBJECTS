/* START OF 'shadermodel.h' FILE */


#pragma once
#ifndef SHADER_MODEL_H
#define SHADER_MODEL_H


#include "objmodel.h"
#include "render.h"
#include "shader.h"


class SHADER_MODEL : public iSHADER {
public:
   SHADER_MODEL (void);
   ~SHADER_MODEL (void) {};

   glm::vec4 Vertex (int index, int vert);
   bool Fragment    (glm::vec3 bar, COLORREF & color);

   void SetModel           (const objMODEL * model);
   void SetTransformMatrix (const glm::mat4x4 & matr);

private:
   glm::vec3 intensity;
   glm::mat3x2 uvs;
   glm::mat4x4 transformMatrix;
   objMODEL * model;
};


#endif // SHADER_MODEL_H


/* END OF 'shadermodel.h' FILE */
