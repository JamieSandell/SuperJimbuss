#include <stdbool.h>
#include <windows.h>

#define internal static
#define local_persist static
#define global static

global BITMAPINFO bitmap_info;
global void *bitmap_memory;
global int bitmap_height;
global int bitmap_width;
global const int bytes_per_pixel = 4;
global bool running;

LRESULT CALLBACK
win32_main_windows_callback(
    HWND window,
    UINT message,
    WPARAM wparam,
    LPARAM lparam
);

internal void
win32_resize_dib_section(
    int width,
    int height
);

internal void
win32_update_window(
    HDC device_context,
    RECT *client_rect
);

int WINAPI
WinMain(
    HINSTANCE instance,
    HINSTANCE prev_instance,
    PSTR command,
    int cmd_show
)
{
    const char class_name[] = "SuperJimbuss";
    WNDCLASS window_class = {0};
    window_class.lpfnWndProc = win32_main_windows_callback;
    window_class.hInstance = instance;
    window_class.lpszClassName = class_name;
    RegisterClassA(&window_class);

    HWND window = CreateWindowExA(
        0,
        class_name,
        "Super Jimbuss",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        instance,
        NULL
    );

    if (window == NULL)
    {
        // TODO: Logging
        return 0;
    }

    ShowWindow(
        window,
        cmd_show
    );

    MSG message = {0};
    running = true;

    while (running)
    {
        GetMessageA(
            &message,
            NULL,
            0,
            0
        );

        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return 0;
}

LRESULT CALLBACK win32_main_windows_callback(
    HWND window,
    UINT message,
    WPARAM wparam,
    LPARAM lparam
)
{
    LRESULT result = 0;
    switch (message)
    {
        case WM_CLOSE:
        {
            running = false;
        } break;
        case WM_DESTROY:
        {
            running = false;
        } break;
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC device_context = BeginPaint(
                window,
                &paint
            );

            RECT client_rect = {0};
            GetClientRect(window, &client_rect);
            win32_update_window(device_context, &client_rect);

            EndPaint(
                window,
                &paint
            );
        } break;
        case WM_SIZE:
        {
            RECT client_rect;

            GetClientRect(
                window,
                &client_rect
            );

            win32_resize_dib_section(
                client_rect.right - client_rect.left,
                client_rect.bottom - client_rect.top
            );
        } break;
        default:
        {
            return DefWindowProcA(
                window,
                message,
                wparam,
                lparam);
        }
    }
    
    return result;
}

internal void
win32_resize_dib_section(int width, int height)
{
    if (bitmap_memory)
    {
        VirtualFree(bitmap_memory, 0, MEM_RELEASE);
    }

    bitmap_width = width;
    bitmap_height = height;

    bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
    bitmap_info.bmiHeader.biWidth = bitmap_width;
    bitmap_info.bmiHeader.biHeight = bitmap_height;
    bitmap_info.bmiHeader.biPlanes = 1;
    bitmap_info.bmiHeader.biBitCount = 32;
    bitmap_info.bmiHeader.biCompression = BI_RGB;
    
    int bitmap_memory_size = bytes_per_pixel * bitmap_width * bitmap_height;
    bitmap_memory = VirtualAlloc(0, bitmap_memory_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

internal void
win32_update_window(
    HDC device_context,
    RECT *client_rect
)
{
    int window_width = client_rect->right - client_rect->left;
    int window_height = client_rect->bottom - client_rect->top;

    StretchDIBits(
        device_context,
        0,
        0,
        window_width,
        window_height,
        0,
        0,
        bitmap_width,
        bitmap_height,
        bitmap_memory,
        &bitmap_info,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}