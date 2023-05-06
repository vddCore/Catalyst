#include <stdio.h>

#include "shim_lifecycle.h"

BOOL _CanCreateDebugConsole(void) {
    BOOL ret = FALSE;
    LPWSTR cmdline = GetCommandLineW();

    int argc;
    LPWSTR* argv = CommandLineToArgvW(cmdline, &argc);

    for (int i = 0; i < argc; i++) {
        if (!wcscmp(argv[i], L"-console")) {
            ret = TRUE;
        }
    }

    LocalFree(argv);
    return ret;
}

void _CreateDebugConsole(void) {
    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

void _DestroyDebugConsole(void) {
    FreeConsole();
}

void ShimLifecycle_Attach(PShimContext context) {
    if (_CanCreateDebugConsole()) {
        _CreateDebugConsole();
    }
}

void ShimLifecycle_Detach(PShimContext context) {
    _DestroyDebugConsole();
}
