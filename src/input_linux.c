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
    void* data;
} GPInputLinux;

static int isInputDevice(const struct dirent* dir)
{
	return strncmp("js", dir->d_name, 2) == 0;
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

    	int fd = open(filename, O_RDONLY);

    	if (fd < 0) continue;

        char name[256];
    	ioctl(fd, JSIOCGNAME(sizeof(name)), name);

        uint8_t numAxis   = 0;
        ioctl(fd, JSIOCGAXES, &numAxis);

        uint8_t numButton = 0;
        ioctl(fd, JSIOCGBUTTONS, &numButton);

    	printf("filename: %s, name: %s, axis: %d, buttons: %d\n", filename, name, numAxis, numButton);

        uint16_t* buttonMap[KEY_MAX - BTN_MISC + 1];
        ioctl(fd, JSIOCGBTNMAP, buttonMap);

        uint8_t* axisMap[ABS_MAX + 1];
        ioctl(fd, JSIOCGAXMAP, axisMap);

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
