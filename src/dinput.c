//
//  gamepads
//

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "input.h"

typedef struct InputDI
{
    IDirectInput8W* directInput;
} InputDI;

int gpInputInit(Input* input)
{
    InputDI* inputDI = malloc(sizeof(InputDI));
    input->opaque = inputDI;

    return 1;
}

int gpInputDestroy(Input* input)
{
    free(input->opaque);

    return 1;
}
