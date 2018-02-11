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

int input_init(Input* input);
int input_destroy(Input* input);
int wait_key(Input* input, uint32_t* usage);
