//
//  gamepads
//

#include <stdlib.h>
#include <string.h>
#include "window.h"
#include "application.h"

typedef struct GPWindowLinux
{
    void* data;
} GPWindowLinux;

int gpWindowInit(GPWindow* window)
{
    GPWindowLinux* windowLinux = malloc(sizeof(GPWindowLinux));
    memset(windowLinux, 0, sizeof(GPWindowLinux));
    window->opaque = windowLinux;

    return 1;
}

int gpWindowDestroy(GPWindow* window)
{
    if (window->opaque)
    {
        GPWindowLinux* windowLinux = (GPWindowLinux*)window->opaque;

        free(windowLinux);
    }

    return 1;
}

int gpLog(GPApplication* application, const char* string)
{
    GPWindowLinux* windowLinux = (GPWindowLinux*)application->window.opaque;

    return 1;
}
