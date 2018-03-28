//
//  gamepads
//

#include "application.h"
#include "window.h"

void gpMain(GPApplication* application);

int gpApplicationInit(GPApplication* application)
{
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

    for (;;)
    {

    }

    gpWindowDestroy(&application->window);

    return 1;
}
