/* START OF 'referenced.h' FILE */

#pragma once
#ifndef REFERENCED_H
#define REFERENCED_H


class REFERENCED {
public:
   REFERENCED (void); 

   inline int Ref            (void) const;
   inline int Unref          (void) const;
   inline int ReferenceCount (void) const { return refCount; }

protected:
   void Delete (void) const;

protected:
   mutable int refCount;
};


#include "referenced.hpp"


#endif // REFERENCED_H

/* END OF 'referenced.h' FILE */
