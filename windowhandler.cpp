/* START OF 'windowhandler.cpp' FILE */

#include "windowhandler.h"


WINDOW_HANDLER::WINDOW_HANDLER (void) : 
   hWnd(NULL), winWidth(0), winHeight(0)
{
}


WINDOW_HANDLER::~WINDOW_HANDLER (void)
{
}


bool WINDOW_HANDLER::WinCreate (HINSTANCE hInstance, int winW, int winH)
{
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
      300, 200, winW, winH, NULL, NULL, hInstance, this);
      

   if (!hWnd) {
      return false;
   }

   // Set window width and height
   winWidth = winW;
   winHeight = winH;

   // Initialize OpenGL
   /*
   HDC hDC = GetDC(hWnd);
   if (!InitGL(hDC)) {
   return false;
   }
   */

   return true;
}


LRESULT WINAPI WINDOW_HANDLER::DisplayProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   WINDOW_HANDLER * wnd = NULL;

   if (message == WM_CREATE) {
      wnd = (WINDOW_HANDLER *)LPCREATESTRUCT(lParam)->lpCreateParams;
      SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(LPCREATESTRUCT(lParam)->lpCreateParams));
      wnd->hWnd = hWnd;
   }

   if (message == WM_DESTROY) {
      PostQuitMessage(0);
   }

   wnd = (WINDOW_HANDLER *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
   if (wnd) {
      std::unordered_map<UINT, MSG_FUNC>::iterator it;
      it = wnd->msgMap.find(message);

      if (it == wnd->msgMap.end()) {
         return DefWindowProc(hWnd, message, wParam, lParam);
      } else {
         const MSG_FUNC & func = it->second;
         LRESULT result = func(lParam, wParam);

         if (result) {
            return result;
         }
      }
   }

   return DefWindowProc(hWnd, message, wParam, lParam);
}


void WINDOW_HANDLER::AddMessage (UINT message, MSG_FUNC func)
{
   msgMap.insert(std::make_pair(message, func));
}


/* END OF 'windowhandler.cpp' FILE */
