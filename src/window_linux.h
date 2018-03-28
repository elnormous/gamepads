//
//  gamepads
//

#pragma once

#include <X11/Xlib.h>
#include "window.h"

typedef struct GPWindowLinux
{
    Display* display;
    Window window;
} GPWindowLinux;
