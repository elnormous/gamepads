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
} InputDI;

static BOOL CALLBACK enumDevicesCallback(const DIDEVICEINSTANCEW* didInstance, VOID* context)
{
    char* name;
    DWORD vendorId, productId;

    int bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, didInstance->tszProductName, -1, NULL, 0, NULL, NULL);
    name = malloc(bytesNeeded);
    WideCharToMultiByte(CP_UTF8, 0, didInstance->tszProductName, -1, name, bytesNeeded, NULL, NULL);

    vendorId = LOWORD(didInstance->guidProduct.Data1);
    productId = HIWORD(didInstance->guidProduct.Data1);

    fprintf(stdout, "Name: %s, vendor ID: 0x%04X, product ID: 0x%04X\n", name, vendorId, productId);

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

int gpInputDestroy(Input* input)
{
    free(input->opaque);

    return 1;
}
