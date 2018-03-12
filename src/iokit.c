//
//  gamepads
//

#import <IOKit/hid/IOHIDManager.h>
#include "input.h"

typedef struct InputIOKit
{
    IOHIDManagerRef hidManager;
    CFRunLoopRef runLoop;
} InputIOKit;

typedef struct DeviceIOKit
{
    int isKeyboard;
    int isGamepad;
} DeviceIOKit;

static void deviceInput(void* ctx, IOReturn result, void* sender, IOHIDValueRef value)
{
    IOHIDElementRef elementRef = IOHIDValueGetElement(value);
    IOHIDElementType type = IOHIDElementGetType(elementRef);
    uint32_t usagePage = IOHIDElementGetUsagePage(elementRef);
    uint32_t usage = IOHIDElementGetUsage(elementRef);
    CFIndex newValue = IOHIDValueGetIntegerValue(value);

    if (usagePage == kHIDPage_KeyboardOrKeypad &&
        type == kIOHIDElementTypeInput_Button)
    {
        if (usage == kHIDUsage_KeyboardEscape)
        {
            printf("Escape %s\n", newValue ? "down" : "up");
        }
    }
}

static void deviceAdded(void* ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
{
    int32_t vendorId = 0;
    int32_t productId = 0;
    char name[256] = "";
    DeviceIOKit* deviceIOKit = malloc(sizeof(DeviceIOKit));

    memset(deviceIOKit, 0, sizeof(DeviceIOKit));

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

                deviceIOKit->isKeyboard = (usage == kHIDUsage_GD_Keyboard);
                deviceIOKit->isGamepad = (usage == kHIDUsage_GD_Joystick ||
                                          usage == kHIDUsage_GD_GamePad ||
                                          usage == kHIDUsage_GD_MultiAxisController);
            }
        }

        IOHIDDeviceRegisterInputValueCallback(device, deviceInput, deviceIOKit);

        if (deviceIOKit->isGamepad)
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

static void deviceRemoved(void* ctx, IOReturn result, void* sender, IOHIDDeviceRef device)
{
    fprintf(stdout, "Device removed\n");
}

static CFMutableDictionaryRef createDeviceMatchingDictionary(UInt32 usagePage, UInt32 usage)
{
    CFMutableDictionaryRef dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                                  &kCFTypeDictionaryKeyCallBacks,
                                                                  &kCFTypeDictionaryValueCallBacks );
    if (!dictionary)
        return NULL;

    CFNumberRef usagePageNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usagePage);
    if (!usagePageNumber)
    {
        CFRelease(dictionary);
        return NULL;
    }

    CFDictionarySetValue(dictionary, CFSTR(kIOHIDDeviceUsagePageKey), usagePageNumber);
    CFRelease(usagePageNumber);

    CFNumberRef usageNumber = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage);

    if (!usageNumber)
    {
        CFRelease(dictionary);
        return NULL;
    }

    CFDictionarySetValue(dictionary, CFSTR(kIOHIDDeviceUsageKey), usageNumber);
    CFRelease(usageNumber);

    return dictionary;
}

int gpInputInit(Input* input)
{
    InputIOKit* inputIOKit = malloc(sizeof(InputIOKit));
    input->opaque = inputIOKit;

    inputIOKit->hidManager = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDOptionsTypeNone);
    inputIOKit->runLoop = NULL;

    IOReturn ret = IOHIDManagerOpen(inputIOKit->hidManager, kIOHIDOptionsTypeNone);
    if (ret != kIOReturnSuccess)
    {
        fprintf(stderr, "Failed to initialize manager, error: %d\n", ret);
        return 0;
    }

    IOHIDManagerRegisterDeviceMatchingCallback(inputIOKit->hidManager, deviceAdded, inputIOKit);
    IOHIDManagerRegisterDeviceRemovalCallback(inputIOKit->hidManager, deviceRemoved, inputIOKit);

    CFMutableDictionaryRef keyboard = createDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Keyboard);
    CFMutableDictionaryRef joystick = createDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_Joystick);
    CFMutableDictionaryRef gamepad = createDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_GamePad);
    CFMutableDictionaryRef multiAxisController = createDeviceMatchingDictionary(kHIDPage_GenericDesktop, kHIDUsage_GD_MultiAxisController);

    CFMutableDictionaryRef matchesList[] = {keyboard, joystick, gamepad, multiAxisController};

    CFArrayRef criteria = CFArrayCreate(kCFAllocatorDefault, (const void**)matchesList, 4, NULL);

    IOHIDManagerSetDeviceMatchingMultiple(inputIOKit->hidManager, criteria);

    CFRelease(criteria);

    inputIOKit->runLoop = CFRunLoopGetCurrent();

    IOHIDManagerScheduleWithRunLoop(inputIOKit->hidManager, inputIOKit->runLoop, kCFRunLoopDefaultMode);

    return 1;
}

int gpInputRun(Input* input)
{
    CFRunLoopRun();

    return 1;
}

int gpInputDestroy(Input* input)
{
    if (input->opaque)
    {
        InputIOKit* inputIOKit = (InputIOKit*)input->opaque;

        if (inputIOKit->runLoop) CFRunLoopStop(inputIOKit->runLoop);

        IOHIDManagerClose(inputIOKit->hidManager, kIOHIDOptionsTypeNone);
        CFRelease(inputIOKit->hidManager);

        free(inputIOKit);
    }

    return 1;
}

int gpInputWaitKey(Input* input, uint32_t* usage)
{
    return 0;
}
