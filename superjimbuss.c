#include <windows.h>

LRESULT CALLBACK
window_proc(
    HWND window,
    UINT message,
    WPARAM wparam,
    LPARAM lparam
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
    window_class.lpfnWndProc = window_proc;
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

    HBITMAP bitmap_image = CreateDIBSection(dc, &bmp_info, DIB_RGB_COLORS, &pv, NULL, 0);

    ShowWindow(window, cmd_show);

    MSG message = {0};

    while (GetMessageA(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return 0;
}

LRESULT CALLBACK window_proc(
    HWND window,
    UINT message,
    WPARAM wparam,
    LPARAM lparam
)
{
    switch (message)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        } return 0;
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC dc = BeginPaint(window, &paint);
            FillRect(dc, &paint.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(window, &paint);
        } return 0;
    }

    return DefWindowProcA(window, message, wparam, lparam);
}