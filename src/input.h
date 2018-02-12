//
//  gamepads
//

#pragma once

typedef struct Input
{
    void* opaque;
} Input;

typedef struct Gamepad
{
    void* opaque;
} Gamepad;

int gpInputInit(Input* input);
int gpInputDestroy(Input* input);
int gpInputWaitKey(Input* input, uint32_t* usage);
