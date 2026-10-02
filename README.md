# Dungeon Hunter 2 — source reconstruction

**Quest activation in the source APK:** Actual native population helpers and
four kill/clear Compile methods match 27,516 population and 21,372 compilation
cases on host/source ARM64. The 906,147-byte source APK passes 23 imports on each
Android 17 page size: all 34 real counted-kill records compile against owned
resolved-ID world snapshots and execute recovered damage/death/progress. Four
synthetic kill/clear records preserve the original level, population, active and
required-count decisions. Record/world generations survive replacement/rejection
and collection. Native/strict gates pass. World loading and ID resolution,
conditions, automatic dispatch, persistence/rewards, loot, full combat/world/AI
and complete source gameplay remain unfinished. See
[compile scope](port/quest-compile/README.md) and
[APK evidence](port/android-app/QUEST-COMPILE-INTEGRATION.md). Earlier entries
retain their checkpoint identities.

**Real quest data in the source APK:** All 64 cache quest records, 80
conditions, 194 objective stubs, 222 rewards and 896 script slots match the actual
original reader on host/source ARM64. All 21,817 host truncations reject. The
902,051-byte APK passes 19 imports on each Android 17 page size. All 34 supported
counted-kill objectives use recorded IDs/required counts with recovered melee
damage, owned health/death and quest progress. Dataset replacement/rejection and
retained objective generations pass. Native/strict runtime gates pass. Original
quest compile/world counts, conditions, automatic dispatch, persistence/rewards,
loot, full combat/world/AI and complete source gameplay remain unfinished. See
[reader scope](port/quest-data/README.md) and
[APK evidence](port/android-app/QUEST-DATA-INTEGRATION.md). Earlier entries retain
their checkpoint identities.

**Kill-objective progress in the source APK:** Four already-dispatched
native kill/clear handlers pass 14,396 original ARM32/host/source ARM64 comparisons.
The 873,379-byte source APK passes 14 imports on both Android 17 page sizes:
three recovered-formula two-hit deaths feed four owned quest counters, complete
at the second kill and preserve progress after replacement/rejection. Stale and
increasing synchronization, repeated completion requests, kind/ID filtering and
argument rollback pass. Native runtime and strict sanitizer checks pass. Actual
quest data loading/compile, automatic dispatch, persistence/rewards, loot, killer
credit/XP, player death, full combat/world/AI and complete source gameplay remain
pending. See [quest scope](port/quest-kill/README.md) and
[APK evidence](port/android-app/QUEST-INTEGRATION.md). Earlier entries retain
their checkpoint identities.

**Non-player death in the source APK:** The null-killer Kill projection
passes 2,510 original ARM32/source ARM64/host comparisons. Owned dead state,
HP zeroing, loot request/ID and four ordered quest requests match. Event match
fields are property/template IDs, not quantities. The 869,283-byte source APK
passes 14 imports on both Android 17 page sizes, including real objective
constants, melee-to-health-to-death, repeated kills, suppression, rollback and
retained state after data replacement. Native runtime and strict sanitizer
checks pass. Actual loot/quest consumers, resolved killer credit/XP, player
death, full combat dispatch, world/AI, progression and saves remain pending.
Full source gameplay is unfinished. See [death scope](port/character-death/README.md)
and [APK evidence](port/android-app/DEATH-INTEGRATION.md). Earlier entries retain
their checkpoint identities.

**Non-player damage in the source APK:** The numeric non-player HitFor
projection passes 2,835 original ARM32/source ARM64/host comparisons, including
health routing, suppression, forced kills, death requests and death reason.
The 865,187-byte source APK passes 11 imports on Android 17 4 KiB/16 KiB:
recovered melee formula damage feeds health, two five-point hits request death
on a ten-point diagnostic actor, policy/argument rollback and retained dataset
recovery pass, then textured walk preview loads. Native runtime selftests pass
on both page sizes and strict host sanitizers. Original Character/death command
ownership, player warnings, resolved attacker achievements, full result dispatch,
world/AI, progression, saves and source gameplay remain unfinished. See
[damage scope](port/character-damage/README.md) and
[APK evidence](port/android-app/DAMAGE-INTEGRATION.md). Earlier entries
retain their checkpoint identities.

**Recovered health and mana in the source APK:** Health setters/getters,
validation, regeneration and mana use pass 4,156 original ARM32/source ARM64/host
comparisons. Owned script actors pass 520 controlled and all 446 real character
cases / 216,384 final-field queries on host, strict sanitizers and Android 17
4 KiB/16 KiB. The 848,803-byte source APK passes 14 imports, 86 selected actor
cases / 19,264 queries, retained generations/error recovery and textured walk
preview on both page sizes. Full damage/result dispatch, death/events, Character
ownership, AI and source gameplay remain unfinished. See
[health scope](port/character-health/README.md) and
[APK evidence](port/android-app/HEALTH-INTEGRATION.md). Earlier entries
retain their checkpoint identities.

