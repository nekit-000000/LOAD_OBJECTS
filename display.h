/* START OF 'display.h' FILE */

#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include <windows.h>
#include <string>
#include <vector>

#include "primitive.h"
#include "camera.h"


class DISPLAY {
public:
   enum class DISPLAY_MODE {
      POINTS,
      WIREFRAME
   };

public:
   DISPLAY (void);

   ~DISPLAY (void);

   DISPLAY & operator<< (PRIMITIVE & prim);

   bool WinCreate               (HINSTANCE hInstance, int winW, int winH);
   int Run                      (void);
   static void SetMoveSpeed     (const float newSpeed);
   static void SetRotationSpeed (const float newSpeed);

private:
   static LONG WINAPI DisplayProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

   // Functions to initialize OpenGL
   static bool SetWindowPixelFormat (HDC hDC);
   static bool InitGL               (HDC hDC);
   static void Close                (void);

   // Auxiliary functions to draw without OpenGL
   static void CreateDIB (HDC hDC, HBITMAP * hBitmap, COLORREF ** bitPointer, BITMAPINFO * bitmap);
   static void PutPixel  (COLORREF * bitPointer, COLORREF color, int x, int y, int winWidth, int winHeight);
   static void Line      (COLORREF * bitPointer, int x0, int y0, int x1, int y1, COLORREF color0, int width, int height);

   // Update functions            
   void OnMouseMoveUpdate         (void);
   void OnOffsetUpdate            (void);
   void OnResizeUpdate            (void);
   void Update                    (void);

   void Draw                      (void)    const;
   glm::mat4 GetProjectionMatrix  (void)    const;
   void SetBackgroundColor        (COLORREF newColor);

private:
   static float moveSpeed;
   static float rotationSpeed;

   CAMERA camera;
   HWND hWnd;
   int winWidth;
   int winHeight;
   DISPLAY_MODE displayMode;
   COLORREF backgroundColor;
   POINT mousePos;
   std::vector<PRIMITIVE> primitives;
};


#endif // DISPLAY_H


/* END OF 'display.h' FILE */
