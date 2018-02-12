//
//  gamepads
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input.h"

int main(int argc, const char* argv[])
{
    const char* output_file = "output.txt";
    Input input;
    int i;
    uint32_t usage;

    for (i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
            if (++i < argc) output_file = argv[i];
        }
    }

    gpInputInit(&input);

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

    for (;;)
    {
    }

    return EXIT_SUCCESS;
}
