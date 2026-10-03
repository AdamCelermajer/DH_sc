# DH2 native Android reconstruction

Latest saved checkpoint: [dh2-native-prince-bank-4f5b7d11.apk](build/checkpoints/dh2-native-prince-bank-4f5b7d11.apk),
20,726,897 bytes, SHA-256 `4f5b7d11891e575793f0b5e99056fe5d3cf3cf7a1a705f7ee0f5b632ba19bd82`.
The live Prince now uses native two-slot playback with all116 exact animation
resources and158 original registration occurrences, including template1111 and
zero-track1138. Source dictionary IDs resolve through the first canonical
resource index; defaults, event payloads, root history and slot metadata retain
owned resources. The APK bundles233 assets and14 ELF64 libraries for ARM64 and
x86_64, with16KiB ELF/ZIP alignment and no original ARM32 runtime.

[Checkpoint validation](reports/prince-bank-checkpoint-validation.json) passes
repository/Studio builds, all bundled assets, source-bound host bank/coordinator
audits and four API37 emulator suites: Walk/Run, stationary/moving combat,
rotation/pause/resume, and complete bank freeze/resume/recreation. World inspection
preserves the composed pose and scene/physics/actor clocks. Development context
recreation restarts the selected sequence; interrupted fades are not restored.
The [source snapshot](build/checkpoints/dh2-native-prince-bank-4f5b7d11-source.zip)
preserves frozen compiler inputs and APK assets. [Current scope](reports/prince-bank-source-status.md)
records what is verified and what remains unfinished. Full CharAI/AIS/Lua,
inventory/equipment, quests, original UI/audio/saves, full GPU/pose parity and
physical ARM64 testing remain unfinished. The native reconstruction goal is active.

Earlier saved checkpoint: [dh2-native-character-timing-6d9782be.apk](build/checkpoints/dh2-native-character-timing-6d9782be.apk),
18,805,603 bytes, SHA-256 `6d9782be431a8a70dabacf5932da4f40ccff2f59ec18a00d6f1f9194b6839cef`.
Recovered character timers now run after physics Step and before state/animator
updates. Attack blur starts the authored delay timer; expiry clears the source
gate before state handling. The original stance getter supplies stance 0 for the
current empty inventory and authored count 5. Both actual APKs pass 6,392 timing,
stance and state cases and 5,786 typed contribution cases each. The typed audit
executes genuine packaged quaternion math across library boundaries.
Stationary/moving combat, Walk/Run/body coordinates, rotation and pause/resume
pass on the API 37 x86_64 emulator. These live player attacks did not exercise a
nonzero delay expiry: the authored Basic/Player AI rows used by the four original
Knight spawn sheets both specify 0 ms ([input inspection](../../.local-inputs/character-timing-authored-delay.json)).
At that checkpoint, full AI callbacks and two-slot live blending were unfinished.
The [timing validation](../level-world/reports/character-timing-source-validation.json)
binds this APK, 189 assets and frozen compiler inputs; later animation-source
edits are explicitly outside this build. The [source snapshot](build/checkpoints/dh2-native-character-timing-6d9782be-source.zip)
preserves its compiler inputs and bundled assets. No original ARM32 engine is
packaged. Physical ARM64 testing and the complete game remain unfinished.

Source work developed after that earlier APK: [raw compiled sampling](../engine-animation/reference/compiled-transforms/NOTES.md)
passes 11,656 original/ARM64 cases and sanitizer replay; [character animation-event routing](../level-world/reference/animation-event-routing/NOTES.md)
passes 1,344 cases and 2,422 ordered callbacks. Native
[AI animation consumers](../level-world/reference/character-animation-ai/NOTES.md)
now compile into the main world library: 2,197 consumers, 90 lookups and 4,530
ordered service requests pass both optimized ARM64 and sanitized source-linked
replay. The actual NDK ARM64 world ELF passes that same corpus and 17 guards,
with 16 KiB LOAD alignment; this machine-code audit supplies explicit borrowed
services and is not a device test. [Script selection](../level-world/reference/character-script-selection/NOTES.md)
passes 1,080 original/ARM64 and sanitized host cases. Source
[script initialization/promotion](../level-world/reference/character-script-lifecycle/NOTES.md)
passes 1,480 cases and 4,339 ordered requests, including nested initialization;
[combat queries](../level-world/reference/character-combat-queries/NOTES.md)
pass 4,401 cases plus 448 decoded-table bridge checks. Both now compile into
the main world library and pass source-linked sanitizer replay. Complete AIS
update/unload, Lua/common bindings, equipment ownership and live observer
integration remain unfinished. The independent
[review](../level-world/reference/animation-blend-composition-reentry/review.md)
corrects an earlier single-slot replay assumption: the source queries IsEnded,
not the loop flag, after SetClip. These component results do not prove a new
APK or full-game behavior.

The actual Prince factory uses `CDynamicAnimationSet::compile` at 0x62f61c.
Its default/channel rules differ from the static 0x660710 compiler covered by
the earlier static sampler proof. The [dynamic compiler](../engine-animation/reference/dynamic-compiled-transforms/NOTES.md)
now passes 117,030 original/ARM64 samples and sanitized host replay. The source-
linked two-slot coordinator passes 3,280 real-asset frames in EACH static and
dynamic domain, plus 48 scheduler, 72 selection and 2,160 public control cases.
The dynamic host model/default registration is an explicit fixture; the actual
Prince [registration/template producer](../engine-animation/reference/prince-registration/NOTES.md)
now identifies 158 ordered library occurrences backed by 116 unique resources,
with dictionary1111 `prince_template_anim.bdae` as the designated default.
Repeated loads append separate library entries; dictionary lookup retains the
first matching resource control identity. The model fixture is not that template.
All116 resource bytes are staged in source APK assets (42 new files), with
[staging evidence](../level-world/reports/prince-animation-assets-staged.json).
The dynamic compiled sampler now accepts their10 position-axis and7 angle
tracks after9,376 original/ARM64 sampling checks and sanitized host replay.
Type9 is an angle interpreter with an authored axis default, not a scale axis.
Registration now has a native occurrence/index adapter. The full116-resource/
158-registration bank passes26,228 samples through the production libraries,
including17 original game-ID mappings and sampling after borrowed Players are
destroyed. This proves bank integration/lifetime safety, not full-bank original
pose parity. Live integration still needs actor selection and renderer wiring. Current native
libraries build for ARM64 and x86_64, but these additions/assets are outside the
saved APK and do not establish full gameplay or live two-slot rendering.

Source libraries additionally include the actual1,322-row ItemTable reader,
selected script update/pause expiry and collision counter producer. Their
original-derived shared-library sanitizer replays pass; ARM64 and x86_64 builds
include them. Lua/common bindings, inventory ownership and real gameplay service
connections remain unfinished.

The following character-combat evidence applies to the preserved prior build.

The live Prince now uses recovered Idle/Move/Attack/Dead state services,
authored playback/root motion and synchronous animation events in the scene
phase before genuine Box2D Step. Finite sequence closure, animation swaps,
path, rotation and subobject coordination follow the recovered ordering.
Stationary and moving attacks dispatch native damage and return to Idle;
Walk/Run, body coordinates, orientation changes and held-input pause/resume
also pass on the API 37 x86_64 emulator. Both actual APKs pass 6,414 original-
derived ARM64 state/blend-weight cases each; sanitizer audits independently
check event order, swaps and scene/root composition. See the
[current validation](../level-world/reports/character-combat-source-validation.json).
Older checkpoint results below apply to their saved builds.
See [actor playback scope](../level-world/reference/actor-playback/NOTES.md)
and the current [roadmap](ROADMAP.md) for remaining gameplay boundaries.

