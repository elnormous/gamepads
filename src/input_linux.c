//
//  gamepads
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "input.h"

typedef struct GPInputLinux
{
    void* data;
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
    	char name[256];

    	snprintf(filename, sizeof(filename), "/dev/input/%s", list[i]->d_name);

    	int fd = open(filename, O_RDONLY);

    	if (fd < 0) continue;

    	ioctl(fd, EVIOCGNAME(sizeof(name)), name);

    	printf("%s, %s\n", filename, name);

    	close(fd);
    }

    return 1;
}

int gpInputDestroy(GPInput* input)
{
    GPInputLinux* inputLinux = (GPInputLinux*)input->opaque;

    free(inputLinux);

    return 1;
}