**Recovered combat calculations in the source APK:** Reconstructed random
streams match 768 original ARM32/source ARM64/host draws and 351 callbacks.
The unchanged combat script runs through owned actor projections: 3,568 real
record/hand/attack cases and 216 controlled cases pass host, strict sanitizers
and Android 17 4 KiB/16 KiB checks. The 840,611-byte source APK passes 26 imports,
80 selected real and all 216 controlled cases, constant replacement/recovery
and textured two-motion preview on both page sizes. Damage application,
Character/state/buff ownership, AI and full source gameplay remain unfinished.
See [binding scope](port/lua-character/COMBAT-BINDING.md) and
[APK evidence](port/android-app/COMBAT-INTEGRATION.md). Earlier entries
retain their checkpoint identities.

**Gear stats and powers in the source APK:** Item/power stat contributions
and base/gear/class recalculation now match 7,147 original ARM32 versus source
ARM64/host comparisons. Owned script objects retain all four datasets and pass
1,012,032 final-field queries across all 1,322 items and 937 powers in both hands
on host, strict sanitizers and Android 17 4 KiB/16 KiB. The 836,515-byte source
APK passes 20 imports / 15,680 selected final-field queries and textured animation
checks on both page sizes. Original Character/inventory ownership, equip
requirements, random powers, buffs, combat and full source gameplay remain
unfinished. See [source scope](port/gear-properties/README.md) and
[APK evidence](port/android-app/GEAR-INTEGRATION.md). Earlier entries retain
their checkpoint identities.

**Equipment calculations in the source APK:** All eight loot tables are
decoded from checked reader layouts, including 1,322 item records. Owned script
objects retain item data and expose supported attack/critical/damage bonuses and
shield queries. The reader/query comparison passes 24,670 original ARM32 versus
source ARM64/host checks; 312 original callback cases match the source bindings.
All-item script checks pass 18,508 queries on host, strict sanitizers and both
Android 17 page sizes. The 807,843-byte APK passes item, class, shared-script and
textured animation integration on both page sizes. Full Character lifecycle,
inventory mutation, gear contributions/powers, combat and source gameplay remain
unfinished. See [source scope](port/equipment-bonuses/README.md) and
[APK evidence](port/android-app/EQUIPMENT-INTEGRATION.md).
Earlier entries retain their checkpoint identities.

**Class calculations in the source APK:** Owned script property objects now
retain class datasets and expose authored `ApplyClass`. All 260 real classes /
116,480 final-field queries pass on host, strict sanitizers and both Android 17
page sizes. The 779,171-byte source APK imports property/class data and passes
40 real applications / 8,960 final values, retained generations, guarded failure
and recovery. Shared-script and textured animation regressions pass in the same
package. Full Character lifecycle, equipment/buffs, combat and source gameplay
remain unfinished. See [binding scope](port/lua-character/CLASS-BINDING.md)
and [APK evidence](port/android-app/CLASS-INTEGRATION.md). Earlier entries keep their checkpoint identities.

**Derived property class rules:** Original readers recover 260 classes / 1,659
rules. Original application, group traversal and calculation bodies match source
ARM64/host in 3,270 cases across all four state sheets and temporary storage.
Strict sanitizers pass 12,000 safety iterations. This standalone checkpoint uses
empty buffs; complete Character construction, class lifecycle, Lua class binding,
APK integration and full gameplay remain unfinished. See [source and evidence](port/character-classes/README.md).
Earlier entries retain their checkpoint scopes and identities.

**Typed script property state in Android:** Numeric/boolean `GetProp`/`SetProp`
matches original ARM32 and source ARM64/host in 1,586 callback checks. Owned Lua
userdata checks all 448 real records / 100,352 composed fields on host, strict
sanitizers and both Android 17 page sizes. The 775,075-byte source APK now imports
character data through Android's document picker. Real queries, typed calls,
dataset replacement/rejection/recovery, shared scripts and character animation
regressions pass on both page sizes. Original Character, derived class stats,
buffs, real combat and full source gameplay remain unfinished. See [binding
evidence](port/lua-character/README.md) and [APK checks](port/android-app/PROPERTY-INTEGRATION.md).
Earlier entries retain their checkpoint scope and binary identities.

**Owned character property writes:** Source state owns four 224-field sheets.
Original property Set/Add/Int operations and current-final integer reads match
source ARM64/host in 3,352 checks. Strict sanitizers pass 12,000 safety iterations.
Lua object methods, derived base stats, Android integration and full source game
remain unfinished. See [source and evidence](port/character-state/README.md). Earlier entries keep their scope.

