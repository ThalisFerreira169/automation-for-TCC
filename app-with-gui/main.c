#include <windows.h>
#include <commctrl.h>
#include "typeText.h"

#define ID_BUTTON       1
#define ID_EDIT         2
#define ID_STATUS       3
#define ID_EDIT_DELAY   4

typedef struct {
    char* text;
    int delaySeconds;
    HWND hStatus;
} TypingData;

DWORD WINAPI TypingThread(LPVOID lpParam)
{
    TypingData* data = (TypingData*)lpParam;
    wchar_t statusMsg[256];

    wsprintfW(statusMsg, L"Iniciando em %d segundos...", data->delaySeconds);
    SetWindowTextW(data->hStatus, statusMsg);

    Sleep(data->delaySeconds * 1000);

    SetWindowTextW(data->hStatus, L"Digitando...");
    type(data->text);
    SetWindowTextW(data->hStatus, L"Concluído!");

    free(data->text);
    free(data);
    return 0;
}

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
    HFONT hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                              DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                              CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                              VARIABLE_PITCH, L"Segoe UI");

    switch (uMsg)
    {
        case WM_CREATE:
        {
            HWND hEditLabel = CreateWindowW(L"STATIC", L"Digite o texto para digitar:",
                                            WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
                                            10, 10, 400, 25, hwnd, NULL,
                                            ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            SendMessage(hEditLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

            HWND hEdit = CreateWindowW(L"EDIT", L"",
                                       WS_CHILD | WS_VISIBLE | WS_BORDER |
                                       ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                                       10, 40, 400, 120, hwnd, (HMENU)ID_EDIT,
                                       ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            SetWindowSubclass(hEdit, EditSubclassProc, 0, 0);
            SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hEdit, EM_SETMARGINS, EC_LEFTMARGIN, MAKELPARAM(5,0));

            HWND hDelayLabel = CreateWindowW(L"STATIC", L"Tempo para iniciar a digitação:",
                                             WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
                                             10, 170, 250, 25, hwnd, NULL,
                                             ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            SendMessage(hDelayLabel, WM_SETFONT, (WPARAM)hFont, TRUE);

            HWND hDelay = CreateWindowW(L"EDIT", L"3",
                                        WS_CHILD | WS_VISIBLE | WS_BORDER,
                                        260, 170, 60, 25, hwnd, (HMENU)ID_EDIT_DELAY,
                                        ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            SendMessage(hDelay, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hDelay, EM_SETMARGINS, EC_LEFTMARGIN, MAKELPARAM(5,0));

            HWND hButton = CreateWindowW(L"BUTTON", L"Simular Digitação",
                                         WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                         10, 210, 150, 40, hwnd, (HMENU)ID_BUTTON,
                                         ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);

            HWND hStatus = CreateWindowW(L"STATIC", L"",
                                         WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
                                         180, 220, 230, 25, hwnd, (HMENU)ID_STATUS,
                                         ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            SendMessage(hStatus, WM_SETFONT, (WPARAM)hFont, TRUE);

            return 0;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON && !typing)
            {
                typing = 1;

                wchar_t wtext[2048];
                GetWindowTextW(GetDlgItem(hwnd, ID_EDIT), wtext, 2048);

                wchar_t wdelay[16];
                GetWindowTextW(GetDlgItem(hwnd, ID_EDIT_DELAY), wdelay, 16);
                int delaySeconds = _wtoi(wdelay);

                char* text = (char*)malloc(4096);
                WideCharToMultiByte(CP_ACP, 0, wtext, -1, text, 4096, NULL, NULL);

                TypingData* data = (TypingData*)malloc(sizeof(TypingData));
                data->text = text;
                data->delaySeconds = delaySeconds;
                data->hStatus = GetDlgItem(hwnd, ID_STATUS);

                CreateThread(NULL, 0, TypingThread, data, 0, NULL);

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

    HWND hwnd = CreateWindowExW(0, className, L"Ferramenta de Automação GUI",
                                WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                                CW_USEDEFAULT, CW_USEDEFAULT, 440, 300,
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
