#include <stdlib.h>

#include "shims.h"
#include "winhttp.h"

static volatile PWinHTTPHook _WinHTTP = NULL;

Shim_CreationResult Shim_CreateWinHTTP(HMODULE hModule, OUT PWinHTTPHook* outHookStruct) {
    _WinHTTP = (PWinHTTPHook)malloc(sizeof(WinHTTPHook));

    if (!_WinHTTP) {
        return SHIM_OUT_OF_MEMORY;
    }

    memset(_WinHTTP, 0, sizeof(WinHTTPHook));

    SHIM_INIT_OR_QUIT(_WinHTTP, WinHttpGetIEProxyConfigForCurrentUser);

    if (outHookStruct) {
        *outHookStruct = _WinHTTP;
    }

    return SHIM_OK;
}

void Shim_DestroyWinHTTP(void) {
    HOOK_DESTROY(_WinHTTP);
}

SHIM HRESULT WINAPI WinHttpGetIEProxyConfigForCurrentUser(void* pProxyConfig) {
    return _WinHTTP->WinHttpGetIEProxyConfigForCurrentUser(pProxyConfig);
}