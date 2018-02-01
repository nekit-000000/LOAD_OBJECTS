/* START OF 'display.h' FILE */

#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include <windows.h>
#include <string>
#include <vector>

#include "render.h"
#include "objmodel.h"
#include "camera.h"


class DISPLAY {
public:
   typedef typename RENDER::DISPLAY_MODE DISPLAY_MODE;

   DISPLAY (void);

   ~DISPLAY (void);

   bool WinCreate                    (HINSTANCE hInstance, int winW, int winH);
   int Run                           (void);
   void SetSceneData                 (SCENE_NODE * root);
   void SetDisplayMode               (const DISPLAY_MODE & mode);
   void SetCamera                    (const CAMERA & newCam);
   static void SetMoveSpeed          (const float newSpeed);
   static void SetRotationSpeed      (const float newSpeed);

private:
   static LONG WINAPI DisplayProc    (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

   // Functions to initialize OpenGL
   static bool SetWindowPixelFormat  (HDC hDC);
   static bool InitGL                (HDC hDC);
   static void Close                 (void);

   // Update functions
   void OnMouseMoveUpdate            (void);
   void OnOffsetUpdate               (void);
   void OnResizeUpdate               (void);
   void OnSwitchModeUpdate           (void);
   void Update                       (void);

   void SetBackgroundColor           (COLORREF newColor);

private:
   static float moveSpeed;
   static float rotationSpeed;

   RENDER render;
   HWND hWnd;
   int winWidth;
   int winHeight;
   POINT mousePos;
};


#endif // DISPLAY_H


/* END OF 'display.h' FILE */
