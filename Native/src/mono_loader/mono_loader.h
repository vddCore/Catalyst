#ifndef CATALYST_MONO_LOADER_H
#define CATALYST_MONO_LOADER_H

#include <windows.h>

#include <ini/ini.h>

typedef enum _MonoLoader_Status {
  MONOLOADER_STATUS_OK = 0,
  MONOLOADER_STATUS_OUT_OF_MEMORY = 1,
  MONOLOADER_STATUS_CFG_READ_ERROR = 2,
  MONOLOADER_STATUS_CFG_LOAD_TARGET_NOT_SET = 3,
  MONOLOADER_STATUS_MONO_DLL_MISSING = 4,
  MONOLOADER_STATUS_CATALYST_DLL_MISSING = 5,
  MONOLOADER_STATUS_MONO_DLL_LOAD_ERROR = 6
} MonoLoader_Status;

typedef struct _CatalystMonoContext {
    WCHAR catalystConfigPath[MAX_PATH];
    ini_t* catalystConfig;
    const char* catalystLoadAfter;
    
    WCHAR unityMonoDllPath[MAX_PATH];
    WCHAR catalystDllPath[MAX_PATH];
    
    HMODULE monoLibraryHandle;
    
    void* catalystMonoDomain;
    void* catalystMonoImage;
    void* catalystMonoAssembly;
} CatalystMonoContext, *PCatalystMonoContext;

MonoLoader_Status MonoLoader_Boot(
    LPWSTR baseAppDirectory, 
    PCatalystMonoContext* catalystMonoContext
);

#endif //CATALYST_MONO_LOADER_H
