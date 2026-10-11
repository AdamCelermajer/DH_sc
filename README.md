# Dungeon Hunter 2 native reconstruction

A native C++ reconstruction of **Dungeon Hunter 2** for modern Android (and a
Windows desktop host). The code is rebuilt from recovered behavior of the
original game and its Irrlicht-derived "glitch" engine. The original APK/cache
evidence is used to check that behavior.

This is **not a complete game** and not an official release. Running it
requires the owner's original game data. See [`RIGHTS.md`](RIGHTS.md) for
provenance and the rights status of recovered material and assets.

## Status

| Area | State |
| --- | --- |
| Windows desktop build | Builds and runs (MinGW-w64 + SDL2 + ANGLE) |
| Main menu | Original authored menu renders, including the character preview |
| SWAMP and Crypt levels | Load and render with the player and HUD; gameplay is **not yet stable** |
| Android | APKs (ARM64/x86_64, API 37) were built at earlier milestones; see Releases |
| Combat, AI, loot, quests, saves, campaign | Partial; many paths are incomplete or unverified |

A checked function or a passing host test covers only its stated scope. It
does not show that a subsystem works or that the game is playable. The current
boundaries and next steps are in
[`docs/PROJECT-CHECKLIST.md`](docs/PROJECT-CHECKLIST.md).

## Repository layout

| Path | Contents |
| --- | --- |
| `port/android-native/` | Android app (Gradle, JNI, packaging) |
| `port/desktop-native/` | Windows desktop host using the same runtime sources |
| `port/engine-*` | Engine subsystems: math, rendering, textures, skinning, animation, camera, particles, audio, resources, UI |
| `port/level-world/` | Level and world runtime, actors, combat, adapters and host tests |
| `port/world-data/` | Import of the original world records (MLX/MGP/MVP) and module placement |
| `port/game-data/` | Readers for the original character, model and animation tables |
| `port/script-runtime/` | Host-side script scheduler for recovered level script flows |
| `port/asset-payloads/` | BRES mesh and animation payload readers |
| `port/player-info-level/` | Player info, input manager and character-level members |
| `port/*` (others) | Smaller focused modules, each with its own README and tests |
| `recovered/` | Recovered evidence (symbols, pseudocode, Java/smali, scripts) |
| `tools/` | Recovery, tracing and verification scripts |
| `docs/` | Checklist, findings, reproduction and source-availability notes |

## Building

### Windows desktop

Requirements:

- MinGW-w64 (the WinLibs distribution is used)
- The SDL2 **MinGW** development package
- ANGLE development and runtime files (libEGL/libGLESv2)
- CMake and Ninja

```powershell
$env:SDL2_ROOT  = '<SDL2 MinGW package directory>'
$env:ANGLE_ROOT = '<ANGLE package directory>'
.\port\desktop-native\tools\build_windows.ps1
```

The script prints the path of the built executable. Command-line options
(`--level-row`, `--frames`, `--capture`, `--runtime-dir`, `--assets`) and
verified checkpoints are listed in
[`port/desktop-native/README.md`](port/desktop-native/README.md).

### Android

Install JDK 17+, Android SDK platform 37, NDK `29.0.14206865` and CMake
`3.22.1`. You can use Android Studio for this. Then build:

```sh
cd port/android-native
./gradlew :app:assembleDebug        # gradlew.bat on Windows
adb install -r app/build/outputs/apk/debug/app-debug.apk
```

For details, see [`port/android-native/README.md`](port/android-native/README.md).

### Original game data

Assets derived from the cache, and some source checks, need the original APK
and cache as local inputs. These are not distributed here. To set them up, see
[`docs/REPRODUCING.md`](docs/REPRODUCING.md) and
[`docs/SOURCE-AVAILABILITY.md`](docs/SOURCE-AVAILABILITY.md).

### Host tests

Each module has its focused host test runners under `port/<module>/tests`.
See each module's README for the scope of its checks.

## Releases

Android debug builds from development milestones (prereleases, except the latest):

- [native-ui-gears-load-api37-2026-10-08](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-ui-gears-load-api37-2026-10-08) (latest)
- [native-ui-movement-api37-2026-10-08](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-ui-movement-api37-2026-10-08)
- [native-combat-quest-tail-api37-2026-10-08](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-combat-quest-tail-api37-2026-10-08)
- [native-item-world-retry-2026-10-07](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-item-world-retry-2026-10-07)

Each release note lists what was verified in that build and what was not.

## Branches

- `main`: the stable state. It is updated from `development` at verified checkpoints.
- `development`: the integration branch. Pull requests target this branch.
- Feature branches: short-lived branches cut from `development` (for example
  `feature/<topic>`, `fix/<topic>` or `docs/<topic>`). They are merged back
  through a pull request and then deleted.

Older branch heads are kept as `archive/*` tags.

## Further reading

- [`docs/PROJECT-CHECKLIST.md`](docs/PROJECT-CHECKLIST.md): verified work and remaining tasks
- [`docs/FINDINGS.md`](docs/FINDINGS.md): cross-system reverse-engineering findings
- [`port/android-native/MODDING.md`](port/android-native/MODDING.md): modding notes
- [`RIGHTS.md`](RIGHTS.md): provenance and rights