Prior bundled Crypt checkpoint:
[dh2-native-character-combat-2241cfe9.apk](build/checkpoints/dh2-native-character-combat-2241cfe9.apk),
18,792,163 bytes, SHA-256
`2241cfe95b4aeae6ff949816fe00260e3ab5f19cec07a12de816d90aae9dcc1b`.
It contains 189 assets and fourteen source-built ARM64/x86_64 libraries, with
verified ELF/ZIP 16 KiB alignment. The complete game and physical ARM64 testing
remain unfinished.

The prior [locomotion checkpoint](build/checkpoints/dh2-native-actor-f6840449.apk)
and its [validation](../level-world/reports/live-actor-source-validation.json)
are preserved. Every checkpoint is a saved development milestone, not a
separate finished game or the full cache.

The current live actor supplies unarmed stance0/base properties. Four bounded
states and single-slot playback were integrated; complete AI, source timers,
equipment stance production and two-slot typed pose blending remain pending.
Death filter/body-removal services are connected and audited as components,
but these two live combat cases do not exercise death. Debug builds expose a
shell/system-permission attack command to test held movement without Activity
relaunches; it calls the same native entry point as the Attack button.

Open this directory as an Android Studio project. It builds a single debug APK with source-built ARM64 and x86_64 libraries, original Crypt room geometry, Prince movement and 95 authored object instances. The models, animations, character/model tables and textures needed by this checkpoint are bundled, with no separate cache installation or runtime download.

This app supports walking with the animated Prince in the original authored eight-room Crypt, with 11 independently animated monsters and 84 animated decors, plus scene and texture inspection. The Prince can attack nearby monsters, and a Crypt skeleton now acquires and attacks him automatically using recovered faction, range and target-event rules. Debug commands separately exercise original monster walk, attack and death sequences. It connects the reconstructed resource/mesh readers, [texture module](../engine-textures/README.md), quaternion math, [scene/material loader](../scene-materials/README.md), [node playback](../engine-animation/README.md), [character skinning](../engine-skinning/README.md), [original data tables and scheduler](../game-data/README.md) and [level/object module](../level-world/README.md) to a new GLES2 renderer. It is not a complete playable game. Original lighting/shaders, game camera/navigation, procedural levels, complete AI/combat, equipment, audio, UI and saves still need reconstruction. The broader active goal is tracked in [ROADMAP.md](ROADMAP.md).

## Build

The latest source work now includes the matching original
[Box2D 2.0.1 backend](../physics-backend/README.md), with DH2's recovered capacity
settings and an aligned 64-bit allocator. Genuine shapes, mass, broadphase,
contacts, solver, continuous collision and world stepping execute from source.
Original gameplay contact dispatch, native body pin/unpin/Stop, and
[visual root motion](../level-world/reference/visual-motion/NOTES.md) are also
recovered. The [bound validation report](../level-world/reports/physics-backend-source-validation.json)
records 40 complete physics scenes, 1,776 genuine-body comparisons, 13,642 new
kernel comparisons per APK, 3,567 real animation samples per APK, sanitizer
checks and ten fresh emulator cases. Both repository and Studio builds pass
with ARM64/x86_64 libraries and 16 KiB alignment. Tests execute real native
physics; their construction fixtures are excluded from the APK.

The current build also includes recovered movement policy/property speed,
decor collision definitions and the original absolute-time animation timeline
with its callback and same-clip restart rules. These add 15,266 original-vs-ARM64
comparisons per APK, with sanitizer host replay and zero mismatches; see
[earlier timing validation](../level-world/reports/actor-timing-source-validation.json).
[Frame-order evidence](../level-world/reference/frame-order/NOTES.md) establishes
scene animation before physics stepping, followed by character scheduling,
path, rotation and subobject synchronization. The live source now composes
these phases for Prince locomotion. [Playback audit](../level-world/reports/actor-playback-host-audit.json)
checks original timeline/notification/replay state and authored root samples;
[actor runtime audit](../level-world/reports/actor-runtime-host-audit.json)
checks the real body, floor/registry and path/rotation/subobject composition
under ASan/UBSan. Neither audit is a full original frame/FSM oracle.

[Decor scene bounds](../level-world/reference/decor-scene/NOTES.md) evaluate
actual authored marker matrices for all 82 collider placements. Unmarked visual
decors receive no invented body. [Character scene bounds](../level-world/reference/character-scene/NOTES.md)
use the selected four warrior controllers' authored joint boxes and cached
factory pose, followed by source visual scale and collision scale. Knight's
base Scale_X/Y/Z values 100/100/100 produce visual scale (.9,.9,1); resolved
Collision_Scale 85 feeds original owner AABB scaling/padding before body creation.
Owner XYZ is passed unchanged to the visual root. Source save/equipment and
complete clone/cache producers remain explicit inputs.

Touch destination production, the follow camera, full Character/Prince AI,
blend ownership, enemy pursuit, avoidance policy and complete combat/frame
ordering remain unfinished. Locomotion type hysteresis and the four bounded
state policies now execute recovered source; general gameplay producers remain
separate reconstruction work.

The saved heading checkpoint is `build/checkpoints/dh2-native-heading-ec46a65d.apk`.
Its [validation](reports/build-validation-navigation-heading.json) binds both
builds to 4,476 heading comparisons each, the nine sanitizer audits, ten fresh
world cases and 112 player attack results. At that checkpoint, movement and melee
facing used the recovered helpers while original position advancement and enemy
pursuit remained pending. The APK retains 126 bundled prototype assets and verified 16 KiB
ELF/ZIP alignment. The full cache and physical ARM64 testing remain unfinished.

Reproduce the world check with `tools/world_smoke.py --heading-control` and the
combat check with `tools/player_combat_smoke.py`, supplying their usual APK,
ADB, fixture and output arguments. Their outputs belong in
`.local-inputs/world-tests-navigation-heading` and
`.local-inputs/player-combat-tests-navigation-heading`. Then run
`python port/android-native/tools/validate_navigation_heading_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2`
from the repository root with the locally prepared Python dependencies. See the
heading notes for the modeled-libm comparison boundary.

The checked-in project uses AGP 9.4.1, Gradle 9.6, Android SDK 37, NDK 29.0.14206865 and CMake 3.22.1. Configure your SDK location through Android Studio or a local `local.properties`. The tested JDK is Android Studio's bundled JBR. Minimum Android API is 24; physical phone compatibility remains unverified.

Supply your own original cache ZIP; original texture assets are deliberately ignored by Git. From this directory:

```powershell
uv run python tools/bundle_samples.py --cache "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip"
uv run python ../level-world/tools/prepare_world.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --output app/src/main/assets/worlds
uv run python ../level-world/tools/prepare_actors.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --project .
uv run python ../level-world/tools/prepare_animation_tables.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --project .
uv run python ../level-world/tools/prepare_actor_states.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --project .
uv run python ../level-world/tools/prepare_class_tables.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --project .
uv run python ../level-world/tools/prepare_player_combat.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --project .
uv run python ../level-world/tools/prepare_player_defender.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --project .
uv run python ../level-world/tools/prepare_ai.py "C:\path\Dungeon-Hunter-2-HD-v1-0-2-cache.zip" --project .
$env:JAVA_HOME = 'C:\Program Files\Android\Android Studio\jbr'
.\gradlew.bat :app:assembleDebug --console=plain
```

All nine preparation tools verify the known cache SHA-256 and ZIP entry CRCs. The sample bundler supplies preview and Prince assets; world preparation follows the authored room links; object preparation resolves character/model links and bundles twelve models. Animation preparation adds the complete five original animation tables; state preparation follows Idle/Walk/Attack/Died and their redirects to bundle 26 unique clips. Class preparation adds four original class files. Android reads the tables, computes cached base-class snapshots and schedules clips natively. Output: `app/build/outputs/apk/debug/app-debug.apk`. It does not bundle the full game's 6,833 cache files yet. Original assets and build products remain ignored by Git.

The existing local Android Studio project at `C:\Users\adamc\AndroidStudioProjects\dh2` contains the same viewer source and uses a `reconstruction-source` junction to the DH_sc checkout. Its CMake input points to the same reconstructed modules. The repo project instead resolves the checkout through `../..`; no junction is required there.

