/* START OF 'display.cpp' FILE */

#include <gl/glew.h>
#include <glm/gtx/transform.hpp>

#include "display.h"
#include "objmodel.h"


// Variables declaration
float DISPLAY::moveSpeed = 0.1f;
float DISPLAY::rotationSpeed = 0.01f;


DISPLAY::DISPLAY (void) 
{
   AddMessage(WM_KEYDOWN, this, &DISPLAY::OnKeydownEvent);
   AddMessage(WM_MOUSEMOVE, this, &DISPLAY::OnMouseMoveEvent);
   AddMessage(WM_LBUTTONDOWN, this, &DISPLAY::OnLButtonDownEvent);
   AddMessage(WM_LBUTTONUP, this, &DISPLAY::OnLButtonUpEvent);
   AddMessage(WM_SIZE, this, &DISPLAY::OnResizeEvent);
}


DISPLAY::~DISPLAY (void)
{
}


LRESULT DISPLAY::OnKeydownEvent (LPARAM lParam, WPARAM wParam)
{
   event = EVENT::MOVE;
   offset = glm::vec3(0, 0, 0);
   switch (wParam) {
   case 65: // A
      offset.x -= DISPLAY::moveSpeed;
      break;
   case 68: // D
      offset.x += DISPLAY::moveSpeed;
      break;
   case 87: // W
      offset.z -= DISPLAY::moveSpeed;
      break;
   case 83: // S
      offset.z += DISPLAY::moveSpeed;
      break;
   case VK_SPACE:
      event = EVENT::SWITCHMODE;
      break;
   case VK_ESCAPE:
      PostQuitMessage(0);
      break;
   }

   return 0;
}


LRESULT DISPLAY::OnMouseMoveEvent (LPARAM lParam, WPARAM wParam)
{
   newMousePos.x = LOWORD(lParam);
   newMousePos.y = HIWORD(lParam);

   return 0;
}



LRESULT DISPLAY::OnLButtonDownEvent (LPARAM lParam, WPARAM wParam)
{
   event = EVENT::MOUSEMOVE;
   oldMousePos.x = LOWORD(lParam);
   oldMousePos.y = HIWORD(lParam);

   return 0;
}


LRESULT DISPLAY::OnLButtonUpEvent (LPARAM lParam, WPARAM wParam)
{
   event = EVENT::NONE;

   return 0;
}


LRESULT DISPLAY::OnResizeEvent (LPARAM lParam, WPARAM wParam)
{
   event = EVENT::RESIZE;

   return 0;
}


void DISPLAY::SetMoveSpeed (const float newSpeed)
{
   moveSpeed = newSpeed;
}


void DISPLAY::SetRotationSpeed (const float newSpeed)
{
   rotationSpeed = newSpeed;
}


bool DISPLAY::SetWindowPixelFormat (HDC hDC)
{
   int m_GLPixelIndex;
   PIXELFORMATDESCRIPTOR pfd;
   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   pfd.nVersion = 1;
   pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
      PFD_DOUBLEBUFFER;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 32;
   pfd.cRedBits = 8;
   pfd.cRedShift = 16;
   pfd.cGreenBits = 8;
   pfd.cGreenShift = 8;
   pfd.cBlueBits = 8;
   pfd.cBlueShift = 0;
   pfd.cAlphaBits = 0;
   pfd.cAlphaShift = 0;
   pfd.cAccumBits = 64;
   pfd.cAccumRedBits = 16;
   pfd.cAccumGreenBits = 16;
   pfd.cAccumBlueBits = 16;
   pfd.cAccumAlphaBits = 0;
   pfd.cDepthBits = 32;
   pfd.cStencilBits = 8;
   pfd.cAuxBuffers = 0;
   pfd.iLayerType = PFD_MAIN_PLANE;
   pfd.bReserved = 0;
   pfd.dwLayerMask = 0;
   pfd.dwVisibleMask = 0;
   pfd.dwDamageMask = 0;
   m_GLPixelIndex = ChoosePixelFormat(hDC, &pfd);

   if (m_GLPixelIndex == 0) {
      m_GLPixelIndex = 1;
      if (DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0) {
         return false;
      }
   }

   if (SetPixelFormat(hDC, m_GLPixelIndex, &pfd) == false) {
      return false;
   }

   return true;
}


