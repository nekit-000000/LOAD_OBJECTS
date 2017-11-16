/* START OF 'main.cpp' FILE */

#include <vld.h>
#include <gl/glew.h>
#include <windows.h>
#include "display.h"


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   DISPLAY Display;
   Display.WinCreate(hInstance, 640, 480);

   PRIMITIVE prim;
   prim.Load("deer.obj", 0.004);

   Display << prim;

   return Display.Run();
}


/* END OF 'main.cpp' FILE */