## Debug

Run the `app` configuration on an Android emulator. The Crypt opens by default; drag its movement control to walk. The camera follows the Prince and original floor triangles determine height and supported movement. Choose a preview model or texture in the selector to inspect it. Drag model views to orbit; projection aspect is preserved across rotations. Texture mode displays transparency over a checkerboard without stretching. Logcat tag `DH2Native` reports asset loading, GPU uploads, scene frames, surface size, locomotion clips, released player position and GL errors.

The candle scene has two textured quads / four triangles, with two scale tracks updating the hierarchy every frame. The swamp menu has three mesh instances / four draws / 2,146 triangles. Its light instance is counted and skipped. Scene transforms use the original engine's transposed-quaternion convention and post-scale order. The orbit camera, preview loop clock and unlit shader are new implementation choices, not recovered game behavior. Alpha-map red-channel selection, normalized vertex colors, triangle winding and transparency ordering have not been checked against the original game's GPU output. Scene visual parity is not claimed.

Useful breakpoint locations are `dh2_node_matrix`, `dh2::scene::load`, `dh2::animation::Player::sample`, `dh2_animation_lerp3`, `dh2_mesh_open`, `dh2_texture_decode` and `model_renderer::load`. Use an Android Studio debugger that includes native C++ debugging when setting C++ breakpoints. No native breakpoint attachment has been verified in this milestone.

To freeze a deterministic candle pose, start the app with `--es model candle_flame.bdae --ei time_ms 100` via `adb shell am start`; force-stop this development app first so the intent applies to a fresh activity. Omitting `time_ms` enables live playback. The frozen time is restored after screen rotation.

The local IDE MCP client is `../engine-textures/tools/ide_rpc.mjs`. It accepts a JSON argument or `@arguments.json`; set `DH2_IDE_MCP_URL` to the current Android Studio MCP endpoint if the port changes. IDE `build_project` returned success with no problems, but that generic IDE check did not refresh the APK here. The verified APK builds use the Gradle `:app:assembleDebug` task.

## Verified emulator test

With the emulator running, from this directory:

```powershell
uv run --with pillow python tools/emulator_smoke.py `
  --adb "$env:LOCALAPPDATA\Android\Sdk\platform-tools\adb.exe" `
  --serial emulator-5554 --apk app/build/outputs/apk/debug/app-debug.apk `
  --output build/emulator-check
```

The harness installs the APK on a named emulator, checks its ELF architectures/load alignment, loads all five samples in portrait and two in landscape, checks Logcat and measures rendered image bounds from screenshots. It restores the previous emulator rotation mode and leaves the app open. It does not clear global Logcat.

[Recorded result](reports/emulator-smoke.json): seven uploads and aspect checks pass on the API 37 x86_64 emulator using the host RTX 4070 SUPER GLES backend. The emulator uses 4 KiB pages. ARM64 code was compiled and exercised by the instruction oracle, not run on a physical ARM64 Android device. These checks do not establish that the original game's green textures, scene scaling or crashes have all been fixed.

The [texture regression result for the 3D APK](reports/texture-regression-3d.json) repeats all seven uploads and aspect checks successfully after scene integration.

Run the additional scene test after installing the current APK:

```powershell
uv run --with pillow python tools/model_smoke.py `
  --adb "$env:LOCALAPPDATA\Android\Sdk\platform-tools\adb.exe" `
  --serial emulator-5554 --apk app/build/outputs/apk/debug/app-debug.apk `
  --output build/model-check
```

The scene test checks portrait and landscape submissions for both models, nonblank screenshot pixels, actual touch-driven orbit changes and GL context recreation. [Recorded scene result](reports/model-smoke.json). The parser also passed 10,000 mutated-input checks under ASan/UBSan. [213 local node matrices](../scene-materials/reports/transform-differential.json) match the original ARM32 instructions byte for byte when executing the compiled ARM64 reconstruction.

The [animation test](tools/animation_smoke.py), with the same arguments as the scene test, checks fixed poses at 100 and 500 ms, frozen-frame stability and live changes in GPU pixels. [Recorded playback result](reports/animation-smoke.json), [current scene regression](reports/model-smoke-animation.json) and [build/source validation](reports/build-validation-animation.json). The ARM64 interpolation matches [284 original-instruction cases](../engine-animation/reports/lerp-differential.json), and the host playback/safety audit samples 2,033 clip times plus 5,000 mutated animation images.

The [current texture regression](reports/texture-regression-animation.json) passes seven uploads and aspect checks against the same animation APK. The harness handles Android 37's package-update relaunch before checking a fresh activity's intent; it does not retry process crashes or GPU failures.

The Prince now renders from four original default-warrior meshes with the original atlas texture. Native CPU skinning follows 29 position/quaternion/scale tracks in the separate idle clip, across two segments. The camera fits the deformed character bounds; skin output already contains the joint world transforms. This preview equipment choice, camera, unlit lighting and loop clock are implementation choices. Original equipment behavior, shader output and game scheduling remain pending.

Run `animation_smoke.py` with `--model prince_modular.bdae --second-ms 1700` to check poses on both sides of the clip boundary. [Character playback](reports/character-smoke.json), [three-model orbit/rotation regression](reports/model-smoke-skinning.json), [texture regression](reports/texture-regression-skinning.json), and [build validation](reports/build-validation-skinning.json) refer to the character checkpoint APK. Earlier reports retain their tested hashes. The [skinning module](../engine-skinning/README.md) records host safety and 500 original-instruction comparisons, with their scope limits.

The [world checkpoint](reports/world-smoke.json) verifies walking down the original stairs, stopping at rubble, idle/walk playback, frozen-pose stability, rotation restoring position and backgrounding cancelling a held control. [Build validation](reports/build-validation-world.json), [three-model regression](reports/model-smoke-world.json), [character regression](reports/character-smoke-world.json), [candle regression](reports/candle-smoke-world.json) and [texture regression](reports/texture-regression-world.json) carry the same tested APK hash. Historical reports retain their earlier hashes.

To reproduce the world test, export the world-space floor JSON with the host `inspect_world` executable, then run:

```powershell
uv run --with pillow python tools/world_smoke.py `
  --adb "$env:LOCALAPPDATA\Android\Sdk\platform-tools\adb.exe" `
  --serial emulator-5554 --apk app/build/outputs/apk/debug/app-debug.apk `
  --floor path/to/world-floor.json --output build/world-check
