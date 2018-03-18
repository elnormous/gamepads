//
//  gamepads
//

#pragma once

#include "window.h"

typedef struct GPApplication
{
    GPWindow window;
    void* opaque;
} GPApplication;

int gpApplicationInit(GPApplication* application);
int gpApplicationDestroy(GPApplication* application);
int gpApplicationRun(GPApplication* application);
