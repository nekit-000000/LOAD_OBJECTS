/* START OF 'referenced.hpp' FILE */

#pragma once
#ifndef REFERENCED_HPP
#define REFERENCED_HPP


template <typename REFERENCED_TYPE>
REFERENCED<REFERENCED_TYPE>::REFERENCED (void) :
   refCount(0)
{

}


template <typename REFERENCED_TYPE>
void REFERENCED<REFERENCED_TYPE>::Delete (void) const
{
   REFERENCED * r = const_cast<REFERENCED *>(this);

   delete static_cast<REFERENCED_TYPE *>(r);
}


template <typename REFERENCED_TYPE>
inline int REFERENCED<REFERENCED_TYPE>::Ref (void) const
{
   return ++refCount;
}


template <typename REFERENCED_TYPE>
inline int REFERENCED<REFERENCED_TYPE>::Unref (void) const
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
