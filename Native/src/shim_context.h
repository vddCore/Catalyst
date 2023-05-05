#ifndef CATALYST_SHIM_CONTEXT_H
#define CATALYST_SHIM_CONTEXT_H

#include <windows.h>

#include "shims/shims.h"

typedef struct _ShimContext {
    DWORD parentProcessId;
    HINSTANCE thisModuleHandle;
    
    WCHAR thisModulePath[MAX_PATH];
    WCHAR appBaseDirectoryPath[MAX_PATH];
    WCHAR systemDirectoryPath[MAX_PATH];
} ShimContext, *PShimContext;

PShimContext ShimContext_Create(void);
void ShimContext_Destroy(PShimContext context);

#endif //CATALYST_SHIM_CONTEXT_H
