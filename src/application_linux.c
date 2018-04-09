//
//  gamepads
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "application.h"
#include "window_linux.h"

void gpMain(GPApplication* application);

int gpApplicationInit(GPApplication* application, int argc, const char** argv)
{
    application->argc = argc;
    application->argv = argv;

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
    gpWindowInit(&application->window, application->argc, application->argv);

    gpMain(application);

    int running = 1;
    XEvent event;
    GPWindowLinux* windowLinux = (GPWindowLinux*)application->window.opaque;

    while (running)
    {
        XNextEvent(windowLinux->display, &event);

        switch (event.type)
        {
            case ClientMessage:
                if (event.xclient.message_type == windowLinux->protocolsAtom &&
                    event.xclient.data.l[0] == windowLinux->deleteAtom)
                    running = 0;
                break;
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

    size_t length = strlen(string);
    if (length == 0) return 0;

    windowLinux->textSize += (length + 1); // include \n
    windowLinux->text = realloc(windowLinux->text, windowLinux->textSize);

    strcat(windowLinux->text, string);
    strcat(windowLinux->text, "\n");

    XTextItem textItem;
    textItem.chars = windowLinux->text;
    textItem.nchars = windowLinux->textSize - 1;
    textItem.delta = 0;
    textItem.font = None;

    XDrawText(windowLinux->display, windowLinux->window, windowLinux->gc, 0, 0, &textItem, 1);
    XFlush(windowLinux->display);

    return 1;
}
