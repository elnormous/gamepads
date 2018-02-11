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

    input_init(&input);

    // TODO: print buttons and collect info

    // TODO: save log to file

    // TODO: create Makefile

    // TODO: Visual Studio project

    // TODO: port to Linux


    printf("Press bottom face button\n");
    printf("Press left face button\n");
    printf("Press top face button\n");
    printf("Press right face button\n");
    printf("Press start button\n");
    printf("Press back button\n");
    printf("Press left shoulder button\n");
    printf("Press right shoulder button\n");
    printf("Press left thumbstick\n");
    printf("Press right thumbstick\n");
    printf("Press left trigger\n");
    printf("Press right trigger\n");
    printf("Move left thumbstick to the right\n");
    printf("Move left thumbstick to the up\n");
    printf("Move right thumbstick to the right\n");
    printf("Move right thumbstick to the up\n");

    for (;;)
    {
    }

    return EXIT_SUCCESS;
}
