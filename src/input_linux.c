//
//  gamepads
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/joystick.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "input.h"

typedef struct GPInputLinux
{
    int maxFd;
} GPInputLinux;

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#define BITS_PER_LONG (8 * sizeof(long))
#define BITS_TO_LONGS(nr) DIV_ROUND_UP(nr, BITS_PER_LONG)

static inline int isBitSet(const unsigned long* array, int bit)
{
    return !!(array[bit / BITS_PER_LONG] & (1LL << (bit % BITS_PER_LONG)));
}

int gpInputInit(GPInput* input)
{
    GPInputLinux* inputLinux = malloc(sizeof(GPInputLinux));
    memset(inputLinux, 0, sizeof(GPInputLinux));
    input->opaque = inputLinux;

    DIR* dir = opendir("/dev/input");

    if (!dir)
    {
        fprintf(stderr, "Failed to open directory\n");
        return 0;
    }

    struct dirent ent;
    struct dirent* p;

    while (readdir_r(dir, &ent, &p) == 0 && p)
    {
        if (strncmp("event", ent.d_name, 5) == 0)
        {
            char filename[64];

            snprintf(filename, sizeof(filename), "/dev/input/%s", ent.d_name);

            int fd = open(filename, O_RDONLY);

            if (fd == -1) continue;

            char name[256];
            ioctl(fd, EVIOCGNAME(sizeof(name)), name);

            struct input_id id;
            ioctl(fd, EVIOCGID, &id);

            printf("filename: %s, name: %s, vendor: %d, product: %d\n", filename, name, id.vendor, id.product);

            close(fd);
        }
    }

    closedir(dir);

    return 1;
}

int gpInputDestroy(GPInput* input)
{
    GPInputLinux* inputLinux = (GPInputLinux*)input->opaque;

    free(inputLinux);

    return 1;
}

int gpInputUpdate(GPInput* input)
{
    return 1;
}
