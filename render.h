/* START OF 'render.h' FILE */

#pragma once
#ifndef RENDER_H
#define RENDER_H

#include <windows.h>

#include "camera.h"
#include "scene.h"


class RENDER {
   friend class DISPLAY;

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

   public:
      COLORREF color;
      double depth;
      glm::ivec2 pos;
   };

public:
   typedef typename SCENE_NODE::NODE_TYPE NODE_TYPE;

   RENDER (void);
   RENDER (SCENE_NODE * newRoot);
   RENDER (SCENE_NODE * newRoot, const CAMERA & newCamera);

   ~RENDER (void);

private:
   // Auxiliary functions to draw without OpenGL
   static void CreateDIB             (HDC hDC, HBITMAP * hBitmap, COLORREF ** bitPointer, BITMAPINFO * bitmap);
   static void PutPixel              (COLORREF * bitPointer, COLORREF color, int x, int y, int winWidth, int winHeight);
   static void DrawTriangle          (COLORREF * bitPointer, COLOURED_POINT point1, COLOURED_POINT point2,
                                      COLOURED_POINT point3, int winWidth, int winHeight, double * zBuffer);
   static void DrawLine              (COLORREF * bitPointer, const COLOURED_POINT & point1, 
                                      const COLOURED_POINT & point2, int width, int height, double * zBuffer);
   static COLORREF InterpolateColors (COLORREF color0, COLORREF color1, double coeff);

   // Node drawing function
   void DrawNode                     (const OBJECT_NODE * node, const glm::mat4 & matrixTransform, 
                                      COLORREF * bitPointer, int winWidth, int winHeight, double * zBuffer) const;

   void DrawScene                    (HWND hWnd, int winWidth, int winHeight) const;
   glm::mat4 GetProjectionMatrix     (void)    const;

private:
   SCENE_NODE * root;
   CAMERA camera;
   DISPLAY_MODE displayMode;
   COLORREF backgroundColor;
};


#endif // RENDER_H

/* END OF 'render.h' FILE */
