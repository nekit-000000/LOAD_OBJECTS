/* START OF 'render.cpp' FILE */


#include "render.h"
#include "shadermodel.h"
#include "nodevisitor.h"


class RENDER_VISITOR : public NODE_VISITOR {
public:
   RENDER_VISITOR (const glm::mat4 worldMatrix, int winWidth, int winHeight, COLORREF * bitPointer, 
      RENDER::DISPLAY_MODE displayMode) :
      worldMatrix(worldMatrix), winHeight(winHeight), winWidth(winWidth), bitPointer(bitPointer),
      displayMode(displayMode)
   {
      zBuffer = new int[winWidth * winHeight];

      for (int i = 0; i < winWidth * winHeight; i++) {
         zBuffer[i] = (int)-1e9;
      }

      transformStack.push(glm::mat4());
   }

   ~RENDER_VISITOR (void) 
   {
      delete [] zBuffer;
   }

   void Apply (OBJECT_NODE & node)
   {
      currentTransform = transformStack.top();
      transformStack.pop();
      RENDER::DrawNode((OBJECT_NODE *)&node, worldMatrix * currentTransform, bitPointer,
         winWidth, winHeight, zBuffer, displayMode);
   }

   void Apply (TRANSFORM_NODE & node)
   {
      currentTransform = transformStack.top();
      transformStack.push(node.GetTransform() * currentTransform);
      node.Descend(*this);
   }

private:
   glm::mat4 worldMatrix;
   glm::mat4 currentTransform;
   std::stack<glm::mat4> transformStack;
   COLORREF * bitPointer;
   RENDER::DISPLAY_MODE displayMode;
   int winHeight;
   int winWidth;
   int * zBuffer;
};


RENDER::RENDER (void) :
   displayMode(DISPLAY_MODE::POINTS), backgroundColor(RGB(0, 0, 0))
{
   root = new TRANSFORM_NODE;
}


RENDER::RENDER (SCENE_NODE * newRoot) :
   displayMode(DISPLAY_MODE::POINTS), backgroundColor(RGB(0, 0, 0))
{
   root->AddChild(newRoot);
}


RENDER::RENDER (SCENE_NODE * newRoot, const CAMERA & newCamera) :
   camera(newCamera), displayMode(DISPLAY_MODE::POINTS), backgroundColor(RGB(0, 0, 0))
{
   root->AddChild(newRoot);
}


RENDER::~RENDER (void)
{
   delete root;
}


RENDER::COLOURED_POINT::COLOURED_POINT (void) :
   pos(0, 0), color(RGB(0, 0, 0)), depth(0)
{
}


RENDER::COLOURED_POINT::COLOURED_POINT (int x, int y, COLORREF newColor) :
   pos(x, y), color(newColor), depth(0)
{
}


RENDER::COLOURED_POINT::COLOURED_POINT (int x, int y, double depth, COLORREF newColor) :
   pos(x, y), color(newColor), depth(depth)
{
}


RENDER::COLOURED_POINT::COLOURED_POINT (const glm::ivec2 & newPos, COLORREF newColor) :
   pos(newPos), color(newColor), depth(0)
{
}


RENDER::COLOURED_POINT::COLOURED_POINT (const glm::ivec2 & newPos, double depth, COLORREF newColor) :
   pos(newPos), color(newColor), depth(depth)
{
}


RENDER::COLOURED_POINT::COLOURED_POINT (const COLOURED_POINT & copyPoint) :
   pos(copyPoint.pos), color(copyPoint.color), depth(copyPoint.depth)
{
}


RENDER::COLOURED_POINT::~COLOURED_POINT (void)
{
}


RENDER::COLOURED_POINT & RENDER::COLOURED_POINT::operator= (const COLOURED_POINT & copyPoint)
{
   pos = copyPoint.pos;
   color = copyPoint.color;
   depth = copyPoint.depth;

   return *this;
}


