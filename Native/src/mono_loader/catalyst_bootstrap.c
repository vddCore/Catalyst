#include "catalyst_bootstrap.h"

#include <stdio.h>
#include <windows.h>

#include <ini/ini.h>
#include <mono/mono-min.h>

#define MONOPROC(x) \
    x = (p ## x)GetProcAddress(h, #x)

#define MONOVECTOR(x) \
    static volatile p ## x x

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

void _InitializeMonoVectors(HMODULE h) {
    MONOPROC(mono_get_root_domain);
    MONOPROC(mono_install_assembly_search_hook);
    MONOPROC(mono_install_assembly_load_hook);
    MONOPROC(mono_stringify_assembly_name);
    MONOPROC(mono_assembly_get_image);
    MONOPROC(mono_image_get_name);
    MONOPROC(mono_image_open);
    MONOPROC(mono_image_get_assembly);
    MONOPROC(mono_domain_assembly_open);
    MONOPROC(mono_class_from_name);
    MONOPROC(mono_object_new);
    MONOPROC(mono_runtime_object_init);
}

void _TryLoadCatalystManagedDll(PCatalystMonoContext context) {
    context->catalystMonoAssembly = mono_domain_assembly_open(
        context->catalystMonoDomain,
        context->catalystDllPath
    );
        
    if (context->catalystMonoAssembly) {
        context->catalystMonoImage = mono_assembly_get_image(context->catalystMonoAssembly);
        
        if (context->catalystMonoImage) {
            MonoClass* class = mono_class_from_name(
                context->catalystMonoImage,
                context->catalystNamespace,
                context->catalystManagedClass
            );
            
            if (class) {
                MonoObject* object = mono_object_new(
                    context->catalystMonoDomain,
                    class
                );
                
                mono_runtime_object_init(object);
            }
        }
    }
}

void _OnMonoAssemblyLoad(MonoAssembly* assembly, void* user_data) {
    PCatalystMonoContext context = (PCatalystMonoContext)user_data;
    MonoImage* image = mono_assembly_get_image(assembly);
    
    if (image) {
        const char* imageName = mono_image_get_name(image);
        
        if (!imageName) {
            return;
        }
        
        if (!strcmp(imageName, context->catalystLoadAfter)) {
            _TryLoadCatalystManagedDll(context);
        }
    }
}

CatalystBootstrap_Status Catalyst_Boot(PCatalystMonoContext context) {
    CatalystBootstrap_Status status = CATALYST_BOOTSTRAP_STATUS_OK;

    if (!context) {
        status = CATALYST_BOOTSTRAP_STATUS_INVALID_CONTEXT;
        goto __failure;
    }

    if (!context->monoLibraryHandle) {
        status = CATALYST_BOOTSTRAP_STATUS_NO_MONO_HANDLE;
        goto __failure;
    }

    _InitializeMonoVectors(context->monoLibraryHandle);

    MonoDomain* domain = NULL;
    while (domain == NULL) {
        domain = mono_get_root_domain();
    }
    context->catalystMonoDomain = domain;
    
    // mono_install_assembly_load_hook(_OnMonoAssemblyLoad, context);

    return status;

    __failure:
    return status;
}