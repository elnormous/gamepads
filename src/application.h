//
//  gamepads
//

#ifndef APPLICATION_H
#define APPLICATION_H

#include "window.h"
#include "input.h"

typedef struct GPApplication
{
    GPWindow window;
    GPInput input;
    int argc;
    const char** argv;
} GPApplication;

int gpApplicationInit(GPApplication* application, int argc, const char** argv);
int gpApplicationDestroy(GPApplication* application);
int gpApplicationRun(GPApplication* application);
int gpLog(GPApplication* application, const char* string);

#endif
