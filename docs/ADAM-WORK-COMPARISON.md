# Adam's work and this reconstruction

Comparison pinned to Adam Celermajer's public [`DH_sc` main](https://github.com/AdamCelermajer/DH_sc) at `45c5348e807607a2825211bb8f26248067ba9106` (2026-10-04). The current local branch is `reconstruction/android17-irrlicht-rebuild-2026-10-03`.

## Work before the integration

| Area | Our branch before Adam's work was added | Adam's pinned project | Practical difference |
|---|---|---|---|
| Playable world | SWAMP and Infected Village source previews, authored encounter, and scene/actor diagnostics. | Native Crypt scene with 8 rooms, 11 monsters, 84 scenery objects, and 20,940 triangles. | Adam has a much more complete playable slice. |
| Player | Source-derived pose previews and bounded movement checks; no source player in the Irrlicht world. | Live Prince with source animation playback, touch movement, collision, camera, AI and attack input. | His runtime already joins character, animation, physics and scene updates. |
| Renderer | Android 17 Irrlicht NativeActivity can load a SWAMP module, but visible black/fallback surfaces remain. | A separate GLES2 renderer integrated with the live Crypt runtime. | Neither renderer is a drop-in replacement for the other's architecture. |
| Reconstructed systems | Broader imported SWAMP/village data, actor/spawn audits, trigger and quest kernels, persistence work, plus Irrlicht Android source. | Deeper live integration of player movement, animation, physics, combat and Crypt actors; additional animation, world, resource and AI research. | Our authored level/source-data breadth complements Adam's running gameplay foundation. |
| Android proof | The current Irrlicht diagnostic passed Android 17/API 37 x86_64 on a 16 KiB emulator; it did not prove full gameplay. | API 37 app targets ARM64 and x86_64 and uses 16 KiB ELF alignment. | Both have modern Android evidence; only Adam's current slice exercises a moving source player in a real level. |

## Work completed during this comparison

- Imported Adam's seven non-overlapping native modules byte-for-byte: `engine-textures`, `scene-materials`, `engine-animation`, `engine-skinning`, `game-data`, `physics-backend`, and `level-world` (577 files, 2,929,226 bytes). Source inventory and origin are recorded in [`port/ADAM-CORE-SOURCE-IMPORT.md`](../port/ADAM-CORE-SOURCE-IMPORT.md).
- Imported Adam's native Android project into [`port/android-native`](../port/android-native/README.md), including its 233 bundled game assets. Source and dependency provenance are recorded in [`RIGHTS.md`](../RIGHTS.md).
- Independently built Adam's exact pinned Android project with API 37 / NDK 29. The debug APK was 20,765,170 bytes (SHA-256 `1e0677d709687ed7547ade1c042ce94545a47fd2033a7af8831211c89338d5e2`). On the Android 17 x86_64 emulator with `getconf PAGE_SIZE = 16384`, the app opened `worlds/crypt01.dwld`, displayed the Prince and Crypt, and joystick input changed the player body from `(-22.2777, 12.2093)` to `(-20.2205, 12.0515)`. The Attack button correctly rejected an out-of-range attempt with “Walk closer to an enemy”; that test did not establish a successful hit.
- Imported Adam's separate 158-file Lua runtime under `port/adam-script-runtime`, leaving our existing `port/script-runtime` untouched. Updated the two `level-world` dependency paths to this isolated copy. The `dh2_level_world` host target now configures and links with physics, game-data, skinning, and Lua dependencies.
- Rebuilt the integrated branch app after adding the copyright attribution footer. The branch APK is [app-debug.apk](../port/android-native/app/build/outputs/apk/debug/app-debug.apk), 20,765,170 bytes, SHA-256 `1811ebe86d693c1e06549061f38b7fb084c1a509a20b6afb185a7c05683d9470`. It installed and launched on Android 17/API 37 x86_64 with `getconf PAGE_SIZE = 16384`. Screenshot: [adam-branch-api37.png](../port/android-native/app/build/adam-branch-api37.png) (ignored build evidence). The screen shows the Crypt, skinned Prince, 8 rooms, 11 monsters, 84 scenery objects, 20,940 triangles, health/mana, controls, and attribution. A joystick drag changed the rendered player pose and native physics position; an Attack tap displayed “Walk closer to an enemy.” No successful hit was established.

## Integration direction

Keep Adam's source world, physics, animation and gameplay updates independent of drawing. The clearest next slice is to replace the green sphere in the Irrlicht SWAMP diagnostic with Adam's skinned Prince, using the imported skinning/animation state and adding a mutable-vertex path beside the existing static `SceneMesh` adapter. Then feed authored room surfaces and transforms into Irrlicht, followed by collision and combat/AI with visible targets. Do not transplant Adam's monolithic GLES2 `model_renderer.cpp` into the Irrlicht loop or run both renderers on the same surface.

Adam's level-world runtime is now buildable against its separate Lua runtime path. The imported Android app and our Irrlicht diagnostic are still separate apps. Keep API 37 support and 16 KiB alignment in the source port; for this task, runtime tests use only the 16 KiB modern-Android target.

## Remaining gap

The imported Android app is a working native Crypt baseline, but its GLES2 renderer is still separate from Irrlicht. Our Irrlicht APK remains a SWAMP diagnostic with a placeholder sphere. Replacing that sphere with Adam's source-skinned Prince is the next implementation milestone; after that, connect a level and test combat end to end. See [`RECONSTRUCTION-HANDOFF.md`](../RECONSTRUCTION-HANDOFF.md) for the overall status.
