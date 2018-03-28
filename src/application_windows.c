//
//  gamepads
//

#include <Strsafe.h>
#include "application.h"
#include "window_windows.h"

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

int gpLog(GPApplication* application, const char* string)
{
    GPWindowWindows* windowWindows = (GPWindowWindows*)application->window.opaque;

    WCHAR wString[256];
    int length = MultiByteToWideChar(CP_UTF8, 0, string, -1, wString, 256);
    if (length == 0) return 0;

    windowWindows->textSize += (length + 2 - 1); // include \r\n, exclude the terminating null char
    windowWindows->text = realloc(windowWindows->text, windowWindows->textSize * sizeof(WCHAR));

    StringCchCatW(windowWindows->text, windowWindows->textSize, wString);
    StringCchCatW(windowWindows->text, windowWindows->textSize, L"\r\n");

    if (!SendMessageTimeoutW(windowWindows->textBox, WM_SETTEXT, 0, (LPARAM)windowWindows->text, SMTO_NORMAL, 5000, NULL)) return 0;

    return 1;
}
