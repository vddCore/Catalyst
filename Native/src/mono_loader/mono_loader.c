#include <stdio.h>

#include <shlwapi.h>

#include "../util.h"
#include "mono_loader.h"

BOOL _LoadCatalystConfig(PCatalystMonoContext context) {
    context->catalystConfig = ini_load(context->catalystConfigPath);
    if (!context->catalystConfig) {
        LOGV("Failed to load config file '%s'.\n", context->catalystConfigPath);
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
        LOG("Configuration read error.\n");
        return MONOLOADER_STATUS_CFG_READ_ERROR;
    }
    
    sprintf(
        context->unityMonoDllPath,
        "%s\\%s",
        appBaseDirectory,
        ini_get(context->catalystConfig, "paths", "unity_mono_dll")
    );

    if (!_FileExists(context->unityMonoDllPath)) {
        LOGV("Could not find Unity Mono DLL path '%s'.\n", context->unityMonoDllPath);
        return MONOLOADER_STATUS_MONO_DLL_MISSING;
    }
    LOGV("paths.unity_mono_dll = '%s'\n", context->unityMonoDllPath);

    sprintf(
        context->catalystDllPath,
        "%s\\%s",
        appBaseDirectory,
        ini_get(context->catalystConfig, "paths", "catalyst_managed_dll")
    );

    if (!_FileExists(context->catalystDllPath)) {
        LOGV("Could not find Catalyst Managed Layer DLL path: '%s'.\n", context->catalystDllPath);
        return MONOLOADER_STATUS_CATALYST_DLL_MISSING;
    }
    LOGV("paths.catalyst_managed_dll = '%s'\n", context->catalystDllPath);

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
    LOGV("boot.load_after = '%s'\n", context->catalystLoadAfter);

    context->catalystNamespace = ini_get(
        context->catalystConfig,
        "boot",
        "namespace"
    );

    if (!context->catalystNamespace) {
        context->catalystNamespace = CATALYST_DEFAULT_NAMESPACE;
    }
    LOGV("boot.namespace = '%s'\n", context->catalystNamespace);

    context->catalystManagedClassName = ini_get(
        context->catalystConfig,
        "boot",
        "class_name"
    );

    if (!context->catalystManagedClassName) {
        context->catalystManagedClassName = CATALYST_DEFAULT_CLASS;
    }
    LOGV("boot.class_name = '%s'\n", context->catalystManagedClassName);
    
    context->catalystCtorFullyQualifiedName = ini_get(
        context->catalystConfig,
        "boot",
        "ctor_desc"
    );
    
    if (!context->catalystCtorFullyQualifiedName) {
        context->catalystCtorFullyQualifiedName = CATALYST_DEFFAULT_CTOR;
    }
    LOGV("boot.catalyst_ctor = '%s'\n", context->catalystCtorFullyQualifiedName);
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
        LOGV(
            "Attempt to load '%s' failed with system error: 0x%lX\n", 
            context->unityMonoDllPath,
            GetLastError()
        );
        
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