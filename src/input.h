//
//  gamepads
//

#pragma once

#include <stdint.h>

typedef struct Input
{
    void* opaque;
} Input;

typedef struct Gamepad
{
    void* opaque;
} Gamepad;

int gpInputInit(Input* input);
int gpInputRun(Input* input);
int gpInputDestroy(Input* input);
int gpInputWaitKey(Input* input, uint32_t* usage);
