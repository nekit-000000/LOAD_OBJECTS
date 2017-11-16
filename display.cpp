/* START OF 'display.cpp' FILE */

#include <gl/glew.h>
#include <glm/gtx/transform.hpp>

#include "display.h"
#include "primitive.h"

#define MOVE_STEP 0.07f


// Variables to interact with window
enum class EVENT {
   NONE,
   MOVE,
   RESIZE
} Event;
glm::vec3 Offset;


DISPLAY::DISPLAY (void) : 
   hWnd(NULL), displayMode(GL_TRIANGLES), width(0), height(0)
{
}


DISPLAY::~DISPLAY (void)
{
}


bool DISPLAY::SetWindowPixelFormat (HDC hDC) const
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


bool DISPLAY::InitGL (HDC hDC) const
{
   if (!SetWindowPixelFormat(hDC)) {
      return false;
   }

   HGLRC hGLRC = wglCreateContext(hDC);
   wglMakeCurrent(hDC, hGLRC);

   glEnable(GL_ALPHA_TEST);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_COLOR_MATERIAL);
   // glEnable(GL_LIGHTING);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glewInit();

   return true;
}


void DISPLAY::Close (void) const
{
   HGLRC hGLRC = wglGetCurrentContext();
   if (hGLRC) {
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(hGLRC);
   }
}


void DISPLAY::Resize (void)
{
   RECT rect;
   if (GetWindowRect(hWnd, &rect)) {
      width = rect.right - rect.left;
      height = rect.bottom - rect.top;
   }

   HDC hDC = GetDC(hWnd);
   HGLRC hGLRC = wglCreateContext(hDC);
   wglMakeCurrent(hDC, hGLRC);
}


void DISPLAY::Draw (void) const
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glBegin(displayMode);
   for (auto it = primitives.begin(); it != primitives.end(); ++it) {
      it->Draw(width, height);
   }
   glEnd();

   SwapBuffers(wglGetCurrentDC());
}


glm::mat4 DISPLAY::GetProjectionMatrix (void) const
{
   glm::mat4 projMatrix({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 1 }, {0, 0, 1, 0});
   return projMatrix;
}


LONG WINAPI DISPLAY::DisplayProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message) {
   case WM_CREATE:
      break;
   case WM_KEYDOWN:
      Event = EVENT::MOVE;
      Offset = glm::vec3(0, 0, 0);
      switch (wParam) {
      case VK_LEFT:
            Offset.x -= MOVE_STEP;
            break;
      case VK_RIGHT:
            Offset.x += MOVE_STEP;
            break;
      case VK_UP:
            Offset.z += MOVE_STEP;
            break;
      case VK_DOWN:
            Offset.z -= MOVE_STEP;
            break;
      }
      break;
   case WM_COMMAND:
      break;
   case WM_ERASEBKGND:
      break;
   case WM_PAINT:
      break;
   case WM_SIZE:
      Event = EVENT::RESIZE;
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
      break;
   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }

   return 0;
}


bool DISPLAY::WinCreate (HINSTANCE hInstance, int winW, int winH)
{
   if (hWnd) {
      return false;
   }

   WNDCLASS wClass;

   wClass.style = 0;
   wClass.cbClsExtra = 0;
   wClass.cbWndExtra = 0;
   wClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wClass.lpfnWndProc = DisplayProc;
   wClass.hInstance = hInstance;
   wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
   wClass.lpszMenuName = NULL;
   wClass.lpszClassName = L"Display";

   if (!RegisterClass(&wClass)) {
      MessageBox(NULL, L"Error register window class", L"ERROR", MB_OK);
      hWnd = NULL;
      return false;
   }

   hWnd = CreateWindow(L"Display", L"Model", WS_OVERLAPPEDWINDOW,
      300, 200, winW, winH, NULL, NULL, hInstance, NULL);

   HDC hDC = GetDC(hWnd);

   if (!InitGL(hDC)) {
      return false;
   }

   width = winW;
   height = winH;

   return true;
}


void DISPLAY::Update (void)
{
   switch (Event) {
   case EVENT::MOVE:
      camera.position += Offset;

      for (auto it = primitives.begin(); it != primitives.end(); ++it) {
         it->SetView(camera.GetWorldToViewMatrix());
      }
      break;
   case EVENT::RESIZE:
      Resize();
      break;
   }

   Event = EVENT::NONE;
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
      Draw();
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return msg.wParam;
}


DISPLAY & DISPLAY::operator<< (PRIMITIVE & prim)
{
   prim.SetProjection(GetProjectionMatrix());
   prim.SetView(camera.GetWorldToViewMatrix());
   primitives.push_back(prim);
   return *this;
}


/* END OF 'display.cpp' FILE */
