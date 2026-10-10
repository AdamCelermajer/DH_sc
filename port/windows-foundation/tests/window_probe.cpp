#include "../platform_win32.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <iostream>
#include <string>

using dh::foundation::Window;

int main() {
    {
        Window window;
        const std::string title = "DH-SC native window lifecycle probe PID " +
            std::to_string(GetCurrentProcessId());
        if (!window.open(title.c_str(), 640, 480)) {
            std::cerr << "FAIL initial open: " << window.error() << '\n';
            return 1;
        }
        window.poll();
        if (window.width() != 640 || window.height() != 480 || window.should_close()) {
            std::cerr << "FAIL initial client size/state: " << window.width() << 'x'
                      << window.height() << '\n';
            return 2;
        }
        if (!window.resize(800, 600)) {
            std::cerr << "FAIL resize: " << window.error() << '\n';
            return 3;
        }
        window.poll();
        if (window.width() != 800 || window.height() != 600 || window.should_close()) {
            std::cerr << "FAIL resized client size/state: " << window.width() << 'x'
                      << window.height() << '\n';
            return 4;
        }
        const std::wstring wide_title(title.begin(), title.end());
        const HWND own_window = FindWindowW(L"DHSCWindowsFoundation", wide_title.c_str());
        DWORD owner_process = 0;
        if (!own_window || !GetWindowThreadProcessId(own_window, &owner_process) ||
            owner_process != GetCurrentProcessId()) {
            std::cerr << "FAIL could not identify a window owned by this probe\n";
            return 8;
        }
        SendMessageW(own_window, WM_KEYDOWN, 'W', 1);
        window.poll();
        if (!window.key_down('W')) {
            std::cerr << "FAIL own-window W keydown was not recorded\n";
            return 9;
        }
        SendMessageW(own_window, WM_KEYUP, 'W', (1LL << 31) | (1LL << 30) | 1);
        window.poll();
        if (window.key_down('W')) {
            std::cerr << "FAIL own-window W keyup did not release the key\n";
            return 10;
        }
        SendMessageW(own_window, WM_KEYDOWN, 'W', 1);
        SendMessageW(own_window, WM_KEYDOWN, 'A', 1);
        if (!window.key_down('W') || !window.key_down('A')) {
            std::cerr << "FAIL own-window keys were not pressed before focus loss\n";
            return 11;
        }
        SendMessageW(own_window, WM_KILLFOCUS, 0, 0);
        window.poll();
        if (window.key_down('W') || window.key_down('A')) {
            std::cerr << "FAIL own-window focus loss did not clear pressed keys\n";
            return 12;
        }
        std::cout << "PASS process-owned window W keydown/up and focus-loss key clearing\n";
        window.swap();
        std::cout << "PASS native open 640x480 -> resize 800x600\n";
    } // Real destructor releases the WGL context, device context, and HWND.
    {
        Window reopened;
        if (!reopened.open("DH-SC native reopen probe", 320, 240)) {
            std::cerr << "FAIL reopen after destruction: " << reopened.error() << '\n';
            return 5;
        }
        reopened.poll();
        if (reopened.should_close() || reopened.width() != 320 || reopened.height() != 240) {
            std::cerr << "FAIL reopened client size/state\n";
            return 6;
        }
        if (reopened.resize(0, 240)) {
            std::cerr << "FAIL accepted invalid resize\n";
            return 7;
        }
        reopened.swap();
        std::cout << "PASS native destruction -> reopen 320x240, invalid resize rejected\n";
    }
    return 0;
}
