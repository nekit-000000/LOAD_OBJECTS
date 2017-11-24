/* START OF 'primitive.h' FILE */

#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <glm/glm.hpp>
#include <string>


class VERTEX {
   friend class PRIMITIVE;
   friend class DISPLAY;

public:
   VERTEX(void);
   VERTEX(const glm::vec3 & position);
   VERTEX(const glm::vec3 & position, const glm::vec3 & normal);
   VERTEX(const glm::vec3 & position, const glm::vec3 & normal, const glm::vec3 & color);

   ~VERTEX(void);

   double GetX(void) const { return position.x; }
   double GetY(void) const { return position.y; }
   double GetZ(void) const { return position.z; }

   glm::vec4 Transform(const glm::mat4 & matr) const;

private:
   glm::vec3 position;
   glm::vec3 color;
   glm::vec3 normal;
};



class POLYGON {
   friend class PRIMITIVE;
   friend class DISPLAY;

public:
   POLYGON(VERTEX vert[3]);

   ~POLYGON(void);

private:
   VERTEX vertices[3];
};


class PRIMITIVE {
   friend class DISPLAY;

public:
   PRIMITIVE(void);

   ~PRIMITIVE(void);

   bool Load(const std::string & fileName, double scale);

private:
   std::vector<POLYGON> polygons;
};


#endif // PRIMITIVE_H

/* END OF 'primitive.h' FILE */
