/* START OF 'referenced.hpp' FILE */

#pragma once
#ifndef REFERENCED_HPP
#define REFERENCED_HPP


inline int REFERENCED::Ref (void) const
{
   return ++refCount;
}


inline int REFERENCED::Unref (void) const
{
   int newRef;
   newRef = --refCount;

   if (newRef == 0) {
      Delete();
   }

   return newRef;
}


#endif // REFERENCED_HPP

/* END OF 'referenced.hpp' FILE */
