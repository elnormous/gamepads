//
//  gamepads
//

#pragma once

struct Input
{
    void* opaque;
};

struct Gamepad
{
    void* opaque;
};

int input_init(struct Input* input);
int input_destroy(struct Input* input);
