#include "catalyst_bootstrap.h"
#include "../util.h"

#include <stdio.h>
#include <windows.h>

#include <mono/mono-min.h>

#define MONOVECTOR(x) static volatile p ## x x

#define MONOPROC(x) \
    x = (p ## x)GetProcAddress(h, #x);                      \
    if (!x) {                                               \
        LOGV("Mono call vector '%s' not found.\n", #x); \
        return FALSE;                                       \
    }

MONOVECTOR(mono_get_root_domain);
MONOVECTOR(mono_install_assembly_search_hook);
MONOVECTOR(mono_install_assembly_load_hook);
MONOVECTOR(mono_stringify_assembly_name);
MONOVECTOR(mono_assembly_get_image);
MONOVECTOR(mono_image_get_name);
MONOVECTOR(mono_image_open);
MONOVECTOR(mono_image_get_assembly);
MONOVECTOR(mono_domain_assembly_open);
MONOVECTOR(mono_class_from_name);
MONOVECTOR(mono_object_new);
MONOVECTOR(mono_runtime_object_init);
MONOVECTOR(mono_gchandle_new);

BOOL _InitializeMonoVectors(HMODULE h) {
    MONOPROC(mono_get_root_domain)
    MONOPROC(mono_install_assembly_search_hook)
    MONOPROC(mono_install_assembly_load_hook)
    MONOPROC(mono_stringify_assembly_name)
    MONOPROC(mono_assembly_get_image)
    MONOPROC(mono_image_get_name)
    MONOPROC(mono_image_open)
    MONOPROC(mono_image_get_assembly)
    MONOPROC(mono_domain_assembly_open)
    MONOPROC(mono_class_from_name)
    MONOPROC(mono_object_new)
    MONOPROC(mono_runtime_object_init)
    MONOPROC(mono_gchandle_new)

    return TRUE;
}

BOOL _TryLoadCatalystManagedDll(PCatalystMonoContext context) {
    if (!context) {
        LOG("Invalid Mono context provided.\n");
        return FALSE;
    }

    context->catalystMonoAssembly = mono_domain_assembly_open(
        context->catalystMonoDomain,
        context->catalystDllPath
    );

    if (!context->catalystMonoAssembly) {
        LOGV("Failed to load assembly '%s'.\n",
             context->catalystDllPath
        );

        return FALSE;
    }

    context->catalystMonoImage = mono_assembly_get_image(context->catalystMonoAssembly);

    if (!context->catalystMonoImage) {
        LOG("Failed to get assembly image.\n");
        return FALSE;
    }

    MonoClass* class = mono_class_from_name(
        context->catalystMonoImage,
        context->catalystNamespace,
        context->catalystManagedClassName
    );

    if (!class) {
        LOGV("Class '%s.%s' was not found in assembly '%s'.\n",
             context->catalystNamespace,
             context->catalystManagedClassName,
             context->catalystDllPath
        );

        return FALSE;
    }

    MonoObject* object = mono_object_new(
        context->catalystMonoDomain,
        class
    );

    if (!object) {
        LOGV("Failed to create an instance of '%s.%s'.\n",
             context->catalystNamespace,
             context->catalystManagedClassName
        );

        return FALSE;
    }

    context->catalystObjectGcHandle = mono_gchandle_new(object, TRUE);
    LOGV("Pinned '%s.%s' instance at GC handle 0x%08X.\n",
         context->catalystNamespace,
         context->catalystManagedClassName,
         context->catalystObjectGcHandle
    );
    
    mono_runtime_object_init(object);
    return TRUE;
}

void _OnMonoAssemblyLoad(MonoAssembly* assembly, void* user_data) {
    PCatalystMonoContext context = (PCatalystMonoContext)user_data;
    MonoImage* image = mono_assembly_get_image(assembly);

    if (image) {
        const char* imageName = mono_image_get_name(image);

        if (!imageName) {
            return;
        }

        LOGV(".NET assembly loaded: '%s'.\n", imageName);
        if (!strcmp(imageName, context->catalystLoadAfter)) {
            LOG("==> Load-after target found. Loading and initializing managed layer.\n");
            _TryLoadCatalystManagedDll(context);
        }
    }
}

CatalystBootstrap_Status Catalyst_Boot(PCatalystMonoContext context) {
    CatalystBootstrap_Status status = CATALYST_BOOTSTRAP_STATUS_OK;

    if (!context) {
        LOG("context == NULL\n");

        status = CATALYST_BOOTSTRAP_STATUS_INVALID_CONTEXT;
        goto __failure;
    }

    if (!context->monoLibraryHandle) {
        LOG("context->monoLibraryHandle == NULL\n");

        status = CATALYST_BOOTSTRAP_STATUS_NO_MONO_HANDLE;
        goto __failure;
    }

    if (!_InitializeMonoVectors(context->monoLibraryHandle)) {
        status = CATALYST_BOOTSTRAP_VECTOR_INITIALIZATION_FAILED;
        goto __failure;
    }

    LOG("Waiting for Unity to create root domain...\n");
    MonoDomain* domain = NULL;
    while (domain == NULL) {
        domain = mono_get_root_domain();
    }
    context->catalystMonoDomain = domain;
    LOG("Unity root domain found, moving on.\n");

    LOG("Installing assembly load hook.\n");
    mono_install_assembly_load_hook(_OnMonoAssemblyLoad, context);

    return status;

    __failure:
    return status;
}