```

The Crypt checkpoint uses an authored backup layout, not the procedural rule generator. The initial movement policy and follow camera are new implementations; original PF/GPU equivalence is unverified. Of 166 linked object records, 95 now render: 84 animated decors and 11 monsters selected through original character/model tables. The remaining 71 records retain their script conditions, spawn flags and template references for later factory/lifecycle reconstruction. Monster idle selection, speed, completion, loop decisions and redirect advancement follow reconstructed original behavior, with independent instance clocks. Seed 1 and the frame-time adapter are development policies; decor clocks remain shared. Monsters do not fight or run AI yet. Blending, root-motion and event dispatch remain pending. The cave entrance effect has three unsupported nontransform tracks, and its geometry is currently static.

For camera-only object inspection, add `--ei object_index 0` to a fresh world launch; index 0 focuses the skeleton without moving the player. Index 2 focuses a slime; 76 focuses a ghost. Moving the touch control restores the player camera. Pair this with `--ei time_ms 100` for repeatable poses. Original instance positions and all indices are recorded in bundled `actor-provenance.json`.

Run [objects_smoke.py](tools/objects_smoke.py) with the same ADB, serial, APK and output arguments as the model test. It inspects twelve model resources at original positions, compares skeleton/slime/ghost/candle poses, checks frozen/live playback and reloads objects after rotation and backgrounding. The [object host audit](../level-world/reports/objects-host-audit.json) covers 44,769 poses and the optimized skeleton's unbound targets; the [700-case arithmetic comparison](../engine-skinning/reports/objects-arm64-differential.json) includes the original three-matrix skin palette. The missing bind-shape multiplication was caught by screenshot review and corrected; earlier checkpoint reports remain historical.

[Object playback](reports/objects-smoke.json), [world movement/lifecycle](reports/world-smoke-objects.json), [preview regression](reports/model-smoke-objects.json), [Prince playback](reports/character-smoke-objects.json) and [texture regression](reports/texture-regression-objects.json) verify 53 screenshot/texture cases against APK `69d84af9...`. [Build validation](reports/build-validation-objects.json) checks both project builds, source synchronization, 72 bundled assets from 66 unique original entries, ELF/ZIP 16 KiB alignment and host/original-instruction audits. Stable local artifact: `build/checkpoints/dh2-native-objects-69d84af9.apk` (15,705,603 bytes). The final suite followed a cold emulator restart after an API 37 system-server watchdog failure; its cause was not established. Physical ARM64 devices remain untested.

## Native animation-table checkpoint

APK `ee1f0820...` (15,873,806 bytes) replaces Android's hard-coded monster idle filenames with original CharAnim/AnimTable/AnimDict links, reconstructed initial random/redirect selection and original playback speeds. Both repo and Studio builds pass; [build validation](reports/build-validation-states.json) verifies 80 bundled assets from 73 unique original entries, source synchronization, native ELF/ZIP 16 KiB alignment and the fresh [22 object cases](reports/objects-smoke-states.json) plus [10 world cases](reports/world-smoke-states.json). The earlier 53-case regression remains attached to its own historical APK.

Stable artifact: `build/checkpoints/dh2-native-states-ee1f0820.apk`. Reproduce validation with `uv run --with pillow python tools/validate_states_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2` after running the two smoke tools into `.local-inputs/objects-tests-states` and `.local-inputs/world-tests-states`, and the host table/idle audits. See [game-data](../game-data/README.md) for exact original reader/selector evidence, 3,000 malformed table checks, 24,103 idle poses and 10,000 ARM64 random-helper comparisons. This checkpoint uses seed 1 and shared object clocks; completion/advance callbacks, blending, combat, AI and the full playable game remain pending.

## Actor state inspection

From a running Crypt activity, select an original actor state without moving the player:

```powershell
adb shell am start -W --activity-single-top -n com.example.dh2/.MainActivity --es world crypt01.dwld --ei object_index 0 --es object_state Attack
```

Use indices 0, 2 and 76 for skeleton, slime and ghost. States are `Idle`, `Walk`, `Attack` and `Died`; `--ei time_ms 700` freezes the selected clip at its original absolute time. Omit time for live scheduling. Attacks complete three stages, death holds the final pose, and idle loops reselect original alternatives. Activity debug intents survive the pause/context-recreation handoff. Actor state persistence beyond this intent and original lifecycle behavior remain pending. Movement restores the Prince camera.

Run `tools/actor_states_smoke.py` with the usual ADB/serial/APK/output arguments to capture 24 cases, verify three-stage attacks, death completion and independent idle clocks. It uses single-top intents after the initial launch; Android may still recreate the GL context on pause. This is animation inspection, with no combat damage or AI dispatch.

## Native completion checkpoint

Stable artifact: `build/checkpoints/dh2-native-completion-5ca9bbf8.apk`, SHA-256 `5ca9bbf873b5fef31efa0dbfa4db34176f374d7c5c6c15c09a2cefe87e77b5f3`, 16,331,098 bytes. It contains 102 assets from 94 unique original entries and fourteen ELF64 libraries across ARM64/x86_64, with verified ELF and APK 16 KiB alignment. Both repo and Studio Gradle builds pass and their six app source files and all assets match. Earlier artifacts retain their own reports.

[Checkpoint validation](reports/build-validation-completion.json) binds the APK to [24 actor-state cases](reports/actor-states-smoke.json), [22 object cases](reports/objects-smoke-completion.json) and [10 movement/lifecycle cases](reports/world-smoke-completion.json). Host ASan/UBSan checks cover 45,218 poses in 26 clips and 6,966 scheduler completions; the isolated completion decision matches 3,000 original-instruction ARM64 comparisons. Screenshot review covers walk, attack and death on all three monsters. Original shading, fading and full engine timing are not established by these checks.

The first actor test run stopped during a verified API 37 WindowManager system-server watchdog; its cause was not established. A cold boot and corrected single-top lifecycle intent handoff preceded these successful suites. Physical ARM64 devices remain untested. Reproduce validation with `uv run --no-project --with pillow python tools/validate_completion_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2`; the validator documents the expected local test/audit paths. Native completion evidence and limits are in [game-data](../game-data/README.md).

## Native class checkpoint

Stable artifact: `build/checkpoints/dh2-native-classes-f41a1346.apk`, SHA-256 `f41a1346f1ea808bef165e6980293f8fdccbef7ca5896d95aaf099d06b5bfabf`, 16,424,577 bytes. The APK contains 107 assets from 98 unique original entries. Both project builds pass, app source/assets match, and all fourteen native libraries plus APK ZIP entries retain verified 16 KiB alignment. The full cache remains unbundled.

[Build validation](reports/build-validation-classes.json) binds this APK to [24 actor-state cases](reports/actor-states-smoke-classes.json) and [10 world movement/lifecycle cases](reports/world-smoke-classes.json). Every monster's cached base-class checksum matches the original-instruction snapshot across all 224 properties. The [class evaluator](../game-data/README.md) matches 3,568 complete property sheets, including all 448 original character records, and passes 3,000 mutated/truncated reads under ASan/UBSan. Earlier 56-case completion and 53-case object checkpoints retain their own APK hashes and reports.

Run `actor_states_smoke.py` with `--class-report ../game-data/reports/classes-arm64-differential.json` to check base snapshots as well as playback. The validator is `tools/validate_classes_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2`. Class loading and base snapshots are logged under `DH2Native`; the game UI does not expose these diagnostic values. Dynamic property resolution, live health initialization, enemy/player level selection, AI/combat, original shaders and physical ARM64 execution remain pending.

The full playable-game goal remains active. Dynamic properties, health initialization, animation events and combat are the next reconstruction work.

## Native property checkpoint

Stable artifact: `build/checkpoints/dh2-native-properties-f7676e5d.apk`, SHA-256 `f7676e5d079df44e48e36e427006000c8e1c63aeb36973e74807ab7df6dd2abc`, 16,426,761 bytes. It retains 107 bundled assets from 98 unique original entries. Default/type rows come from the already bundled original CharacterTable; no added cache files are needed. Repo and Studio builds pass, sources/assets match and all fourteen ELF64 libraries plus APK ZIP entries satisfy 16 KiB alignment.

[Build validation](reports/build-validation-properties.json) binds this APK to fresh [24 actor-state cases](reports/actor-states-smoke-properties.json) and [10 movement/lifecycle cases](reports/world-smoke-properties.json). Every monster owns base/saved/gear/resolved arrays; all eleven resolved checksums match original-instruction snapshots. The [native resolver and mutations](../game-data/README.md) match 119,168 original resolutions and 7,168 mutations. The host ASan/UBSan audit compares all 448 raw character sheets to original-instruction references and exercises precedence, wrapping, fixed-point health writes and invalid input rejection.

Run `actor_states_smoke.py` with both `--class-report ../game-data/reports/classes-arm64-differential.json` and `--property-report ../game-data/reports/properties-arm64-differential.json`. Validate with `tools/validate_properties_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2`. Diagnostic logs use `Property rules ready` and `Resolved properties`; gameplay UI remains unchanged. Prior checkpoint reports retain their own APK hashes.

Resolution consumes supplied sheets and ordered buff groups. Original equipment/buff sheet producers, uncached class reads, spawn health initialization, live combat, complete cache packaging and physical ARM64 testing remain pending. HP/MP retain their original unresolved values until their initialization is reconstructed. The full playable-game goal remains active.

## Native spawn-vitals checkpoint

Stable artifact: `build/checkpoints/dh2-native-vitals-6b97f48e.apk`, SHA-256 `6b97f48e7f71eec6227998001b117f0c6dd26faeedebe771f382a49b52e06ef9`, 16,444,657 bytes. It keeps 107 assets from 98 original entries, with verified fourteen ELF64 libraries and ELF/ZIP 16 KiB alignment. Both repo and Studio builds pass and app source/assets match.

[Validation](reports/build-validation-vitals.json) binds the artifact to [24 actor-state cases](reports/actor-states-smoke-vitals.json) and [10 movement/lifecycle cases](reports/world-smoke-vitals.json). Every Crypt monster now applies its original class through normal uncached source resolution and initializes HP/MP through the two-pass fill sequence recovered from `InitPost`/`Revive`. All eleven full resolved-sheet checksums, HP 12,160, MP 5,632 and both passes' requests match original-instruction fixtures. Diagnostics use `Spawn vitals`; no development health values are added to gameplay UI.

The [native source evidence](../game-data/README.md) covers 2,080 uncached class comparisons, 10,000 regeneration cases, and 448 complete character recalculation/initialization cases each. ASan/UBSan compares all four sheets before and after initialization and verifies a saved Level affects class formulas. A fresh 3,568-case cached-class regression and existing host class/property audits pass. Regeneration's discarded original debug-switch query blocks are omitted in the oracle; original cap/mutation/resolution instructions execute. Full `InitPost`/`Revive` is inspected for HP/MP call order, not emulated end to end.

Use the actor smoke tool with `--class-report ../game-data/reports/classes-regression-vitals.json`, `--property-report ../game-data/reports/properties-arm64-differential.json` and `--vitals-report ../game-data/reports/vitals-arm64-differential.json`. The world tool installs and checks the device's APK SHA before exercising movement. Run `tools/validate_vitals_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2` after both suites and host audits. Android Studio is open at the new `game-data/vitals.cpp` source.

The fresh HP/MP helper reconstructs that portion of initialization. Real attack/damage events, timed regeneration, death/revive state transitions, saved-state restoration, dungeon level selection, equipment/buff producers, full cache packaging and physical ARM64 testing remain pending. Debug attack/death commands remain animation inspection. The playable-game goal remains active.

## Native animation-event checkpoint

Stable artifact: `build/checkpoints/dh2-native-events-bc4d957e.apk`, SHA-256
`bc4d957e2e2d9f3b08e2f8c1d419d50d73a4bd747984719c7d15d99e798fff99`,
16,472,473 bytes. The single APK contains the same 107 assets from 98 original
entries and fourteen ELF64 libraries with 16 KiB ELF/ZIP alignment. Both native
Android builds pass, and Studio app source/assets match the repository.

The [event source](../engine-animation/events.cpp) now reads and dispatches the
original `attack_mainhand` signals from five attack clips. Event managers are
independent per monster. [Original-instruction verification](../engine-animation/reports/events-arm64-differential.json)
compares 30,775 calls and 109,462 callbacks, including repeated boundaries,
duplicates, frame/millisecond keys and a loop wrap. The sanitizer reader audit
checks 38 original actor images and 5,000 malformed images; the fresh transform/
skinning regression still samples all 26 clips at 45,218 millisecond positions.

[Checkpoint validation](reports/build-validation-events.json) binds this APK to
[24 actor-state cases](reports/actor-states-smoke-events.json) and
[10 movement/lifecycle cases](reports/world-smoke-events.json). Live skeleton,
slime and ghost attacks each emit one event, with the exact expected callback
lag; tested death clips emit none. All eleven class/property/spawn-vitals
snapshots continue to pass. The world suite checks the installed APK hash.
Android Studio is open at `engine-animation/events.cpp` through its MCP.

Use the actor smoke tool's three property/vitals report arguments from the
previous checkpoint plus `--event-report ../engine-animation/reports/events-arm64-differential.json`.
Run `tools/validate_events_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2`
after the fresh host and emulator suites (Python requires Pillow for the shared
APK inspector). Frozen poses do not dispatch events. Logs expose
`Actor event track ready` and `Actor animation event`; gameplay UI is unchanged.

These callbacks currently record the authored signals. Original event-to-state-
machine routing, target selection, damage, full timeline scheduling, FX/audio,
root motion, complete cache packaging and physical ARM64 testing remain pending.
This is a reconstruction checkpoint, not a complete playable game. The goal
remains active.

## Native damage and attack-event checkpoint

Stable artifact: `build/checkpoints/dh2-native-combat-19f2f6eb.apk`, SHA-256
`19f2f6eb1fe6e57bbba5d33847488ac9369321ae6cad9d25a42b8acd0b3d8008`,
16,472,601 bytes. It retains 107 bundled assets from 98 original inputs and
fourteen ELF64 libraries. Both Android builds pass, Studio source/assets match,
and ELF/ZIP 16 KiB alignment passes. The validator checks that damage, DoT,
weapon bonus, RNG and event routing are defined exports in the packaged
game-data libraries for both ABIs.

The [native combat source](../game-data/combat.cpp) matches 23,792 original
calculations, and [attack-event decisions](../game-data/combat_events.cpp) match
6,000 original calls. The host sanitizer audit independently compares 1,792
original character damage outputs/RNG states and all 6,000 event decisions;
invalid calls commit no output. The fresh linked vitals regression continues
to match all 448 owners. Historical event-reader and pose checks are retained
only after verifying their source hashes are unchanged.

[Build validation](reports/build-validation-combat.json) binds the saved APK
to fresh [24 actor checks](reports/actor-states-smoke-combat.json) and
[10 world checks](reports/world-smoke-combat.json). Skeleton, slime and ghost
each dispatch exactly one melee request from the authored main-hand signal,
preserving outer sequence and inner attack-step arguments. Frozen inspection
and death clips produce no requests. All eleven class/property/spawn-vitals
snapshots pass. Touch movement, stairs, boundary blocking, rotation, background
touch cancellation and the installed APK hash pass. Physical ARM64 execution
remains untested. Studio is open at `game-data/combat.cpp` through its MCP.

Use the actor tool with the previous four reference-report arguments plus
`--combat-event-report ../game-data/reports/combat-events-arm64-differential.json`.
Run `tools/validate_combat_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2`
after the fresh suites and host audits; the shared inspector requires Pillow.
Original input/reference binaries remain ignored by Git.

The damage helpers and state-5 routing are reconstructed. `Actor combat action`
logs a request with `execution pending`: target acquisition, hit rolls,
combat-result application, real health/status/death transitions and equipment
capability fallback remain pending. Other event categories and the full
timeline/AI lifecycle are still incomplete. The full cache, game UI/audio/saves
and finished playable game are not delivered by this checkpoint. The goal
remains active.

## Native hit/result checkpoint

Stable artifact: `build/checkpoints/dh2-native-combat-result-8a2c2571.apk`,
SHA-256 `8a2c257135c720773a59dba97ba7a98a5953c240456673dd5c332bba0541d8b8`,
16,491,289 bytes. Both Android builds pass, Studio source/assets match, and all
fourteen ELF64 libraries plus ZIP entries satisfy 16 KiB alignment. The APK
retains 107 assets from 98 original entries. The validator confirms defined
result and melee exports in both packaged native game-data libraries.

[Native source](../game-data/combat_result.cpp) reconstructs the complete
ten-word hit/status/damage result and melee request/category/mask construction.
[Original comparisons](../game-data/reports/combat-result-arm64-differential.json)
match 9,792 complete result/RNG states plus 2,000 melee requests, including all
nine hit/status outcomes, wrapping extremes and original float conversion
order. ASan/UBSan compares the same 9,792 captured original outputs. The melee
alternate flag's optional buff-removal side effect remains unimplemented; the
oracle supplies an empty buff dictionary and records that scope.

[Checkpoint validation](reports/build-validation-combat-result.json) binds
the APK to fresh [24 actor cases](reports/actor-states-smoke-combat-result.json)
and [10 world cases](reports/world-smoke-combat-result.json). All eleven
Crypt actors execute the packaged melee/result calculator at initialization
using supplied unarmed attacker/defender sheets and a copied test RNG. Their
ten-word outputs and RNG state match original snapshots. These development
probes log `validation only`, leave HP/state untouched and do not alter the
animation RNG. All class/property/vitals/event checks, three-stage attack
requests, death end poses, movement, rotation, touch cancellation and installed
APK hash checks pass. Prior unchanged reader/pose/helper evidence retains its
own original report/ABI hashes.

Add `--combat-result-report ../game-data/reports/combat-result-arm64-differential.json`
to the actor tool's previous five reference-report arguments. Run
`tools/validate_combat_result_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2`
after the fresh host/emulator checks; Pillow is required. Studio is open at
`game-data/combat_result.cpp` through its MCP. Original inputs and large oracle
fixtures remain ignored by Git.

Target acquisition, result application to health/status/death, combo/aggro
ownership, complete AI/lifecycle, full cache packaging, gameplay UI/audio/saves
and physical ARM64 testing remain pending. Authored animation events still
produce requests marked `execution pending`; the probes do not make this a
playable combat build. The full game goal remains active.

## Native health checkpoint

Stable artifact: `build/checkpoints/dh2-native-health-b348f872.apk`, SHA-256
`b348f872d0434824370bed7c4fcf845732375428593b0149edfb9a7c5d7fcbe5`,
16,492,473 bytes. Both Android projects build; source/assets match, fourteen
ELF64 libraries and their ZIP entries satisfy 16 KiB alignment, and the
bundled game-data libraries export `dh2_health_hit` for ARM64 and x86_64.
The original engine is absent from the APK. The payload remains 107 assets
from 98 original inputs; full cache packaging is still pending.

[Health source](../game-data/health.cpp) reconstructs `Character::HitFor`'s
property writes, session/debug gates, kill and lifecycle requests, dead-target
skip, and low-health hysteresis. The warning rearms at 75% HP inclusive using
the original float conversion/comparison. Offline damage requires a present,
living main player; online damage permits a missing game object or session
state 0/5. Raw subtraction wraps, HP writes use the original property rules,
and the lifecycle value 3 is kept separate from the `IsDead` byte.

[Original comparisons](../game-data/reports/health-arm64-differential.json)
match all four property sheets and eight output words in 10,926 cases:
10,000 synthetic inputs, 30 threshold boundaries and 896 original character
inputs. Original health and property instructions execute. Caller fixtures
supply classification, remoteness, session and debug facts; controller kill
requests are observed without executing the kill backend. Tutorial, audio,
party credit and achievement services are omitted. The buff dictionary is
empty. These limits are recorded in the comparison report.

[Checkpoint validation](reports/build-validation-health.json) binds the APK
to the same 10,926 cases under ASan/UBSan, unchanged 9,792 combat-result and
448 class/spawn host regressions, fresh [24 actor checks](reports/actor-states-smoke-health.json)
and [10 world checks](reports/world-smoke-health.json). The Android build
executes two health probes per Crypt actor: copied HP 12,160 becomes 11,904
after raw damage 256, and raw damage 16,384 requests a kill with HP zero.
All 22 packaged outputs match original snapshots. The probes use independent
copies, leave live actor HP/state untouched, and log `validation only`.

Add `--health-report ../game-data/reports/health-arm64-differential.json`
to the actor tool's previous six reference-report arguments. Build the
isolated ARM64 oracle with `../game-data/tools/build_health_oracle.ps1` and
run `tools/validate_health_checkpoint.py --studio C:\Users\adamc\AndroidStudioProjects\dh2`
after the host/emulator suites; Pillow is required for APK inspection.
Studio is open at `game-data/health.cpp` through its MCP. Original inputs
and the large captured fixture remain ignored by Git.

Full `F_ApplyResult`, target acquisition, combo/aggro/status ownership,
the kill backend and death state transition remain pending. Authored attack
events still log `execution pending`; this is a health reconstruction
checkpoint, not a playable combat release. Gameplay UI/audio/saves, the full
cache and physical ARM64 testing remain pending. The goal remains active.

## Live native monster combat checkpoint

Stable artifact: `build/checkpoints/dh2-native-combat-application-39529cc6.apk`,
SHA-256 `39529cc6403c31b6c03eb0596ad512676513f862bfe9e95f8f5607e230998553`,
16,571,881 bytes. Both Android projects build and share identical source and
assets. Fourteen ELF64 libraries for ARM64/x86_64 and their ZIP entries satisfy
16 KiB alignment. The APK bundles 107 assets from 98 original inputs, includes
the native application export in both ABIs, and contains no original ARM32
engine. Full cache packaging remains pending.

[Native application source](../game-data/combat_application.cpp) reconstructs
the offline, single-player, nonplayer direct-health-owner path. It preserves
combo updates, staged float threat calculation, health and core kill writes,
death-dependent result suppression, HP-then-MP leech and ordered status
requests. [Original comparisons](../game-data/reports/combat-application-arm64-differential.json)
match both complete property owners, result/state words and service request
order in 6,914 cases, including 18 consecutive original melee hits. The host
audit repeats those captured cases under ASan/UBSan. Default debug switches,
a living main player and no gold-damage mask are required by this API;
aggro/status requests are returned for services that remain unfinished.

Authored `attack_mainhand` events now calculate and apply real hits to supplied
development target indices. Each actor owns its HP, combo and death state.
Lethal damage queues the original target death clip outside the animation
callback, and dead actors reject live-state commands. CPU actor state,
scheduler/event cursor and combat RNG survive renderer recreation. The fixed
combat seed and separate animation RNG are development policies; original
RNG initialization/sharing is not yet reconstructed.

[Checkpoint validation](reports/build-validation-combat-application.json)
binds this APK to [18 live hits and three automatic deaths](reports/combat-application-smoke.json),
two rotations and an in-flight pause/resume without repeated hits, frozen
attacks with no damage, and dead-target rejection. Fresh
[24 actor cases](reports/actor-states-smoke-combat-application.json) and
[10 world cases](reports/world-smoke-combat-application.json) pass alongside
the 22 health probes and eleven result probes. The actor suite exposed a
valid outer attack sequence 2; a separate expanded 6,000-call original event
fixture now covers runtime sequences 0, 1 and 2. Historical fixtures are
preserved. Fresh sanitizer health and damage/event regressions also pass.

Run `tools/combat_application_smoke.py --help` for live verification and
`tools/validate_combat_application_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after the host and emulator suites.
Use `../game-data/reports/combat-events-application-arm64-differential.json`
as the actor tool's event fixture for this checkpoint. The new validator and
shared inspector require Pillow. Studio is open at `combat_application.cpp`
through its MCP. Original inputs and large captured references remain ignored.

