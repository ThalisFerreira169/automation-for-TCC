#include <windows.h>
#include <commctrl.h>
#include "typeText.c"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1)
            {
                char text[1024];
                GetWindowText(GetDlgItem(hwnd, 2), text, sizeof(text));
                Sleep(3000);
                type(text);
            }
            return 0;

        case WM_CREATE:
            CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE, 
                         10, 10, 380, 100, hwnd, (HMENU)2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            CreateWindow("BUTTON", "Simulate Typing", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                         10, 120, 150, 30, hwnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
    const char* className = "TypingToolClass";
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = className;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, className, "Typing Tool", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
                               NULL, NULL, wc.hInstance, NULL);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}