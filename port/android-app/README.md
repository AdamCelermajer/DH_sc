# Android 17 source renderer milestone

## Current kill-objective progress APK milestone

Current source APK: 873,379 bytes, SHA-256 `3005249a0ef0ece743c66ed6d67a01687e0b2590ea27c1ddd4997c6366c782cb`.
Recovered melee damage, owned health/death and four reconstructed kill-objective
counters execute together. Both Android 17 page sizes pass 14 imports, threshold
completion, repeated/stale/increasing events, rejection and retained state.
See [instructions/evidence](QUEST-INTEGRATION.md). Full original quest/world
ownership, persistence/rewards and complete source gameplay remain unfinished.

## Earlier non-player death APK milestone

Current source APK: 869,283 bytes, SHA-256 `7ab0f1e1b8e61405c4221f9e33d4b46c2efc150538306c2e8b643abb3be32599`.
Recovered melee damage feeds the owned health/death transition. Both Android 17
page sizes pass 14 imports, four ordered quest request IDs/match identifiers,
loot requests, dead-state retention and repeated-kill no-op. Strict host and
device native runtime checks pass. See [instructions/evidence](DEATH-INTEGRATION.md).
Actual event/loot consumers, resolved killer/XP, full combat and world ownership,
progression/saves and complete source gameplay remain unfinished.

## Earlier non-player damage APK milestone

The current source APK is 865,187 bytes, SHA-256
`1692549ee46c4ca241dd1c31ce1334a3d7a8bc20865816bbb22d146e66ed63d9`.
Recovered melee formula damage feeds the original-matched non-player health
projection. Both Android 17 page sizes pass 11 imports, a lethal two-hit sequence,
boundary policies, rejection/recovery, retained property generations and textured
walk preview. Native runner selftests also pass. See
[instructions/evidence](DAMAGE-INTEGRATION.md). Original Character, real death
owner, full result dispatch, world/AI, progression, saves and source gameplay
remain unfinished.

## Earlier health/mana APK milestone

The current source APK is 848,803 bytes, SHA-256
`991355b1c6dcd9a42e595ec5e0e56c12b2aedcde055efb1821b770d9eb3b89f8`.
It adds health reporting, validation, regeneration and normal offline mana use
on owned script actors. Both Android 17 page sizes pass 14 imports, 86 selected
original-derived cases / 19,264 final-field queries, retained property generations,
rejection/recovery and textured character walk preview. See
[instructions/evidence](HEALTH-INTEGRATION.md). Full damage/result dispatch,
death/events, Character ownership, AI and source gameplay remain unfinished.

## Earlier combat calculation APK milestone

The current source APK is 840,611 bytes, SHA-256
`122af9795982829d6f15e7c45a417824b465461d6d9f9cfd660cbb3bbe55c795`.
It adds owned script constant imports, offline Rand and actor state/hit/name
projections for the unchanged recovered combat formula. Both Android 17 page
sizes pass 26 imports, 80 real and 216 controlled combat cases, replacement/error
recovery and textured two-motion preview. See [instructions/evidence](COMBAT-INTEGRATION.md).
Damage application, original Character/state/buff lifecycle, AI and full source
gameplay remain unfinished.

## Earlier gear/power stat APK milestone

The current source APK is 836,515 bytes, SHA-256
`d0005277e5839989c63a38cb2e86add55347b99150181e0baf944ca9db26e40f`.
It imports owned property, class, item and power data. Gear/base update order,
retained generations, typed/index rejection and recovery pass 20 imports /
15,680 selected final queries on each Android 17 page size. The same package
passes textured two-motion animation; all paused mix screenshots were inspected.
See [instructions and evidence](GEAR-INTEGRATION.md). Full Character/inventory
ownership, equip requirements, random powers, buffs, combat and source gameplay
remain unfinished.

## Earlier equipment calculation APK milestone

The current source APK is 807,843 bytes, SHA-256
`2974a802e258582a13c20a0d38aad19a44ceff313170473a85c0ff68322cafdd`.
It imports owned property, class and item data. Supported bonus and shield
queries, retained data generations, rejection/recovery, class rules, shared
scripts and textured two-motion animation pass on both Android 17 page sizes.
See [instructions and evidence](EQUIPMENT-INTEGRATION.md). Full Character and
inventory lifecycle, gear contributions/powers and source gameplay are unfinished.

## Earlier class calculation APK milestone

