#ifndef CATALYST_MONO_LOADER_H
#define CATALYST_MONO_LOADER_H

#include <windows.h>

#include <ini/ini.h>
#include <stdint.h>

#define CATALYST_DEFAULT_NAMESPACE "Catalyst"
#define CATALYST_DEFAULT_CLASS "Catalyst"
#define CATALYST_DEFAULT_LOAD_AFTER "Assembly-CSharp"

typedef enum _MonoLoader_Status {
  MONOLOADER_STATUS_OK = 0,
  MONOLOADER_STATUS_OUT_OF_MEMORY = 1,
  MONOLOADER_STATUS_CFG_READ_ERROR = 2,
  MONOLOADER_STATUS_MONO_DLL_MISSING = 3,
  MONOLOADER_STATUS_CATALYST_DLL_MISSING = 4,
  MONOLOADER_STATUS_MONO_DLL_LOAD_ERROR = 5
} MonoLoader_Status;

typedef struct _CatalystMonoContext {
    CHAR catalystConfigPath[MAX_PATH];
    ini_t* catalystConfig;
    const char* catalystLoadAfter;
    
    CHAR unityMonoDllPath[MAX_PATH];
    CHAR catalystDllPath[MAX_PATH];
    
    HMODULE monoLibraryHandle;
    
    void* catalystMonoDomain;
    void* catalystMonoImage;
    void* catalystMonoAssembly;
    uint32_t catalystObjectGcHandle;
    
    const char* catalystNamespace;
    const char* catalystManagedClassName;
    
} CatalystMonoContext, *PCatalystMonoContext;

MonoLoader_Status MonoLoader_Boot(
    LPCSTR baseAppDirectory, 
    PCatalystMonoContext* catalystMonoContext
);

#endif //CATALYST_MONO_LOADER_H
