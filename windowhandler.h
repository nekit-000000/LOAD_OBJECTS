/* START OF 'windowhandler.h' FILE */

#pragma once
#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H

#include <windows.h>
#include <map>


class WINDOW_HANDLER {
public:
   WINDOW_HANDLER (void);

   virtual ~WINDOW_HANDLER (void);

   bool WinCreate (HINSTANCE hInstance, int winW, int winH);

private:
   typedef LRESULT (WINDOW_HANDLER::*FuncPointer)(LPARAM, WPARAM);

   // pointer struct to  the handle function
   struct POINTER {
   public:
      WINDOW_HANDLER * wnd;
      FuncPointer func;
   };

   static LONG WINAPI DisplayProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
   template<typename T>
   bool AddMessage (UINT message, WINDOW_HANDLER * wnd,
      LRESULT(T::*funcpointer)(LPARAM, WPARAM));

protected:
   HWND hWnd;
   int winWidth;
   int winHeight;
   std::map<UINT, POINTER> msgMap;
};


#include "windowhandler.hpp"


#endif // WINDOW_HANDLER_H

/* END OF 'windowhandler.h' FILE */
