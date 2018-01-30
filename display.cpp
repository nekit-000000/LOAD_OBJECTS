/* START OF 'display.cpp' FILE */

#include <gl/glew.h>
#include <glm/gtx/transform.hpp>

#include "display.h"
#include "primitive.h"


// Variables declaration
float DISPLAY::moveSpeed = 0.1f;
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
   displayMode = DISPLAY_MODE::POINTS;
   backgroundColor = RGB(0, 0, 0);
}


DISPLAY::~DISPLAY (void)
{
}


DISPLAY::COLOURED_POINT::COLOURED_POINT (void) :
   pos(0, 0), color(RGB(0, 0, 0)), depth(0)
{
}


DISPLAY::COLOURED_POINT::COLOURED_POINT (int x, int y, COLORREF newColor) :
   pos(x, y), color(newColor), depth(0)
{
}


DISPLAY::COLOURED_POINT::COLOURED_POINT(int x, int y, double depth, COLORREF newColor) :
   pos(x, y), color(newColor), depth(depth)
{
}

DISPLAY::COLOURED_POINT::COLOURED_POINT(const glm::ivec2 & newPos, COLORREF newColor) :
   pos(newPos), color(newColor), depth(0)
{
}


DISPLAY::COLOURED_POINT::COLOURED_POINT(const glm::ivec2 & newPos, double depth, COLORREF newColor) :
   pos(newPos), color(newColor), depth(depth)
{
}


DISPLAY::COLOURED_POINT::COLOURED_POINT (const COLOURED_POINT & copyPoint) :
   pos(copyPoint.pos), color(copyPoint.color), depth(copyPoint.depth)
{
}


DISPLAY::COLOURED_POINT::~COLOURED_POINT (void)
{
}


