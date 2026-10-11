# DH2 Windows desktop runtime

This target links the same 22 selected runtime source units used by the Android app, including the world renderer, save/game owners, SWF UI session, and authored player HUD. SDL2 owns the desktop window and input; ANGLE supplies the GLES2 context, so the renderer and UI assets use their existing paths and shaders.

The default launch loads and renders the original authored main-menu SWF. Mouse and touch events enter the retained `OriginalUiSession`; character-name text and special keys use its GameSWF input path. The session's existing save-slot, class assignment, preview, continue, and deferred `NativeStartGame` callbacks use the same native providers as Android. Starting a game calls the canonical `start_menu_game` route and attaches the same player HUD. A gameplay main-menu request unloads the world through the existing renderer owner and reloads the original front menu.

`--level-row 23` or `--level-row 41` is an optional bounded-development path that skips the front menu and starts Crypt or the normal fresh-save SWAMP route. It does not create a parallel game or Save implementation. In gameplay, WASD and Space call the shared movement and nearest-reachable-enemy attack owners; mouse input reaches the authored HUD before camera orbit, and the wheel controls camera zoom. Losing window focus releases movement and active touch input.

SDL consumes the original menu sound/control queues and shared gameplay audio queue through one output owner. The dummy-driver checks verify initialization and frame-loop pumping, but not audible output. Cinematic playback is unsupported. The default save/config directory is SDL's per-user AppData path; `--runtime-dir` overrides it. `--assets` overrides the Android asset tree. `--frames N` exits after a bounded number of UI or gameplay frames and `--capture FILE.ppm` writes a checked GLES framebuffer readback.

Build on Windows with MinGW-w64, SDL2 development files, and ANGLE development/runtime files:

```powershell
$env:SDL2_ROOT = '<SDL2 package directory>'
$env:ANGLE_ROOT = '<ANGLE package directory>'
.\port\desktop-native\tools\build_windows.ps1
```

The script configures a temporary Ninja build and prints the executable path. For example, capture the default authored main menu with:

```powershell
& '<path printed by build_windows.ps1>' --frames 3 --capture "$env:TEMP\dh2-menu.ppm" --runtime-dir "$env:TEMP\dh2-menu-save"
```

To run a bounded source Crypt check instead, pass:

```powershell
& '<path printed by build_windows.ps1>' --level-row 23 --frames 3 --capture "$env:TEMP\dh2-crypt.ppm" --runtime-dir "$env:TEMP\dh2-desktop-save"
```

For a bounded normal-input scenario, run:

```powershell
& '<path printed by build_windows.ps1>' --scenario move-attack --frames 300 --capture-initial "$env:TEMP\dh2-before.ppm" --capture "$env:TEMP\dh2-after.ppm" --runtime-dir "$env:TEMP\dh2-desktop-save"
```

The scenario follows waypoints returned by the shared selected-floor path query, sends ordinary `move_axis` input, then asks the shared combat owner to attack its nearest reachable enemy once. It does not teleport or force a target.

## Verified checkpoint

The selected desktop runtime built successfully with MinGW-w64 after fixing a condition-table static API call. A bounded default launch rendered the original main-menu scene and controls for three frames, exited successfully, and produced a checked 1280x720 ANGLE framebuffer capture (2,764,816-byte PPM). The SWF logged missing optional core/title targets, but the main menu rendered. The physical-width source rule now selects the standard menu SWF at 1280 pixels. Its seven missing original textures are restored. After the entrance animation (60/120 frames), Start, Options and Info fit within the display; decorative artwork outside the authored stage still extends beyond the edge. An earlier bounded direct Crypt run created and assigned source campaign slot 0 through canonical save APIs and rendered the player and authored HUD.

The fixed Crypt scenario followed normal input to cultist07, attacked at 142.424 XY units, and reduced HP from 12160 through 3711 to 0 (`dead=1`). The process exited 0 and captured the rendered world/player/HUD after the SCT ownership fix. XP distribution still lacks active-Level/killer/DesignSettings providers; combat sound and the full loot tail remain unverified. This proves one enemy hit/death path, not full AI or campaign parity.

A fresh bounded normal-save launch selected SWAMP row 41, imported its nine source modules, and passed the corrected census with four Monster rows and one ungated merchant. After the restored LizardMan/prisoner/SwampKing assets and material-binding correction, the Windows runtime rendered three SWAMP gameplay frames. Its checked 1280x720 ANGLE capture shows the environment, player, and HUD. This validates startup and rendering for the selected row; it does not validate merchant transactions or a full SWAMP playthrough.