void RENDER::CreateDIB (HDC hDC, HBITMAP * hBitmap, COLORREF ** bitPointer, BITMAPINFO * bitmap)
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


COLORREF RENDER::InterpolateColors (COLORREF color0, COLORREF color1, double coeff)
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


void RENDER::PutPixel (COLORREF * bitPointer, COLORREF color, int x, int y, int winWidth, int winHeight)
{
   if ((winHeight - y) <= 0 || x >= winWidth || x <= 0 || (winHeight - y) >= winHeight) {
      return;
   }

   color = FlipColor(color);
   bitPointer[(winHeight - y) * winWidth + x] = color;
}


void RENDER::PutPixel (COLORREF * bitPointer, COLOURED_POINT point, int winWidth, int winHeight)
{
   PutPixel(bitPointer, point.color, point.pos.x, point.pos.y, winWidth, winHeight);
}


bool RENDER::CheckIfPointInFrame (int width, int height, int px, int py)
{
   if (py <= 0 || px >= width || px <= 0 || py >= height) {
      return false;
   }
   return true;
}


void RENDER::DrawPoint (COLORREF * bitPointer, const glm::ivec3 & point,
   int winWidth, int winHeight, int * zBuffer)
{
   if (!CheckIfPointInFrame(winWidth, winHeight, point.x, point.y) || point.z < 0) {
      return;
   }

   int x = point.x;
   int y = point.y;

   int idx = x + y * winWidth;
   if (zBuffer[idx] < point.z) {
      zBuffer[x] = point.z;
      PutPixelNoCheck(bitPointer, WireFrameColor, x, y, winWidth, winHeight);
   }
}


void RENDER::DrawLine (COLORREF * bitPointer, const glm::ivec3 * pts, int width, int height,
   int * zBuffer)
{
   if (pts[0].z < 0 || pts[1].z < 0 || 
      (!CheckIfPointInFrame(width, height, (int)pts[0].x, (int)pts[0].y) &&
      !CheckIfPointInFrame(width, height, (int)pts[1].x, (int)pts[1].y))) {
      return;
   }
   
   int dx = (int)abs(pts[1].x - pts[0].x);
   int dy = (int)abs(pts[1].y - pts[0].y);
   int sx = (int)(pts[1].x >= pts[0].x) ? 1 : -1;
   int sy = (int)(pts[1].y >= pts[0].y) ? 1 : -1;
   double segmentLen = sqrt(dx * dx + dy * dy);

   if (dy < dx) {
      int d = (dy << 1) - dx;
      int d1 = dy << 1;
      int d2 = (dy - dx) << 1;

      PutPixel(bitPointer, WireFrameColor, (int)pts[0].x, (int)pts[0].y, width, height);

      int x = (int)pts[0].x + sx;
      int y = (int)pts[0].y;
      for (int i = 1; i <= dx; i++) {
         if (d > 0) {
            d += d2;
            y += sy;
         } else {
            d += d1;
         }

         if (x <= 0) {
            if (sx < 0) {
               break;
            } else {
               x += sx;
               continue;
            }
         } else if (x >= width) {
            if (sx > 0) {
               break;
            } else {
               x += sx;
               continue;
            }
         }

         if (y <= 0) {
            if (sy < 0) {
               break;
            } else {
               x += sx;
               continue;
            }
         } else if (y >= height) {
            if (sy > 0) {
               break;
            } else {
               x += sx;
               continue;
            }
         }

         int dz = pts[0].z + (pts[1].z - pts[0].z) * (i / dx);
         int idx = x + y * width;
         if (zBuffer[idx] < dz) {
            zBuffer[idx] = dz;
            PutPixelNoCheck(bitPointer, WireFrameColor, x, y, width, height);
         }
         x += sx;
      }
   } else {
      int d = (dx << 1) - dy;
      int d1 = dx << 1;
      int d2 = (dx - dy) << 1;

      PutPixel(bitPointer, WireFrameColor, (int)pts[0].x, (int)pts[0].y, width, height);

      int x = (int)pts[0].x;
      int y = (int)pts[0].y + sy;
      for (int i = 1; i <= dy; i++) {
         if (d > 0) {
            d += d2;
            x += sx;
         } else {
            d += d1;
         }

         if (x <= 0) {
            if (sx < 0) {
               break;
            } else {
               y += sy;
               continue;
            }
         } else if (x >= width) {
            if (sx > 0) {
               break;
            } else {
               y += sy;
               continue;
            }
         }

         if (y <= 0) {
            if (sy < 0) {
               break;
            }
            else {
               y += sy;
               continue;
            }
         } else if (y >= height) {
            if (sy > 0) {
               break;
            } else {
               y += sy;
               continue;
            }
         }

         int dz = pts[0].z + (pts[1].z - pts[0].z) * (i / dy);
         int idx = x + y * width;
         if (zBuffer[idx] < dz) {
            zBuffer[idx] = dz;
            PutPixelNoCheck(bitPointer, WireFrameColor, x, y, width, height);
         }
         y += sy;
      }
   }
}