**Character stat composition:** The original calculation, tree traversal and
deque iterator bodies match source ARM64/host across 585 cases, including all
224 cache types, buff priority and container block boundaries. Strict sanitizers
pass 10,000 safety iterations. This remains standalone: character ownership,
property write routing, Lua methods and Android game integration are unfinished.
See [source scope and evidence](port/property-composition/README.md). Earlier checkpoints retain their identities.

**Character property data:** Original readers fully consume all three tables in
the 401,608-byte character cache file. Source ARM64/host loading matches all
100,352 character values, and 2,022 original property-operation checks pass.
Strict sanitizers pass 12,000 safety iterations. This standalone component has
no entity ownership, aggregate equipment/buffs, Lua object bridge or APK wiring
yet; full source gameplay remains unfinished. See [scope and evidence](port/character-properties/README.md).

**Source scripts in the Android APK:** The 730,019-byte source package now
initializes three exact recovered shared scripts inside the character preview.
Imports, budget/error rejection and recovery pass in the same process on Android
17 with both 4 KiB and 16 KiB pages. The same APK passes textured character
import, seeking, mixing, Play and Pause. Native game objects, real combat and full
source gameplay remain unfinished. See [APK source and evidence](port/android-app/SCRIPT-INTEGRATION.md).
Earlier checkpoints below retain their original scope and binary identities.

**Structured fields and shared scripts:** The source runtime now supplies
`GetPyStruct` for 636 original field entries, including all 224 character
properties. Original initializer/lookup tracing passes 693 checks. Three unchanged
shared scripts execute, with authored animation-event, skill-selection and
no-combatant checks passing on host, strict sanitizers and both Android 17 page
sizes. Actual combat, entity callbacks, source APK integration and full source
gameplay remain unfinished. See [source and exact scope](port/pydata-names/STRUCT-FIELDS.md).
Earlier checkpoints below preserve their original test scope.

**Named script records:** Source `GetPyOID` now resolves 8,863 names across 71
array tables. Original reader output and 266 original lookup cases are checked;
all authored Lua ID queries pass on host and both Android 17 page sizes.
The updated runtime still passes constant/arithmetic/script parsing checks.
Structured fields, array records, original scripts and full source gameplay
remain unfinished. See [name-table source](port/pydata-names/README.md).

**Source Lua on Android 17:** The official Lua 5.1.4 source and a new owned
runtime use the observed original float32 number profile and build for ARM64/x86_64.
504 original arithmetic outputs match the source runtime on host and both
Android page sizes. The exact x86_64 runner passed on both
4 KiB and 16 KiB Android 17 emulators: 218 original scripts and the separate
sandworm repair parse, while the unchanged malformed original fails as expected.
Authored library, memory/instruction budget and error-recovery tests passed.
Nine numeric callbacks now pass 2,375 original ARM32/compiled ARM64/host
comparisons and authored Lua checks on both Android test environments.
Strict sanitizers also pass after a documented table-key safety repair that
preserves the imported vendor files. Game scripts were not executed;
gameplay object callbacks and source-built gameplay remain unfinished.
See [runtime source and evidence](port/lua-runtime/README.md).

**Checked script constants:** The source loader matches all 5,608 integer entries
from 26 complete files against original reader writes. Owned `GetPyCst` mappings
pass every authored query on host and both Android 17 page sizes. Malformed and
allocation-failing imports retain existing data. The mixed sound file is rejected;
original scripts and source gameplay remain unfinished.
See [table source and scope](port/pydata-constants/README.md).

**Readable game scripts:** The owner's complete cache contains 219 plaintext
Lua source files despite their `.luac` extensions. Their exact 900,493 bytes
are now browsable in Git. 218 originals pass syntax; a separate one-character
sandworm repair also passes. Source hashes match the pinned cache archive and
Git index. The native scripting bridge and full source game remain unfinished.
See [the script source](recovered/scripts/README.md).

**Two-motion source preview:** Checked absolute layers now combine the warrior's
dual walk and Dark Queen scene 03a motion. The exact updated APK passed imports,
midpoint seek, 0/50/100% mix, advancing Play and stable Pause on Android 17 with
both 4 KiB and 16 KiB pages. Visually inspected screenshots show distinct textured
poses. Host endpoint/palette and 3,000 damaged-input probes passed.
See [layer scope](port/animation-layers/README.md) and
[current runtime evidence](port/android-app/layers-runtime-validation.json).
Original transition scheduling and full source-built gameplay remain unfinished.

