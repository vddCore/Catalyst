#include <stdio.h>
#include <locale.h>

#include <windows.h>
#include <strsafe.h>
#include <shlwapi.h>

#include "shim_context.h"
#include "shim_bridge.h"
#include "console_alloc.h"

#include "mono_loader/mono_loader.h"
#include "mono_loader/catalyst_bootstrap.h"
#include "util.h"

static volatile PShimContext _CurrentShimContext;
static PCatalystMonoContext _CurrentCatalystContext;

BOOL ResolveContextPaths(PShimContext context) {
    if (!context) {
        LOG("Invalid shim context provided.\n");
        return FALSE;
    }

    if (!GetSystemDirectoryA(
        context->systemDirectoryPath,
        sizeof(context->systemDirectoryPath))
        ) {
        LOGV("Failed to retrieve the system library directory: 0x%08lX.\n", GetLastError());
        return FALSE;
    }

    if (!GetModuleFileNameA(
        context->thisModuleHandle,
        context->thisModulePath,
        sizeof(context->thisModulePath))
        ) {
        LOGV("Failed to retrieve this DLL's file path: 0x%08lX.\n", GetLastError());
        return FALSE;
    }

    StringCchCopyA(
        context->appBaseDirectoryPath,
        sizeof(context->appBaseDirectoryPath),
        context->thisModulePath
    );

    if (!PathRemoveFileSpecA(context->appBaseDirectoryPath)) {
        LOG("Failed to retrieve application base directory path.\n");
        return FALSE;
    }

    return TRUE;
}

PShimContext InitializeShimContext(HINSTANCE hInstDLL, DWORD dwProcessId) {
    PShimContext context = ShimContext_Create();

    if (!context) {
        LOG("Shim context creation failed.\n");
        return NULL;
    }

    context->thisModuleHandle = hInstDLL;
    context->parentProcessId = dwProcessId;

    if (!ResolveContextPaths(context)) {
        free(context);
        context = NULL;
        LOG("One or more critical paths have failed to resolve.\n");
    }

    return context;
}

BOOL Attach(HINSTANCE hInstDLL) {
    ConsoleAlloc_Attach();
    LOGV("Catalyst Native [%s] starting...\n", CATALYST_VERSION);

    _CurrentShimContext = InitializeShimContext(
        hInstDLL,
        GetCurrentProcessId()
    );

    if (_CurrentShimContext) {
        ShimBridge_Create(_CurrentShimContext);
    }

    return (BOOL)(_CurrentShimContext != NULL);
}

void __stdcall Begin(HINSTANCE hInstDLL) {
    BOOL result = Attach(hInstDLL);

    if (result) {
        MonoLoader_Status loaderStatus = MonoLoader_Boot(_CurrentShimContext->appBaseDirectoryPath,
                                                         &_CurrentCatalystContext);
        if (loaderStatus != MONOLOADER_STATUS_OK) {
            LOGV(
                "Mono loader has failed with status code %d.\n",
                loaderStatus
            );
            LOG("Mods will *not* be loaded.\n");

            return;
        }

        CatalystBootstrap_Status bootstrapStatus = Catalyst_Boot(_CurrentCatalystContext);
        if (bootstrapStatus != CATALYST_BOOTSTRAP_STATUS_OK) {
            LOGV(
                "Catalyst managed layer bootstrapper failed with status code %d.\n",
                bootstrapStatus
            );
            LOG("Mods will *not* be loaded.\n");
            return;
        }
    } else {
        LOG("Shim context initialization has failed.\n");
        LOG("Mods will *not* be loaded.\n");
    }
}

void Detach() {
    if (_CurrentShimContext) {
        ConsoleAlloc_Detach();
        ShimContext_Destroy(_CurrentShimContext);
    }
}

BOOL APIENTRY DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    setlocale(LC_ALL, "C.UTF-8");

    switch (fdwReason) {
        case DLL_PROCESS_ATTACH: {
            CloseHandle(
                CreateThread(
                    NULL,
                    0,
                    (LPTHREAD_START_ROUTINE)Begin,
                    hInstDLL,
                    0,
                    NULL
                )
            );
            break;
        }

        case DLL_PROCESS_DETACH:
            Detach();
            break;

        default:
/* Ignore request, not something we care about. */
            break;
    }

    return TRUE;
}