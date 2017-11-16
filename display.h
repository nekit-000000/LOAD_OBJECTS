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
   DISPLAY (void);

   ~DISPLAY (void);

   DISPLAY & operator<< (PRIMITIVE & prim);

   bool WinCreate (HINSTANCE hInstance, int winW, int winH);
   int Run        (void);

private:
   static LONG WINAPI DisplayProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

   bool SetWindowPixelFormat      (HDC hDC) const;
   void Draw                      (void)    const;
   bool InitGL                    (HDC hDC) const;
   void Close                     (void)    const;
   void Resize                    (void);
   void Update                    (void);
   glm::mat4 GetProjectionMatrix  (void)    const;

private:
   CAMERA camera;
   HWND hWnd;
   int width;
   int height;
   GLenum displayMode;
   std::vector<PRIMITIVE> primitives;
};


#endif // DISPLAY_H

/* END OF 'display.h' FILE */
