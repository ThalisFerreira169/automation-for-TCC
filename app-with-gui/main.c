#include <windows.h>
#include <commctrl.h>
#include "typeText.h"

#define ID_BUTTON 1
#define ID_EDIT   2
#define ID_STATUS 3

LRESULT CALLBACK EditSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    if (uMsg == WM_KEYDOWN && (GetKeyState(VK_CONTROL) & 0x8000))
    {
        if (wParam == 'A')
        {
            SendMessage(hwnd, EM_SETSEL, 0, -1);
            return 0;
        }
    }
    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static int typing = 0;

    switch (uMsg)
    {
        case WM_CREATE:
        {
            HWND hEdit = CreateWindowW(L"EDIT", L"",
                                       WS_CHILD | WS_VISIBLE | WS_BORDER |
                                       ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                                       10, 10, 380, 100, hwnd, (HMENU)ID_EDIT,
                                       ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            SetWindowSubclass(hEdit, EditSubclassProc, 0, 0);

            CreateWindowW(L"BUTTON", L"Simular Digitação",
                          WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                          10, 120, 150, 30, hwnd, (HMENU)ID_BUTTON,
                          ((LPCREATESTRUCT)lParam)->hInstance, NULL);

            CreateWindowW(L"STATIC", L"",
                          WS_CHILD | WS_VISIBLE,
                          170, 125, 200, 20, hwnd, (HMENU)ID_STATUS,
                          ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            return 0;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON && !typing)
            {
                typing = 1;

                wchar_t wtext[1024];
                GetWindowTextW(GetDlgItem(hwnd, ID_EDIT), wtext, 1024);

                SetWindowTextW(GetDlgItem(hwnd, ID_STATUS), L"Digitando em 3 segundos...");
                Sleep(3000);

                SetWindowTextW(GetDlgItem(hwnd, ID_STATUS), L"Digitando...");

                char text[2048];
                WideCharToMultiByte(CP_ACP, 0, wtext, -1, text, sizeof(text), NULL, NULL);

                type(text);

                SetWindowTextW(GetDlgItem(hwnd, ID_STATUS), L"Concluído!");

                typing = 0;
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t* className = L"TypingToolClass";

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (!RegisterClassW(&wc))
        return 1;

    HWND hwnd = CreateWindowExW(0, className, L"Typing Tool",
                                WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                                CW_USEDEFAULT, CW_USEDEFAULT, 420, 220,
                                NULL, NULL, hInstance, NULL);

    if (!hwnd)
        return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
