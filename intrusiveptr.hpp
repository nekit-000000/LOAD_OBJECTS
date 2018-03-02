/* START OF 'intrusiveptr.hpp' FILE */

#pragma once
#ifndef INTRUSIVEPTR_HPP
#define INTRUSIVEPTR_HPP


template <typename PTR_TYPE>
INTRUSIVE_PTR<PTR_TYPE>::INTRUSIVE_PTR (void) : ptr(NULL)
{

}


template <typename PTR_TYPE>
INTRUSIVE_PTR<PTR_TYPE>::INTRUSIVE_PTR (PTR_TYPE * ptr) : ptr(ptr)
{
   if (ptr) {
      ptr->Ref();
   }
}


template <typename PTR_TYPE>
INTRUSIVE_PTR<PTR_TYPE>::INTRUSIVE_PTR (const INTRUSIVE_PTR & intrPtr) :
   ptr(intrPtr.ptr)
{
   if (ptr) {
      ptr->Ref();
   }
}


template <typename PTR_TYPE>
template <typename OTHER>
INTRUSIVE_PTR<PTR_TYPE>::INTRUSIVE_PTR (const INTRUSIVE_PTR<OTHER> & intrPtr) :
   ptr(intrPtr.ptr)
{
   if (ptr) {
      ptr->Ref();
   }
}


template <typename PTR_TYPE>
INTRUSIVE_PTR<PTR_TYPE>::~INTRUSIVE_PTR (void)
{
   if (ptr) {
      ptr->Unref();
   }

   ptr = NULL;
}


template <typename PTR_TYPE>
template <typename OTHER>
void INTRUSIVE_PTR<PTR_TYPE>::Assign (const INTRUSIVE_PTR<OTHER> & intrPtr)
{
   if (ptr == intrPtr.ptr) {
      return;
   }

   PTR_TYPE * tmpPtr = ptr;
   ptr = intrPtr.ptr;

   if (ptr) {
      ptr->Ref();
   }

   if (tmpPtr) {
      tmpPtr->Unref();
   }
}


template <typename PTR_TYPE>
INTRUSIVE_PTR<PTR_TYPE> & INTRUSIVE_PTR<PTR_TYPE>::operator= (const INTRUSIVE_PTR & intrPtr)
{
   Assign(intrPtr);
   return *this;
}


template <typename PTR_TYPE>
template <typename OTHER>
INTRUSIVE_PTR<PTR_TYPE> & INTRUSIVE_PTR<PTR_TYPE>::operator= (const INTRUSIVE_PTR<OTHER> & intrPtr)
{
   Assign(intrPtr);
   return *this;
}


template <typename PTR_TYPE>
INTRUSIVE_PTR<PTR_TYPE> & INTRUSIVE_PTR<PTR_TYPE>::operator= (PTR_TYPE * newPtr)
{
   if (ptr == newPtr) {
      return *this;
   }

   PTR_TYPE * tmpPtr = ptr;
   ptr = newPtr;

   if (ptr) {
      ptr->Ref();
   }

   if (tmpPtr) {
      tmpPtr->Unref();
   }

   return *this;
}


template <typename PTR_TYPE>
bool INTRUSIVE_PTR<PTR_TYPE>::operator== (const INTRUSIVE_PTR & intrPtr) const
{
   return ptr == intrPtr.ptr;
}


template <typename PTR_TYPE>
bool INTRUSIVE_PTR<PTR_TYPE>::operator== (const PTR_TYPE * compPtr) const
{
   return ptr == compPtr;
}


template <typename PTR_TYPE>
bool INTRUSIVE_PTR<PTR_TYPE>::operator!= (const INTRUSIVE_PTR & intrPtr) const
{
   return ptr != intrPtr.ptr;
}


template <typename PTR_TYPE>
bool INTRUSIVE_PTR<PTR_TYPE>::operator!= (const PTR_TYPE * compPtr) const
{
   return ptr != compPtr;
}


template <typename PTR_TYPE>
bool INTRUSIVE_PTR<PTR_TYPE>::operator< (const INTRUSIVE_PTR & intrPtr) const
{
   return ptr < intrPtr;
}


template <typename PTR_TYPE>
PTR_TYPE & INTRUSIVE_PTR<PTR_TYPE>::operator* (void) const
{
   return *ptr;
}


template <typename PTR_TYPE>
PTR_TYPE * INTRUSIVE_PTR<PTR_TYPE>::operator-> (void) const
{
   return ptr;
}


template <typename PTR_TYPE>
bool INTRUSIVE_PTR<PTR_TYPE>::operator! (void) const
{
   return ptr == NULL;
}


template <typename PTR_TYPE>
PTR_TYPE * INTRUSIVE_PTR<PTR_TYPE>::Get (void) const
{
   return ptr;
}


template <typename PTR_TYPE>
bool INTRUSIVE_PTR<PTR_TYPE>::IsValid (void) const
{
   return ptr != NULL;
}


template <typename PTR_TYPE>
void INTRUSIVE_PTR<PTR_TYPE>::Swap (INTRUSIVE_PTR & intrPtr)
{
   PTR_TYPE * tmp = ptr;
   ptr = intrPtr.ptr;
   intrPtr.ptr = tmp;
}




#endif // INTRUSIVEPTR_HPP

/* END OF 'intrusiveptr.hpp' FILE */