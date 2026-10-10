# DH2 Windows desktop runtime

This target links the same 22 selected runtime source units used by the Android app, including the world renderer, save/game owners, SWF UI session, and authored player HUD. SDL2 owns the desktop window and input; ANGLE supplies the GLES2 context, so the renderer and UI assets use their existing paths and shaders.

On launch it initializes the real loading SWF settings owner, reuses the first valid source campaign slot in the runtime directory or creates and assigns a canonical Knight profile through the shared Native menu APIs, then starts source Crypt row 23 through `start_menu_game`. It renders the shared world and player HUD. WASD moves through the shared movement owner, Space calls the shared nearest-reachable-enemy attack route, mouse input reaches the authored HUD before camera orbit, and the wheel controls camera zoom. Losing window focus releases movement and touch input.

The desktop host does not wire the Android front-menu/activity lifecycle. It enters Crypt through the source Save and NativeStartGame flow. Audio output and cinematic playback are unsupported. The default save/config directory is SDL's per-user AppData path; `--runtime-dir` overrides it. `--assets` overrides the Android asset tree. `--frames N` exits after a bounded number of gameplay frames and `--capture FILE.ppm` writes a checked GLES framebuffer readback.

Build on Windows with MinGW-w64, SDL2 development files, and ANGLE development/runtime files:

```powershell
$env:SDL2_ROOT = '<SDL2 package directory>'
$env:ANGLE_ROOT = '<ANGLE package directory>'
.\port\desktop-native\tools\build_windows.ps1
```

The script configures a temporary Ninja build and prints the executable path. For example, run a bounded source Crypt check with:

```powershell
& '<path printed by build_windows.ps1>' --frames 3 --capture "$env:TEMP\dh2-crypt.ppm" --runtime-dir "$env:TEMP\dh2-desktop-save"
```

## Verified checkpoint

The Windows executable and selected shared-source libraries built successfully with MinGW-w64. A bounded run created and assigned source campaign slot 0, launched Crypt through `start_menu_game`, and rendered 7 rooms, 26 monsters, 36 scenery objects, the player character, and the authored player HUD. Three gameplay frames completed with the renderer active; logs recorded live player AI timers and an enemy-spotted event. The final ANGLE framebuffer readback passed GLES error and viewport checks. The captured 1280x720 PPM contains nonblack world/HUD pixels (109,852 distinct colors). This validates world startup and rendering; no movement or attack interaction has been runtime-verified yet.
