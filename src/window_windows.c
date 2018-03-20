//
//  gamepads
//

#include <Windows.h>
#include "window.h"
#include "application.h"

typedef struct GPWindowWindows
{
    HWND window;
} GPWindowWindows;

int gpWindowInit(GPWindow* window)
{
    GPWindowWindows* windowWindows = malloc(sizeof(GPWindowWindows));
    window->opaque = windowWindows;

    return 1;
}

int gpWindowDestroy(GPWindow* window)
{
    if (window->opaque)
    {
        GPWindowWindows* windowWindows = (GPWindowWindows*)window->opaque;

        free(windowWindows);
    }

    return 1;
}

int gpLog(GPApplication* application, const char* string)
{
    return 1;
}