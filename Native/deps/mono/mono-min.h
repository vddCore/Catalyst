#ifndef CATALYST_MONO_MIN_H
#define CATALYST_MONO_MIN_H

#include <stdint.h>

#define MONOTYPEDEF(x) \
    typedef struct _ ## x x

MONOTYPEDEF(MonoJitInfo);
MONOTYPEDEF(MonoDomain);
MONOTYPEDEF(MonoAppContext);
MONOTYPEDEF(MonoAppDomain);
MONOTYPEDEF(MonoImage);
MONOTYPEDEF(MonoAssembly);
MONOTYPEDEF(MonoAssemblyName);
MONOTYPEDEF(MonoReflectionAssembly);
MONOTYPEDEF(MonoType);
MONOTYPEDEF(MonoObject);
MONOTYPEDEF(MonoClass);
MONOTYPEDEF(MonoMethod);
MONOTYPEDEF(MonoMethodDesc);
MONOTYPEDEF(MonoArray);

MONOTYPEDEF(MonoReflectionModule);
MONOTYPEDEF(MonoReflectionType);
MONOTYPEDEF(MonoReflectionMethod);
MONOTYPEDEF(MonoReflectionField);
MONOTYPEDEF(MonoReflectionProperty);
MONOTYPEDEF(MonoReflectionEvent);

typedef enum {
    MONO_IMAGE_OK,
    MONO_IMAGE_ERROR_ERRNO,
    MONO_IMAGE_MISSING_ASSEMBLYREF,
    MONO_IMAGE_IMAGE_INVALID
} MonoImageOpenStatus;

typedef void (*MonoDomainFunc)(MonoDomain* domain, void* user_data);
typedef void (*MonoAssemblyLoadFunc)(MonoAssembly* assembly, void* user_data);
typedef MonoAssembly* (*MonoAssemblySearchFunc)(MonoAssemblyName* aname, void* user_data);

typedef MonoDomain* (*pmono_get_root_domain)(void);
typedef void (*pmono_install_assembly_search_hook)(MonoAssemblySearchFunc func, void* user_data);
typedef void (*pmono_install_assembly_load_hook)(MonoAssemblyLoadFunc func, void* user_data);
typedef char* (*pmono_stringify_assembly_name)(MonoAssemblyName* aname);
typedef MonoImage* (*pmono_assembly_get_image)(MonoAssembly* assembly);
typedef const char* (*pmono_image_get_name)(MonoImage* image);
typedef MonoImage* (*pmono_image_open)(const char* fname, MonoImageOpenStatus* status);
typedef MonoAssembly* (*pmono_image_get_assembly)(MonoImage* image);
typedef MonoAssembly* (*pmono_domain_assembly_open)(MonoDomain* domain, const char* name);
typedef MonoClass* (*pmono_class_from_name)(MonoImage* image, const char* name_space, const char* name);
typedef MonoObject* (*pmono_object_new)(MonoDomain* domain, MonoClass* klass);
typedef void (*pmono_runtime_object_init)(MonoObject* this_obj);
typedef uint32_t (*pmono_gchandle_new)(MonoObject* obj, int pinned);

#endif //CATALYST_MONO_MIN_H