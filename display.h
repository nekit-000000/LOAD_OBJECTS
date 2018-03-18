/* START OF 'display.h' FILE */

#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include "render.h"
#include "objmodel.h"
#include "camera.h"
#include "windowhandler.h"


class DISPLAY : public WINDOW_HANDLER {
public:
   typedef typename RENDER::DISPLAY_MODE DISPLAY_MODE;

   DISPLAY (void);

   ~DISPLAY (void);

   WPARAM Run                        (void);
   void SetSceneData                 (SCENE_NODE * root);
   void SetDisplayMode               (const DISPLAY_MODE & mode);
   void SetCamera                    (const CAMERA & newCam);
   static void SetMoveSpeed          (const float newSpeed);
   static void SetRotationSpeed      (const float newSpeed);
   void SetBackgroundColor           (COLORREF newColor);

private:
   // Functions to initialize OpenGL
   static bool SetWindowPixelFormat  (HDC hDC);
   static bool InitGL                (HDC hDC);
   static void Close                 (void);

   // Window events functions
   LRESULT OnKeydownEvent            (LPARAM lParam, WPARAM wParam);
   LRESULT OnMouseMoveEvent          (LPARAM lParam, WPARAM wParam);
   LRESULT OnLButtonDownEvent        (LPARAM lParam, WPARAM wParam);
   LRESULT OnLButtonUpEvent          (LPARAM lParam, WPARAM wParam);
   LRESULT OnResizeEvent             (LPARAM lParam, WPARAM wParam);

   // Update functions
   void OnMouseMoveUpdate            (void);
   void OnOffsetUpdate               (void);
   void OnResizeUpdate               (void);
   void OnSwitchModeUpdate           (void);
   void Update                       (void);

private:
   enum class EVENT {
      NONE,
      MOVE,
      RESIZE,
      MOUSEMOVE,
      SWITCHMODE
   } event;

   static float moveSpeed;
   static float rotationSpeed;
   RENDER render;
   glm::vec3 offset;
   POINT newMousePos;
   POINT oldMousePos;
   POINT mousePos;
};


#endif // DISPLAY_H


/* END OF 'display.h' FILE */
