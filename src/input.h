//
//  gamepads
//

#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

typedef struct GPInput
{
    void* opaque;
} GPInput;

typedef struct GPGamepad
{
    void* opaque;
} GPGamepad;

int gpInputInit(GPInput* input);
int gpInputDestroy(GPInput* input);
int gpInputUpdate(GPInput* input);
int gpInputWaitKey(GPInput* input, uint32_t* usage);

#endif
