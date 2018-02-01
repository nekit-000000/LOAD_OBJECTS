/* START OF 'main.cpp' FILE */

#include <windows.h>

#include "display.h"
#include "loader.h"


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   DISPLAY Display;
   Display.WinCreate(hInstance, 640, 480);

   OBJECT_NODE * deer = LoadObj("deer.obj");
   TRANSFORM_NODE * transform = new TRANSFORM_NODE;

   transform->Scale(0.005f);
   transform->AddChild(deer);

   Display.SetSceneData(transform);

   return Display.Run();
}


/* END OF 'main.cpp' FILE */
