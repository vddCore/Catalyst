#ifndef CATALYST_SHIMS_H
#define CATALYST_SHIMS_H

#include <windows.h>

#include "winhttp.h"

#define SHIM __declspec(dllexport)

#define SHIM_INIT_OR_QUIT(x, y)               \
  x->y = (P ## y)GetProcAddress(hModule, #y); \
  if (!(x->y)) return SHIM_FUNCTION_MISSING   \

#define HOOK_DESTROY(x) \
  if (x) {              \
    free(x);            \
    x = NULL;           \
  }                     \

typedef enum _ShimCreateResult {
    SHIM_OK = 0,
    SHIM_OUT_OF_MEMORY = -1,
    SHIM_FUNCTION_MISSING = -2
} Shim_CreationResult;

Shim_CreationResult Shim_CreateWinHTTP(
    HMODULE hModule,
    OUT PWinHTTPHook * outHookStruct
);

void Shim_DestroyWinHTTP(void);

#endif //CATALYST_SHIMS_H
