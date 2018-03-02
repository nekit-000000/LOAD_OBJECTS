/* START OF 'referenced.cpp' FILE */


#include "referenced.h"


REFERENCED::REFERENCED (void) :
   refCount(0)
{

}


void REFERENCED::Delete (void) const
{
   if (refCount != 0) {
      return;
   }

   delete this;
}


/* END OF 'referenced.cpp' FILE */