Targets are explicitly supplied through development commands; monsters do
not acquire enemies themselves. Player combat, full AI/death FSM, actual
aggro/status effects, kill rewards, impact FX/audio, inventory, UI/saves and
the complete asset cache remain pending. Physical ARM64 execution is untested.
This checkpoint is not the finished playable game; the full goal stays active.


## Touch-controlled Prince combat checkpoint

Stable artifact: `build/checkpoints/dh2-native-player-combat-503e2171.apk`,
SHA-256 `503e2171fbbbcc81fa4eba795088f94d045bfe21dfcca05dba6b082d645be99b`,
16,609,057 bytes. The APK bundles 117 assets, including 107 original inputs;
it contains fourteen ELF64 libraries for ARM64 and x86_64, with 16 KiB
ELF/ZIP alignment and no supplied ARM32 engine or translation runtime.
It installs as one APK without a separately copied cache for this checkpoint.
The complete game's cache is still pending.

The Prince initializes the original KnightPlayerBase fallback through the
verified normal base/vitals helpers. Nine original stationary combo clips
resolve from animation table 48 / AttackStatic root 243. The original
main-hand events execute the native melee/result and player-attacker
application path. A touch Attack button chooses a nearby living enemy and
plays the three-combo sequence while the Prince stays in place.

