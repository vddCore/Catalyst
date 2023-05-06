#include <stdio.h>

#include <shlwapi.h>

#include "mono_loader.h"

BOOL _LoadCatalystConfig(PCatalystMonoContext context) {
    context->catalystConfig = ini_load(context->catalystConfigPath);
    if (!context->catalystConfig) {
        VERBOSEV("Failed to load config file %s.\n", context->catalystConfigPath);
        return FALSE;
    }

    return TRUE;
}

BOOL _FileExists(LPCSTR path) {
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

MonoLoader_Status _InitializeContextPaths(LPCSTR appBaseDirectory, PCatalystMonoContext context) {
    sprintf(
        context->catalystConfigPath,
        "%s\\catalyst.cfg",
        appBaseDirectory
    );

    if (!_LoadCatalystConfig(context)) {
        VERBOSE("Configuration read error.");
        return MONOLOADER_STATUS_CFG_READ_ERROR;
    }
    
    sprintf(
        context->unityMonoDllPath,
        "%s\\%s",
        appBaseDirectory,
        ini_get(context->catalystConfig, "paths", "unity_mono_dll")
    );

    if (!_FileExists(context->unityMonoDllPath)) {
        VERBOSEV("Could not find Unity Mono DLL path '%s'", context->unityMonoDllPath);
        return MONOLOADER_STATUS_MONO_DLL_MISSING;
    }

    sprintf(
        context->catalystDllPath,
        "%s\\%s",
        appBaseDirectory,
        ini_get(context->catalystConfig, "paths", "catalyst_managed_dll")
    );

    if (!_FileExists(context->catalystDllPath)) {
        return MONOLOADER_STATUS_CATALYST_DLL_MISSING;
    }

    return MONOLOADER_STATUS_OK;
}

void _InitializeBootSettings(PCatalystMonoContext context) {
    context->catalystLoadAfter = ini_get(
        context->catalystConfig,
        "boot",
        "load_after"
    );

    if (!context->catalystLoadAfter) {
        context->catalystLoadAfter = CATALYST_DEFAULT_LOAD_AFTER;
    }

    context->catalystNamespace = ini_get(
        context->catalystConfig,
        "boot",
        "namespace"
    );

    if (!context->catalystNamespace) {
        context->catalystNamespace = CATALYST_DEFAULT_NAMESPACE;
    }

    context->catalystManagedClass = ini_get(
        context->catalystConfig,
        "boot",
        "class_name"
    );

    if (!context->catalystManagedClass) {
        context->catalystManagedClass = CATALYST_DEFAULT_CLASS;
    }
}

MonoLoader_Status MonoLoader_Boot(LPCSTR appBaseDirectory, OUT PCatalystMonoContext* catalystMonoContext) {
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

    context->monoLibraryHandle = LoadLibraryA(context->unityMonoDllPath);
    if (!context->monoLibraryHandle) {
        status = MONOLOADER_STATUS_MONO_DLL_LOAD_ERROR;
        goto __emotional_damage;
    }

    _InitializeBootSettings(context);

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