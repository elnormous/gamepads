//
//  gamepads
//

#include <Windows.h>
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

    MSG msg;
    BOOL ret;
    for (;;)
    {
        ret = GetMessage(&msg, NULL, 0, 0);

        if (ret > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (ret < 0)
        {
            // error
            gpWindowDestroy(&application->window);
            return 0;
        }
        else
        {
            break;
        }
    }

    gpWindowDestroy(&application->window);

    return 1;
}