//
//  gamepads
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "application.h"
#include "input.h"
#include "thread.h"

void threadFunc(void* argument)
{
    uint32_t usage;
    
    // TODO: print buttons and collect info

    // TODO: save log to file

    // TODO: create Makefile

    // TODO: Visual Studio project

    // TODO: port to Linux

    printf("Press bottom face button (escape if the gamepad doesn't have it)\n");
    printf("Press left face button (escape if the gamepad doesn't have it)\n");
    printf("Press top face butto (escape if the gamepad doesn't have it)n\n");
    printf("Press right face button (escape if the gamepad doesn't have it)\n");
    printf("Press start button (escape if the gamepad doesn't have it)\n");
    printf("Press back button (escape if the gamepad doesn't have it)\n");
    printf("Press left shoulder button (escape if the gamepad doesn't have it)\n");
    printf("Press right shoulder button (escape if the gamepad doesn't have it)\n");
    printf("Press left thumbstick (escape if the gamepad doesn't have it)\n");
    printf("Press right thumbstick (escape if the gamepad doesn't have it)\n");
    printf("Press left trigger (escape if the gamepad doesn't have it)\n");
    printf("Press right trigger (escape if the gamepad doesn't have it)\n");
    printf("Move left thumbstick to the right (escape if the gamepad doesn't have it)\n");
    printf("Move left thumbstick to the up (escape if the gamepad doesn't have it)\n");
    printf("Move right thumbstick to the right (escape if the gamepad doesn't have it)\n");
    printf("Move right thumbstick to the up (escape if the gamepad doesn't have it)\n");
}

#ifdef WIN32
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
#else
int main(int argc, const char* argv[])
#endif
{
    const char* output_file = "output.txt";
    GPInput input;
    int i;
    GPThread thread;
    GPCondition startCondition;
    GPMutex startMutex;
    GPApplication application;
    int result;

#ifdef WIN32
    // TODO: implement
#else
    for (i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
            if (++i < argc) output_file = argv[i];
        }
    }
#endif

    gpInputInit(&input);

    gpThreadInit(&thread, threadFunc, NULL, "Capture");

    gpApplicationInit(&application);
    result = gpApplicationRun(&application) ? EXIT_SUCCESS : EXIT_FAILURE;
    gpApplicationDestroy(&application);

    return result;
}
