/* START OF 'render.h' FILE */

#pragma once
#ifndef RENDER_H
#define RENDER_H


#include "camera.h"
#include "intrusiveptr.h"
#include "transformnode.h"
#include "objectnode.h"
#include "shader.h"

#define WireFrameColor RGB(0, 0, 255)

#define FlipColor(a) ((a) & 0x00FF00) | ((a) << 16) | ((a) >> 16)


class RENDER {
   friend class DISPLAY;
   friend class RENDER_VISITOR;

public:
   enum class DISPLAY_MODE {
      POINTS,
      WIREFRAME,
      TRIANGLES
   };

   class COLOURED_POINT {
   public:
      COLOURED_POINT (void);
      COLOURED_POINT (int x, int y, COLORREF newColor);
      COLOURED_POINT (int x, int y, double depth, COLORREF newColor);
      COLOURED_POINT (const glm::ivec2 & newPos, COLORREF newColor);
      COLOURED_POINT (const glm::ivec2 & newPos, double depth, COLORREF newColor);
      COLOURED_POINT (const COLOURED_POINT & copyPoint);

      ~COLOURED_POINT (void);

      COLOURED_POINT & operator= (const COLOURED_POINT & copyPoint);
      int operator[]             (const int i) { return pos[i]; };

   public:
      COLORREF color;
      double depth;
      glm::ivec2 pos;
   };

public:
   RENDER (void);
   RENDER (SCENE_NODE * newRoot);
   RENDER (SCENE_NODE * newRoot, const CAMERA & newCamera);

   ~RENDER (void);

private:
   // Auxiliary functions to draw without OpenGL
   static void CreateDIB               (HDC hDC, HBITMAP * hBitmap, COLORREF ** bitPointer, BITMAPINFO * bitmap);
   static void PutPixel                (COLORREF * bitPointer, COLORREF color, int x, int y, int winWidth, int winHeight);
   static void PutPixel                (COLORREF * bitPointer, COLOURED_POINT point, int winWidth, int winHeight);
   static void PutPixelNoCheck         (COLORREF * bitPointer, COLORREF color, int x, int y, int winWidth, int winHeight);
   static void DrawPoint               (COLORREF * bitPointer, const glm::ivec3 & point,
                                        int winWidth, int winHeight, int * zBuffer);
   static void DrawTriangle            (COLORREF * bitPointer, COLOURED_POINT point1, COLOURED_POINT point2,
                                        COLOURED_POINT point3, int winWidth, int winHeight, double * zBuffer);
   static void DrawLine                (COLORREF * bitPointer, const glm::ivec3 * pts, int width, int height, 
                                        int * zBuffer);
   static void DrawLine                (COLORREF * bitPointer, const glm::ivec3 & pt1, const glm::vec3 & pt2, int width, int height,
                                        int * zBuffer);
   static COLORREF InterpolateColors   (COLORREF color0, COLORREF color1, double coeff);
   static void DrawTriangleBarycentric (COLORREF * bitPointer, glm::vec4 * pts,
                                        glm::vec2 * uvs, const TEXTURE & tex, int winWidth, int winHeight, int * zBuffer);
   static void DrawTriangleBarycentric (COLORREF * bitPointer, glm::vec4 * pts,
                                        iSHADER & shader, int winWidth, int winHeight, int * zBuffer);

   // Auxiliary functions for drawing
   static bool CheckIfPointInFrame     (int width, int height, int px, int py);
   static glm::vec3 Barycentric        (const glm::vec2 & a, const glm::vec2 & b, const glm::vec2 & c, const glm::vec2 & p);

   // Node drawing function
   static void DrawNode              (const OBJECT_NODE * node, const glm::mat4 & matrixTransform, 
                                      COLORREF * bitPointer, int winWidth, int winHeight, int * zBuffer, 
                                      DISPLAY_MODE displayMode);

   // Scene drawing function
   void DrawScene                    (HWND hWnd, int winWidth, int winHeight) const;

   // Matrices
   glm::mat4 GetProjectionMatrix      (void)    const;
   static glm::mat4 GetViewportMatrix (int x0, int y0, int width, int height);

private:
   TRANSFORM_NODE * root;
   CAMERA camera;
   DISPLAY_MODE displayMode;
   COLORREF backgroundColor;
};


#endif // RENDER_H

/* END OF 'render.h' FILE */
