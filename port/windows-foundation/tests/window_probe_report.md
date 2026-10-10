# Native Windows window lifecycle verification

Executed on Windows on 2026-10-09 using LLVM-Mingw 20261006 (x86_64 UCRT), C++17, `-Wall -Wextra -static`.

Compiled `tests/window_probe.cpp` and `platform_win32.cpp` with `opengl32`, `gdi32`, and `user32`. Compilation produced no warnings. The native executable exited with code 0.

Verified against actual Win32/WGL windows created by the probe:

- Opened a 640 by 480 client area with a current OpenGL context.
- Resized its client area to 800 by 600 and polled real window messages.
- Swapped buffers and destroyed the first Window instance.
- Created a second Window instance after destruction, with a 320 by 240 client area.
- Rejected a zero-width resize and swapped the reopened window's buffers.

Output:

```text
PASS native open 640x480 -> resize 800x600
PASS native destruction -> reopen 320x240, invalid resize rejected
```

This probe does not inspect or send input to other applications. It verifies native lifecycle and client dimensions; it does not establish gameplay, asset-rendering correctness, or a manually confirmed visual appearance.
