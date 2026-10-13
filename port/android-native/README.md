# Native Android app

This project builds the current source reconstruction for Android. It is still
an incomplete development game; see the [project checklist](../../docs/PROJECT-CHECKLIST.md)
for verified systems and open work. Download the current ARM64/x86_64 debug APK
from the [GitHub prerelease](https://github.com/Noamcelermajer/DH_sc/releases/tag/native-ui-movement-api37-2026-10-08).
On API 37 x86_64/16 KiB, it reached SWAMP, opened Character/Inventory/Skills/
Faeries screens by touch, and moved the player with the joystick. Skill save/
confirmation and live item drop remain incomplete; see release notes.

## Build and install

Use Android Studio or install JDK 17+, Android SDK platform 37, NDK
`29.0.14206865`, and CMake `3.22.1`. Open this directory in Android Studio, or
run the Gradle wrapper here:

```sh
./gradlew :app:assembleDebug
```

On Windows, use `gradlew.bat :app:assembleDebug`. Configure the Android SDK
through Android Studio or an untracked `local.properties`. The APK is written
to `app/build/outputs/apk/debug/app-debug.apk`; install it with:

```sh
adb install -r app/build/outputs/apk/debug/app-debug.apk
```

The app has minimum API 24 and targets API 37. The build packages ARM64 and
x86_64 native libraries. The selected assets needed by this build are tracked;
the complete original game cache is not bundled. To regenerate cache-derived
assets, use the original cache ZIP as a local input and follow the relevant
preparation instructions in [level-world](../level-world/README.md) and
[source availability](../../docs/SOURCE-AVAILABILITY.md).

For supported asset, data, Lua, and level overrides, see the [native modding
guide](MODDING.md). It distinguishes device-verified overrides from inputs
that are only routed by the current source.

## Gameplay sound effects

Every gameplay sound (script `PlaySound`/`PlaySound3D`, combat and animation-step
sounds, authored HUD `NativePlaySoundFX`) enters the single queue in
`player_gameplay_audio.hpp`. On Android `FrontAudio` forwards its `dh2fx` commands
to the native engine (`gameplay_sfx_engine_v1.hpp`): a software mixer that
reproduces the original Vox priority banks, Sounds.xml loop flags, positional
Q14 gains, fades, pause/resume and the FX volume option, output through an
OpenSL ES buffer queue (`gameplay_sfx_android.cpp`; AAudio needs API 26 and the
app supports 24). If the output cannot be opened the previous Java `MediaPlayer`
path still plays the same queue. Music stays on `GameplayVxnMusic`.

`tools/stage_gameplay_sfx.py --cache <cache.zip>` copies the Sounds.xml PCM sounds
present in the supplied cache byte-exact into `assets/original-media/gameplay` and
regenerates `gameplay_sfx_catalog_v1.hpp` and `sfx-catalog-provenance.json`
(`--check` re-verifies them without the cache). Sounds whose files are not in the
cache (most monster voices) stay unavailable and play nothing. Run
`tests/run_gameplay_sfx_mixer_host.py` for the host check of the mixer, bank
rules, queue consumer and every staged asset.

## Verification and scope

Run focused host checks with the test runners in
[`port/game-data/tests`](../game-data/tests) and
[`port/level-world/tests`](../level-world/tests). Each module README identifies
its supported checks and evidence scope. `:app:assembleDebug` verifies Android
compilation and packaging; it does not establish live gameplay correctness.

The app currently provides a menu/character-selection path and a Crypt
development level with selected movement, animation, combat and loot behavior.
Enemy AI, full campaign progression, original camera and level lifecycle,
complete saves, and broad live gameplay remain unfinished. See the concise
[findings](../../docs/FINDINGS.md) and [roadmap](ROADMAP.md).
