//
//  gamepads
//

#pragma once

typedef struct GPApplication
{
    void* opaque;
} GPApplication;

int gpApplicationInit(GPApplication* application);
int gpApplicationDestroy(GPApplication* application);
int gpApplicationRun(GPApplication* application);