void RENDER::PutPixelNoCheck (COLORREF * bitPointer, COLORREF color, int x, int y, int winWidth, int winHeight)
{
   bitPointer[(winHeight - y) * winWidth + x] = color;
}


void RENDER::DrawLine (COLORREF * bitPointer, const glm::ivec3 & pt1, const glm::vec3 & pt2, 
   int width, int height, int * zBuffer)
{
   glm::ivec3 pts[2] = { pt1, pt2 };
   DrawLine(bitPointer, pts, width, height, zBuffer);
}


void RENDER::DrawTriangle (COLORREF * bitPointer, COLOURED_POINT point1,
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
      if (y + point1.pos.y >= winHeight) {
         break;
      }

      if (point1.pos.y + y <= 0) {
         y -= point1.pos.y;
         y++;
      }

      // Counting coordinates
      float alpha = (float)y / totalHeight;
      int x1 = point1.pos.x + (int)((point3.pos.x - point1.pos.x) * alpha);

      float beta;
      int x2;
      if (y < firstHalfHeight) { // Drawing first half
         beta = (float)y / firstHalfHeight;
         x2 = point1.pos.x + (int)((point2.pos.x - point1.pos.x) * beta);
      }
      else { // Drawing second half
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
      }
      else {
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

      for (int j = x1 > 0 ? x1 : 1; j <= x2 && j < winWidth; j++) {
         float phi = x2 == x1 ? 1.0f : (float)(j - x1) / (float)(x2 - x1);
         double pz = z1 + (z2 - z1) * phi;
         int idx = j + (y + point1.pos.y) * winWidth;

         if (zBuffer[idx] < pz) {
            zBuffer[idx] = pz;
            double currLen = sqrt((j - x1) * (j - x1));
            double lenCoeff = currLen / segmentLen;
            COLORREF resColor = InterpolateColors(color1, color2, lenCoeff);
            resColor = RGB(GetBValue(resColor), GetGValue(resColor), GetRValue(resColor));
            bitPointer[(winHeight - (point1.pos.y + y)) * winWidth + j] = resColor;
         }
      }
   }
}


glm::vec3 RENDER::Barycentric (const glm::vec2 & a, const glm::vec2 & b, 
   const glm::vec2 & c, const glm::vec2 & p)
{
   glm::vec3 s[2];

   for (int i = 2; i--; ) {
      s[i][0] = c[i] - a[i];
      s[i][1] = b[i] - a[i];
      s[i][2] = a[i] - p[i];
   }

   glm::vec3 u = glm::cross(s[0], s[1]);

   if (std::abs(u[2]) > 0) {
      return glm::vec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
   }
   return glm::vec3(-1, 1, 1);
}


