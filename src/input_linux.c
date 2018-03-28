//
//  gamepads
//

#include <stdlib.h>
#include <string.h>
#include "input.h"

typedef struct GPInputLinux
{
    void* data;
} GPInputLinux;

int gpInputInit(GPInput* input)
{
    GPInputLinux* inputLinux = malloc(sizeof(GPInputLinux));
    memset(inputLinux, 0, sizeof(GPInputLinux));
    input->opaque = inputLinux;

    return 1;
}

int gpInputDestroy(GPInput* input)
{
    GPInputLinux* inputLinux = (GPInputLinux*)input->opaque;

    free(inputLinux);

    return 1;
}
