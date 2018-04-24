/* START OF 'shader.h' FILE */


#pragma once
#ifndef SHADER_H
#define SHADER_H


class iSHADER {
public:
   virtual ~iSHADER (void) {};

   virtual glm::vec4 Vertex (int index, int vert) = 0;
   virtual bool Fragment    (glm::vec3 bar, COLORREF & color) = 0;
};


#endif // SHADER_H


/* END OF 'shader.h' FILE */
