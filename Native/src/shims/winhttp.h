#ifndef CATALYST_SHIMS_WINHTTP_H
#define CATALYST_SHIMS_WINHTTP_H

#include <windows.h>

typedef BOOL(WINAPI* PWinHttpGetIEProxyConfigForCurrentUser)(void *pProxyConfig);

typedef struct _WinHTTPHook {
    PWinHttpGetIEProxyConfigForCurrentUser WinHttpGetIEProxyConfigForCurrentUser;
} WinHTTPHook, *PWinHTTPHook;

#endif //CATALYST_SHIMS_WINHTTP_H
