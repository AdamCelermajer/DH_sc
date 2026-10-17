# SWAMP source preview and intro trace

The APK keeps the authored `void_maze` development encounter as its launcher
and opens the original SWAMP module zero in a separate diagnostic preview. The
preview imports `001_swamp.mlx` and its catalogue BRES, resolves source diffuse
and AlphaMap references, and assembles module zero at its MLX placement. It
also bundles module one's original `obj_3of4_brdwalk_sw_00.mgp`, which contains
the preloaded Lizard intro records. Previewing and running the trace do not
change encounter progress.

Module zero contains 103 scene records, 54 draw commands, 10,816 vertices and
13,284 indices. Forty-nine draws bind the source `env_swamp` diffuse texture;
22 reference the bundled AlphaMap. Its entrypoint record places the preview
warrior at `(1090.75, -212.202, 258)`. The renderer uses the cached model,
256x256 texture and `prince_idle_shield` clip (0–1,066 ms): 335 posed vertices,
1,092 indices and 18 joints. The diagnostic player draw depth-tests against
opaque module geometry. The source draw
`_module_obj_4of4_brdwalk_sw_00-node` / `ColorMaterial` is retained in the
imported 54-command scene, but omitted from the diagnostic preview because its
native Collada effect/pass state is unresolved. The preview renders the other
53 source draws. This avoids assigning synthetic transparency and draw order to
a material whose original blend and depth state are unknown. Five other draws
use muted solid-color fallbacks, and external effects, specular materials and
lighting are not reproduced. The omitted source draw is the verified visible
36-index root geometry. Its former preview fallback used muted green RGB with
synthetic alpha; this did not establish the original effect state. The runtime
status reports 53 drawn diagnostics / 54 source draw commands. The other five
no-texture commands include the source exit markers and `Standard_19`
floor/water draws; they remain in the preview.

The black bridge overlay is `Material__11598` on source draws 13 and 24. The
serialized `SRenderState` pass words (`0xff001111`, `0xffff00ff`,
`0x001c0f00`, `0x01583007`) and the original factor/equation maps resolve to
`GL_ONE, GL_ONE`, `GL_FUNC_ADD`, depth testing enabled, and depth writes
disabled. Both draws sample the fully opaque `env_swamp.tga`; under ordinary
source-alpha blending, black texture pixels cover the bridge. The preview now
applies the recovered additive blend and depth-write state to those two draws.
The host scene regression pins the policy to their four-triangle draw records.
The exact current APK was visually checked on Android 17/API 37 with both 4 KiB
and 16 KiB pages: the tall black prism is absent in both captures. A broad
dark/olive patch remains because the `Standard_19` floor and water draws still
use fallback materials; overall SWAMP material fidelity remains incomplete.
Native pass details are traced in the recovered
[`SRenderState` conversion](../../recovered/native/decompiled/libDungeonHunter2.so/functions-015.pseudo.c),
[`GL state application`](../../recovered/native/decompiled/libDungeonHunter2.so/functions-014.pseudo.c),
and [`state deserialization`](../../recovered/native/decompiled/libDungeonHunter2.so/functions-016.pseudo.c).

## Playable module-zero movement test

The lower-left X/Y touch pad moves a render-only source-coordinate actor through
a following isometric view. Its projection exposes changes to both horizontal
source axes; the controller retains X/Y/Z in the source world and starts at the
module-zero entry above. The Android activity advances movement in 20 ms fixed
steps and caps catch-up work per rendered frame. Touch release, cancellation and
activity pause clear the stick input. Source idle and walk clips are selected
from the input state, with animation root translation removed before applying
the controller's source position and heading.

The landscape preview keeps a two-line movement HUD at the upper left with the
current mode, source position, heading and path mask. The **Details** control
opens a scrollable panel for the longer scene/script checks, complete movement
report and trace transcript; it is closed during ordinary movement so the
player view stays clear. The lower-left joystick and lower-right trace/return
actions remain available while the panel is open.

Each movement step checks the current and candidate endpoints against a
path-eligible floor. The reader resolves the source `floortypes` user property;
the recovered `PFObject::CanPathOn` subset rule accepts untagged/zero-requirement
floors and otherwise requires `(floor_flags & object_mask) == floor_flags`.
Native `Void` and `Wall` category floors are excluded. The preview actor uses
the constructor-derived baseline mask `2`: source water (mask `2`) is eligible,
while a source hole (mask `1`, found in module 7) is not. SWAMP module zero has
no hole surface, so the source hole check and endpoint rollback are covered by
the host tests with a separate adjacent-floor fixture.

The movement check follows the native strict vertical acceptance boundary
`abs(candidate_z - floor_z) < 100`; successful native object-mode validation
writes the sampled floor height into candidate Z. Speed `30` world
units/second, module-zero restriction, and endpoint-only stepping are explicit
preview choices, not recovered player controller parity. The Details panel
retains the sampled floor tag and complete mask report. The compact HUD reports
a rejected candidate as blocked while holding the prior position. Thin walls or gaps can
still be crossed between endpoints; radius/swept collision, native Character
behavior, acceleration, combat and camera parity remain unimplemented. The
host movement module documents its input bounds and checks in
[`../swamp-movement/README.md`](../swamp-movement/README.md).