bool DISPLAY::InitGL (HDC hDC)
{
   if (!SetWindowPixelFormat(hDC)) {
      return false;
   }

   HGLRC hGLRC = wglCreateContext(hDC);
   wglMakeCurrent(hDC, hGLRC);

   glEnable(GL_ALPHA_TEST);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_COLOR_MATERIAL);
   glEnable(GL_LIGHTING);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glewInit();

   return true;
}


void DISPLAY::Close (void)
{
   HGLRC hGLRC = wglGetCurrentContext();
   if (hGLRC) {
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(hGLRC);
   }
}


void DISPLAY::OnResizeUpdate (void)
{
   RECT rect;
   if (GetWindowRect(hWnd, &rect)) {
      winWidth = rect.right - rect.left;
      winHeight = rect.bottom - rect.top;
   }
   event = EVENT::NONE;

   // OpenGL alternative
   /*
   HDC hDC = GetDC(hWnd);
   HGLRC hGLRC = wglCreateContext(hDC);
   wglMakeCurrent(hDC, hGLRC);
   */
}


void DISPLAY::OnMouseMoveUpdate (void)
{
   mousePos = oldMousePos;

   mousePos.x -= newMousePos.x;
   mousePos.y -= newMousePos.y;
   mousePos.x *= -1;
   mousePos.y *= -1;

   render.camera.MouseUpdate(mousePos, rotationSpeed);
   mousePos = newMousePos;
   oldMousePos = newMousePos;
}


void DISPLAY::OnOffsetUpdate (void)
{
   render.camera.position += offset.z * render.camera.viewDirection;
   render.camera.position += offset.x * glm::cross(render.camera.viewDirection, render.camera.up);

   offset = glm::vec3(0, 0, 0);
   event = EVENT::NONE;
}


void DISPLAY::OnSwitchModeUpdate (void)
{
   switch (render.displayMode) {
   case DISPLAY_MODE::POINTS:
      render.displayMode = DISPLAY_MODE::WIREFRAME;
      break;
   case DISPLAY_MODE::WIREFRAME:
      render.displayMode = DISPLAY_MODE::TRIANGLES;
      break;
   case DISPLAY_MODE::TRIANGLES:
      render.displayMode = DISPLAY_MODE::POINTS;
      break;
   default:
      break;
   }

   event = EVENT::NONE;
}


void DISPLAY::Update (void)
{
   switch (event) {
   case EVENT::MOVE:
      OnOffsetUpdate();
      break;
   case EVENT::RESIZE:
      OnResizeUpdate();
      break;
   case EVENT::MOUSEMOVE:
      OnMouseMoveUpdate();
      break;
   case EVENT::SWITCHMODE:
      OnSwitchModeUpdate();
      break;
   default:
      break;
   }
}


int DISPLAY::Run (void)
{
   if (hWnd == NULL) {
      return 0;
   }

   ShowWindow(hWnd, true);
   UpdateWindow(hWnd);
   
   MSG msg;
   while (GetMessage(&msg, NULL, 0, 0)) {
      Update();
      render.DrawScene(hWnd, winWidth, winHeight);
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return msg.wParam;
}


void DISPLAY::SetBackgroundColor (COLORREF newColor)
{
   render.backgroundColor = newColor;
}


void DISPLAY::SetDisplayMode (const DISPLAY_MODE & mode)
{
   render.displayMode = mode;
}


void DISPLAY::SetSceneData (SCENE_NODE * root)
{
   render.root->link = root;
}


void DISPLAY::SetCamera (const CAMERA & newCam)
{
   render.camera = newCam;
}


/* END OF 'display.cpp' FILE */
