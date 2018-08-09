//
//  gamepads
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "application.h"

void gpMain(GPApplication* application);

typedef struct GPWindowLinux
{
    Display* display;
    Window window;
    Atom protocolsAtom;
    Atom deleteAtom;
    GC gc;
    XFontStruct* font;
    char* text;
    size_t textSize;
} GPWindowLinux;

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
    gpInputInit(&application->input);

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

        gpInputUpdate(&application->input);
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


int gpWindowInit(GPWindow* window)
{
    GPWindowLinux* windowLinux = malloc(sizeof(GPWindowLinux));
    memset(windowLinux, 0, sizeof(GPWindowLinux));
    window->opaque = windowLinux;

    windowLinux->display = XOpenDisplay(NULL);

    if (!windowLinux->display)
    {
        fprintf(stderr, "Failed to open display\n");
        return 0;
    }

    Screen* screen = XDefaultScreenOfDisplay(windowLinux->display);
    int screenIndex = XScreenNumberOfScreen(screen);

    unsigned int width = XWidthOfScreen(screen) * 0.6f;
    unsigned int height = XHeightOfScreen(screen) * 0.6f;

    XSetWindowAttributes swa;
    swa.background_pixel = XWhitePixel(windowLinux->display, screenIndex);
    swa.border_pixel = 0;
    swa.event_mask = KeyPressMask;

    windowLinux->window = XCreateWindow(windowLinux->display,
        RootWindow(windowLinux->display, screenIndex),
        0, 0, width, height,
        0, DefaultDepth(windowLinux->display, screenIndex), InputOutput,
        DefaultVisual(windowLinux->display, screenIndex),
        CWBorderPixel | CWBackPixel | CWEventMask, &swa);

    XSetStandardProperties(windowLinux->display, windowLinux->window,
                           "Gamepads", "Gamepads", None, NULL, 0, NULL);

    XMapWindow(windowLinux->display, windowLinux->window);

    windowLinux->protocolsAtom = XInternAtom(windowLinux->display, "WM_PROTOCOLS", False);
    windowLinux->deleteAtom = XInternAtom(windowLinux->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(windowLinux->display, windowLinux->window, &windowLinux->deleteAtom, 1);

    windowLinux->gc = XCreateGC(windowLinux->display, windowLinux->window, 0, 0);
    XSetForeground(windowLinux->display, windowLinux->gc, 0);

    windowLinux->font = XLoadQueryFont(windowLinux->display, "fixed");
    if (!windowLinux->font)
    {
        fprintf(stderr, "Failed to load font\n");
        return 0;
    }

    XSetFont(windowLinux->display, windowLinux->gc, windowLinux->font->fid);

    windowLinux->textSize = 1;
    windowLinux->text = malloc(windowLinux->textSize);
    windowLinux->text[0] = 0;

    return 1;
}

int gpWindowDestroy(GPWindow* window)
{
    if (window->opaque)
    {
        GPWindowLinux* windowLinux = (GPWindowLinux*)window->opaque;

        if (windowLinux->text) free(windowLinux->text);

        if (windowLinux->display)
        {
            if (windowLinux->font) XFreeFont(windowLinux->display, windowLinux->font);
            if (windowLinux->gc) XFreeGC(windowLinux->display, windowLinux->gc);

            if (windowLinux->window)
                XDestroyWindow(windowLinux->display, windowLinux->window);

            XCloseDisplay(windowLinux->display);
        }

        free(windowLinux);
    }

    return 1;
}