[Original player comparisons](../game-data/reports/player-application-arm64-differential.json)
verify 2,732 cases and 336 persistent melee attempts across three enemy types.
The [sanitized host audit](../game-data/reports/player-application-host-audit.json)
checks the same owner/result/state/request outputs. The nine original clips
also pass [3,475 skinned host poses](../level-world/reports/player-combat-host-audit.json)
with no unsupported or unbound tracks on the four warrior preview skins.

The [actual touch test](reports/player-combat-smoke.json) walks through the
Crypt stairs and attacks a skeleton 112 times with unmodified original base
stats. Every result word, health/combo/death state and RNG state matches the
original persistent reference through the original skeleton death animation.
Out-of-range and busy inputs are rejected; frozen inspection applies no
hits; two rotations and an in-flight background/resume do not replay hits.
A deferred command during rendering-surface recreation is retained.

[Checkpoint validation](reports/build-validation-player-combat.json) binds
these checks to source/asset hashes, both repository and Studio builds,
24 actor cases, ten world cases, and the existing eighteen live monster
hits/three automatic deaths. The 6,914-case nonplayer host regression passes.

Run `tools/player_combat_smoke.py --help` for the touch test, and
`tools/validate_player_combat_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2 --cache path/to/cache.zip` to
validate the captured checkpoint. Re-run the actor/world/monster tools into
the `*-player-combat` directories expected by the validator. Studio is open
at `app/src/main/cpp/model_renderer.cpp`; native source is shared through its
`reconstruction-source` junction.

