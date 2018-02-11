//
//  gamepads
//

#import <IOKit/hid/IOHIDManager.h>
#include "iokit.h"
#include "input.h"

static void device_added(void* ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
{
    fprintf(stdout, "Device added\n");
}

static void device_removed(void *ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
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

int input_init(Input* input)
{
    IOHIDManagerRef hid_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);

    CFMutableDictionaryRef joystick = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
    CFMutableDictionaryRef gamepad = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
    CFMutableDictionaryRef multi_axis_controller = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController);

    CFMutableDictionaryRef matches_list[] = {joystick, gamepad, multi_axis_controller};

    CFArrayRef criteria = CFArrayCreate(kCFAllocatorDefault, (const void**)matches_list, 3, NULL);

    IOHIDManagerSetDeviceMatchingMultiple(hid_manager, criteria);

    CFRelease(criteria);

    IOReturn ret = IOHIDManagerOpen(hid_manager, kIOHIDOptionsTypeNone);
    if (ret != kIOReturnSuccess)
    {
        fprintf(stderr, "Failed to initialize manager, error: %d\n", ret);
        return 0;
    }
    else
    {
        IOHIDManagerRegisterDeviceMatchingCallback(hid_manager, device_added, input);
        IOHIDManagerRegisterDeviceRemovalCallback(hid_manager, device_removed, input);
        IOHIDManagerScheduleWithRunLoop(hid_manager, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    }

    return 1;
}
