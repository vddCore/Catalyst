#ifndef CATALYST_SHIM_CONTEXT_H
#define CATALYST_SHIM_CONTEXT_H

#include <windows.h>

#include "shims/shims.h"

typedef struct _ShimContext {
    DWORD parentProcessId;
    HINSTANCE thisModuleHandle;
    
    CHAR thisModulePath[MAX_PATH];
    CHAR appBaseDirectoryPath[MAX_PATH];
    CHAR systemDirectoryPath[MAX_PATH];
} ShimContext, *PShimContext;

PShimContext ShimContext_Create(void);
void ShimContext_Destroy(PShimContext context);

#endif //CATALYST_SHIM_CONTEXT_H