**Original timeline update:** The source Android app now uses reconstructed
range-clock arithmetic for animation playback, seeking and resume. All 400
original ARM32/compiled ARM64/host sequences matched (4,000 updates and 400 jumps);
50,000 sanitizer updates passed. The exact updated APK passed advancing Play,
stable Pause and visible pose changes on Android 17 with 4 KiB and 16 KiB pages.
See [the timeline component](port/animation-timeline/README.md) and
[Android runtime evidence](port/android-app/timeline-runtime-validation.json).
Full source-built gameplay remains unfinished.

**Player animation audit:** 333 of 342 clips produce checked source poses on
the warrior model, including 327 animated clips. Negative key times are now
supported. The current source APK passed dual-walk controls on Android 17 with 4 KiB
pages and a position-component cutscene on 16 KiB pages. Earlier builds
passed walk and negative-start aura checks. See the
[pose audit](port/animation-pose/README.md) for rejected formats and bindings.
The complete source game remains unfinished.

**Original animation calculation update:** [Float track values](port/animation-values/README.md)
now reproduce 29 original position, scale, quaternion interpolation/delta and
weighted-blend bodies. All 4,705 original ARM32/compiled ARM64/host cases matched,
and 6,000 sanitizer cases passed. The updated source Android preview uses these
absolute calculations and passed dual-walk/cutscene controls on Android 17.
This remains a source component and diagnostic preview; the complete source game
is unfinished.

**Character animation preview update:** The [source pose evaluator](port/animation-pose/README.md)
now previews the warrior's 27 stored walk tracks on 18 bones. The exact
source APK passed import, time selection, Play and Pause on Android 17 with
both 4 KiB and 16 KiB pages; the textured character visibly changed pose.
Host sampling checks and 3,000 sanitizer corruption/truncation cases passed.
This is an absolute-key preview. Original animator blending, transitions and
complete source-built gameplay remain unfinished.
[Runtime evidence](port/android-app/animation-runtime-validation.json).

**Character reconstruction update:** [Checked skin controllers](port/skin-payloads/README.md)
now decode 531 locally resolved skins across the complete cache. A software
bone-palette calculation passed 304 comparisons against original ARM
instructions; 5,000 damaged-file probes passed memory/undefined-behaviour
checks. The source Android app resolves 18 warrior bones and visibly renders
and rotates the textured player pose on Android 17 with both 4 KiB and 16 KiB
pages. This remains a diagnostic preview: original animation control and complete source-built
gameplay are unfinished. [Runtime evidence](port/android-app/character-runtime-validation.json).

**New local input, 2026-10-02:** a separately supplied 433,189,197-byte cache ZIP passed full member CRC/length checks and contains 6,833 files. See [the complete-cache audit](docs/COMPLETE-CACHE.md). Earlier ten-part recovery figures below describe a different, truncated input.

**Current Android 17 check:** The [source-built Android renderer](port/android-app/README.md) installed on the official API 37.2, 16 KiB page emulator and drew a real candle mesh with its decoded PVRTC texture; it is an asset preview, not gameplay. A separate, self-contained ARM64 compatibility APK installed the verified guest and complete cache on Android 17. On the API 37.0 4 KiB emulator, a clean install of the final Test 10 build displayed a full-width menu, entered a saved 3D level, responded to attack-button taps with a nearby enemy disappearing, and moved the player and camera after a joystick swipe. Pause/return updated the visible save time; a force-stop/cold relaunch kept that timestamp, reentered the saved level, and accepted another movement input. The standard compatibility build retains its page-size guard on the 16 KiB image. A separate strict experimental translator build on the API 37.2 16 KiB emulator reached a saved 3D level, moved the player/camera, and returned through pause to an updated save menu in one uninterrupted run. Exact-package cold relaunches with both default and graphics-preservation settings visibly reached the saved-character menu with the previous save time after a slow loading phase. The default-setting run reentered the saved 3D level and moved the player/camera again. Both game APKs still run the original ARM32 engine through a translator; extended play, all levels, physical devices and complete save/reload semantics remain unverified. See [the Android 17 results](docs/ANDROID-17-EMULATOR-RESULTS.md). No Fold7 device testing was done for this update.

**Test 11 English update:** The [saved-language change](compatibility/work/fold7-build/STANDALONE-TEST11-LANGUAGE.md) was built as a separate version 14 compatibility APK. On the 4 KiB Android 17 emulator it upgraded the saved Test 10 installation, preserved its checkpoint, changed only the recognized saved language byte, and showed English menus and loading text. It entered the saved 3D level, moved the player, accepted attack input, returned through pause with a new save time, and after force-stop/cold relaunch retained the English menu and resumed that level with movement. A separate strict 16 KiB Test 11 APK also showed an English saved-character menu, loaded the saved 3D level, accepted movement, returned through pause with a new save time, then survived force-stop/cold relaunch, reentered that level and moved again. The complete source-built game remains unfinished.

