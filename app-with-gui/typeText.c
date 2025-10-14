#include "typeText.h"
#include <stdlib.h>
#include <windows.h>

void pressKey(WORD vk)
{
    INPUT input[2] = {0};
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = vk;

    input[1] = input[0];
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, input, sizeof(INPUT));
}

int isCapsLockOn()
{
    return (GetKeyState(VK_CAPITAL) & 0x0001);
}

void type(const char* text)
{
    if (!text) return;

    int len = MultiByteToWideChar(CP_ACP, 0, text, -1, NULL, 0);
    if (len <= 0) return;

    wchar_t* wtext = (wchar_t*)malloc(len * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, text, -1, wtext, len);

    for (int i = 0; wtext[i] != L'\0'; i++)
    {
        wchar_t wc = wtext[i];
        INPUT input[2] = {0};

        if (wc == L'\n')
        {
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_RETURN;

            input[1] = input[0];
            input[1].ki.dwFlags = KEYEVENTF_KEYUP;
        }
        else if (wc == L'\t')
        {
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_TAB;

            input[1] = input[0];
            input[1].ki.dwFlags = KEYEVENTF_KEYUP;
        }
        else
        {
            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wScan = wc;
            input[0].ki.dwFlags = KEYEVENTF_UNICODE;

            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wScan = wc;
            input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        }

        SendInput(2, input, sizeof(INPUT));
        Sleep(50);
    }

    free(wtext);
}
