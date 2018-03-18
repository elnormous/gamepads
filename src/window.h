//
//  gamepads
//

#pragma once

typedef struct GPWindow
{
    void* opaque
} GPWindow;

int gpWindowInit(GPWindow* window);
int gpWindowDestroy(GPWindow* window);
