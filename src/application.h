//
//  gamepads
//

#pragma once

#include "window.h"

typedef struct GPApplication
{
    GPWindow window;
} GPApplication;

int gpApplicationInit(GPApplication* application);
int gpApplicationDestroy(GPApplication* application);
int gpApplicationRun(GPApplication* application);
int gpLog(GPApplication* application, const char* string);
