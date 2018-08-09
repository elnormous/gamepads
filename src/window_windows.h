//
//  gamepads
//

#ifndef WINDOW_WINDOWS_H
#define WINDOW_WINDOWS_H

#include <Windows.h>
#include "window.h"

typedef struct GPWindowWindows
{
    ATOM windowClass;
    HWND window;
    HWND textBox;
    LPWSTR* text;
    size_t textSize;
} GPWindowWindows;

#endif
