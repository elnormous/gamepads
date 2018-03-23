//
//  gamepads
//

#include <stdio.h>
#include <Windows.h>
#include "window.h"
#include "application.h"

typedef struct GPWindowWindows
{
    ATOM windowClass;
    HWND window;
} GPWindowWindows;

static LRESULT CALLBACK windowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(window, msg, wParam, lParam);   
}

static const LPCWSTR WINDOW_CLASS_NAME = L"gamepads";

int gpWindowInit(GPWindow* window)
{
    GPWindowWindows* windowWindows = malloc(sizeof(GPWindowWindows));
    window->opaque = windowWindows;

    HINSTANCE instance = GetModuleHandleW(NULL);

    WNDCLASSEXW wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = windowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = instance;
    // Application icon should be the first resource
    //wc.hIcon = LoadIconW(instance, MAKEINTRESOURCEW(101));
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(COLOR_WINDOW);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm = NULL;

    windowWindows->windowClass = RegisterClassExW(&wc);
    if (!windowWindows->windowClass)
    {
        fprintf(stderr, "Failed to register window class\n");
        return 0;
    }

    DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_BORDER | WS_DLGFRAME | WS_THICKFRAME | WS_GROUP | WS_TABSTOP | WS_SIZEBOX | WS_MAXIMIZEBOX;

    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;

    DWORD windowExStyle = WS_EX_APPWINDOW;

    int width = CW_USEDEFAULT;
    int height = CW_USEDEFAULT;

    windowWindows->window = CreateWindowExW(windowExStyle, WINDOW_CLASS_NAME, L"Gamepads", windowStyle,
                                            x, y, width, height, NULL, NULL, instance, NULL);

    if (!windowWindows->window)
    {
        fprintf(stderr, "Failed to create window\n");
        return 0;
    }

    ShowWindow(windowWindows->window, SW_SHOW);
    SetWindowLongPtr(windowWindows->window, GWLP_USERDATA, (LONG_PTR)windowWindows);

    return 1;
}

int gpWindowDestroy(GPWindow* window)
{
    if (window->opaque)
    {
        GPWindowWindows* windowWindows = (GPWindowWindows*)window->opaque;

        if (windowWindows->window) DestroyWindow(windowWindows->window);
        if (windowWindows->windowClass) UnregisterClassW(WINDOW_CLASS_NAME, GetModuleHandleW(NULL));

        free(windowWindows);
    }

    return 1;
}

int gpLog(GPApplication* application, const char* string)
{
    return 1;
}