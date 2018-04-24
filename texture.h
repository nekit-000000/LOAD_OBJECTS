/* START OF 'texture.h' FILE */


#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H


class TEXTURE {
   friend class RENDER;

public:
   TEXTURE (void);
   TEXTURE (HBITMAP bmp);

   int GetHeight     (void) const { return height; };
   int GetWidth      (void) const { return width; };
   COLORREF GetColor (int i) const { return pixels[i]; };

private:
   int width;
   int height;
   std::vector<COLORREF> pixels;
};


#endif // TEXTURE_H


/* END OF 'texture.h' FILE */
