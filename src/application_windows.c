//
//  gamepads
//

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <Strsafe.h>
#include "application.h"

void gpMain(GPApplication* application);

typedef struct GPWindowWindows
{
    ATOM windowClass;
    HWND window;
    HWND textBox;
    LPWSTR* text;
    size_t textSize;
} GPWindowWindows;

int gpApplicationInit(GPApplication* application)
{
    return 1;
}

int gpApplicationDestroy(GPApplication* application)
{
    return 1;
}

int gpApplicationRun(GPApplication* application)
{
    if (!gpWindowInit(&application->window))
        return 0;

    if (!gpInputInit(&application->input))
        return 0;

    gpMain(application);

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
            gpWindowDestroy(&application->window);
            return 0;
        }
        else
            break;

        gpInputUpdate(&application->input);
    }

    gpWindowDestroy(&application->window);

    return 1;
}

int gpLog(GPApplication* application, const char* string)
{
    GPWindowWindows* windowWindows = (GPWindowWindows*)application->window.opaque;

    WCHAR wString[256];
    int length = MultiByteToWideChar(CP_UTF8, 0, string, -1, wString, 256);
    if (length == 0) return 0;

    windowWindows->textSize += (length + 2 - 1); // include \r\n, exclude the terminating null char
    windowWindows->text = realloc(windowWindows->text, windowWindows->textSize * sizeof(WCHAR));

    StringCchCatW(windowWindows->text, windowWindows->textSize, wString);
    StringCchCatW(windowWindows->text, windowWindows->textSize, L"\r\n");

    if (!SendMessageTimeoutW(windowWindows->textBox, WM_SETTEXT, 0, (LPARAM)windowWindows->text, SMTO_NORMAL, 5000, NULL)) return 0;

    return 1;
}

static LRESULT CALLBACK windowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    GPWindowWindows* windowWindows = (GPWindowWindows*)GetWindowLongPtr(window, GWLP_USERDATA);
    if (!windowWindows) return DefWindowProcW(window, msg, wParam, lParam);

    switch (msg)
    {
        case WM_SIZE:
        {
            RECT clientRect;
            GetClientRect(window, &clientRect);

            SetWindowPos(windowWindows->textBox, NULL,
                clientRect.left, clientRect.top,
                clientRect.right - clientRect.left,
                clientRect.bottom - clientRect.top,
                0);

            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
    }

    return DefWindowProcW(window, msg, wParam, lParam); 
}

static const LPCWSTR WINDOW_CLASS_NAME = L"gamepads";

int gpWindowInit(GPWindow* window)
{
    GPWindowWindows* windowWindows = malloc(sizeof(GPWindowWindows));
    memset(windowWindows, 0, sizeof(GPWindowWindows));
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

    RECT clientRect;
    GetClientRect(windowWindows->window, &clientRect);

    windowWindows->textBox = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
        clientRect.left, clientRect.top,
        clientRect.right - clientRect.left,
        clientRect.bottom - clientRect.top,
        windowWindows->window, NULL, instance, NULL);

    if (!windowWindows->textBox)
    {
        fprintf(stderr, "Failed to create text box\n");
        return 0;
    }

    ShowWindow(windowWindows->window, SW_SHOW);
    SetWindowLongPtr(windowWindows->window, GWLP_USERDATA, (LONG_PTR)windowWindows);

    windowWindows->textSize = 1;
    windowWindows->text = malloc(windowWindows->textSize * sizeof(WCHAR));
    windowWindows->text[0] = 0;

    return 1;
}

int gpWindowDestroy(GPWindow* window)
{
    if (window->opaque)
    {
        GPWindowWindows* windowWindows = (GPWindowWindows*)window->opaque;

        if (windowWindows->text) free(windowWindows->text);
        if (windowWindows->textBox) DestroyWindow(windowWindows->textBox);
        if (windowWindows->window) DestroyWindow(windowWindows->window);
        if (windowWindows->windowClass) UnregisterClassW(WINDOW_CLASS_NAME, GetModuleHandleW(NULL));

        free(windowWindows);
    }

    return 1;
}