The current source APK is 779,171 bytes, SHA-256
`7d2c82ab3ba433e06c006bbe962f35b78f8266b4cd030d2be2440942a8231c79`.
It imports owned property and class data and executes checked class rules through
diagnostic script objects. Both Android 17 page sizes pass 40 real applications /
8,960 final queries, retained generations, rejection/recovery, shared-script
checks and the textured animation preview. See [instructions and evidence](CLASS-INTEGRATION.md).
Actual Character lifecycle and full source gameplay remain unfinished.

## Earlier property data APK milestone

The earlier source APK is 775,075 bytes, SHA-256
`31c8e586004ce0932cc9853ec6a03b3140c6752f9b29d4ebcc1c16cfc3e61be7`.
It imports character property data and exposes owned typed property state to
scripts. Data replacement/rejection/recovery, shared scripts and character
animation checks pass on both Android 17 page sizes. See [instructions and
evidence](PROPERTY-INTEGRATION.md). Full source gameplay remains unfinished.

## Earlier shared-script APK milestone

The earlier source APK is 730,019 bytes, SHA-256
`ffcb32a34ed5de300ca05420bfb67637a89e64264b581a855f918a1e79d8b211`.
Three exact recovered shared scripts now initialize in an activity-owned source
Lua runtime. Script imports, budget/error rejection and recovery pass in the
same process on Android 17 with both page sizes. The same package passes the
textured two-motion character preview checks. See [instructions, evidence and
remaining work](SCRIPT-INTEGRATION.md). Actual source gameplay is unfinished.

## Earlier two-motion preview milestone

The earlier source APK is 254,496 bytes, SHA-256
`2e9b30c1560aee54083dfe7ba77273bc9a610ad78a828bba16a0885027c9b71d`.
It combines two absolute poses using reconstructed weight/vector and quaternion
mixing calculations. Import the warrior and texture, the dual walk as the primary
animation, then Dark Queen scene 03a prince through **Import second animation**.
The upper slider selects time; the lower slider mixes the second motion.

The exact package passed import, midpoint seek, 0/50/100% mixing, advancing Play
and stable Pause on both Android 17 4 KiB and 16 KiB emulators. Installed hashes
matched. Visually inspected screenshots show three distinct textured poses with
no fatal error or rejected frame. Play was checked after selecting 100%; other
weights were checked while paused. [Runtime evidence](layers-runtime-validation.json)
and [source layer scope](../animation-layers/README.md) record the limits.

This combines source poses in a diagnostic viewer. Original target compilation,
relative application, transition scheduling, synchronization, events and gameplay
remain unfinished. The UI aligns the clips by normalized phase; that timing
choice has not been compared to the original synchronized animator.

## Earlier reconstructed timeline milestone

The earlier source APK is 246,304 bytes, SHA-256
`56f57478bea49e60c787a318c0852786f464d9d277fcd5031ac81103fcc5592d`.
It uses the [reconstructed original range timeline](../animation-timeline/README.md)
for playback, seek and resume. The on-screen time now follows native playback.
The module passed 400 original ARM32/compiled ARM64/host sequences (4,000 updates
and 400 jumps), plus 50,000 sanitizer valid/corrupted-state updates.

The exact APK passed the 25-track dual walk on Android 17 with 4 KiB pages and
29-track Dark Queen scene 03a prince clip with 16 KiB pages. The installed hashes
matched; imports, midpoint seek, Play/Pause, advancing playback positions and
stable paused positions passed. Start/midpoint screenshots visibly show different
textured poses, with no fatal error in either run.
[Runtime evidence](timeline-runtime-validation.json) preserves the results.

The source preview still has no gameplay, original event dispatch, animator
transitions or blending. Its Play/resume clock epoch and diagnostic rendering
are documented separately from the original arithmetic. ARM64 is built and
checked for alignment; execution on ARM64 hardware remains unverified.

## Earlier scalar-track preview milestone

The earlier 242,208-byte source APK, SHA-256
`c6679410b521bb70dc0374b5449efa4489fe27f04c057d55243b8d81332140d2`,
supports scalar angle rotation and individual position components. It passed
the 25-track, 799 ms dual walk on Android 17 x86_64 with 4 KiB pages and the
29-track, 1,099 ms Dark Queen scene 03a prince clip with 16 KiB pages. Imports,
midpoint seek and Play/Pause passed; both showed changing textured poses with
no fatal error and matching installed APK hashes. [Runtime evidence](scalar-track-runtime-validation.json)
records these two fixtures and exact binary identity.

