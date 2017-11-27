/* START OF 'display.cpp' FILE */

#include <gl/glew.h>
#include <glm/gtx/transform.hpp>

#include "display.h"
#include "primitive.h"


// Variables declaration
float DISPLAY::moveSpeed = 0.07f;
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


DISPLAY::DISPLAY (void) : 
   hWnd(NULL), winWidth(0), winHeight(0)
{
   displayMode = DISPLAY_MODE::POINTS;
   backgroundColor = RGB(0, 0, 0);
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
   // glEnable(GL_LIGHTING);
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


void DISPLAY::CreateDIB (HDC hDC, HBITMAP * hBitmap, COLORREF ** bitPointer, BITMAPINFO *bitmap)
{
   (*hBitmap) = (HBITMAP)GetCurrentObject(hDC, OBJ_BITMAP);
   GetObject((*hBitmap), sizeof(BITMAP), bitmap);

   bitmap->bmiHeader.biSize = sizeof(bitmap->bmiHeader);
   bitmap->bmiHeader.biPlanes = 1;
   bitmap->bmiHeader.biBitCount = 32;
   bitmap->bmiHeader.biCompression = BI_RGB;
   bitmap->bmiHeader.biSizeImage = bitmap->bmiHeader.biWidth * 4 * bitmap->bmiHeader.biHeight;
   bitmap->bmiHeader.biClrUsed = 0;
   bitmap->bmiHeader.biClrImportant = 0;

   (*hBitmap) = CreateDIBSection(NULL, bitmap, DIB_RGB_COLORS, (void**)bitPointer, NULL, NULL);
}


void DISPLAY::PutPixel (COLORREF * bitPointer, COLORREF color, int x, int y, int winWidth, int winHeight)
{
   if ((winHeight - y) <= 0 || x >= winWidth || x <= 0 || (winHeight - y) >= winHeight) {
      return;
   }

   bitPointer[(winHeight - y) * winWidth + x] = color;
}


void DISPLAY::Line (COLORREF * bitPointer, int x0, int y0, int x1, int y1, COLORREF color, int winWidth, int winHeight)
{
   int dx = abs(x1 - x0);
   int dy = abs(y1 - y0);
   int sx = (x1 >= x0) ? 1 : -1;
   int sy = (y1 >= y0) ? 1 : -1;

   if (dy < dx) {
      int d = (dy << 1) - dx;
      int d1 = dy << 1;
      int d2 = (dy - dx) << 1;

      PutPixel(bitPointer, color, x0, y0, winWidth, winHeight);

      int x = x0 + sx;
      int y = y0;
      for (int i = 1; i <= dx; i++) {
         if (d > 0) {
            d += d2;
            y += sy;
         } else {
            d += d1;
         }

         PutPixel(bitPointer, color, x, y, winWidth, winHeight);
         x += sx;
      }
   } else {
      int d = (dx << 1) - dy;
      int d1 = dx << 1;
      int d2 = (dx - dy) << 1;

      PutPixel(bitPointer, color, x0, y0, winWidth, winHeight);

      int x = x0;
      int y = y0 + sy;
      for (int i = 1; i <= dy; i++) {
         if (d > 0) {
            d += d2;
            x += sx;
         } else {
            d += d1;
         }

         PutPixel(bitPointer, color, x, y, winWidth, winHeight);
         y += sy;
      }
   }
}


void DISPLAY::Draw (void) const
{
   BITMAPINFO bitmap;
   HBITMAP hBitmap;
   RECT rect;
   COLORREF * bitPointer;
   PAINTSTRUCT ps;

   // OpenGL alternative
   /*
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glBegin(displayMode);
   */

   BeginPaint(hWnd, &ps);

   // Get current DC
   HDC hDC = GetDC(hWnd);

   // Create memory DC
   HDC hMemDC = CreateCompatibleDC(hDC);

   // Create DIB section
   CreateDIB(hDC, &hBitmap, &bitPointer, &bitmap);

   SelectObject(hMemDC, hBitmap);

   // Copy window to memory
   BitBlt(hMemDC, 0, 0, winWidth, winHeight, hDC, 0, 0, SRCCOPY);

   // Fill the memory window with black color
   HBRUSH brush = CreateSolidBrush(backgroundColor);
   GetClientRect(hWnd, &rect);
   FillRect(hMemDC, &rect, brush);

   glm::mat4 worldMatrix = GetProjectionMatrix() * camera.GetWorldToViewMatrix();

   for (size_t i = 0; i < primitives.size(); i++) {
      std::vector<POLYGON> polygons = primitives[i].polygons;

      for (size_t j = 0; j < polygons.size(); j++) {
         VERTEX coords[3];

         for (int i = 0; i < 3; i++) {
            VERTEX vert = polygons[j].vertices[i];
            glm::vec4 temp_vec(vert.position, 1);
            glm::vec4 vec = worldMatrix * temp_vec;
            glm::vec3 & pos = coords[i].position;

            pos.x = vec.x;
            pos.y = vec.y;
            pos.x *= ((float)winHeight) / winWidth;
            pos.x /= vec.w;
            pos.y /= vec.w;
            pos.x = pos.x * (winWidth / 2) + winWidth / 2;
            pos.y = pos.y * (winHeight / 2) + winHeight / 2;
            pos.y = (float)(winHeight - (int)pos.y);
            
            coords[i].color = vert.color;
            if (displayMode == DISPLAY_MODE::POINTS) {
               COLORREF color = RGB(255 * vert.color.b, 255 *  vert.color.g, 255 *  vert.color.r);
               PutPixel(bitPointer, color, (int)pos.x, (int)pos.y, winWidth, winHeight);
            }
         }


         if (displayMode == DISPLAY_MODE::WIREFRAME) {
            COLORREF color = RGB(255 * coords[0].color.b, 255 *  coords[0].color.g, 255 *  coords[0].color.r);
            Line(bitPointer, (int)coords[0].position.x, (int)coords[0].position.y, 
               (int)coords[1].position.x, (int)coords[1].position.y, color, winWidth, winHeight);
            Line(bitPointer, (int)coords[1].position.x, (int)coords[1].position.y, 
               (int)coords[2].position.x, (int)coords[2].position.y, color, winWidth, winHeight);
            Line(bitPointer, (int)coords[2].position.x, (int)coords[2].position.y, 
               (int)coords[0].position.x, (int)coords[0].position.y, color, winWidth, winHeight);
         }
      }
   }

   // Copy memory to window
   BitBlt(hDC, 0, 0, winWidth, winHeight, hMemDC, 0, 0, SRCCOPY);

   // Clear up
   DeleteObject(hBitmap);
   DeleteObject(hMemDC);

   EndPaint(hWnd, &ps);

   // OpenGL alternative
   /*
   glEnd();
   SwapBuffers(wglGetCurrentDC());
   */
}


glm::mat4 DISPLAY::GetProjectionMatrix (void) const
{
   return glm::mat4({1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 1 }, {0, 0, 1, 0});
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
         Offset.x -= DISPLAY::moveSpeed;
         break;
      case VK_RIGHT:
         Offset.x += DISPLAY::moveSpeed;
         break;
      case VK_UP: 
         Offset.z -= DISPLAY::moveSpeed;
         break;
      case VK_DOWN:
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
      Event = EVENT::MOUSEMOVE;
      NewMousePos.x = LOWORD(lParam);
      NewMousePos.y = HIWORD(lParam);
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

   // Set mouse position
   POINT p;
   GetCursorPos(&p);
   mousePos = p;

   return true;
}


void DISPLAY::OnResizeUpdate (void)
{
   RECT rect;
   if (GetWindowRect(hWnd, &rect)) {
      winWidth = rect.right - rect.left;
      winHeight = rect.bottom - rect.top;
   }

   /*
   HDC hDC = GetDC(hWnd);
   HGLRC hGLRC = wglCreateContext(hDC);
   wglMakeCurrent(hDC, hGLRC);
   */
}


void DISPLAY::OnMouseMoveUpdate (void)
{
   mousePos.x -= NewMousePos.x;
   mousePos.y -= NewMousePos.y;
   mousePos.x *= -1;
   mousePos.y *= -1;

   camera.MouseUpdate(mousePos, rotationSpeed);
   mousePos = NewMousePos;
}


void DISPLAY::OnOffsetUpdate (void)
{
   camera.position += Offset.z * camera.viewDirection;
   camera.position += Offset.x * glm::cross(camera.viewDirection, camera.up);

   Offset = glm::vec3(0, 0, 0);
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
      displayMode = displayMode == DISPLAY_MODE::POINTS ?
         DISPLAY_MODE::WIREFRAME : DISPLAY_MODE::POINTS;
      break;
   default:
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
   primitives.push_back(prim);
   return *this;
}


void DISPLAY::SetBackgroundColor (COLORREF newColor)
{
   backgroundColor = newColor;
}


/* END OF 'display.cpp' FILE */