## Engine source reconstruction — texture, material and render preview checkpoint

[Texture views and PVRTC decoding](port/texture-assets/README.md) classify all 363 external cache textures and decode the 234 BTEX/PVRTC images to RGBA8. Every decoded pixel in those 234 images matched the official PowerVR software decoder; the revised component also builds for Android ARM64. [Checked image/material views](port/material-bindings/README.md) cover nested BRES effect/material parameters and image-index links across all 2,904 BRES files. They resolve 3,854 local effect IDs and also build for Android ARM64. These are isolated source components; the ARM64 builds have not been loaded on an Android device.

[Checked scene hierarchy views](port/scene-payloads/README.md) follow visual-scene references, traverse nodes, compose static world matrices and classify geometry instances across the complete 2,904-file BRES cache. The audit traversed 21,472 nodes and 11,648 instances; 10,403 of 10,444 local geometry links resolved. A [static draw descriptor bridge](port/scene-draw/README.md) now joins those transforms with mesh primitives and material IDs, emitting 11,311 checked draw descriptors covering 995,715 triangles. Its Android 17 16 KiB emulator smoke check passed for a real candle scene. The source Android renderer now consumes checked static draw commands: it displayed the crypt background and candle flame from two commands in one imported BRES, then a larger owner-supplied void-maze scene with 77 commands in a touch-rotatable 3D preview. The exact 3D preview APK ran on both Android 17 4 KiB and 16 KiB emulators. This is still an asset preview; original animation control, level streaming and gameplay remain separate work.

The [host WebGL renderer preview](port/renderer-preview/README.md) draws one real candle mesh primitive in flat and textured diagnostic modes. The textured mode follows its BRES diffuse image link, decodes the cached PVRTC texture, uploads RGBA pixels to WebGL and uses an original unlit textured GLSL pair. Both draws passed 53,185-pixel readback; the textured image has 6,460 distinct RGB colors. The shader choice and state are diagnostic, so original game material behavior, scenes, Android engine texture upload and a playable rebuilt engine remain unfinished.

The [Android 17 source smoke check](port/android-smoke/README.md) ran reconstructed PVRTC and BRES/material readers in a native x86_64 executable on the official API 37 emulator with 16 KiB pages. Synthetic checks and two owner-supplied cache fixtures passed with matching on-device hashes. This validates those isolated source functions on current Android; it is not a game build.

**Local agent: start with [the Android 17 results](docs/ANDROID-17-EMULATOR-RESULTS.md), [the older emulator diagnostic](docs/EMULATOR-TEST-2026-10-02.md) and [LOCAL_AGENT_HANDOFF.md](LOCAL_AGENT_HANDOFF.md).** They document the tested Android version, build layout, prior device history and verified workspace-preparation helper. For the independent native-source route, read [RECONSTRUCTION-HANDOFF.md](RECONSTRUCTION-HANDOFF.md).

## Engine source reconstruction — mesh and animation checkpoint

[Browse the mesh/animation C++ module](port/asset-payloads/README.md) and [verified binary layouts](port/asset-payloads/FORMATS.md). The loader decodes all 10,924 type-0 meshes and 890,301 animation time keys in the recovered BRES cache, including the Prince model's deferred buffers. A separate checked reader exposes embedded mesh data in all nine type-1 records while preserving the original runtime's type-1 rejection. Twenty-six complete animation accessor/search bodies pass [271,970 original-ARM32/compiled-ARM64 comparisons](reports/asset-payloads-arm-validation.json) with zero mismatches; all 456 original instruction addresses were exercised. Separate target checks pass 25,682 comparisons on 1,149 meshes. Host/ARM64 builds and 5,000 sanitizer probes pass. [OBJ and animation JSON examples](port/asset-payloads/examples/README.md) are included. Type-1 spline semantics, scene/controller and full material/effect semantics, GPU ownership, game rendering and gameplay remain unfinished. This checkpoint is published directly on GitHub.

## Engine source reconstruction — resource checkpoint

[Browse the resource-loading C++ module](port/engine-resources/README.md). This adds 35 complete reader/Collada-accessor bodies and the whole-buffer BRES relocation branch, reconstructed from original ARM instructions and compiled for ARM64. [Validation](reports/engine-resources-validation.json) passed 10,449 reader comparisons and every fixup in all 2,901 earlier recovered BRES images: 2,577,206 fixups, with zero mismatches. The port exposes top-level animation/image/material/geometry tables using native pointers while preserving the serialized 32-bit offsets. Later components add mesh/raw-animation decoding, bounded image/material bindings and PVRTC pixels. Game rendering and gameplay remain unfinished. This checkpoint is published directly on GitHub; the existing Drive ZIP remains the earlier math checkpoint.

