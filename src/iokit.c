//
//  gamepads
//

#import <IOKit/hid/IOHIDManager.h>
#include "iokit.h"
#include "input.h"
#include "thread.h"

typedef struct InputIOKit
{
    IOHIDManagerRef hid_manager;
    Thread thread;
    CFRunLoopRef run_loop;
    Condition start_condition;
    Mutex start_mutex;
} InputIOKit;

static void device_added(void* ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
{
    fprintf(stdout, "Device added\n");
}

static void device_removed(void* ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
{
    fprintf(stdout, "Device removed\n");
}

CFMutableDictionaryRef create_device_matching_dictionary(UInt32 usage_page, UInt32 usage)
{
    CFMutableDictionaryRef dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                                  &kCFTypeDictionaryKeyCallBacks,
                                                                  &kCFTypeDictionaryValueCallBacks );
    if (!dictionary)
        return NULL;

    CFNumberRef page_number = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage_page);
    if (!page_number)
    {
        CFRelease(dictionary);
        return NULL;
    }

    CFDictionarySetValue(dictionary, CFSTR(kIOHIDDeviceUsagePageKey), page_number);
    CFRelease(page_number);

    CFNumberRef usage_number = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);

    if (!usage_number)
    {
        CFRelease(dictionary);
        return NULL;
    }

    CFDictionarySetValue(dictionary, CFSTR(kIOHIDDeviceUsageKey), usage_number);
    CFRelease(usage_number);

    return dictionary;
}

static void* thread_func(void* argument)
{
    InputIOKit* input_io_kit = (InputIOKit*)argument;

    input_io_kit->run_loop = CFRunLoopGetCurrent();

    IOHIDManagerScheduleWithRunLoop(input_io_kit->hid_manager, input_io_kit->run_loop, kCFRunLoopDefaultMode);

    // signal that the thread has started
    mutex_lock(&input_io_kit->start_mutex);
    condition_broadcast(&input_io_kit->start_condition);
    mutex_unlock(&input_io_kit->start_mutex);

    CFRunLoopRun();

    return NULL;
}

int input_init(Input* input)
{
    InputIOKit* input_io_kit = malloc(sizeof(InputIOKit));
    input->opaque = input_io_kit;

    input_io_kit->hid_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    input_io_kit->run_loop = NULL;
    condition_init(&input_io_kit->start_condition);
    mutex_init(&input_io_kit->start_mutex);

    CFMutableDictionaryRef joystick = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
    CFMutableDictionaryRef gamepad = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
    CFMutableDictionaryRef multi_axis_controller = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController);

    CFMutableDictionaryRef matches_list[] = {joystick, gamepad, multi_axis_controller};

    CFArrayRef criteria = CFArrayCreate(kCFAllocatorDefault, (const void**)matches_list, 3, NULL);

    IOHIDManagerSetDeviceMatchingMultiple(input_io_kit->hid_manager, criteria);

    CFRelease(criteria);

    IOReturn ret = IOHIDManagerOpen(input_io_kit->hid_manager, kIOHIDOptionsTypeNone);
    if (ret != kIOReturnSuccess)
    {
        fprintf(stderr, "Failed to initialize manager, error: %d\n", ret);
        return 0;
    }

    IOHIDManagerRegisterDeviceMatchingCallback(input_io_kit->hid_manager, device_added, input_io_kit);
    IOHIDManagerRegisterDeviceRemovalCallback(input_io_kit->hid_manager, device_removed, input_io_kit);

    mutex_lock(&input_io_kit->start_mutex);

    thread_init(&input_io_kit->thread, thread_func, input_io_kit);

    while (!input_io_kit->run_loop)
    {
        condition_wait(&input_io_kit->start_condition, &input_io_kit->start_mutex);
    }

    mutex_unlock(&input_io_kit->start_mutex);

    return 1;
}

int input_destroy(Input* input)
{
    if (input->opaque)
    {
        InputIOKit* input_io_kit = (InputIOKit*)input->opaque;

        if (input_io_kit->run_loop) CFRunLoopStop(input_io_kit->run_loop);

        thread_join(&input_io_kit->thread);

        free(input_io_kit);
    }

    return 1;
}
