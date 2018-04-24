/* START OF 'image.cpp' FILE */


#include "material.h"
#include "texture.h"


TEXTURE::TEXTURE (void)
{

}


TEXTURE::TEXTURE (HBITMAP hBmp)
{
   BITMAP bm;
   HDC tempDC = CreateCompatibleDC(NULL);

   SelectObject(tempDC, hBmp);
   GetObject(hBmp, sizeof(BITMAP), (LPVOID)&bm);

   width = bm.bmWidth;
   height = bm.bmHeight;

   for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
         COLORREF color = GetPixel(tempDC, x, y);
         pixels.push_back(color);
      }
   }
}


MATERIAL::MATERIAL (void) :
   ka(glm::vec3(1, 1, 1)), kd(glm::vec3(1, 1, 1)), ks(glm::vec3(1, 1, 1))
{

}


MATERIAL::MATERIAL (const TEXTURE & tex, const glm::vec3 & ka, const glm::vec3 & kd, const glm::vec3 & ks) :
   ka(ka), kd(kd), ks(ks), map(tex)
{

}


void MATERIAL::SetMap (HBITMAP & hBmp)
{
   map = TEXTURE(hBmp);
}


/* END OF 'image.cpp' FILE */
