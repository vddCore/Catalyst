#include <stdio.h>
#include <windows.h>

#include "console_alloc.h"

void _CreateDebugConsole(void) {
    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

void _DestroyDebugConsole(void) {
    FreeConsole();
}

void ConsoleAlloc_Attach() { _CreateDebugConsole(); }
void ConsoleAlloc_Detach() { _DestroyDebugConsole(); }