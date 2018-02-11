/* START OF 'windowhandler.hpp' FILE */

#pragma once
#ifndef WINDOW_HANDLER_HPP
#define WINDOW_HANDLER_HPP

#include "windowhandler.h"


template<typename T>
bool WINDOW_HANDLER::AddMessage(UINT message, WINDOW_HANDLER * wnd,
   LRESULT(T::*funcpointer)(LPARAM, WPARAM))
{
   if (!wnd || !funcpointer) {
      return false;
   }

   POINTER msg;
   msg.wnd = wnd;
   msg.func = reinterpret_cast<FuncPointer>(funcpointer);

   msgMap.insert(std::make_pair(message, msg));

   return true;
}


#endif // WINDOW_HANDLER_H

/* END OF 'windowhandler.hpp' FILE */