void RENDER::DrawTriangleBarycentric (COLORREF * bitPointer, glm::vec4 * pts, 
   glm::vec2 * uvs, const TEXTURE & tex, int winWidth, int winHeight, int * zBuffer)
{
   glm::vec2 bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
   glm::vec2 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
   glm::vec2 clampMax(winWidth - 1, winHeight - 1);
   glm::vec2 clampMin(1, 1);

   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 2; j++) {
         bboxmin[j] = std::max(clampMin[j], std::min(bboxmin[j], pts[i][j] / pts[i][3]));
         bboxmax[j] = std::min(clampMax[j], std::max(bboxmax[j], pts[i][j] / pts[i][3]));
      }
   }

   glm::ivec2 p;
   COLORREF color;
   for (p.x = (int)bboxmin.x; p.x <= bboxmax.x; p.x++) {
      for (p.y = (int)bboxmin.y; p.y <= bboxmax.y; p.y++) {
         glm::vec3 screen = Barycentric(pts[0] / pts[0][3], pts[1] / pts[1][3], pts[2] / pts[2][3], p);
         float z = pts[0][2] * screen.x + pts[1][2] * screen.y + pts[2][2] * screen.z;
         float w = pts[0][3] * screen.x + pts[1][3] * screen.y + pts[2][3] * screen.z;
         int fragDepth = (int)(z / w);

         // This is used for texture perspective correction
         glm::vec3 clip = glm::vec3(screen.x / pts[0][3], screen.y / pts[1][3], screen.z / pts[2][3]);
         clip /= clip.x + clip.y + clip.z;
         //

         if (screen.x < 0 || screen.y < 0 || screen.z < 0 || zBuffer[p.x + p.y * winWidth] > fragDepth) {
            continue;
         }

         if (tex.width > 0) {
            glm::vec2 uv;
            glm::mat3x2 m(uvs[0], uvs[1], uvs[2]);
            uv = m * clip;
            int y = (int)(uv.y * (tex.height - 1));
            int x = (int)(uv.x * (tex.width - 1));
            color = tex.pixels[y * tex.width + x];
         } else {
            color = WireFrameColor;
         }

         color = FlipColor(color);
         zBuffer[p.x + p.y * winWidth] = fragDepth;
         PutPixelNoCheck(bitPointer, color, p.x, p.y, winWidth, winHeight);
      }
   }
}


void RENDER::DrawTriangleBarycentric (COLORREF * bitPointer, glm::vec4 * pts,
   iSHADER & shader, int winWidth, int winHeight, int * zBuffer)
{
   glm::vec2 bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
   glm::vec2 bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
   glm::vec2 clampMax(winWidth - 1, winHeight - 1);
   glm::vec2 clampMin(1, 1);

   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 2; j++) {
         bboxmin[j] = std::max(clampMin[j], std::min(bboxmin[j], pts[i][j] / pts[i][3]));
         bboxmax[j] = std::min(clampMax[j], std::max(bboxmax[j], pts[i][j] / pts[i][3]));
      }
   }

   glm::ivec2 p;
   COLORREF color;
   for (p.x = (int)bboxmin.x; p.x <= bboxmax.x; p.x++) {
      for (p.y = (int)bboxmin.y; p.y <= bboxmax.y; p.y++) {
         glm::vec3 screen = Barycentric(pts[0] / pts[0][3], pts[1] / pts[1][3], pts[2] / pts[2][3], p);
         float z = pts[0][2] * screen.x + pts[1][2] * screen.y + pts[2][2] * screen.z;
         float w = pts[0][3] * screen.x + pts[1][3] * screen.y + pts[2][3] * screen.z;
         int fragDepth = (int)(z / w);

         // This is used for texture perspective correction
         glm::vec3 clip = glm::vec3(screen.x / pts[0][3], screen.y / pts[1][3], screen.z / pts[2][3]);
         clip /= clip.x + clip.y + clip.z;
         //

         if (screen.x < 0 || screen.y < 0 || screen.z < 0 || zBuffer[p.x + p.y * winWidth] > fragDepth) {
            continue;
         }

         bool discard = shader.Fragment(clip, color);
         if (!discard) {
            color = FlipColor(color);
            zBuffer[p.x + p.y * winWidth] = fragDepth;
            PutPixelNoCheck(bitPointer, color, p.x, p.y, winWidth, winHeight);
         }
      }
   }
}


