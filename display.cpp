/* START OF 'display.cpp' FILE */

#include <gl/glew.h>
#include <glm/gtx/transform.hpp>

#include "display.h"
#include "objmodel.h"


// Variables declaration
float DISPLAY::moveSpeed = 0.4f;
float DISPLAY::rotationSpeed = 0.01f;


// Variables to interact with window
enum class EVENT {
   NONE,
   MOVE,
   RESIZE,
   MOUSEMOVE,
   SWITCHMODE
} Event;
glm::vec3 Offset;
POINT NewMousePos;
POINT OldMousePos;


DISPLAY::DISPLAY (void) : 
   hWnd(NULL), winWidth(0), winHeight(0)
{
}


DISPLAY::~DISPLAY (void)
{
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


LONG WINAPI DISPLAY::DisplayProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message) {
   case WM_CREATE:
      break;
   case WM_KEYDOWN:
      Event = EVENT::MOVE;
      Offset = glm::vec3(0, 0, 0);
      switch (wParam) {
      case 65: // A
         Offset.x -= DISPLAY::moveSpeed;
         break;
      case 68: // D
         Offset.x += DISPLAY::moveSpeed;
         break;
      case 87: // W
         Offset.z -= DISPLAY::moveSpeed;
         break;
      case 83: // S
         Offset.z += DISPLAY::moveSpeed;
         break;
      case VK_SPACE:
         Event = EVENT::SWITCHMODE;
         break;
      case VK_ESCAPE:
         PostQuitMessage(0);
         break;
      }
      break;
   case WM_MOUSEMOVE:
      NewMousePos.x = LOWORD(lParam);
      NewMousePos.y = HIWORD(lParam);
      break;
   case WM_LBUTTONDOWN:
      Event = EVENT::MOUSEMOVE;
      OldMousePos.x = LOWORD(lParam);
      OldMousePos.y = HIWORD(lParam);
      break;
   case WM_LBUTTONUP:
      Event = EVENT::NONE;
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

   // Initialize OpenGL
   /*
   HDC hDC = GetDC(hWnd);
   if (!InitGL(hDC)) {
      return false;
   }
   */

   // Set window width and height
   winWidth = winW;
   winHeight = winH;

   return true;
}


void DISPLAY::OnResizeUpdate (void)
{
   RECT rect;
   if (GetWindowRect(hWnd, &rect)) {
      winWidth = rect.right - rect.left;
      winHeight = rect.bottom - rect.top;
   }
   Event = EVENT::NONE;

   // OpenGL alternative
   /*
   HDC hDC = GetDC(hWnd);
   HGLRC hGLRC = wglCreateContext(hDC);
   wglMakeCurrent(hDC, hGLRC);
   */
}


void DISPLAY::OnMouseMoveUpdate (void)
{
   mousePos = OldMousePos;

   mousePos.x -= NewMousePos.x;
   mousePos.y -= NewMousePos.y;
   mousePos.x *= -1;
   mousePos.y *= -1;

   render.camera.MouseUpdate(mousePos, rotationSpeed);
   mousePos = NewMousePos;
   OldMousePos = NewMousePos;
}


void DISPLAY::OnOffsetUpdate (void)
{
   render.camera.position += Offset.z * render.camera.viewDirection;
   render.camera.position += Offset.x * glm::cross(render.camera.viewDirection, render.camera.up);

   Offset = glm::vec3(0, 0, 0);
   Event = EVENT::NONE;
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

   Event = EVENT::NONE;
}


void DISPLAY::Update (void)
{
   switch (Event) {
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
   render.root = root;
}


void DISPLAY::SetCamera (const CAMERA & newCam)
{
   render.camera = newCam;
}


/* END OF 'display.cpp' FILE */
