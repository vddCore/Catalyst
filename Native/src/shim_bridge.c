#include <stdio.h>

#include <shlwapi.h>

#include "shim_bridge.h"
#include "shims/shims.h"

int ShimBridge_Create(PShimContext context) {
    if (!context) {
        return -1;
    }

    LPWSTR thisModuleFileName = PathFindFileNameW(context->thisModulePath);
    if (!thisModuleFileName) {
        return -1;
    }

    WCHAR impersonatedModulePath[MAX_PATH] = {0};
    PathCombineW(
        impersonatedModulePath,
        context->systemDirectoryPath,
        thisModuleFileName
    );

    HMODULE originalModuleHandle = LoadLibraryW(impersonatedModulePath);
    if (!originalModuleHandle) {
        return -1;
    }

    Shim_CreateWinHTTP(originalModuleHandle, NULL);
    return 0;
}

void ShimBridge_Destroy(void) {
    Shim_DestroyWinHTTP();
}