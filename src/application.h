//
//  gamepads
//

#pragma once

#include "window.h"
#include "input.h"

typedef struct GPApplication
{
    GPWindow window;
    GPInput input;
} GPApplication;

int gpApplicationInit(GPApplication* application);
int gpApplicationDestroy(GPApplication* application);
int gpApplicationRun(GPApplication* application);
int gpLog(GPApplication* application, const char* string);
