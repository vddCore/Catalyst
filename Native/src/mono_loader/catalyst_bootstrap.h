﻿#ifndef CATALYST_MONO_LOADER_CATALYST_BOOTSTRAP_H
#define CATALYST_MONO_LOADER_CATALYST_BOOTSTRAP_H

#include "mono_loader.h"

typedef enum _CatalystBootstrap_Status {
    CATALYST_BOOTSTRAP_STATUS_OK = 0,
    CATALYST_BOOTSTRAP_STATUS_INVALID_CONTEXT = 1,
    CATALYST_BOOTSTRAP_STATUS_NO_MONO_HANDLE = 2,
} CatalystBootstrap_Status;

CatalystBootstrap_Status Catalyst_Boot(PCatalystMonoContext context);

#endif //CATALYST_MONO_LOADER_CATALYST_BOOTSTRAP_H