## Engine source reconstruction — math checkpoint

[Browse the reconstructed C++ module](port/engine-math/README.md). Twenty original vector/quaternion/matrix function starts have been rewritten from ARM assembly and compile for ARM64. The [recorded test report](reports/engine-math-validation.json) contains 21,477 original-ARM32 versus compiled-ARM64 comparisons with zero mismatches, under the documented external arithmetic/libm model. All 1,704 original instruction addresses in these routines were exercised. This is a tested engine component; asset loading, rendering and gameplay reconstruction remain unfinished. The full source ZIP and validation-artifact ZIP in the Drive folder below now include this checkpoint.

## Fold7 compatibility build — test 5

[Download the Test 5 APK and complete work archive](https://github.com/Noamcelermajer/DH_sc/releases/tag/v1.0.2-fold7-test5). The Test 4 phone report confirms its directory guard ran, followed by a new failure reopening the prince character model with a duplicated cache root. An original-engine probe reproduces the bad absolute-path classification. Test 5 changes five ARM instructions to recognize Android absolute paths and records model-open results.

Thirteen path cases, five earlier file cases, native library/hook checks and APK/signature checks pass. The historical Test 5 device instructions are preserved in `TEST5.md`; this Android 17 effort uses the official emulator and did not test a Fold7. Russian menus, display behavior and the earlier GL error remain unresolved or unverified.

Read [TEST5.md](compatibility/work/fold7-build/TEST5.md) for the diagnosis, assembly, test limits and device procedure. Test 5 modifies the engine binary; earlier byte-identical-engine statements refer to Tests 1–4.

The complete Test 5 compatibility snapshot is preserved in `compatibility-work-test5.zip`. Prepare a new work directory with current Git source overlaid on that verified snapshot:

```sh
python3 tools/prepare_local_agent.py ../DH2-local-work
```

The helper verifies the archive and runtime inputs, restores the original-library layout expected by the patchers, and leaves the checkout unchanged. The destination must not exist. `unpack_compatibility.py` remains the exact historical restorer; see the handoff before using it with newer files.

Authored source and reports are browsable under `compatibility/`; the standard full-directory ZIP is attached to the release. Previous releases and independent engine reconstruction are retained. External toolchains and duplicate intermediates are represented by versions, hashes and instructions. Original rights and third-party notices apply.

**Browse the verified recovery-source import:** The repaired [Android Java](port/android-java/README.md), reconstructed [JNI component](port/nativeinterface/README.md), authored recovery scripts and reports are ordinary Git files. [The first import ledger](docs/RECOVERY-SOURCE-IMPORT.md) maps 333 selected files to the checksum-verified older source ZIP. The same verified ZIP also supplies 44 browsable [native pseudocode/index files](recovered/native/decompiled/README.md) and 721 [raw Java/smali code exports](recovered/android/README.md), each with per-file hashes. Another exact import makes 3,625 [native assembly files and 71 symbol records](recovered/native/README.md) browseable with a per-file manifest. These generated exports are archival evidence, not a buildable game. The subsequent [text and debug import](docs/RECOVERED-TEXT-AND-DEBUG.md) adds 2,164 shader/configuration resources and 48 native debug exports as exact Git files, with their historical provenance. The separate [Google Drive handoff](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW) retains the compressed native evidence bundles, original inputs and validation artifacts. Start with `START-HERE.txt` there. The complete cache ZIP used for newer audits was supplied separately and is not in this checkout.

This repository contains independently reconstructed port components, selected original-instruction evidence, archived code exports, compatibility work and documentation for the supplied **Dungeon Hunter 2 HD v1.0.2 Android APK**. The original game inputs and some recovery evidence remain separate materials. The repository is a reviewable starting point for restoring the game on modern Android.

**This is not the original studio source repository or a completed, playable source-built ARM64 port.** Native decompiler output is pseudocode that still requires reconstruction and behavioral validation. A successful export or compilation is not evidence that the game runs.

## What is available

The paths below are in this Git checkout unless marked **external recovery package**. The historical coverage totals describe the supplied APK and external package; they do not mean the complete recovered source tree is committed here. See the [source-availability inventory](docs/SOURCE-AVAILABILITY.md) for the Git-versus-archive accounting.

| Component | Evidence and scope | Location |
| --- | --- | --- |
| Repaired Android Java and reconstructed JNI | 288 hash-checked Java sources, buildable JNI component, recovery/build scripts and provenance ledger | [`port/android-java`](port/android-java/README.md), [`port/nativeinterface`](port/nativeinterface/README.md), [import ledger](docs/RECOVERY-SOURCE-IMPORT.md) |
| Archived code exports | 44 native pseudocode/index files and 721 raw Java/smali files copied byte-for-byte from the verified recovery ZIP; generated, not buildable studio source | [`recovered/native/decompiled`](recovered/native/decompiled/README.md), [`recovered/android`](recovered/android/README.md) |
| Native instruction and symbol evidence | 3,625 exact assembly text files and 71 exact symbol/index records copied from checksum-verified bundles; generated, not studio source | [`recovered/native`](recovered/native/README.md) |
| Text and debug evidence | 2,164 shader/configuration resources and 48 native debug exports | [Import guide](docs/RECOVERED-TEXT-AND-DEBUG.md) |
| External recovery evidence | Compressed native evidence bundles and original inputs | [Drive handoff](https://drive.google.com/drive/folders/1njTxLAJHt08SinEskn7gC6IWylXemwDW) |
| Cache audits | Complete local cache: 6,833 checked files. Earlier ten-part input: 5,839 verified files and 241 directories | [`docs/COMPLETE-CACHE.md`](docs/COMPLETE-CACHE.md); original cache archives are separate inputs |
| Engine math reconstruction | Buildable C++ for 20 original vector/quaternion/matrix routines; 21,477 ARM32/ARM64 comparisons passed | [`port/engine-math`](port/engine-math), [`reports/engine-math-validation.json`](reports/engine-math-validation.json) |
| Resource reconstruction | Buildable C++ for 35 complete reader/accessor bodies plus whole-buffer BRES relocation; all 2,901 recovered BRES files validated | [`port/engine-resources`](port/engine-resources), [`reports/engine-resources-validation.json`](reports/engine-resources-validation.json) |
| Mesh/animation reconstruction | ARM64 C++ decoder, OBJ/raw-key export, 26 animation bodies and 271,970 original-instruction comparisons | [`port/asset-payloads`](port/asset-payloads), [`reports/asset-payloads-arm-validation.json`](reports/asset-payloads-arm-validation.json) |
| External texture decoding | Checked BTEX/PVR, TGA and PNG views; PVRTC RGBA8 pixels matched a PowerVR reference for all 234 cache images; ARM64 build | [`port/texture-assets`](port/texture-assets), [`port/texture-assets/reference-validation.json`](port/texture-assets/reference-validation.json) |
| Image/material bindings | Checked nested BRES effect/material parameters and image indices across all 2,904 BRES files; ARM64 build | [`port/material-bindings`](port/material-bindings), [`port/material-bindings/validation.json`](port/material-bindings/validation.json) |
| Scene hierarchy | Checked node traversal, static world matrices and geometry-instance references across all 2,904 BRES files; host and ARM64 builds | [`port/scene-payloads`](port/scene-payloads/README.md), [`validation`](port/scene-payloads/validation.json) |
| Static scene draw descriptors | Joins world transforms, mesh primitives and material IDs; 11,311 checked descriptors across the full cache; Android 17 16 KiB smoke check | [`port/scene-draw`](port/scene-draw/README.md), [`validation`](port/scene-draw/validation.json) |
| Host graphics preview | One candle mesh primitive drawn flat and with its decoded diffuse texture using original unlit GLSL in WebGL1; pixel readback passed | [`port/renderer-preview`](port/renderer-preview/README.md) |
| Android 17 source renderer | Signed target-SDK-37 APK built from reconstructed asset readers; first-primitive candle rendering, then two-command candle and 77-command rotatable void-maze preview; character pose and absolute walk animation previews ran on both 4 KiB and 16 KiB Android 17 emulators; no gameplay | [`port/android-app`](port/android-app/README.md), [`3D runtime validation`](port/android-app/scene-3d-runtime-validation.json) |
| Compatibility preparation | Snapshot unpacking and local-agent preparation scripts | [`unpack_compatibility.py`](unpack_compatibility.py), [`tools/prepare_local_agent.py`](tools/prepare_local_agent.py) |

Selected original instructions for the reconstructed functions are included in the module reference directories. Full generated assembly and symbol text, raw decompiler output and Java/smali exports can now be inspected in Git. The original ELF binaries and recovered configuration/shader exports remain separate. The checked-in `recovered/native/bundles/manifest.json` contains hashes only; the compressed bundle archives are external. See [reproduction steps](docs/REPRODUCING.md) before using `tools/unpack_native.py`.

## Findings that change the restoration plan

1. **The main engine is not stripped.** Its symbol tables preserve gameplay and engine names, virtual tables, relocations, 867 build-source filenames and ARM/Thumb mapping symbols. Assembly recovery round-trips all executable bytes across the three libraries: **6,488,258 bytes**. This is strong binary evidence; it is not C++ source recovery or a runtime test.
2. **DWARF does not contain the gameplay implementation.** All 32 identified compilation units cover licensing/online glue (7), STLport (22) or libgcc (3). The debugging metadata is useful for these components, but cannot substitute for reconstructing the engine's gameplay classes.
3. **The earlier ten-part cache upload was incomplete.** Those parts total exactly 314,572,800 bytes and stop inside `data/sounds/m_world_map.wav`; their ZIP central directory is absent. Files reported from that input passed decompressed-length and CRC-32 checks. A separately supplied complete cache ZIP is now available locally and passed all 6,833 member checks; see [the complete-cache audit](docs/COMPLETE-CACHE.md).
4. **A considerable amount of game data is already readable.** Most module/game-object formats are XML, and the shader package contains original GLSL text. Binary resources include BRES `.bdae`, BTEX/PVR texture wrappers, WAV and VoxN audio. Three original XML files contain duplicate attributes; they are preserved unchanged and documented for parser compatibility.
5. **The supplied APK's provenance is not established as an untouched studio release.** Its manifest has compile-SDK 33 metadata and its DEX contains save-restoration/additional support classes. The repository faithfully documents this supplied binary. Those observations do not establish who repackaged it or what changes were made.
6. **Rebuilding for ARM64 is a separate engineering project.** The three supplied native libraries are ELF32 ARM. Renaming ABI folders, changing target SDK, or compiling the small JNI library cannot turn the engine into a native ARM64 game. Class layouts, pointers, JNI/reflection names, graphics, storage, audio, networking and gameplay must be validated together.

See [`docs/FINDINGS.md`](docs/FINDINGS.md) for evidence interpretation and [`docs/PORTING.md`](docs/PORTING.md) for the remaining work.

## Start reviewing

- The exact input APK SHA-256 is `32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200`.
- Read [`docs/STATUS.md`](docs/STATUS.md) for generated coverage and test results.
- For behavior, prefer exact smali/assembly over a decompiler's inferred types or control flow. A `.pseudo.c` file deliberately does not claim to compile.
- Inspect [`port/engine-math/README.md`](port/engine-math/README.md) for the math module, original-address mapping and differential-test limits. Floating-point helper metadata incorrectly truncates 16 of 19 vector/quaternion pseudocode bodies; their assembly remains complete.
- Inspect [`port/engine-resources/README.md`](port/engine-resources/README.md) for memory/subfile readers, checked BRES offset loading, Collada table layouts and original-instruction checks. The later payload, texture and material components extend these checked views; game rendering remains unfinished.
- Inspect [`port/asset-payloads/README.md`](port/asset-payloads/README.md) for checked mesh/deferred-buffer decoding, raw animation keys, original keyframe timing/search and small real exports. Its format tables and reports distinguish complete animation functions from partial GPU/ownership evidence.
- Inspect [`port/texture-assets/README.md`](port/texture-assets/README.md) and [`port/material-bindings/README.md`](port/material-bindings/README.md) for the pixel decoder, nested image links, cache audits and ARM64 build limits; see the [renderer preview](port/renderer-preview/README.md) for one host WebGL draw.
- Inspect [`port/scene-payloads/README.md`](port/scene-payloads/README.md) for scene/node traversal, geometry-instance links, malformed-input checks and remaining opaque instance types.
- Inspect [`port/scene-draw/README.md`](port/scene-draw/README.md) for static scene draw descriptors, unresolved links and the Android 17 runtime check.
- Review the [reconstructed JNI](port/nativeinterface/README.md), [repaired Android Java](port/android-java/README.md), [generated pseudocode](recovered/native/decompiled/README.md), [assembly and symbols](recovered/native/README.md) and [import ledger](docs/RECOVERY-SOURCE-IMPORT.md) directly in Git. Original binary inputs and compressed archive copies remain separate.
- Give the rights holder this repository, the separate recovery package, the exact input APK and the complete original cache. Ask for original engine/build metadata and asset tooling; source-file names and subsystem inventories help focus that search.

The independent reconstruction modules use owner-supplied inputs. The compatibility snapshot is an explicit exception to their earlier packaging policy: it includes original `.so` inputs, the tested runtime bundle, and a deliberately public development signing-key fixture. Complete art/audio cache archives and external SDK/NDK/JDK/emulator toolchains are not included. The [source inventory](docs/SOURCE-AVAILABILITY.md) identifies which earlier recovery-package paths remain external. Recovered materials carry their original provenance; this repository does not assert an open-source license for the game. See [`RIGHTS.md`](RIGHTS.md).

