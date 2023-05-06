#include <stdio.h>

#include <shlwapi.h>

#include "shim_bridge.h"
#include "shims/shims.h"

int ShimBridge_Create(PShimContext context) {
    if (!context) {
        return -1;
    }

    LPCSTR thisModuleFileName = PathFindFileNameA(context->thisModulePath);
    if (!thisModuleFileName) {
        return -1;
    }

    CHAR impersonatedModulePath[MAX_PATH] = {0};
    PathCombineA(
        impersonatedModulePath,
        context->systemDirectoryPath,
        thisModuleFileName
    );

    HMODULE originalModuleHandle = LoadLibraryA(impersonatedModulePath);
    if (!originalModuleHandle) {
        return -1;
    }

    Shim_CreateWinHTTP(originalModuleHandle, NULL);
    return 0;
}

void ShimBridge_Destroy(void) {
    Shim_DestroyWinHTTP();
}