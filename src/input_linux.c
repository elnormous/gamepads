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

static int isInputDevice(const struct dirent* dir)
{
	return strncmp("event", dir->d_name, 5) == 0;
}

int gpInputInit(GPInput* input)
{
    GPInputLinux* inputLinux = malloc(sizeof(GPInputLinux));
    memset(inputLinux, 0, sizeof(GPInputLinux));
    input->opaque = inputLinux;

    struct dirent** list;
    int count = scandir("/dev/input", &list, isInputDevice, versionsort);

    if (count <= 0) return 0;

    for (int i = 0; i < count; ++i)
    {
    	char filename[64];

    	snprintf(filename, sizeof(filename), "/dev/input/%s", list[i]->d_name);
        free(list[i]);

    	int fd = open(filename, O_RDONLY);

    	if (fd == -1) continue;

        char name[256];
    	ioctl(fd, EVIOCGNAME(sizeof(name)), name);

    	struct input_id id;
    	ioctl(fd, EVIOCGID, &id);

    	printf("filename: %s, name: %s, vendor: %d, product: %d\n", filename, name, id.vendor, id.product);

    	close(fd);
    }

    free(list);

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
