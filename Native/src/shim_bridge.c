#include <stdio.h>

#include <shlwapi.h>

#include "shim_bridge.h"
#include "shims/shims.h"
#include "util.h"

int ShimBridge_Create(PShimContext context) {
    if (!context) {
        return -1;
    }

    LPCSTR thisModuleFileName = PathFindFileNameA(context->thisModulePath);
    if (!thisModuleFileName) {
        LOG("Could not retrieve this DLL's module file name.\n");
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
        LOGV("Could not load proxied DLL: 0x%08lX\n", GetLastError());
        return -1;
    }

    Shim_CreateWinHTTP(originalModuleHandle, NULL);
    return 0;
}

void ShimBridge_Destroy(void) {
    Shim_DestroyWinHTTP();
}