DISPLAY::COLOURED_POINT & DISPLAY::COLOURED_POINT::operator= (const COLOURED_POINT & copyPoint)
{
   pos = copyPoint.pos;
   color = copyPoint.color;
   depth = copyPoint.depth;

   return *this;
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


COLORREF DISPLAY::InterpolateColors (COLORREF color0, COLORREF color1, double coeff)
{
   int r[2];
   int g[2];
   int b[2];

   r[0] = GetRValue(color0);
   r[1] = GetRValue(color1);
   g[0] = GetGValue(color0);
   g[1] = GetGValue(color1);
   b[0] = GetBValue(color0);
   b[1] = GetBValue(color1);

   int newR;
   int newG;
   int newB;

   newR = (int)((1 - coeff) * r[0] + coeff * r[1]);
   newG = (int)((1 - coeff) * g[0] + coeff * g[1]);   newB = (int)((1 - coeff) * b[0] + coeff * b[1]);
   return RGB(newR, newG, newB);
}


void DISPLAY::DrawLine (COLORREF * bitPointer, COLOURED_POINT & point1, 
   COLOURED_POINT & point2, int winWidth, int winHeight, double * zBuffer)
{
   int dx = abs(point2.pos.x - point1.pos.x);
   int dy = abs(point2.pos.y - point1.pos.y);
   int sx = (point2.pos.x >= point1.pos.x) ? 1 : -1;
   int sy = (point2.pos.y >= point1.pos.y) ? 1 : -1;
   double segmentLen = sqrt(dx * dx + dy * dy);
    
   if ((point1.pos.x < 0 && point2.pos.x < 0) || 
       (point1.pos.x > winWidth && point2.pos.x > winWidth) ||
       (point1.pos.y < 0 && point2.pos.y < 0) ||
       (point1.pos.y > winHeight && point2.pos.y > winHeight)) {
      return;
   }

   if (dy < dx) {
      int d = (dy << 1) - dx;
      int d1 = dy << 1;
      int d2 = (dy - dx) << 1;

      PutPixel(bitPointer, point1.color, point1.pos.x, point1.pos.y, winWidth, winHeight);

      int x = point1.pos.x + sx;
      int y = point1.pos.y;
      for (int i = 1; i <= dx; i++) {
         if (d > 0) {
            d += d2;
            y += sy;
         } else {
            d += d1;
         }

         if (x < 0) {
            if (sx < 0) {
               break;
            } else {
               x += sx;
               continue;
            }
         } else if (x >= winWidth) {
            if (sx > 0) {
               break;
            } else {
               x += sx;
               continue;
            }
         }

         if (y < 0) {
            if (sy < 0) {
               break;
            } else {
               x += sx;
               continue;
            }
         } else if (y >= winHeight) {
            if (sy > 0) {
               break;
            } else {
               x += sx;
               continue;
            }
         }

         double dz = point1.depth + (point2.depth - point1.depth) * ((float)i / dx);
         int idx = x + y * winWidth;
         if (zBuffer[idx] <= dz) {
            zBuffer[idx] = dz;
            double currLen = sqrt((x - point1.pos.x) * (x - point1.pos.x) +
               (y - point1.pos.y) * (y - point1.pos.y));
            double lenCoeff = currLen / segmentLen;
            COLORREF resColor = InterpolateColors(point1.color, point2.color, lenCoeff);
            PutPixel(bitPointer, resColor, x, y, winWidth, winHeight);
         }
         x += sx;
      }
   } else {
      int d = (dx << 1) - dy;
      int d1 = dx << 1;
      int d2 = (dx - dy) << 1;

      PutPixel(bitPointer, point1.color, point1.pos.x, point1.pos.y, winWidth, winHeight);

      int x = point1.pos.x;
      int y = point1.pos.y + sy;
      for (int i = 1; i <= dy; i++) {
         if (d > 0) {
            d += d2;
            x += sx;
         } else {
            d += d1;
         }

         if (x < 0) {
            if (sx < 0) {
               break;
            } else {
               y += sy;
               continue;
            }
         } else if (x >= winWidth) {
            if (sx > 0) {
               break;
            } else {
               y += sy;
               continue;
            }
         }

         if (y < 0) {
            if (sy < 0) {
               break;
            } else {
               y += sy;
               continue;
            }
         } else if (y >= winHeight) {
            if (sy > 0) {
               break;
            } else {
               y += sy;
               continue;
            }
         }

         double dz = point1.depth + (point2.depth - point1.depth) * ((float)i / dy);
         int idx = x + y * winWidth;
         if (zBuffer[idx] <= dz) {
            zBuffer[idx] = dz;
            double currLen = sqrt((x - point1.pos.x) * (x - point1.pos.x) +
               (y - point1.pos.y) * (y - point1.pos.y));
            double lenCoeff = currLen / segmentLen;
            COLORREF resColor = InterpolateColors(point1.color, point2.color, lenCoeff);
            PutPixel(bitPointer, resColor, x, y, winWidth, winHeight);
         }
         y += sy;
      }
   }
}


void DISPLAY::DrawTriangle (COLORREF * bitPointer, COLOURED_POINT point1,
   COLOURED_POINT point2, COLOURED_POINT point3, int winWidth, int winHeight, double * zBuffer)
{
   // Sorting points by y
   if (point1.pos.y > point2.pos.y) {
      std::swap(point1, point2);
   }
   if (point1.pos.y > point3.pos.y) {
      std::swap(point1, point3);
   }
   if (point2.pos.y > point3.pos.y) {
      std::swap(point2, point3);
   }

   int totalHeight = point3.pos.y - point1.pos.y;
   if (totalHeight == 0 || point1.pos.y + totalHeight < 0 || point1.pos.y > winHeight) {
      return;
   }

   int firstHalfHeight = point2.pos.y - point1.pos.y;
   int secondHalfHeight = point3.pos.y - point2.pos.y;
   if (firstHalfHeight == 0) {
      firstHalfHeight++;
   }
   if (secondHalfHeight == 0) {
      secondHalfHeight++;
   }

   glm::ivec2 segment13 = point3.pos - point1.pos;
   glm::ivec2 segment12 = point2.pos - point1.pos;
   glm::ivec2 segment32 = point3.pos - point2.pos;
   double length32 = sqrt(segment32.x * segment32.x + segment32.y * segment32.y);
   double length13 = sqrt(segment13.x * segment13.x + segment13.y * segment13.y);
   double length12 = sqrt(segment12.x * segment12.x + segment12.y * segment12.y);
   for (int y = 0; y <= totalHeight; y++) {
      if (y + point1.pos.y >=  winHeight) {
         break;
      } else if (point1.pos.y + y < 0) {
         y -= point1.pos.y;
      }

      // Counting coordinates
      float alpha = (float)y / totalHeight;
      int x1 = point1.pos.x + (int)((point3.pos.x - point1.pos.x) * alpha);

      float beta;
      int x2;
      if (y < firstHalfHeight) { // Drawing first half
         beta = (float)y / firstHalfHeight;
         x2 = point1.pos.x + (int)((point2.pos.x - point1.pos.x) * beta);
      } else { // Drawing second half
         beta = (float)(y - firstHalfHeight) / secondHalfHeight;
         x2 = point2.pos.x + (int)((point3.pos.x - point2.pos.x) * beta);
      }


      // Counting interpolated color value
      double currLen1 = sqrt(y * y + (x1 - point1.pos.x) * (x1 - point1.pos.x));
      double lenCoeff1 = currLen1 / length13;
      COLORREF color1 = InterpolateColors(point1.color, point3.color, lenCoeff1);

      double currLen2;
      double lenCoeff2;
      COLORREF color2;
      if (y < firstHalfHeight) {
         currLen2 = sqrt(y * y + (x2 - point1.pos.x) * (x2 - point1.pos.x));
         lenCoeff2 = currLen2 / length12;
         color2 = InterpolateColors(point1.color, point2.color, lenCoeff2);
      } else {
         currLen2 = sqrt((y - firstHalfHeight) * (y - firstHalfHeight) +
            (x2 - point2.pos.x) * (x2 - point2.pos.x));
         lenCoeff2 = currLen2 / length32;
         color2 = InterpolateColors(point2.color, point3.color, lenCoeff2);
      }

      // Filling in between lines
      double z1 = point1.depth + ((point3.depth - point1.depth) * alpha);
      double z2;
      if (y < firstHalfHeight) {
         z2 = point1.depth + ((point2.depth - point1.depth) * beta);
      } else {
         z2 = point2.depth + ((point3.depth - point2.depth) * beta);
      }

      if (x1 > x2) {
         std::swap(x1, x2);
         std::swap(color1, color2);
         std::swap(z1, z2);
      }

      double segmentLen = sqrt((x2 - x1) * (x2 - x1));
      if (segmentLen == 0) {
         segmentLen++;
      }

      for (int j = x1 > 0 ? x1 : 0; j <= x2 && j < winWidth; j++) {
         float phi = x2 == x1 ? 1.0f : (float)(j - x1) / (float)(x2 - x1);
         double pz = z1 + (z2 - z1) * phi;
         int idx = j + (y + point1.pos.y) * winWidth;

         if (zBuffer[idx] < pz) {
            zBuffer[idx] = pz;
            double currLen = sqrt((j - x1) * (j - x1));
            double lenCoeff = currLen / segmentLen;
            COLORREF resColor = InterpolateColors(color1, color2, lenCoeff);
            PutPixel(bitPointer, resColor, j, point1.pos.y + y, winWidth, winHeight);
         }
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
   double * zBuffer = new double[winWidth * winHeight];

   for (auto primIt = primitives.begin(); primIt != primitives.end(); ++primIt) {
      size_t indSize = primIt->vertexIndices.size();
      std::vector<glm::vec3> transformedCoords;

      for (size_t i = 0; i < primIt->vertices.size(); i++) {
         glm::vec3 vert = primIt->vertices[i];
         glm::vec4 tempVec(vert, 1);
         glm::vec4 vec = worldMatrix * tempVec;
         glm::vec3 pos;

         pos = vec;
         pos.x *= ((float)winHeight / winWidth);
         pos.x /= fabs(vec.w);
         pos.y /= fabs(vec.w);
         pos.z /= fabs(vec.w);

         if (vec.w < 0) {
            pos.z = -fabs(pos.z);
         }

         pos.x = pos.x * (winWidth / 2) + winWidth / 2;
         pos.y = pos.y * (winHeight / 2) + winHeight / 2;
         pos.y = (float)(winHeight - pos.y);

         transformedCoords.push_back(pos);
      }

      if (displayMode == DISPLAY_MODE::POINTS) {
         for (size_t i = 0; i < indSize; i++) {
            glm::vec3 vec = transformedCoords[primIt->vertexIndices[i]];

            if (vec.z > 0) {
               PutPixel(bitPointer, RGB(0, 125, 255), (int)vec.x, (int)vec.y, winWidth, winHeight);
            }
         }
      } else {
         for (int i = 0; i < winWidth * winHeight; i++) {
            zBuffer[i] = -1e9;
         }

         for (size_t i = 0; i < indSize; i += 3) {
            const size_t * firstIndex = &primIt->vertexIndices[i];
            COLOURED_POINT points[3];

            for (int c = 0; c < 3; c++) {
               points[c].pos.x = (int)transformedCoords[firstIndex[c]].x;
               points[c].pos.y = (int)transformedCoords[firstIndex[c]].y;
               points[c].depth = transformedCoords[firstIndex[c]].z;
               points[c].color = primIt->vertexColors[firstIndex[c]];
            }

            if (displayMode == DISPLAY_MODE::WIREFRAME) {
               if (points[0].depth > 0 || points[1].depth > 0) {
                  DrawLine(bitPointer, points[0], points[1], winWidth, winHeight, zBuffer);
               }

               if (points[1].depth > 0 || points[2].depth > 0) {
                  DrawLine(bitPointer, points[1], points[2], winWidth, winHeight, zBuffer);
               }

               if (points[2].depth > 0 || points[0].depth > 0) {
                  DrawLine(bitPointer, points[2], points[0], winWidth, winHeight, zBuffer);
               }
            } else {
               if (points[2].depth > 0 || points[1].depth > 0 || points[0].depth > 0) {
                  DrawTriangle(bitPointer, points[0], points[1], points[2], winWidth, winHeight, zBuffer);
               }
            }
         }
      }
   }

   // Copy memory to window
   BitBlt(hDC, 0, 0, winWidth, winHeight, hMemDC, 0, 0, SRCCOPY);

   // Clear up
   delete [] zBuffer;
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

   camera.MouseUpdate(mousePos, rotationSpeed);
   mousePos = NewMousePos;
   OldMousePos = NewMousePos;
}


void DISPLAY::OnOffsetUpdate (void)
{
   camera.position += Offset.z * camera.viewDirection;
   camera.position += Offset.x * glm::cross(camera.viewDirection, camera.up);

   Offset = glm::vec3(0, 0, 0);
   Event = EVENT::NONE;
}


void DISPLAY::OnSwitchModeUpdate (void)
{
   switch (displayMode) {
   case DISPLAY_MODE::POINTS:
      displayMode = DISPLAY_MODE::WIREFRAME;
      break;
   case DISPLAY_MODE::WIREFRAME:
      displayMode = DISPLAY_MODE::TRIANGLES;
      break;
   case DISPLAY_MODE::TRIANGLES:
      displayMode = DISPLAY_MODE::POINTS;
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
