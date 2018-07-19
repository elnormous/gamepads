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
    GPApplication* application = (GPApplication*)argument;
    uint32_t usage;
    
    // TODO: print buttons and collect info

    // TODO: save log to file

    // TODO: create Makefile

    // TODO: Visual Studio project

    // TODO: port to Linux

    gpLog(application, "Press bottom face button (escape if the gamepad doesn't have it)");
    gpLog(application, "Press left face button (escape if the gamepad doesn't have it)");
    gpLog(application, "Press top face butto (escape if the gamepad doesn't have it)n");
    gpLog(application, "Press right face button (escape if the gamepad doesn't have it)");
    gpLog(application, "Press start button (escape if the gamepad doesn't have it)");
    gpLog(application, "Press back button (escape if the gamepad doesn't have it)");
    gpLog(application, "Press left shoulder button (escape if the gamepad doesn't have it)");
    gpLog(application, "Press right shoulder button (escape if the gamepad doesn't have it)");
    gpLog(application, "Press left thumbstick (escape if the gamepad doesn't have it)");
    gpLog(application, "Press right thumbstick (escape if the gamepad doesn't have it)");
    gpLog(application, "Press left trigger (escape if the gamepad doesn't have it)");
    gpLog(application, "Press right trigger (escape if the gamepad doesn't have it)");
    gpLog(application, "Move left thumbstick to the right (escape if the gamepad doesn't have it)");
    gpLog(application, "Move left thumbstick to the up (escape if the gamepad doesn't have it)");
    gpLog(application, "Move right thumbstick to the right (escape if the gamepad doesn't have it)");
    gpLog(application, "Move right thumbstick to the up (escape if the gamepad doesn't have it)");
}

void gpMain(GPApplication* application)
{
    GPThread thread;
    GPCondition startCondition;
    GPMutex startMutex;

    gpThreadInit(&thread, threadFunc, application, "Capture");
}

#ifdef WIN32
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
#else
int main(int argc, const char* argv[])
#endif
{
    GPApplication application;
    int i;
    int result;
    const char* outputFile = "output.txt";

#ifdef WIN32
    // TODO: implement command line parsing
    int argc = 0;
    const char** argv = NULL;
#else
    for (i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
            if (++i < argc) outputFile = argv[i];
        }
    }
#endif

    gpApplicationInit(&application);
    result = gpApplicationRun(&application) ? EXIT_SUCCESS : EXIT_FAILURE;
    gpApplicationDestroy(&application);

    return result;
}
