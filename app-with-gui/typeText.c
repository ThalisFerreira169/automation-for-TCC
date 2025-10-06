#include <windows.h>
#include <commctrl.h>

void pressKey(WORD vk)
{
    keybd_event(vk, 0, 0, 0);
    keybd_event(vk, 0, KEYEVENTF_KEYUP, 0);
}

int isCapsLockOn()
{
    return (GetKeyState(VK_CAPITAL) & 0x0001);
}

void type(const char* text)
{
    int capsLockOn = isCapsLockOn();

    for (int i = 0; text[i] != '\0'; i++)
    {
        char c = text[i];

        SHORT vkNormal = VkKeyScan(c);
        INPUT input[2] = {0};

        int isUpper = (vkNormal & 0x0100) != 0;

        if (isUpper && !capsLockOn) {
            keybd_event(VK_SHIFT, 0, 0, 0);
        }

        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = (WORD)vkNormal;
        input[0].ki.dwFlags = 0;

        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = (WORD)vkNormal;
        input[1].ki.dwFlags = KEYEVENTF_KEYUP;

        SendInput(2, input, sizeof(INPUT));

        if (isUpper && !capsLockOn) {
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        }

        Sleep(500);
    }
}
