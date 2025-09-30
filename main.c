
#include <windows.h>
#include <stdio.h>

DWORD WINAPI CloseMsgBox(LPVOID lpParam) {
    HWND hwnd;
    const char* title = (const char*)lpParam;

    Sleep(1000);

    hwnd = FindWindow(NULL, title);
    if (hwnd) {
        SendMessage(hwnd, WM_CLOSE, 0, 0);
    }

    return 0;
}

HANDLE spawnCloseThread(const char* title) {
    return CreateThread(NULL, 0, CloseMsgBox, (LPVOID)title, 0, NULL);
}

void deleteAndWaitThreadHandle(HANDLE handle) {
    WaitForSingleObject(handle, INFINITE);
    CloseHandle(handle);
}

int okMessageBox(const char* text, const char* title) {
    if (!title) {
        return MessageBox(NULL, text, "dontMatter", MB_OK | MB_ICONASTERISK);
    }
    else {
        return MessageBox(NULL, text, title, MB_OK | MB_ICONASTERISK);
    }
}

int yesNoMessageBox(const char* text, const char* title) {
    if (!title) {
        return MessageBox(NULL, text, "dontMatter", MB_YESNO | MB_ICONASTERISK);
    }
    else {
        return MessageBox(NULL, text, title, MB_YESNO | MB_ICONASTERISK);
    }

}

void okMessageBoxWithKill(const char* title) {
    HANDLE handle = spawnCloseThread("yoNo");
    okMessageBox(title, "yoNo");
    deleteAndWaitThreadHandle(handle);
}

void yesNoMessageBoxWithKill(const char* title) {
    HANDLE handle = spawnCloseThread("yoNo");
    yesNoMessageBox(title, "yoNo");
    deleteAndWaitThreadHandle(handle);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmd, int nShowCmd) {

    // Intro
    MessageBox(NULL, "The Following Game will only be with MessageBoxes!", "Yoo", MB_OK | MB_ICONASTERISK);
    MessageBox(NULL, "You must Answer quickly!", "Yooo", MB_OK | MB_ICONASTERISK);
    
    Sleep(1000);
    
    // Count Down
    for (int i = 10; i > 0; i--) {
        char buffer[100];
        sprintf(buffer, "The game starts in %d", i);
        HANDLE handle = spawnCloseThread("Number");
        MessageBox(NULL, buffer, "Number", MB_OK | MB_ICONASTERISK);
        deleteAndWaitThreadHandle(handle);;
    }

    int id = MessageBox(NULL, "Yooo something went wrong my friend", "Wsp", MB_RETRYCANCEL | MB_ICONHAND);
    if (id == IDCANCEL) {
        int id2 = MessageBox(NULL, "Oh you want to quit?", "Wsp", MB_OKCANCEL | MB_ICONHAND);
        if (id2 == IDOK) {
            okMessageBox("Ok byeee", "dontMatter");
            exit(1);
        }
        else {
            okMessageBox("Ok you shall continue!", "dontMatter");
        }
    }
    else {
        HANDLE handle = spawnCloseThread("test");
        okMessageBox("Oh nvm i just wanted to test you", "test");
        deleteAndWaitThreadHandle(handle);
    }
    
    okMessageBox("Ok the game is a number guesser game but in reverse heheheh", "dontMatter");
    okMessageBox("You need to think of a number and I need to guess it", "dontMatter");
    okMessageBox("You will need to give me hints tho", NULL);
    okMessageBox("Ok choose a number between 0-100 included!", NULL);
    okMessageBox("Press Escape if I guessed you number correctly", NULL);
    okMessageBoxWithKill("Ok lets start guessing");

    int success = 0;
    int guess = 50;
    int num = 50;
    char buffer[100];

    while (!success) {
        sprintf(buffer, "Is your number lower than %d?", guess);
        int id = yesNoMessageBox(buffer, NULL);

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }

        num = num / 2;
        if (num < 1) num = 1;
        if (id == IDYES) {
            guess -= num;
        }
        else {
            guess += num;
        }
    }
    
    sprintf(buffer, "Yoo so your number was %d fire", guess);
    okMessageBox(buffer, NULL);
    okMessageBox("Ok cya", NULL);

    return 0;
}

