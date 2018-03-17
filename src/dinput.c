//
//  gamepads
//

#include <stdio.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "input.h"

typedef struct InputDI
{
    IDirectInput8* directInput;
    HWND window;
} InputDI;

static BOOL CALLBACK enumDevicesCallback(const DIDEVICEINSTANCEW* didInstance, VOID* context)
{
    InputDI* inputDI = (InputDI*)context;
    char* name;
    DWORD vendorId, productId;

    int bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, didInstance->tszProductName, -1, NULL, 0, NULL, NULL);
    name = malloc(bytesNeeded);
    WideCharToMultiByte(CP_UTF8, 0, didInstance->tszProductName, -1, name, bytesNeeded, NULL, NULL);

    vendorId = LOWORD(didInstance->guidProduct.Data1);
    productId = HIWORD(didInstance->guidProduct.Data1);

    fprintf(stdout, "Name: %s, vendor ID: 0x%04X, product ID: 0x%04X\n", name, vendorId, productId);

    IDirectInputDevice8* device;
    HRESULT hr = IDirectInput8_CreateDevice(inputDI->directInput, &didInstance->guidInstance, &device, NULL);

    if (FAILED(hr))
    {
        fprintf(stderr, "Failed to create DirectInput device, error: %d\n", hr);
        return DIENUM_CONTINUE;
    }

    hr = IDirectInputDevice_SetDataFormat(device, &c_dfDIJoystick2);

    if (FAILED(hr))
    {
        fprintf(stderr, "Failed to set DirectInput device format, error: %d\n", hr);
        return DIENUM_CONTINUE;
    }

    return DIENUM_CONTINUE;
}

int gpInputInit(Input* input)
{
    InputDI* inputDI = malloc(sizeof(InputDI));
    input->opaque = inputDI;

    HINSTANCE instance = GetModuleHandleW(NULL);
    HRESULT hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, &IID_IDirectInput8, &inputDI->directInput, NULL);

    if (FAILED(hr))
    {
        fprintf(stderr, "Failed to initialize DirectInput, error: %d\n", hr);
        return 0;
    }

    hr = IDirectInput8_EnumDevices(inputDI->directInput, DI8DEVCLASS_GAMECTRL, enumDevicesCallback, inputDI, DIEDFL_ATTACHEDONLY);

    if (FAILED(hr))
    {
        fprintf(stderr, "Failed to enumerate devices, error: %d\n", hr);
        return 0;
    }

    return 1;
}

int gpInputRun()
{
    MSG msg;
    BOOL ret;
    for (;;)
    {
        ret = GetMessage(&msg, NULL, 0, 0);

        if (ret > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else if (ret < 0)
        {
            // error
            return 0;
        }
        else
        {
            break;
        }
    }

    return 1;
}

int gpInputDestroy(Input* input)
{
    InputDI* inputDI = (InputDI*)input->opaque;

    IDirectInput8_Release(inputDI->directInput);

    free(input->opaque);

    return 1;
}
