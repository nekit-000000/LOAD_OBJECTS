/* START OF 'main.cpp' FILE */


#include <vld.h>

#include "display.h"
#include "intrusiveptr.h"
#include "transformnode.h"
#include "loader.h"


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   DISPLAY Display;
   Display.WinCreate(hInstance, 640, 480);

   INTRUSIVE_PTR<OBJECT_NODE> deer = LoadObj("deer.obj");
   INTRUSIVE_PTR<TRANSFORM_NODE> parent(new TRANSFORM_NODE);
   INTRUSIVE_PTR<TRANSFORM_NODE> transform(new TRANSFORM_NODE);
   INTRUSIVE_PTR<TRANSFORM_NODE> transform1(new TRANSFORM_NODE);

   transform1->Scale(0.009f);
   transform1->Rotate({0, 1, 0}, 3.14f);
   transform1->Translate({10, 0, 8});
   transform1->AddChild(deer);

   transform->Scale(0.005f);
   transform->AddChild(deer);
   parent->AddChild(transform);

   transform->GetParent(0)->AddChild(transform1);

   Display.SetSceneData(parent.Get());

   return Display.Run();
}


/* END OF 'main.cpp' FILE */
