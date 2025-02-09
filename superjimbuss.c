#include <stdbool.h>
#include <windows.h>

#define internal static
#define local_persist static
#define global static

global bool running;

LRESULT CALLBACK
win32_main_windows_callback(
    HWND window,
    UINT message,
    WPARAM wparam,
    LPARAM lparam
);

internal void
win32_resize_dib_section(int width, int height);

internal void
win32_update_window

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

    HDC dc = GETDC(NULL);

    BITMAPINFOHEADER bmp_info_header = {0};
    bmp_info_header.biSize = sizeof(BITMAPINFOHEADER);
    bmp_info_header.biWidth = 200;
    bmp_info_header.biHeight = 200;
    bmp_info_header.biPlanes = 1;
    bmp_info_header.biBitCount = 24;
    bmp_info_header.biCompression = BI_RGB;
    bmp_info_header.biSizeImage = 0;
    bmp_info_header.biClrUsed = 0;
    bmp_info_header.biClrImportant = 0;

    BITMAPINFO bmp_info = {.bmiHeader = bmp_info_header};
    void *pv;

    HBITMAP bitmap_image = CreateDIBSection(
        dc,
        &bmp_info,
        DIB_RGB_COLORS,
        &pv,
        NULL,
        0
    );

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

internal void
win32_resize_dib_section(int width, int height)
{

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
            HDC dc = BeginPaint(
                window,
                &paint
            );

            FillRect(
                dc,
                &paint.rcPaint,
                (HBRUSH)(COLOR_WINDOW + 1)
            );

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