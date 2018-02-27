/* START OF 'windowhandler.h' FILE */

#pragma once
#ifndef WINDOW_HANDLER_H
#define WINDOW_HANDLER_H


class WINDOW_HANDLER {
public:
   using MSG_FUNC = std::function<LRESULT(LPARAM, WPARAM)>;

public:
   WINDOW_HANDLER (void);

   virtual ~WINDOW_HANDLER (void);

   bool WinCreate (HINSTANCE hInstance, int winW, int winH);

private:
   static LONG WINAPI DisplayProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
   void AddMessage (UINT message, MSG_FUNC func);

protected:
   HWND hWnd;
   int winWidth;
   int winHeight;
   std::unordered_map<UINT, MSG_FUNC> msgMap;
};


#endif // WINDOW_HANDLER_H

/* END OF 'windowhandler.h' FILE */
