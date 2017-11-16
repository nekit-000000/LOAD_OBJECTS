/* START OF 'primitive.h' FILE */

#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>


class VERTEX {
   friend class PRIMITIVE;
public:
   VERTEX (void);
   VERTEX (const glm::vec3 & position);
   VERTEX (const glm::vec3 & position, const glm::vec3 & normal);
   VERTEX (const glm::vec3 & position, const glm::vec3 & normal, const glm::vec3 & color);

   ~VERTEX (void);

   double GetX (void) const { return position.x; }
   double GetY (void) const { return position.y; }
   double GetZ (void) const { return position.z; }

   glm::vec4 Transform (const glm::mat4 & matr) const;

private:
   glm::vec3 position;
   glm::vec3 color;
   glm::vec3 normal;
};


class PRIMITIVE {
public:
   PRIMITIVE (void);
   PRIMITIVE (VERTEX * vert, size_t numOfVert);
   PRIMITIVE (std::vector<VERTEX> & vertices);

   ~PRIMITIVE (void);

   void Draw          (int width, int height) const;
   bool Load          (const std::string & fileName, double scale);
   void SetView       (const glm::mat4 & matr);
   void SetProjection (const glm::mat4 & matr);

private:
   std::vector<VERTEX> vertices;
   glm::mat4 viewMatrix;
   glm::mat4 projectionMatrix;
};


#endif // PRIMITIVE_H

/* END OF 'primitive.h' FILE */