void RENDER::DrawNode (const OBJECT_NODE * node, const glm::mat4 & matrixTransform, COLORREF * bitPointer,
   int winWidth, int winHeight, int * zBuffer, DISPLAY_MODE displayMode)
{
   const objMODEL * prim = &node->object;
   size_t indSize = prim->vertexIndices.size();

   SHADER_MODEL shader;

   shader.SetTransformMatrix(matrixTransform);
   shader.SetModel(prim);

   if (displayMode == DISPLAY_MODE::POINTS) {
      for (size_t i = 0; i < prim->vertices.size(); i++) {
         glm::vec4 vec = shader.Vertex(i, 0);
         vec /= vec.w;
         DrawPoint(bitPointer, vec,  winWidth, winHeight, zBuffer);
      }
   } else {
      for (size_t i = 0; i < indSize; i += 3) {
         glm::vec4 pts[3];

         for (int c = 0; c < 3; c++) {
            pts[c] = shader.Vertex(i + c, c);
         }

         if (displayMode == DISPLAY_MODE::WIREFRAME) {
            DrawLine(bitPointer, pts[0] / pts[0].w, pts[1] / pts[1].w, winWidth, winHeight, zBuffer);
            DrawLine(bitPointer, pts[1] / pts[1].w, pts[2] / pts[2].w, winWidth, winHeight, zBuffer);
            DrawLine(bitPointer, pts[2] / pts[2].w, pts[0] / pts[0].w, winWidth, winHeight, zBuffer);
         } else { // displayMode == DISPLAY_MODE::TRIANGLES
            DrawTriangleBarycentric(bitPointer, pts, shader, winWidth, winHeight, zBuffer);
         }
      }
   }
}


void RENDER::DrawScene (HWND hWnd, int winWidth, int winHeight) const
{
   BITMAPINFO bitmap;
   HBITMAP hBitmap;
   RECT rect;
   COLORREF * bitPointer;
   PAINTSTRUCT ps;

   // Get current DC
   HDC hDC = GetDC(hWnd);
   BeginPaint(hWnd, &ps);

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

   // Drawing scene
   glm::mat4 worldMatrix = GetViewportMatrix(0, 0, winWidth, winHeight) * 
      GetProjectionMatrix() * camera.GetWorldToViewMatrix();
   RENDER_VISITOR rv(worldMatrix, winWidth, winHeight, bitPointer, displayMode);
   rv.Apply(*root);

   // Copy memory to window
   BitBlt(hDC, 0, 0, winWidth, winHeight, hMemDC, 0, 0, SRCCOPY);

   // Clear up
   DeleteObject(hBitmap);
   DeleteObject(hMemDC);

   EndPaint(hWnd, &ps);
}


glm::mat4 RENDER::GetProjectionMatrix (void) const
{
   return glm::mat4({ 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 1 }, { 0, 0, 1, 0 });
}


glm::mat4 RENDER::GetViewportMatrix (int x0, int y0, int width, int height)
{
   glm::mat4 m = glm::mat4();
   m[3][0] = x0 + width / 2.f;
   m[3][1] = y0 + height / 2.f;
   m[3][2] = 255 / 2.f;

   m[0][0] = width / 2.f;
   m[1][1] = height / 2.f;
   m[2][2] = 255 / 2.f;

   return m;
}


/* END OF 'render.cpp' FILE */
