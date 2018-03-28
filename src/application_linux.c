//
//  gamepads
//

#include <stdio.h>
#include "application.h"
#include "window_linux.h"

void gpMain(GPApplication* application);

int gpApplicationInit(GPApplication* application)
{
    if (!XInitThreads())
    {
        fprintf(stderr, "Failed to initialize thread support\n");
        return 0;
    }

    return 1;
}

int gpApplicationDestroy(GPApplication* application)
{
    return 1;
}

int gpApplicationRun(GPApplication* application)
{
    gpWindowInit(&application->window);

    gpMain(application);

    int running = 1;
    XEvent event;
    GPWindowLinux* windowLinux = (GPWindowLinux*)application->window.opaque;

    while (running)
    {
        XNextEvent(windowLinux->display, &event);

        switch (event.type)
        {
            case KeyPress:
                break;
        }
    }

    gpWindowDestroy(&application->window);

    return 1;
}

int gpLog(GPApplication* application, const char* string)
{
    GPWindowLinux* windowLinux = (GPWindowLinux*)application->window.opaque;

    return 1;
}
