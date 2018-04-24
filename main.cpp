/* START OF 'main.cpp' FILE */


#include "display.h"
#include "intrusiveptr.h"
#include "transformnode.h"
#include "loader.h"


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   DISPLAY Display;
   Display.WinCreate(hInstance, 640, 480);

   INTRUSIVE_PTR<OBJECT_NODE> deer = LoadObj("Objs/cube.obj");
   INTRUSIVE_PTR<TRANSFORM_NODE> parent(new TRANSFORM_NODE);
   INTRUSIVE_PTR<TRANSFORM_NODE> transform(new TRANSFORM_NODE);
   
   transform->Scale(1);
   transform->AddChild(deer);
   parent->AddChild(transform);

   Display.SetSceneData(parent.Get());

   return (int)Display.Run();
}


/* END OF 'main.cpp' FILE */
