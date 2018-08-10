//
//  gamepads
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include "input.h"

typedef struct GPWindow
{
    void* opaque;
} GPWindow;

int gpWindowInit(GPWindow* window);
int gpWindowDestroy(GPWindow* window);

typedef struct GPApplication
{
    GPWindow window;
    GPInput input;
} GPApplication;

int gpApplicationInit(GPApplication* application);
int gpApplicationDestroy(GPApplication* application);
int gpApplicationRun(GPApplication* application);
int gpLog(GPApplication* application, const char* string);

#endif
