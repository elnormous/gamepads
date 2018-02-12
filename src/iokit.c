//
//  gamepads
//

#import <IOKit/hid/IOHIDManager.h>
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

typedef struct DeviceIOKit
{
    int is_keyboard;
    int is_gamepad;
} DeviceIOKit;

static void device_input(void* ctx, IOReturn result, void* sender, IOHIDValueRef value)
{
    IOHIDElementRef element_ref = IOHIDValueGetElement(value);
    IOHIDElementType type = IOHIDElementGetType(element_ref);
    uint32_t usage_page = IOHIDElementGetUsagePage(element_ref);
    uint32_t usage = IOHIDElementGetUsage(element_ref);
    CFIndex new_value = IOHIDValueGetIntegerValue(value);

    if (usage_page == kHIDPage_KeyboardOrKeypad &&
        type == kIOHIDElementTypeInput_Button)
    {
        if (usage == kHIDUsage_KeyboardEscape)
        {
            printf("Escape %s\n", new_value ? "down" : "up");
        }
    }
}

static void device_added(void* ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
{
    int32_t vendorId = 0;
    int32_t productId = 0;
    char name[256] = "";
    DeviceIOKit* io_kit_device = malloc(sizeof(DeviceIOKit));

    memset(io_kit_device, 0, sizeof(DeviceIOKit));

    CFArrayRef usages = (CFArrayRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDDeviceUsagePairsKey));

    if (usages)
    {
        int32_t usage;
        CFIndex i;

        CFIndex count = CFArrayGetCount(usages);

        for (i = 0; i < count; ++i)
        {
            CFDictionaryRef value = (CFDictionaryRef)CFArrayGetValueAtIndex(usages, i);

            CFNumberRef usageNumber = CFDictionaryGetValue(value, CFSTR(kIOHIDDeviceUsageKey));

            if (usageNumber)
            {
                CFNumberGetValue(usageNumber, kCFNumberSInt32Type, &usage);

                io_kit_device->is_keyboard = (usage == kHIDUsage_GD_Keyboard);
                io_kit_device->is_gamepad = (usage == kHIDUsage_GD_Joystick ||
                                             usage == kHIDUsage_GD_GamePad ||
                                             usage == kHIDUsage_GD_MultiAxisController);
            }
        }

        IOHIDDeviceRegisterInputValueCallback(device, device_input, io_kit_device);

        if (io_kit_device->is_gamepad)
        {
            CFNumberRef vendor = (CFNumberRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey));
            if (vendor)
            {
                CFNumberGetValue(vendor, kCFNumberSInt32Type, &vendorId);
            }

            CFNumberRef product = (CFNumberRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey));
            if (product)
            {
                CFNumberGetValue(product, kCFNumberSInt32Type, &productId);
            }

            CFStringRef productName = (CFStringRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));

            if (productName)
            {
                CFStringGetCString(productName, name, sizeof(name), kCFStringEncodingUTF8);
            }

            fprintf(stdout, "Name: %s, vendor ID: 0x%04X, product ID: 0x%04X\n", name, vendorId, productId);
        }
    }
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

static void thread_func(void* argument)
{
    InputIOKit* input_io_kit = (InputIOKit*)argument;

    input_io_kit->run_loop = CFRunLoopGetCurrent();

    IOHIDManagerScheduleWithRunLoop(input_io_kit->hid_manager, input_io_kit->run_loop, kCFRunLoopDefaultMode);

    // signal that the thread has started
    mutex_lock(&input_io_kit->start_mutex);
    condition_broadcast(&input_io_kit->start_condition);
    mutex_unlock(&input_io_kit->start_mutex);

    CFRunLoopRun();
}

int input_init(Input* input)
{
    InputIOKit* input_io_kit = malloc(sizeof(InputIOKit));
    input->opaque = input_io_kit;

    input_io_kit->hid_manager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    input_io_kit->run_loop = NULL;
    condition_init(&input_io_kit->start_condition);
    mutex_init(&input_io_kit->start_mutex);

    CFMutableDictionaryRef keyboard = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Keyboard);
    CFMutableDictionaryRef joystick = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
    CFMutableDictionaryRef gamepad = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
    CFMutableDictionaryRef multi_axis_controller = create_device_matching_dictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController);

    CFMutableDictionaryRef matches_list[] = {keyboard, joystick, gamepad, multi_axis_controller};

    CFArrayRef criteria = CFArrayCreate(kCFAllocatorDefault, (const void**)matches_list, 4, NULL);

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

    thread_init(&input_io_kit->thread, thread_func, input_io_kit, "Input");

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

int wait_key(Input* input, uint32_t* usage)
{
    return 0;
}