Import the warrior model and texture listed below, followed by
`data/3d/characters/prince/animations/prince_walk_dual.bdae` or
`data/3d/characters/prince/animations/cs_darkqueen_scene03a_prince.bdae`.
The repeated UI test accepts `--animation`, `--tracks` and `--duration` for
these fixtures. The updated calculations passed 4,705 original ARM32/compiled
ARM64/host comparisons, and 6,000 safety cases. The warrior corpus now produces
333 supported poses out of 342 player clips (327 animated).

This remains absolute-key diagnostic rendering, with no gameplay. ARM64 is
built and checked for alignment; execution on ARM64 hardware remains unverified.
The following runtime records preserve the earlier APKs they actually tested.

## Earlier character animation preview milestone

Import `data/3d/characters/prince/prince_low_poly_warrior.bdae`,
`data/3d/textures/prince-warrior.tga`, then
`data/3d/characters/prince/animations/prince_walk_1hand.bdae` from your own
cache. **Play animation**, **Pause animation** and the time slider show a
27-track, 799 ms walk preview on the 18-bone warrior. Every animation target
must resolve once in the imported model; mismatched clips are rejected.

The exact 242,208-byte APK, SHA-256
`6c4e50f278348b80c15b0856ea43338e4fe245fb1821343ece5db0fd9d7f688e`,
was installed and pulled back with matching hashes on Android 17 x86_64
emulators with both 4 KiB and 16 KiB pages. Import, midpoint seek, Play and
Pause passed, with visibly different textured leg/arm poses and no fatal
error during each run. [Runtime evidence](animation-runtime-validation.json)
records this build. [The pose component](../animation-pose/README.md)
documents sampling and safety checks. The latest build uses the
[verified float track calculations](../animation-values/README.md), which
passed 1,685 original ARM32/compiled ARM64/host comparisons and another
3,000 sanitizer checks.

This previews absolute stored keys. Original default-relative blending,
animation transitions, root motion and gameplay are unfinished. Each frame
uses diagnostic bounds normalization, one texture and one resolved skin
controller. ARM64 libraries are structurally checked but have not run on
ARM64 hardware. Selected input files are bounded to 32 MiB each.

### Repeat the emulator animation check

With an English Android emulator running and the APK built, run:

```powershell
python port/android-app/tests/animation_runtime.py --adb PATH_TO_ADB --serial emulator-5554 --apk port/android-app/build/dh2-source-renderer-debug.apk --cache PATH_TO_PRIVATE_CACHE --evidence PATH_OUTSIDE_GIT --ui-helper port/android-app/tests/ui-helper/build/dh2-ui-helper.apk
```

Build the local [UI snapshot helper](tests/ui-helper/README.md) first. It reads
the live playback interface without requiring it to become idle.
The script requires an emulator, installs the selected APK, copies three
owner-supplied fixtures to Downloads, selects them through the system file
picker, checks the midpoint slider and Play/Pause, and compares the installed
APK hash. It saves screenshots and runtime logs locally. Visually inspect
the start and midpoint screenshots; UI text alone does not prove rendering.
The script was run successfully on the Android 17 16 KiB emulator with this
exact build and the negative-start guarding-aura fixture (25 tracks, 799 ms). Its file-picker selectors require the tested English system UI.

The optional `--animation` cache-relative path, `--tracks` and `--duration`
arguments select a different clip and its expected metadata. The tested
guarding-aura path is
`data/3d/characters/prince/animations/skill_dh2_prince_warrior_paladin_guarding_aura.bdae`.
Its keys span -333 to 466 ms; the slider spans the 799 ms duration.
[Runtime evidence](negative-time-runtime-validation.json) records the visible
poses and controls. [The corpus audit](../animation-pose/character-corpus-validation.json)
records 333 supported poses out of 342 player clips on the warrior model.

## Earlier character pose milestone

The current build adds a checked software-skinning fallback when a BRES has
no static scene draws. Import
`data/3d/characters/prince/prince_low_poly_warrior.bdae`, then
`data/3d/textures/prince-warrior.tga`. It resolves 18 bone scope IDs and
renders 335 vertices/1,092 indices. Drag rotates the textured pose. The
diagnostic camera maps character Z-up coordinates to the display's Y-up
basis; recovered data stays unchanged.

