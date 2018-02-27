/* START OF 'main.cpp' FILE */

#include "display.h"
#include "loader.h"


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   DISPLAY Display;
   Display.WinCreate(hInstance, 640, 480);

   OBJECT_NODE * deer = LoadObj("deer.obj");
   TRANSFORM_NODE * transform = new TRANSFORM_NODE;
   TRANSFORM_NODE * transform1 = new TRANSFORM_NODE;
   
   transform1->Scale(0.009f);
   transform1->Rotate({0, 1, 0}, 3.14f);
   transform1->Translate({10, 0, 8});
   transform1->AddChild(deer);

   transform->Scale(0.005f);
   transform->AddChild(deer);
   transform->AddBrother(transform1);

   Display.SetSceneData(transform);

   return Display.Run();
}


/* END OF 'main.cpp' FILE */
