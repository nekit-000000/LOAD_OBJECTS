/* START OF "loader.h" FILE */

#pragma once
#ifndef LOADER_H
#define LOADER_H

#include "objmodel.h"
#include "objectnode.h"


INTRUSIVE_PTR<OBJECT_NODE> LoadObj (const std::string & fileName);


#endif // LOADER_H

/* END OF "loader.h" FILE */
