#ifndef ARGENTEA_SHIM_LIFECYCLE_H
#define ARGENTEA_SHIM_LIFECYCLE_H

#include <windows.h>

#include "shim_context.h"

void ConsoleAlloc_Attach(PShimContext context);
void ConsoleAlloc_Detach(PShimContext context);

#endif //ARGENTEA_SHIM_LIFECYCLE_H