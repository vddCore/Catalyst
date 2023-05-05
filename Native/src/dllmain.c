#include <stdio.h>
#include <locale.h>

#include <windows.h>
#include <strsafe.h>
#include <shlwapi.h>

#include "shim_context.h"
#include "shim_bridge.h"
#include "shim_lifecycle.h"

#include "mono_loader/mono_loader.h"
#include "mono_loader/catalyst_bootstrap.h"

static volatile PShimContext _CurrentShimContext;
static PCatalystMonoContext _CurrentCatalystContext;

BOOL ResolveContextPaths(PShimContext context) {
    if (!context) {
        return FALSE;
    }

    if (!GetSystemDirectoryW(
        context->systemDirectoryPath,
        sizeof(context->systemDirectoryPath))
    ) return FALSE;

    if (!GetModuleFileNameW(
        context->thisModuleHandle,
        context->thisModulePath,
        sizeof(context->thisModulePath))
    ) return FALSE;

    StringCchCopyW(
        context->appBaseDirectoryPath,
        sizeof(context->appBaseDirectoryPath),
        context->thisModulePath
    );

    if (!PathRemoveFileSpecW(context->appBaseDirectoryPath)) {
        return FALSE;
    }

    return TRUE;
}

PShimContext InitializeShimContext(HINSTANCE hInstDLL, DWORD dwProcessId) {
    PShimContext context = ShimContext_Create();

    if (context) {
        context->thisModuleHandle = hInstDLL;
        context->parentProcessId = dwProcessId;

        if (!ResolveContextPaths(context)) {
            goto __error;
        }
    }

    return context;

    __error:
    free(context);
    context = NULL;

    return context;
}

BOOL Attach(HINSTANCE hInstDLL) {
    _CurrentShimContext = InitializeShimContext(
        hInstDLL,
        GetCurrentProcessId()
    );

    if (_CurrentShimContext) {
        ShimLifecycle_Attach(_CurrentShimContext);
        ShimBridge_Create(_CurrentShimContext);
    }

    return (BOOL)(_CurrentShimContext != NULL);
}

void __stdcall Begin(HINSTANCE hInstDLL) {
    BOOL result = Attach(hInstDLL);

    if (result) {
        if (MonoLoader_Boot(_CurrentShimContext->appBaseDirectoryPath, &_CurrentCatalystContext) != MONOLOADER_STATUS_OK) {
            MessageBox(
                NULL,
                "Catalyst has failed to initialize Mono runtime.\nThe game will still function, but mods will not load.",
                "Catalyst boot failure",
                MB_OK
            );
        }
        
        if (Catalyst_Boot(_CurrentCatalystContext) != CATALYST_BOOTSTRAP_STATUS_OK) {
            MessageBox(
                NULL,
                "Catalyst has failed to bootstrap the .NET module.\nThe game will still function, but mods will not load.",
                "Catalyst boot failure",
                MB_OK
            );
        }
    }
}

void Detach() {
    if (_CurrentShimContext) {
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