/* START OF 'intrusiveptr.h' FILE */

#pragma once
#ifndef INTRUSIVEPTR_H
#define INTRUSIVEPTR_H


template<typename PTR_TYPE>
class INTRUSIVE_PTR {
   template<typename OTHER> 
   friend class INTRUSIVE_PTR;

public:
   INTRUSIVE_PTR (void);
   INTRUSIVE_PTR (PTR_TYPE * ptr);
   INTRUSIVE_PTR (const INTRUSIVE_PTR & intrPtr);

   template<typename OTHER>
   INTRUSIVE_PTR (const INTRUSIVE_PTR<OTHER> & intrPtr);

   ~INTRUSIVE_PTR (void);

   INTRUSIVE_PTR & operator= (const INTRUSIVE_PTR & intrPtr);
   INTRUSIVE_PTR & operator= (PTR_TYPE * ptr);
   bool operator==           (const INTRUSIVE_PTR & intrPtr) const;
   bool operator==           (const PTR_TYPE * ptr) const;
   bool operator!=           (const INTRUSIVE_PTR & intrPtr) const;
   bool operator!=           (const PTR_TYPE * ptr) const;
   bool operator<            (const INTRUSIVE_PTR & intrPtr) const;
   PTR_TYPE & operator*      (void) const;
   PTR_TYPE * operator->     (void) const;
   bool operator!            (void) const;

   friend bool operator==    (const PTR_TYPE * ptr, const INTRUSIVE_PTR & intrPtr) { return ptr == intrPtr.ptr; }
   friend bool operator!=    (const PTR_TYPE * ptr, const INTRUSIVE_PTR & intrPtr) { return ptr != intrPtr.ptr; }

   template<typename OTHER>
   INTRUSIVE_PTR & operator= (const INTRUSIVE_PTR<OTHER> & intrPtr);

   PTR_TYPE * Get            (void) const;
   bool IsValid              (void) const;
   void Swap                 (INTRUSIVE_PTR & rp);

private:
   template<typename OTHER>
   void Assign (const INTRUSIVE_PTR<OTHER> & ptr);

private:
   PTR_TYPE * ptr;
};


#include "intrusiveptr.hpp"


#endif // INTRUSIVEPTR_H

/* END OF 'intrusiveptr.h' FILE */