The player application has an empty active critical-skill manager and
external achievement/aggro/status/reward/FX/audio services. Save/new-game
creation, equipment, player defenders, enemy AI, full UI/audio/saves and
full assets remain pending. The button's nearest-target range and whole-combo
policy do not establish original targeting, line-of-sight, approach or input
FSM parity. Original shaders, blending/root motion and complete GPU parity
also remain pending. Only the x86_64 API 37 emulator has run this APK;
physical ARM64 testing remains pending. The full game goal stays active.


## Native player damage and death checkpoint

Stable artifact: `build/checkpoints/dh2-native-player-defender-c47cb955.apk`,
SHA-256 `c47cb9553284981b3d33993ba6e4cdbea825407262930307f80f2af99e9cd18a`,
16,629,881 bytes. It bundles 119 assets, including 108 original inputs,
and fourteen ELF64 libraries for ARM64/x86_64 with 16 KiB ELF/ZIP alignment.
This is one APK for the current prototype; the complete game's cache and
physical ARM64 execution remain pending.

The native player-defender entry point now applies original health/death,
low-health hysteresis, idle hurt/dodge result mutations and saved
block/evade/knockdown counters. [Original comparisons](../game-data/reports/player-defender-arm64-differential.json)
cover 2,969 cases, including 73 persistent enemy-to-Knight attempts.
All owner sheets, result/state/request words and observed request order
match. The same fixtures pass [ASan/UBSan](../game-data/reports/player-defender-host-audit.json),
alongside unchanged 6,914 nonplayer and 2,732 player-attacker cases.

[Live Android checks](reports/player-defender-smoke.json) execute 24 attacks
each from skeleton, slime and ghost through low health and automatic Prince
death. Every result, health/combo/death/RNG state and resolved-property
checksum matches the original. Each pair emits exactly one low-health
warning request. A further real hit while the movement control is held
matches the original state-13 case; movement does not suppress its reaction.
A new HP/MP display reads native properties and marks defeat. Four rotations
and an in-flight background/resume retain health and death state. Defeated
movement/attack inputs and further enemy targets are rejected.

The original Died root 259 / clip 1023 now plays once and holds its final
pose. [Its host audit](../level-world/reports/player-death-host-audit.json)
checks all 1,400 millisecond poses and 27 bound tracks across four skins.
Full player FSM, blending/root motion, revive/game-over and save services
remain separate work.

[Checkpoint validation](reports/build-validation-player-defender.json)
binds the APK, source/assets and both builds to the player-damage checks,
a fresh 112-attempt Prince touch-combat regression, 24 actor cases and ten
world cases. Retained attack skinning checks cover all 3,475 previous poses.
Run `tools/player_defender_smoke.py --help` and
`tools/validate_player_defender_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2 --cache path/to/cache.zip`.
Studio is open at the shared `game-data/combat_application.cpp` source.

Enemy target `-2` explicitly selects the Prince for development commands.
Automatic enemy acquisition/AI, actual aggro/status services, warning audio,
player stats and threshold achievement dispatch are not implemented here.
The original aggression/target routines have been [captured for follow-up](../game-data/reference/aggro/original-functions.json).
The prototype's control/state hints and health display are newly implemented;
original controls/FSM/HUD remain pending. Equipment, quests/progression,
original shader/render parity, audio/saves and full assets still need work.
The full reconstruction goal remains active.

## Standalone native aggression module checkpoint