The exact 213,536-byte APK, SHA-256
`664595b19de049d9a745d6e3b8f629ff1d65c9121abf927de895f6ad4be8fdb8`,
was installed and pulled back with matching hashes on both Android 17 page
sizes. Both previews visibly showed the warrior and changed angle after a
drag. [The runtime record](character-runtime-validation.json) records this
check. [Skin payloads](../skin-payloads/README.md) documents the parser,
original ARM palette comparison and safety checks.

This fallback selects one locally resolved controller. It does not assemble
modular equipment, skin normals or provide gameplay. The
older scene runtime records below belong to earlier APKs.

This is a new Android APK built from the repository's checked C++ BRES, scene, mesh, material and PVRTC readers. It uses a small Java file-picker UI and an OpenGL ES 2.0 shader to draw checked static triangle commands from an owner-supplied BRES file. It walks the scene hierarchy, applies each command's world transform, retains all three world coordinates, and combines the triangles into one bounded diagnostic draw. The preview has an oblique 3D camera, depth buffer, and touch rotation. The native libraries are built for `arm64-v8a` and `x86_64`, and the manifest targets API 37. The APK contains no original game code, game assets, cache, or proxy engine.

From the repository root, with JDK, Android SDK platform 37, build-tools 35, and NDK r29 installed:

```powershell
python port/android-app/build.py --sdk PATH_TO_ANDROID_SDK --ndk PATH_TO_NDK_R29
PATH_TO_ANDROID_SDK/platform-tools/adb install -r port/android-app/build/dh2-source-renderer-debug.apk
```

Open **DH2 Source Renderer**. Tap **Import BRES scene** and select `data/3d/modules/void_maze/void_maze.bdae` from a private copy of the supplied cache. This larger sample yields 77 static commands, 3,140 vertices, and 4,695 indices; its first resolved material diffuse image is `env_voidmaze.tga`. Tap **Import PVRTC texture** and select `data/3d/textures/env_voidmaze.tga`, then drag the preview to rotate the view. The smaller `data/3d/animateddecors/candle_flame.bdae` plus `data/3d/textures/env_crypt.tga` sample also remains usable. Android's file picker gives the app access only to selected files; it does not require broad storage permission. The source files are read into bounded memory, and the decoded image and scene buffers are not persisted by the app.

`build.py` compiles both ABIs and checks every native `PT_LOAD` segment for 16 KiB alignment. It also checks APK ZIP alignment and signature. The output APK and local debug signing key stay in ignored `build/`. The tracked [`build-validation.json`](build-validation.json) records the current binary hashes; [`scalar-track-runtime-validation.json`](scalar-track-runtime-validation.json) records this build's emulator check. The earlier scene check is in [`scene-3d-runtime-validation.json`](scene-3d-runtime-validation.json). Earlier builds' evidence remains in [`scene-runtime-validation.json`](scene-runtime-validation.json) and [`runtime-validation.json`](runtime-validation.json). The APK and screenshots remain outside Git.

The earlier 3D scene APK was installed on Android 17 x86_64 emulators with both 4 KiB (API 37.0) and 16 KiB (API 37.2) pages. On each it loaded the real 199,300-byte void maze BRES, reported 77 draws/3,140 vertices/4,695 indices, decoded its 256Ã—256 PVRTC texture, and visibly displayed textured stone geometry. A touch drag changed the 3D view angle, and the process remained alive. The candle sample also loaded and displayed on the 4 KiB emulator. ARM64 was checked structurally but has not run on an ARM64 device or emulator.

This app is an asset renderer milestone. Its coordinate normalization, orthographic camera, shader, texture sampling, and UI are new diagnostic choices. It applies one imported texture to every command, with no per-command material/shader state, transparency rules, or draw ordering reconstruction. A BRES with more than 256 static commands, 8,192 vertices, or 24,000 indices is rejected rather than partially drawn. It does not include the original game engine, level streaming, original animation state control, combat, input controls, audio, saved-game handling, or gameplay. Importing a BRES and a texture does not verify that the chosen files belong together; use the filename returned by the material link. The original rights situation remains described in [`RIGHTS.md`](../../RIGHTS.md).

### Repeat the two-motion check

Use the same runtime script with the primary dual-walk arguments
`--animation data/3d/characters/prince/animations/prince_walk_dual.bdae --tracks 25 --duration 799`
and add `--blend-animation data/3d/characters/prince/animations/cs_darkqueen_scene03a_prince.bdae --blend-tracks 29`.
The script stages four owner fixtures, checks the second import and 0/50/100%
mixing, then checks advancing Play and stable Pause. Inspect all three mix
screenshots; status text alone does not prove a changed character pose.