## Manually run `LizardMan_Intro`

Use **Run LizardMan_Intro trace** in the preview. This is an explicit developer
action; no collision volume or per-frame trigger activation is wired. The
native session decodes and retains the common and SWAMP command tables, imports
the bundled module-one MGP through the original MLX record, checks the source
TriggerZone properties (`script=LizardMan_Intro`, count `1`, delay `0`), copies
the source object records into the bounded scheduler, and activates the trigger
once. The module-one records are read-only source data; no object is added to
the rendered module-zero scene.

The verified sequence is:

1. At 0 ms the script logically locks all players, requests the waypoint camera
   target, starts common script `BeginScriptedCutScene`, and waits 500 ms.
2. At 500 ms `SpawnCharacter` requests native state 1 on the already loaded
   `_prim_Monster_LizManIntro1` Character record. It waits 1,500 ms.
3. At 2,000 ms the same state request is made for
   `_prim_Monster_LizManIntro2`. It waits 2,000 ms.
4. At 4,000 ms the script logically unlocks, issues its `LocalPlayer` camera
   target request, and schedules `EndScriptedCutScene`. The common child runs on
   the next 10 ms scheduler update, at 4,010 ms.

The panel reports scheduler time, program counter, waits, logical lock and
cutscene flags, object states, and recent events. Its 180 dp transcript area
scrolls independently and ends above the Return control. Camera commands only
update the logical target; there is no camera movement. `SpawnCharacter` only changes
the modeled state request on an existing Character record; actor allocation,
AI and rendering remain unimplemented. This preview's warrior is a render-only
pose, not a native player Character. The return-to-player camera lookup and
`DoTutorial` therefore remain unresolved/closed. Unsupported dialog, save,
HUD, flash, animation and other commands are reported as explicit no-ops. The
trace is not a complete game-script engine and does not run collision, combat,
campaign saves or quest flow.

The Android integration keeps the decoded tables alive only for the trace
session and releases them when the preview Activity is destroyed. Its
fixed-size scheduler limits are 256 objects, 16 tasks, call depth 8, 512
events, 2,048 commands per update and 96 bytes per runtime name. The table
decoder retains its own explicit table, string, array and command allocation
limits. The UI advances in bounded 10 ms steps and pauses while the Activity is
not resumed.

## Build and verification

From the repository root, using the supplied unpacked cache and Android SDK:

```powershell
python port/android-app/build.py --sdk <Android-SDK> --ndk <Android-NDK> --cache <unpacked-cache-files>
python port/android-app/tests/swamp_intro_trace_runtime.py --adb <Android-SDK>/platform-tools/adb --serial emulator-5558 --serial emulator-5560 --output <evidence-directory>
python port/script-runtime/tests/run_host.py --cache <unpacked-cache-files>
```

The trace runtime host test checks the Wait(500/1,500/2,000 ms) sequence,
state-request events at exactly 500 and 2,000 ms, the 4,000 ms unlock, and the
deferred common script update at 4,010 ms. The Android test verifies module-zero
touch controls on both API 37 x86_64 emulators: +X and +Y movement and heading,
stable idle position after release, no drift after pause/resume, and an on-screen
no-floor candidate rejection at the mesh edge. It then runs and scrolls the
manual trace, verifies completion and return to the unchanged authored
encounter, records screenshots, checks installed APK bytes and hashes, and
filters app-scoped fatal/native error logs. Both 16 KiB and 4 KiB emulator page
sizes are covered.

The exact current debug APK is 5,465,698 bytes, targets API 37, contains ARM64
and x86_64 libraries, has 16 KiB ELF load-segment alignment, and has SHA-256
`ea9d0aef09125f0dac6b4cdb3d37727aae277e6bdabd2faf92b56e219d09583b`. It passed
on Android 17/API 37 x86_64 with 16 KiB pages: X/Y movement and release,
pause/resume with no drift, off-floor endpoint rejection, the complete bounded
intro trace, return to the same encounter HUD, installed APK hash matching, and
zero filtered app errors. The exact current candidate passed the same movement,
pause/resume, edge rejection, trace and return checks on Android 17/API 37 with
4 KiB pages. The [current APK runtime report](swamp-current-apk-runtime-validation.json)
records both exact-build runs; [older multi-page-size evidence](swamp-preview-runtime-validation.json)
belongs to an earlier APK. Host tests cover source water/hole tags,
baseline-mask filtering, module-zero boundary rollback, and a separate
adjacent-floor hole rollback fixture. No Fold7 was tested; ARM64 execution on
physical hardware remains unverified.

Keep authored replacement behavior separate from cache-derived records and
recovered native behavior. See [provenance and rights](../../RIGHTS.md).