APK: `build/checkpoints/dh2-native-aggro-module-58fd4190.apk` (16,629,881 bytes),
SHA-256 `58fd41900139ff3c87a7b16fb4f8396ab4b0d44320726d2ad15cfc63e3c07901`.
The native game-data library now contains the reconstructed aggression table
service, with reciprocal incoming/outgoing relations, original gates,
float threat arithmetic and highest-threat selection. All 6,114 comparisons
pass for the standalone ARM64 oracle and the actual ARM64 libraries from both
the repo and Studio APKs. The sanitizer replay passes the same records and
checks atomic capacity/invalid-input failures. Finite values are checked bit
for bit; arithmetic NaNs are checked as unordered values without sign/payload
parity. See [the module](../game-data/README.md#native-aggression-tables).

[Validation](reports/build-validation-aggro-module.json) binds the source,
libraries, reports and unchanged 119 assets / 108 original inputs to this APK.
Only the two ABI copies of `libdh2_game_data.so` changed from c47cb955;
app sources and the other twelve native libraries remain byte-identical.
Both ABI sets remain ELF64 with 16 KiB ZIP and ELF load alignment.
The [fresh ten-case world regression](reports/world-smoke-aggro-module.json)
passes stairs, boundaries, rotation, idle playback and pause/resume. An initial
run was interrupted by the emulator's WindowManager/display watchdog killing
system_server; logs were preserved and the same AVD cold-booted without a wipe
before the successful run. The previous 112 player-attack and 73 player-damage
checks remain historical evidence for c47cb955, not fresh tests of this APK.

This is a module extraction checkpoint. The renderer still uses explicit
development enemy targets; **aggression tables are not yet integrated into
live combat or automatic AI**. Acquisition, pursuit, target/range/visibility
rules, callback/FSM services and death cleanup are next. No physical ARM64
test, full cache bundle or full playable game is claimed. Studio is open at
the shared `game-data/aggro.cpp` source. Revalidate with
`tools/validate_aggro_module_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after running the documented audits
and `world_smoke.py` against the checkpoint APK. The full goal remains active.


## Native enemy melee checkpoint

APK: `build/checkpoints/dh2-native-enemy-ai-08bd79f2.apk`.
[Build/source validation](reports/build-validation-enemy-ai.json),
[automatic enemy fight](reports/enemy-ai-smoke.json),
[player attack regression](reports/player-combat-smoke-enemy-ai.json), and
[world regression](reports/world-smoke-enemy-ai.json).

Automatic targeting/standing attacks are enabled
in the Crypt scene. Original AI/faction data, 3D melee/sight rules and target
transition decisions are reconstructed; live threat relations are maintained.
The isolated combat/animation harnesses use `--ez enemy_ai false` explicitly.
The automatic enemy test supplies no development target/state command. Original
spatial query filters/order/timer, pursuit/pathfinding, full FSM/attack delay,
callback ordering and status/audio services remain pending. No physical ARM64
phone has been tested and the full native game goal remains active. See
`../game-data/reference/ai-target/NOTES.md` for the precise current contract.

## Native graph search checkpoint

Original graph-node search now builds from `level-world/navigation_search.cpp`.
The standalone oracle and both packaged ARM64 libraries match 1,084 original
requests, including all ordered Crypt floor pairs and four expansion limits.
The actual native floor loader produces the same graph; its search adapter
also passes the original corpus under ASan/UBSan. Android startup verifies
64 successful routes, 1,598 segments and checksum `5454073390c7d916`.
Ten fresh emulator cases verify rendering, supported-floor movement and
rotation/background restoration. The graph probe does not drive characters.

Revalidate with `tools/validate_navigation_search_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after the original differentials,
host replay, APK builds and `world_smoke.py --graph-search` checks.
[Validation](reports/build-validation-navigation-search.json) binds source,
assets, ELF hashes and live emulator evidence to the saved checkpoint APK.
[Scope](../level-world/reference/navigation-search/NOTES.md) records the exact
algorithm and supplied services. Original world endpoint/obstacle/radius
handling, FindPath/smoothing and moving pursuit remain next. Full gameplay,
full asset bundling, original GPU behavior and physical ARM64 testing remain
unfinished; the goal stays active.

## Native world-coordinate search checkpoint

The original PFWorld wrapper now searches from source/target world positions
through native room/floor collision, midpoint attachment, radius/capability
predicates and the recovered graph search. Each ARM64 binary matches 525
original cases. The actual BRES/DWLD world repeats every cache session and
output under ASan/UBSan. Android startup verifies all 64 ordered floor pairs,
2,086 segments and checksum `3a3ab2a724cc383b`; ten fresh emulator cases also
check rendering, stairs, boundaries and GL recreation.

[Checkpoint validation](reports/build-validation-navigation-world.json) binds
the APKs and original references. [Scope](../level-world/reference/navigation-world/NOTES.md)
records direct-edge and cache semantics, supplied scene ownership and the
ordinary collision mode under test. Revalidate with
`tools/validate_navigation_world_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after the audits and
`world_smoke.py --world-route` run. Full FindPath/smoothing, cache lifecycle,
waypoints and moving pursuit remain pending. The live probe does not drive
characters; full gameplay, asset bundling and physical ARM64 tests remain open.


## Native FindPath and waypoint checkpoint

Original FindPath now drops the previous route, performs world-coordinate search,
refines the first portal and calculates the waypoint. MovePath, DropPath,
source-plane passage checks and per-edge squared lengths are reconstructed.
The standalone and both APK ARM64 binaries pass 366 complete FindPath requests,
661 line-intersection cases and 4,509 path operations. ASan/UBSan replay these
corpora; the complete wrapper loads the actual authored Crypt assets.

Android startup checks 64 FindPath routes, including previous-path replacement:
2,086 segments and checksum `9356b419cae2bc57`. Ten emulator cases retain
rendering, supported-floor movement and lifecycle checks. Characters do not yet
use these paths. Original position/direction validation, obstacle response,
actor initialization, cache invalidation and movement/pursuit remain pending.

[Scope](../level-world/reference/navigation-path/NOTES.md) documents the original
incremental smoothing and source-plane waypoint test. Revalidate with
`tools/validate_navigation_path_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after the instruction/host audits,
builds and `world_smoke.py --findpath` run.
[Validation](reports/build-validation-navigation-path.json) binds source,
original references, both APKs and live emulator evidence. Full gameplay,
full asset bundle, original GPU parity and physical ARM64 testing remain open.


## Native floor motion checkpoint

Original height/normal lookup, cached-floor position validation, capability and
strict height checks, finite-segment intersection and floor-boundary sliding are
now C++ source. All three ARM64 binaries match 2,353 original cases and 2,860
ordered floor queries. The sanitizer replay uses actual Crypt assets, including
six atomic caller rejections and complete path/search regressions.

The live motion probe accepts all 64 floor-pair position/direction requests and
matches original checksum `d60ad48039cc85c6`; ten emulator cases retain rendering,
supported-floor movement and lifecycle checks. The original controller is still
pending. The probe does not move characters or implement dynamic obstacles.

[Scope](../level-world/reference/navigation-motion/NOTES.md) distinguishes
reconstructed floor motion from pending obstacle-parent lists/forces, actor
InitObject/InitObstacle producers and speed/root-motion/pursuit. Revalidate with
`tools/validate_navigation_motion_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after original/host audits,
builds and `world_smoke.py --floor-motion`.
[Validation](reports/build-validation-navigation-motion.json) binds both APKs,
source, gold and live evidence. Full game flow, asset bundling, original GPU
parity and physical ARM64 testing remain unfinished; the goal remains active.

## Native object and obstacle registry checkpoint

Original motion/obstacle defaults, InitObject, InitObstacle, capability flags and
floor-list relocation are now recovered C++. Position validation calls the actual
native registry backend. All three ARM64 binaries match 1,093 original requests,
including duplicate membership, null floor keys, missing old membership and deque
block growth/erase. Sanitizers replay the actual assets and nine atomic rejection
cases alongside the complete navigation regressions.

The startup probe initializes/registers objects across all 64 floor pairs, accepts
all positions and performs 56 relocations, matching checksum `22a98dfc4102db51`.
Actor movement still uses the supported-floor adapter. Original forces/avoidance,
actor producers and character controller remain pending. See
[scope](../level-world/reference/navigation-objects/NOTES.md) and
[validation](reports/build-validation-navigation-objects.json). Revalidate with
`tools/validate_navigation_objects_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after instruction/host comparisons,
both builds and `world_smoke.py --native-objects`. Full game flow, all assets,
GPU parity and physical ARM64 testing remain unfinished; the goal is active.

## Concrete actor obstacle producers

Recovered GameObject::UpdatePFObject, five concrete obstacle trait groups and
physical radius conversion in C++. Original registration runs before the radius
refresh; the null-user gate preserves existing state. All three ARM64 libraries
match 1,238 original comparisons. Actual-asset sanitizer replay, ten atomic
rejection checks and all complete navigation regressions pass.

Android matches the eight-floor producer probe (`30c1f3ac1bf81145`), previous
navigation probes and ten emulator cases. See
[scope](../level-world/reference/navigation-producers/NOTES.md) and
[validation](reports/build-validation-navigation-producers.json). Revalidate with
`tools/validate_navigation_producers_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after original/host comparisons,
both builds and `world_smoke.py --actor-producers`. These services currently run
in the startup probe. Physical bodies/filters/bounds, capability initialization
and the original moving controller still need reconstruction and integration.
Full game flow, all assets, GPU fidelity and physical ARM64 testing remain open.

## Native obstacle avoidance checkpoint

Original obstacle force accumulation, AvoidObstacles and concrete physical
collision filtering are now recovered C++. Each of three ARM64 binaries matches
2,008 original requests with zero mismatches. The actual-asset sanitizer replay,
eight atomic rejection checks and all previous navigation regressions pass.
Android matches the eight-floor avoidance probe (`09d1d79d2612c945`) and all ten
movement/display/lifecycle cases. Both APK builds and 16 KiB alignment checks pass.

Avoidance currently runs in the startup probe. The original actor producers and
controller still need reconstruction and integration. See
[scope](../level-world/reference/navigation-avoidance/NOTES.md) and
[validation](reports/build-validation-navigation-avoidance.json). Revalidate with
`tools/validate_navigation_avoidance_checkpoint.py --studio
C:\Users\adamc\AndroidStudioProjects\dh2` after instruction/host comparisons,
both builds and `world_smoke.py --avoidance`. Full game flow, complete assets,
GPU parity and physical ARM64 testing remain unfinished; the goal is active.
