//
//  gamepads
//

#include <Windows.h>
#include "application.h"

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
            return 0;
        }
        else
        {
            break;
        }
    }

    return 1;
}