#include <stdio.h>

#include <shlwapi.h>

#include "mono_loader.h"

BOOL _LoadCatalystConfig(PCatalystMonoContext context) {
    char asciiPath[MAX_PATH] = { 0 };
    wcstombs(asciiPath, context->catalystConfigPath, sizeof(asciiPath));

    context->catalystConfig = ini_load(asciiPath);
    if (!context->catalystConfig) {
        return FALSE;
    }
    
    return TRUE;
}

BOOL _FileExists(LPWSTR path) {
    return GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES;
}

MonoLoader_Status _InitializeContextPaths(LPWSTR appBaseDirectory, PCatalystMonoContext context) {
    wsprintfW(
        context->catalystConfigPath,
        L"%ls\\catalyst.cfg",
        appBaseDirectory
    );  

    if (!_LoadCatalystConfig(context)) {
        return MONOLOADER_STATUS_CFG_READ_ERROR;
    }

    WCHAR monoDllRelativePath[MAX_PATH] = { 0 };
    ini_wget(
        context->catalystConfig,
        "paths",
        "unity_mono_dll",
        monoDllRelativePath,
        sizeof(monoDllRelativePath)
    );
    
    wsprintfW(
        context->unityMonoDllPath,
        L"%ls\\%s",
        appBaseDirectory,
        monoDllRelativePath
    );

    if (!_FileExists(context->unityMonoDllPath)) {
        return MONOLOADER_STATUS_MONO_DLL_MISSING;
    }

    WCHAR catalystDllRelativePath[MAX_PATH] = { 0 };
    ini_wget(
        context->catalystConfig,
        "paths",
        "catalyst_managed_dll",
        catalystDllRelativePath,
        sizeof(catalystDllRelativePath)
    );
    
    wsprintfW(
        context->catalystDllPath,
        L"%ls\\%s",
        appBaseDirectory,
        catalystDllRelativePath
    );
    
    if (!_FileExists(context->catalystDllPath)) {
        return MONOLOADER_STATUS_CATALYST_DLL_MISSING;
    }
    
    return MONOLOADER_STATUS_OK;
}

MonoLoader_Status MonoLoader_Boot(LPWSTR appBaseDirectory, OUT PCatalystMonoContext* catalystMonoContext) {   
    MonoLoader_Status status = MONOLOADER_STATUS_OK;
    PCatalystMonoContext context = (PCatalystMonoContext)malloc(sizeof(CatalystMonoContext));
    
    if (!context) {
        status = MONOLOADER_STATUS_OUT_OF_MEMORY;
        goto __emotional_damage;
    }
    memset(context, 0, sizeof(catalystMonoContext));
    
    if ((status = _InitializeContextPaths(appBaseDirectory, context)) != MONOLOADER_STATUS_OK) {
        goto __emotional_damage;
    }
    
    context->monoLibraryHandle = LoadLibraryW(context->unityMonoDllPath);
    if (!context->monoLibraryHandle) {
        status = MONOLOADER_STATUS_MONO_DLL_LOAD_ERROR;
        goto __emotional_damage;
    }
    
    context->catalystLoadAfter = ini_get(
        context->catalystConfig, 
        "boot", 
        "load_after"
    );
    
    if (!context->catalystLoadAfter) {
        status = MONOLOADER_STATUS_CFG_LOAD_TARGET_NOT_SET;
        goto __emotional_damage;
    }
    
    *catalystMonoContext = context;
    return status;

__emotional_damage:
    if (context) {
        if (context->catalystConfig) {
            ini_free(context->catalystConfig);
        }
        
        free(context);
    }
    
    *catalystMonoContext = NULL;
    return status;
}