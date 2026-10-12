# Native script owner integration: source status

The reconstruction goal remains active. The latest tested checkpoint is
`../build/checkpoints/dh2-native-authored-ui-skills-b0c2093c.apk`, 25,919,351 bytes,
SHA256 `b0c2093c64ee6a3b1b197c72bc18a2904fd0c07af35c3b07ec011a48926fb9be`.
It is a native source reconstruction of a limited Crypt prototype. Full game
completion, original rendering fidelity and physical ARM64 testing remain.
The sections below record successive historical stages. This 82-suite source
checkpoint passes six emulator suites on those exact bytes: shader pixels and
texture scaling, rendering/rotation, movement, lifecycle, animation bank, and
combat (stationary and moving). Portrait and landscape were visually inspected;
eleven retained monster owners and effects/debug identities survive rotation.
See `native-authored-ui-skills-b0c2093c-checkpoint-validation.json`.
Its bundled modules include script owner services, skills/Faery ownership,
complete queued startup, vitals and retained particle emission. Original
GameSWF shader primitives run on the GPU; full SWF display lists, actions,
menus and HUD remain. The newly recovered runtime paths have source/host
evidence; the live development
renderer still stages early monster initialization and does not yet execute
the complete original delayed monster frame or full particle effects.

## Current source and milestone order

The current central source stage passes 88 linked host suites with zero sanitizer
findings (`../../level-world/reports/render-ui-movie-font-main-linked-host-audit-v2.json`,
SHA256 `1fdc51e09e89ac729eab5dafb7a9a62e31303be3fe38b583f08ef7a3ba89ade5`).
It integrates the source-built GameSWF r1714 core, retained movie facade and
matching FreeType 2.3.7 backend into the actual main UI DSO. C and C++ sources
both execute under address/undefined/leak checks; the core's documented
disabled-JPEG vptr exclusion remains explicit. The real HUD submits 24,468
vertices and 140 masks in the movie audit. The Fontin provider audit submits
238 alpha uploads and 1,032 glyph quads. Texture identities and localization
are still host fixtures; that bounded font test records its exact Arial miss.
Recovered general font resolution is a separate connection, not a substitution.
The prior 85-suite and 84-suite receipts are preserved.
The last packaged checkpoint remains the 82-suite `b0c2093c` stage above.
The newer source connects the owned GFNT bitmap-font module and original
eight-string shader assembly to the main CMake targets. The font check covers
8,532 original cases, including glyph metrics, pixels and signed height behavior;
shader assembly checks 860 original cases and all 34 stored shader members.
The Android shader adapter now consumes the verified original `shaders.pak`
through that module and submits the original eight source strings to GL.
It passes an NDK ARM64 compile check; this newer adapter has not yet been
packaged or rerun through the emulator GPU suite. The existing checkpoint's
GPU receipt applies only to its unchanged bytes.
The current source additionally connects original SWF filename/wrap/state/color
helpers to the scene DSO (2,879 original comparisons and 11 atomic guards).
The new `swf_gpu.cpp` uses the recovered solid and bitmap Color0 producers
directly, preserves glyph caller RGBA, and selects the four recovered blend
passes. It owns uploaded bitmap bytes across context recreation and supplies
ordered geometry, nested stencil masks and a shared-stencil GPU query target.
This is a modern GLES2 backend, with private premultiplied composition; it is
not proof of complete original renderer or mask/hit-test parity.
`native-swf-gpu-source-color-compile.json` records compile-only success for
ARM64 and x86_64. The backend has not yet been wired to a live movie or verified
through GPU readback. The movie and real font adapters now pass the integrated
host ownership and sanitizer checks. They still need live Android provider,
viewport/input and GPU connections before an original HUD APK is validated.

`original-ui-assets-stage.json` verifies 44 unchanged original resources,
including 30 SWFs and seven TTFs, in the repository APK asset tree.
`original-ui-text-assets-stage.json` separately verifies 337 text resources:
333 language/symbol files (37 sheets) and four common-text metadata files.
`original-ui-texture-assets-stage.json` adds four missing canonical UI textures
and an index. The compiled native resource catalog binds all 385 unique original
UI/font/text/texture resources by URI, size and SHA256. The native APK reader
owns delivered bytes and uses explicit case-folded URI lookup.
The repository now has 770 assets; Studio still has the prior 382 until the
next integrated build synchronization. Neither new asset stage is packaged.
Native localization recovery is in progress; the HUD asks for a fast-travel
label and randomized death text, including a required player-name substitution.
It adds private script path/cache/call/InitVCB services needed by genuine skill
setup and distinguishes required native service failures from authored Lua
errors, including errors caught by Lua. Deferred initialization retains completed
prefix effects and refuses to continue after a required service failure.
Historical source proofs retain their exact seven-file baseline; the central
receipt identifies changed sources separately. The earlier 76-suite services
stage had no separate APK. The integrated checkpoint below adds a visible shader
connection and has a complete six-suite emulator receipt.

The subsequent integrated source batch includes the complete queued startup
entry, owned Faery tables and CharacterSkillOwner/private-VM services, plus
retained particle emission. Actual eleven-monster skill setup preserves 55
authored null spell slots; nonempty skill behavior uses explicitly controlled
inputs. Full authored skill scripts, whole enemy frames and complete effects
simulation/material/render providers remain open.

The authored-UI checkpoint `b0c2093c` builds in both Android projects with 489
captured inputs, 382 bundled prototype assets and sixteen native ELF64 libraries.
Capture `../../../.local-inputs/native-authored-ui-skills-build-capture.zip`
has SHA256 `0dd7dc70367d963f3e8fb7a627be84c88f0720cd18ee3c8780e75371f8de5eae`.
`native-authored-ui-skills-source-build-inspection.json` verifies both ABI
compiler inputs, module exports and all packaged asset hashes. Exact original
GameSWF vertex/normal/premultiplied fragment shaders now compile on the emulator;
eight GPU color/alpha readback cases per context pass with maximum byte error 1.
Seven real texture frames pass portrait/landscape scaling and visible-support
checks in `../../../.local-inputs/authored-ui-live-b0c2093c-retry1`.
The first smoke attempt is retained: its frame predates Android's status-label
layout resize. The retry compares the latest submitted frame and actual UI
hierarchy before capture; APK bytes are unchanged. This is a genuine shader
primitive connection, not an original SWF display-list/menu/HUD implementation.
All five core prototype suites subsequently pass without retries on these
same bytes, including both stationary and moving combat. Candidate and promoted
checkpoint bytes are identical. The prior fully tested `47536bfe` checkpoint
and its historical 72-suite capture remain preserved.

Original UI ownership is mapped in `../../level-world/reference/ui-layout/NOTES.md`:
30 authored SWFs contain menu/HUD timelines, vector shapes, masks, embedded
fonts and ActionScript. The first planned connected subtree is the actual
HealthBars.player HP/MP panel. Original font provider selection and GFNT
discovery are recorded in `../../level-world/reference/font-text/NOTES.md`.
The native upstream GameSWF core now runs a real authored HUD probe, resolves
`_root.menu_HUD_0.HUDelements.HealthBars.player.bar_hp` and `bar_mp`, and emits
the authored meshes and masks. This is a native CPU probe, not a rendered HUD
checkpoint. A retained drawing facade is being connected to the Android GPU
backend. External bitmap exports, native localization calls and package lookup
differences remain required connections; full menu/action correctness is open.

The user's development order is now:

1. Connect the rendering capabilities DH2 requires: authored meshes/animation,
   textures/materials/lights/effects, UI panels/fonts/icons, aspect-correct layout
   and input, and retained ownership across Android lifecycle changes.
2. Reconstruct original menus and level selection, inventory/items/equipment,
   and HP/mana/skill HUD, with genuine data and working interactions.
   Validate presentation and UI interaction with controlled state first, then
   connect authoritative HP/mana/skill/item producers as those systems mature.
3. Complete the first playable level, including combat, loot, progression,
   transitions and save/load, then extend to the remaining campaign.

Accept rendering through representative connected scenes and original UI
resources, then use the first level to expose remaining engine requirements.
Do not gate gameplay on reconstructing every unused engine routine. Complete
in-flight queued startup, skills and particle emission work; further parallel
work targets UI resources/layout and actual effects rendering connections.
New checkpoints should demonstrate a consequential visible milestone rather
than merely packaging additional isolated source routines.

The effects-table/InitFX
stage passes 62 central suites and builds in both Android projects. Frozen
repository candidate `d78647af` bundles 347 prototype assets, including all
five exact effects streams, and sixteen native ELF64 libraries. Build capture
`../../../.local-inputs/native-effects-tables-build-capture.zip` contains 458
compiler inputs. All five emulator suites pass on that exact frozen APK:
rendering/rotation, movement, lifecycle, animation bank and combat. Eleven
monster CPU/script owners and the effects snapshot/debug-module identities
survive rotation. Both portrait and landscape screenshots were visually
inspected. See `native-effects-tables-d78647af-checkpoint-validation.json`.

The subsequent CanUpdate/material-color source stage passes 64 central suites,
with zero sanitizer findings. That stage is not included in candidate
`d78647af`. Material-color comparison covers 7,899 original/O2 cases;
CanUpdate covers 4,752 cases and 13,833 ordered services. Their native kernels
retain explicit missing visibility, world-player, material-factory and scene
attachment providers; no complete Character frame is claimed.

Both Android projects also build this 64-suite stage for ARM64 and x86_64.
`native-can-update-material-color-source-build-inspection.json` passes against
the 462-input capture, SHA256
`3f8f0c0bded3e0ad626a4d12ea071fbe982856c7642f90cd6e02cba8561270cd`.
Repository APK is `8d862f03`; Studio is `9456c60c`. Their live validation remains
NOT_RUN. The latest promoted, installed and fully emulator-tested checkpoint
at that historical stage was `d78647af`, whose captured source stage has 62 suites.

The delayed-Idle/startup/particle source stage now passes 69 central suites,
with zero sanitizer findings. Both Android projects build ARM64 and x86_64;
`native-delayed-idle-startup-particle-source-build-inspection.json` verifies
469 compiler inputs, 347 prototype assets and sixteen ELF64 libraries.
Capture SHA256 is `04e91f3689b734575e346da2500a70cd8402ab028ebdbf8ba7354c4fbce8026a`.
Repository APK is `11cf71b4`, Studio is `730a1a69`; live validation is NOT_RUN.
The original startup prefix explicitly stops at unreconstructed queue/controller
boundaries. Particle parameter sampling/blending/apply is recovered; particle
emission and full cloud ownership remain separate work.

The combined delayed-Idle audit covers all eleven actual Crypt placements and
four actual animation banks, with unchanged authored commons/monster scripts.
It selects source state 3 before AIS publication, samples CPU animation, then
loads the deferred script while preserving the same FSM and CPU owner. It passes
5,065 checks and 22 scene/animator phase pairs. Preceding InitPost/body/zone gates,
vitals and skill delivery remain explicit fixtures in this audit; it does not
establish a complete Character Update or frame order. Protected source Lua errors
and required native callback failures still need distinct provenance.

The subsequent vitals integration passes 72 central suites with zero sanitizer
findings. Its production DSO is tested through actual PropertyAdd symbol
interposition, without compiling a second vitals kernel into the test. Original
instruction comparison covers 900 cases and 3,333 ordered debug/property services;
the ARM64 proof also includes 133 live debug mutations. Identity and missing-skill
guards remain explicit. The new combined delayed-Idle/vitals audit passes 5,110
checks across eleven actual Crypt monsters, preserves the same FSM/CPU owners and
shared combat state, and delivers genuine HP/MP initialization and shared Debug
services. Skills and preceding body/zone/InitPost producers remain fixtures.

Both Android projects build the 72-suite stage. Inspection
`native-delayed-idle-vitals-source-build-inspection.json` passes against a
472-input capture, SHA256
`89319a018b64aa9b530d493874ca3eb431da0231047b38d0daf8f67827b89ac5`.
Repository APK is `47536bfe`, Studio is `f7c4fc00`; live validation is NOT_RUN.
Live validation subsequently passed on frozen `47536bfe` in the separate
checkpoint proof; the build-inspection report retains its historical NOT_RUN.
Rendering's first attempt compared against the previously installed APK before
installation and failed that guard; retry1 passes after explicit installation.
Bank's first attempt failed on a zero-displacement touch pulse. Its preserved
retry1 passes after bounded real-touch repeats, with every zero-movement record
retained and actual waypoint arrival still required. No game movement/collision
or range code changed. The checkpoint validator now accepts a separately bound
bank retry proof as well as the prior render/combat retry proofs.
The first central integration attempt exposed a test-linking mismatch: its
historical standalone linker wrapper could not observe the production DSO.
The corrected linked observer retains that historical test and all failed
attempt inputs; `character-delayed-idle-vitals-main-linked-host-audit-retry1.json`
is the successful central report.

The next queue/particle-ownership stage passes 74 central suites with zero
sanitizer findings. `character-queue-particle-factory-main-linked-host-audit.json`
executes the real central world and animation DSOs. Shared AI queue coverage is
10,463 operations, 3,729 callbacks and 110 reentries; missing Kill/unload providers
remain failures. Particle ownership coverage is 1,850 original cases, eleven
lifetime checks and four provider-contract checks. Exact constructors/registers
and retained FX emitter metadata are recovered; full cloud animation database,
simulation and rendering remain required. This 74-suite stage has no new SDK
capture or live APK claim and is not included in checkpoint `47536bfe`.

Development cadence now targets a complete runtime behavior milestone: native
monster startup/state ownership and genuine skill setup in the playable Crypt,
with the existing source script owner supplying required path/load/call services.
New leaf kernels are batched into central regressions; APK rebuild/rotation/
combat cycles should follow an actual runtime integration milestone instead of
each independently recovered helper. Each progress report must state the code
change, observable runtime effect, verification and remaining provider boundary.

Startup provenance now distinguishes the original delayed Crypt lifecycle:
AI.delayed_load is one, InitPost defers LoadScriptProcess and InitScriptProcess,
and LoadStates may select Idle before active AIS publication. The first
eligible original Character Update loads and initializes the script. The
current earlier-initialized live session is a development staging choice;
the recovered InitFX helper is not invoked after it with a false source-order
claim. A delayed-session adapter and original update-prefix gates now have
bounded native source implementations and central regression coverage.

Latest source reports:

- `../../level-world/reports/character-init-fx-main-linked-host-audit.json`
- `../../level-world/reports/character-can-update-material-color-main-linked-host-audit.json`
- `../../level-world/reports/character-delayed-idle-startup-particle-main-linked-host-audit.json`
- `native-delayed-idle-startup-particle-source-build-inspection.json`
- `../../level-world/reports/character-delayed-idle-vitals-main-linked-host-audit-retry1.json`
- `native-delayed-idle-vitals-source-build-inspection.json`
- `../../level-world/reports/character-queue-particle-factory-main-linked-host-audit.json`
- `native-effects-tables-source-build-inspection.json`
- `effects-tables-stage.json`

Combat's first attempt is preserved under
`../../../.local-inputs/player-scene-live-combat-d78647af/`. Its final movement
leg met the Y waypoint but drifted away from the previously reached X waypoint;
the source attack bridge correctly rejected the resulting out-of-range request.
The smoke now requires joint X/Y arrival through bounded real touch correction,
preserving every movement observation. The unchanged APK passes the stronger
test in `player-scene-live-combat-d78647af-retry1`, including stationary and
moving damage and finite return to Idle. No collision or range guard was
relaxed. Historical APK77 and all failed evidence remain preserved.

## Integrated native source

The central world library now includes the original-derived target search,
alias-aware script timer call, and private script owner. The Lua runtime adds
deferred VM construction and source-ordered library opening with retained
stack results; existing eager VM creation remains available. Alias contents
can be cleared before VM close while their wrapper remains alive.

The actual central owner/world/runtime libraries passed the main-linked
6,283-check host audit with ASan/UBSan and zero sanitizer findings. It covers
private sessions, constructor and ordered registration descriptors, source
null method gates, lifecycle, genuine Trace/alias/timer bindings and close
finalizers. Unimplemented gameplay registrations are explicit provider
fixtures; this does not establish the full original Lua namespace or live
enemy AI. Evidence:
`../../level-world/reports/character-script-owner-main-linked-host-audit.json`.

Target search and timer call have separate main-linked host reports. Their
reports bind the binaries at each proof stage; subsequent integration changes
the central library hash. They are not physical-device or complete gameplay
proofs.

Both the repository and linked Android Studio projects completed offline
`assembleDebug` for ARM64 and x86_64 with the initial integrated owner:

- Repository debug APK: 22,497,590 bytes; SHA256
  `0df1de6db8ba3bac58f2e5ecb47011676b5f8bdf48d3fe2b71a6f315d06f9d13`.
- Android Studio debug APK: 21,956,918 bytes; SHA256
  `f1b7941300282079b08624dd9634537daad2e2a504b7c53c0546ebc3d3c258e1`.

The repository APK was frozen, installed and tested on the API 37 x86_64
emulator. All four suites passed: movement, orientation/pause/resume lifecycle,
the complete recovered Prince animation bank, and stationary/moving combat.
The bank checks covered 116 resources, 158 requests, both animation slots and
finite attacks returning to Idle. Moving combat displaced the player about
20.31 world units. No physical ARM64 device has been verified.

The bank traversal harness uses a single Android touch swipe and fixed 200 ms
Walk pulses near waypoints. This avoids unstable timing estimates during
heading startup. It still requires actual source Move/Idle transitions and
displacement; no position injection or assertion relaxation was used. Earlier
blocked/timeout attempts and the wrong-ADB-path prelaunch failure are retained
in separate observation folders.

`script-owner-checkpoint-validation.json` reports build, assets and live PASS.
The APK contains eight ELF64 libraries per ABI with at least 16 KiB native load
alignment, and 233 bundled prototype assets. It does not bundle the original
ARM32 engine. The newly compiled private owner is not yet wired to live enemy
AI in the prototype.

`../../level-world/reports/script-owner-target-search-packaged-arm64-differential.json`
checks the actual stripped ARM64 world library extracted from this APK:
412 searches, 26,580 ordered callbacks, 1,480 accepted targets and nine
same-object reentry cases passed against the original binary. Virtual services
and libm fixtures remain explicit caller-provided boundaries.

The matching source archive is
`../build/checkpoints/dh2-native-script-owner-0df1de6d-source.zip`, 7,701,408 bytes,
SHA256 `4cf45f9a8657539aacd526049610ea1e3f540d0e2b96f311f120f2fcbae766d3`.
Its 568 source/build inputs were extracted and independently built offline for
both architectures: 39 tasks, 27 seconds, PASS. All 233 rebuilt APK assets match
the tested checkpoint. SDK/JDK/dependency caches remain external requirements;
the rebuilt APK is not byte-identical and was not separately gameplay-tested.
See `script-owner-source-rebuild-validation.json`.

## Engine and game scope

The working engine path loads Crypt geometry and textures, renders skinned
characters, blends animations and root motion, and supports collision and
navigation. Rendering/camera/lighting fidelity across the original game and
modern physical GPUs remains incomplete. The APK is a self-contained prototype
with bundled assets, not proof that all original game assets are covered.

The game prototype supports player movement, basic attacks, health/damage and
animation-event-driven combat. Full live AI/state transitions, quests,
progression, equipment/skills, original UI/camera, audio, saves and all levels
remain. Reconstructed source describes recovered behavior; it is not verbatim
recovery of the original developers' source text.

## Current parallel recovery

Newer source below is not included in the frozen 0df1 checkpoint:

- Native target providers passed 3,550 original/ARM64 comparison cases and
  2,241 ordered callbacks. The central linked audit passed 11,865 atomic,
  1,671 provider and 25 Sneak checks with zero sanitizer findings.
- External script virtual initialization and callback flags passed a central
  linked 49,244-check audit against the genuine Lua runtime.
- External private script ownership passed 2,863 extension checks plus all
  6,283 prior checks against the central world/runtime libraries. Actual cache
  follower scripts load two files and yield callback flags 0x3c2. Monster and
  Rene scripts expose missing property/table bindings; their failures are
  retained rather than replaced with invented globals.
- Exact source root loading and scoped Include passed host audits. The three
  workers continue persistent owner/cache integration, fixed-point/bit script
  functions, and original enemy/friend relationship queries.

Subsequent central integration passed the actual linked relationship audit:
4,332 original-gold cases, 4,789 ordered callbacks, nine nested relationships,
21,660 atomic and 12,996 provider/capacity checks, zero sanitizer findings.
Owned Include also passed centrally: 2,005 new checks plus 2,863 external-owner
and all 6,283 legacy checks. Source fixed/bit callbacks are now in the main
runtime; their original/ARM64 proof covers 13,978 cases. Earlier scalar
main-linked observations mixed source/test build stages. The separate
`script-scalar-current-main-linked-host-audit.json` now binds the restored
37-check baseline and its rebuilt binary; the three extra root checks live in
the separate 40-check source test. The correction is documented without
rewriting historical reports.

The combined central bridge audit passes GetPyCst/GetPyStruct/GetPyOID wrapper
gold1,200, actual VM457, scoped calls14/20/411 guards, Include80/34/56/17 root
checks/341 guards, and owned Include2,005/follower flags962. Actual original
design registration captures142 calls for138 effective names, and source
CharacterProperties member lookup passes228 real cache-backed queries. Full
design-manager lookup/ownership remains a genuine service boundary. Scoped
argument storage in that historical stage used extra Lua userdata. The corrected
central stage is `central-script-bridge-native-storage-host-audit.json`: native
argument storage, 16 scoped callbacks, 85 real Lua calls, 543 guards and 64
projection-error cleanup cases. Both scalar tests, design, Include and owned
Include pass in that same stage, with zero sanitizer findings.

GetProp and real Crypt property fixtures now pass centrally: 25,200 callback
cases, 432 direct sheet cases, 2,464 genuine cache words and 11 actual monster
script sessions with 165 Lua property checks. Other gameplay inputs in that
component test remain explicit fixtures. Owned Lua state registration and
transition order also pass centrally: 22,005 checks, 1,476 original cases and
1,800 ordered services. Actual monster, Rene and commons scripts have zero
RegisterAIState/ChangeAIState calls: their native Character state machine is a
separate required integration path.

The integrated source subsequently built both Android architectures in both
projects. Build-only outputs are repository APK 23,643,206 bytes, SHA256
`ed07c0e787214c8767dad6062495bea8ebc2099f999ff48f70569771b5820169`, and Studio
APK 22,549,142 bytes, SHA256
`0be815fed0dfa73785bae124ab8caf358a8d13d050d2da20d43bc9d6b8dab706`.
The frozen 342-input capture is `.local-inputs/script-bridge-native-build-capture.zip`
at repository root, SHA256
`48286837b634c1e28f5b04a35ead3447ac07616c9817ce95f08e20ebe68af50a`.
`script-bridge-native-build-inspection.json` verifies the actual packaged exports,
all16 ELF64 libraries, ELF and ZIP 16 KiB alignment, and the same233 assets in
both builds. Live validation is NOT_RUN: these outputs do not supersede 0df1
as the latest emulator-tested checkpoint.

Native constant backend now executes the original loader, integer/name readers,
owned map insertion/string helpers and getConstant in the source oracle. The
native central runtime passes 36 loads, 5,969 source-produced queries, 5,646
assignments and nine real Lua checks with zero sanitizer findings. Its ordinary
26 cache inputs contain 5,608 values, including AIStates and the exact
CharacterDesign/MaxLevelDVeryHard value100. Those 26 files (176,340 bytes) are now
bundled in the Android source assets, preserving the old frozen APK. The earlier
PARTIAL inventory report remains historical. The sound input mixes string-valued
records: the actual generic source loader stops at byte994/1,283 after27
assignments. This proves its early stop rather than complete sound decoding.
Original Application file order remains unproved.

Private SetInt/GetInt maps/coercions, SetLevel/property-cache producers and the
native Character FSM prelude are now compiled into the central libraries.
`../../script-runtime/reports/native-game-foundation-main-linked-host-audit.json`
passes all five actual central targets with zero sanitizer findings: constants
5,969 queries; integer bindings3,163 gold cases/35 real VM checks; Level1,136
cases/1,017,856 sheet words; constants-backed Level870 cases/779,520 words,
two genuine table loads/ten actual monster Init sessions/80 VM checks; FSM
77,771 checks/14 real linked-state compositions/25 VM getters. The existing
scalar/design/scoped/Include/owner suite also passes in the same binary stage:
`../../script-runtime/reports/native-game-foundation-bridge-host-audit.json`.
Runtime SHA256 is55feeec0e85cd336b716aee555636ec12cc299a01e624f907bb51f8d3851756c;
world SHA256 is9b3d2343bce8348b6a8ff22c9a49aed56f2655eada9b512480bc9833c9e62ae2.
The FSM bridges only four reconstructed state bodies; other state/effect
implementations remain explicit required services. Integer maps are not yet
owned/bound by every live private ScriptOwner session. No automatic enemy AI
is claimed.

Both Android projects then built ARM64 and x86_64 successfully. The NEW frozen
capture `.local-inputs/native-game-foundation-build-capture.zip` has350 compiled
source/build inputs, SHA256245701d5d556f5b080b084dd0d09ed1e8af829945cda7cbdc17b0b8992b862d1.
The repository APK is23,653,057 bytes,
SHA25622835b1ea01babbad9d8078e4df247a1bcdffe2db81172c1e25e69fea3f7b748;
Studio APK is22,608,265 bytes,
SHA256ade72619bddd1a628f9678619b479063570012dfa48462f0faf7fd27ac10e84f.
`native-game-foundation-build-inspection.json` verifies actual compiled inputs,
exports, all16 ELF64 libraries with16 KiB alignment,258 matching assets and
the26 original ordinary constants files in both projects. Live validation is
NOT_RUN. These build-only APKs do not replace0df1 as the latest tested checkpoint.

Parallel work now targets the actual registered design-table lookup backend,
native stun/scare state/effect bodies, and private-owner integer-map integration.

These are prerequisites for connecting original scripts to live game objects.
Separate historical reports bind their own source and binary hashes; they do
not establish APK or physical-device execution for newer source.

## Latest tested checkpoint: 22835b1e

The frozen `build/checkpoints/dh2-native-game-foundation-22835b1e.apk` now passes
all four emulator suites: live movement, rotation/pause/resume, Prince animation
bank, and stationary/moving combat. Every suite verifies the same installed APK
SHA256 above. `native-game-foundation-checkpoint-validation.json` records their
report hashes, the frozen 350-input build capture and a visual inspection of the
textured Crypt scene. The earlier build inspection's NOT_RUN remains historical.
This APK supersedes 0df1 as the latest emulator-tested checkpoint.

Its additions are the constant loader and 26 ordinary constant assets, private
integer binding kernels, level/property recalculation and the native Character
FSM update prelude. These compile into the APK but are not all connected to
live original character scripts. The visible app remains a Crypt prototype.
Full enemy AI, all game assets, campaign, original UI/audio/saves and physical
ARM64-device verification remain unfinished.

New private-owner integer integration and the design-table backend have since
passed separate source/host tests; they are not in this frozen APK. Native
stun/scare effects are also being extended separately. Central integration and
another Android build are the next stage.

The recovered AISDefault constructor allocates 0xc4 and initializes fields at
+0xb8/+0xbc/+0xc0; player-only vector/skill fields differ. Current extension
notes correct earlier prose about a smaller allocation without changing
historical proof inputs. Runtime callback flags are separate from the initial
constructor field snapshot.

## Central gameplay services and authored level data

Private-owner native integer mode and the actual registered design-table backend
are now in the central world/game-data libraries. The main CMake owner tests pass
6,979 native-mode checks, 6,283 legacy checks, 2,863 external-owner checks and
2,005 owned-Include checks. The same central stage passes ten original monster
Init sessions with 160 VM checks, 1,110 registered queries and 870 Level cases.
World/session inputs in that test remain explicit fixtures: its borrowed
GetHostPlayerLevel calls Character.GetLevel, whereas the original wrapper reads
the separate PlayerInfo cached level. New host-context recovery preserves that
distinction. These tests do not establish live enemy AI.

Native stun/scare producer, Focus, Update, Blur and event bodies are also in the
central world library. Both actual CMake audits pass 3,824 original-gold cases,
15,549 ordered service requests and 203,118 checks with zero sanitizer findings.
Registered StateInfo ownership and destination-state integration are still
required; no successful missing-state transition is fabricated.

The new native Level/FastTravel decoder executes all 33 travel destinations and
51 authored levels. Original array/row readers produced the cache and synthetic
gold; optimized ARM64 matches 212 records, 2,749 scalar words and 327 strings.
Central host replay passes 14,445 checks, including 11,214 truncated-prefix
guards. Actual Crypt01 ranges are minimum [8,45,74], maximum [10,47,76]. Current
Application row selection and difficulty are separate required session producers.

`monster-init-assets-bundled.json` verifies five additional exact cache assets in
both source projects: level records/names/schema and AI `_commons`/`monster`.
They are staged for the next build; the frozen 22835 checkpoint is unchanged.
All-game packaging and original scripts executing in the APK remain unfinished.

Spatial getters have separately passed original/optimized ARM64 and sanitized
VM checks. Host-session wrappers and persistent table/constant ownership are
being completed for the next live stage: all eleven original Crypt monster
Init sessions, retained across GL recreation. Monster has no OnUpdate override;
its live behavior must come from source target events. Target ownership/Lua
object wrappers, per-monster physical actor/FSM/timer/playback ownership and
the full character frame ordering remain required after initialization.

## Main-library monster initialization prerequisites

Spatial getters, host-session wrappers and the persistent CharacterGameDesign
snapshot owner are now compiled into the central world library. The actual
CMake targets pass in `../../level-world/reports/character-init-services-main-linked-host-audit.json`:
2,746 original spatial cases, 1,233 host-context cases with 31,004 checks and all
51 level rows across three tiers, and 10,709 design-owner checks including
2,061 real VM checks and a finalizer after the design owner has been destroyed.
Six recorded CMake compilation commands use ASan/UBSan; all three targets have
zero sanitizer findings. The main world SHA256 is
`277060ecf89d9d636f2a803b3612d39bfa32140ab9185cd0a5a8b99bbd3e068d`.
Data and runtime retain their prior level-stage hashes.

The design snapshot pins copied tables, constants and class/name views for each
borrower's lifetime, including Lua finalizers. Explicit reload is rejected while
borrowers exist; this adapter does not claim original Application reload order.
Host queries preserve PlayerInfo cached level and the separate active-Level
difficulty/range inputs. Application/player-session ownership is still an
explicit required producer.

These changes are source/host verified only. The latest emulator-tested APK
remains 22835b1e. Eleven persistent Crypt monster sessions, live initialization,
GL recreation preservation, full target objects and enemy frame execution remain
the next integration work. Original owner timer constants are now identified as
CharacterDesign/AI_Tick (event 0x33) and CharacterDesign/DoT_Tick (0x34); a genuine
DebugSwitches backend for native level/stat changes is being recovered separately.

## Latest tested checkpoint: f19abb6a, original monster initialization

`build/checkpoints/dh2-native-monster-init-f19abb6a.apk` supersedes 22835 as
the latest emulator-tested checkpoint. SHA256 is
`f19abb6af431c3b61c64380fb9501f5bfdf92fc2d90c83e19ee16fc0f1cb223d`.
The packaged APK is 23,598,227 bytes and contains 263 assets and sixteen ELF64
libraries across ARM64 and x86_64, with 16 KiB LOAD and ZIP alignment.
Both the repository and Android Studio projects build successfully.

All eleven authored Crypt monster placements now own independent persistent Lua
sessions and execute the actual `_commons` and `monster` initialization scripts.
Their host-level query reads the separate player cached level; the development
Crypt selection explicitly chooses normal difficulty and its authored range
[8,10], yielding monster level 8 for host level 1. Original native level changes
refill the actual monster property sheets. Authored timer records are events
0x33/0x34 with intervals 3000/1000. Timer expiry/full enemy frame execution is
still pending. Graphics recreation retains the same script sessions, properties,
combat backing and initialized globals; it does not repeat initialization/refill.

The real app-private DebugSwitches missing-file path is connected. Existing-file
parsing remains an explicit unsupported service. Persistent design tables,
constants, spatial and host queries, owner integer kernels, target kernels and
the new Lua object table/metatable bridge compile into the APK. The object bridge
still needs scene identity resolution and target-event integration for live AI.
Unreconstructed callbacks fail explicitly rather than reporting success.

The first candidate 1e4f916b crashed in Lua weak-table GC under Android FORTIFY.
The final APK uses bounded searches over the known TString length, preserving
first-NUL semantics. Main CMake replay includes 451 Lua checks with weak-key,
weak-value and embedded-NUL regression cases, plus eleven initialization/service
audits, with zero sanitizer findings.

`native-monster-init-f19abb6a-checkpoint-validation.json` binds five passing
emulator suites to the same installed APK: original initialization/rotation,
movement, lifecycle, Prince animation bank and stationary/moving combat.
Portrait and landscape screenshots show the textured Crypt and player using the
prototype controls. The frozen build capture records 375 source inputs; the final
build inspection matches the main-library audited sources, including Lua GC.
Earlier captures/reports remain historical and unchanged.

The engine foundation is operational in the emulator; complete rendering parity
and physical ARM64 testing remain unfinished. The game remains a Crypt prototype.
Full enemy FSM/target/timer behavior, original Application/session creation,
campaign/quests, inventory/skills/loot, original UI/audio/saves and all-game asset
packaging remain required. This checkpoint is one self-contained prototype APK;
it is not yet the complete game in one APK.

## Source integration after f19: target callbacks, state bodies and timer effects

`../../level-world/reports/character-target-events-state-bodies-main-linked-host-audit.json`
binds seventeen passing actual main CMake audits to the current source and DSOs,
with recorded ASan/UBSan compiler commands and zero sanitizer findings. The
world/runtime/data SHA256 values are respectively
`1db6a7059a23889ba8f4fdc68ab0527c063067b6c445dd7e694bfec9a5e0d0d1`,
`509d1c0eeff5012b9772c426a6e6f765d456e0797edabba3b73aef539a333c14`,
and `777e58075cab4a266df8c8dafbff4273e5dff7c1268dbbdab84ef5b512fe60fb`.

Optional caller-owned target/FSM/object providers now deliver GetID, HasTarget,
GetTarget, SetTarget, ClearTarget, GetState and GetStateTime at their original
registration positions. A 490-check real Lua session audit verifies fresh target
tables, scoped object arguments, return-table metamethod projection, nineteen
guards, fresh callback alias lookup and the actual monster enemy callback's
target mutation before its unsupported HeadTo error. Generic APIs retain their
source-object rejection. The supplied target/FSM/host records are explicit
test projections; these adapters are not yet passed by the live renderer.

The bounded Idle/Move/Attack/Dead Focus/Blur bodies compose against 937 original
transition cases, and their isolated OnEvent bodies against 690 original cases.
They omit the outer StateInfo transition choreography, which remains a separate
owner integration task. Central identity primitives replay 1,166 original cases;
active AISExternal target wrappers replay 465. Direct AIS dispatch is an endpoint,
not a replacement for Character RaiseEvent and its CharAI handler prefixes.
Those genuine event producers and live world identity ownership remain required.

Central timer effects replay 1,456 original expiry cases and 106,022 checks,
including actual TimerStore expiry to regeneration and inactive DoT paths.
Positive DoT Calculate/Apply, full buff ownership, source FSM ownership and full
enemy frame composition remain under reconstruction. Live monster timer expiry
is not claimed. Lua extreme signed integer lookup now uses the original unsigned
range semantics without signed overflow; all 454 runtime checks pass.

Both repository and Android Studio assembleDebug builds pass for ARM64 and
x86_64 with these source additions. They have not been installed or emulator
tested in this stage. The frozen f19abb6a APK remains the latest tested checkpoint;
its original five-suite proof and build capture remain historical and unchanged.

## Source integration: native state owner, commands and target producers

`../../level-world/reports/character-frame-command-AI-route-main-linked-host-audit.json`
binds 25 passing actual main CMake suites to current source, rebuilt executable
headers and production DSOs. ASan/UBSan and leak checks report zero findings.
The earlier 19-, 23- and 24-suite reports remain historical stage proofs.

The native Character state owner now owns all twenty independent StateInfo/event
maps, including the original 109 registration operations. Bounded Idle, Move,
Attack and Dead behavior bodies compose through the actual transition owner;
the frame adapter runs the original outer Update once, advances elapsed once
and reloads the current state after synchronous stun/scare calls. Its original
replays cover 906 bounded frames and 1,536 outer frames. Other nonempty behavior
families and Spawn retain required provider boundaries.

Native buff ownership composes with the actual TimerStore. Original/O2 replay
covers 528 cases, and the linked host audit has 268,980 checks and six genuine
timer expiries. Full source property recalculation, FX lifecycle and positive
DoT application still require the appropriate live backends. Timer expiration
is not yet connected to live Crypt monsters.

The full Lua Stop/HeadTo/MoveTo/Attack/Flee/HasPath wrapper bodies now replay
4,196 original cases. The actual monster OnEnemySpotted script runs through
SetTarget and HeadTo into the real reconstructed ControllerCharacter and PathTo
kernels in the 544-check Lua session audit. Navigation deliberately returns zero
through an explicit failed-FindPath provider; this proves the command chain, not
scene navigation. Optional session bindings support forty original registrations
when supplied all target/FSM/object/command providers; the default live monster
session still supports twenty-seven and has not acquired those providers.

The original target update producer replays 2,225 cases and 1,625 Character event
deliveries. Target-handler prefixes replay 2,013 cases using the actual 76 AI
rows for source sound IDs. These prefixes retain tracing, aggro, sound and
current-AIS ordering. The existing complete CharAI event dispatcher passes
11,344 cases: permitted target events stop after their handler; blocked events
forward to the FSM; EnemySpotted reloads the owner after its handler and then
forwards. Direct AIS Lua delivery alone does not replace that routing.

`state-frame-command-source-build-inspection.json` binds both successful Android
builds to `.local-inputs/native-state-frame-command-source-build-capture.zip`.
The capture records 394 source/build inputs. Both APKs contain the expected
native exports, sixteen ELF64 libraries with 16 KiB alignment and the same 263
prototype assets. The original ARM32 game library is absent. Repository APK
SHA256 is `bc902d745557c552ec7cd9759b3a40032583114d6370a1303caf8837d5b7b3b3`;
Studio APK SHA256 is
`a75a44e444008eacc47c74caefb4911cf7e2e83a6ea1b09ea6d60404963385bb`.
These are compilation/provenance proofs; live validation was not run.

The latest installed and five-suite emulator-tested checkpoint is still
`dh2-native-monster-init-f19abb6a.apk`. No new visible game behavior or physical
ARM64 testing is claimed by the source builds above. Required next integration:
per-scene native identity/lifetime ownership, genuine owner/AI/physics/navigation
and animation services, full Character frame scheduling and positive DoT combat
delivery. Full campaign, original menus/UI, audio, saves and all-game packaging
remain unfinished.

## Latest tested checkpoint: aef5c9f4, native scene objects

`../build/checkpoints/dh2-native-scene-objects-aef5c9f4.apk` is the latest
fully emulator-tested checkpoint. The frozen build capture records 401 inputs;
both repository and Android Studio builds pass. The package contains 263
prototype assets and sixteen ELF64 ARM64/x86_64 libraries with 16 KiB load and
ZIP alignment. It contains no original ARM32 game library.

All eleven original Crypt monster Init scripts now use retained native scene
object and CharAI records backed by their actual property, life and position
data. Original GetID/HasTarget calls are verified at startup, and the same
records and script sessions survive rotation. Each live session supports 32
original registrations. Commands and full FSM providers are not yet connected
to live enemy AI.

Lua VM destruction now preserves the valid callback scope while lua_close
runs finalizers. The native ownership audit verifies that an actual finalizer
can read its target and clear it before the providers are destroyed. The
29-suite actual main-CMake host audit passes with zero ASan/UBSan/leak findings.

`native-scene-objects-aef5c9f4-checkpoint-validation.json` binds five passing
API 37 x86_64 emulator suites to this exact installed APK: monster initialization
and rendering, movement, pause/rotation lifecycle, Prince animation bank, and
stationary/moving combat. Portrait and landscape captures show the textured
Crypt and player with prototype controls.

The enemy controller remains the prototype controller. Source damage-over-time,
state/frame and command kernels are compiled, but their complete live service
chains remain unfinished. New EnemySpotted, PreSpawn and HitFor work has separate
bounded proofs and is not included in this frozen checkpoint. Full original
enemy AI, physics/navigation integration, campaign, inventory, UI, audio, saves,
all-game asset packaging and physical ARM64 device validation remain.

## Source integration: enemy events, PreSpawn, damage and ClearAggro

`../../level-world/reports/character-clear-aggro-spawn-enemy-hit-main-linked-host-audit.json`
binds thirty-five actual main-CMake suites to current compiler/source/input/DSO
hashes. All pass with zero ASan/UBSan/leak findings. Earlier thirty-three- and
thirty-four-suite reports and their captures remain historical stages.

The shared library now contains complete original-derived EnemySpotted and
PreSpawn bodies, the supported offline/nonplayer HitFor coordinator, and genuine
ClearAggro storage/notification/target/Stop ordering with its scoped Lua wrapper.
Original/O2 proofs cover 409 EnemySpotted cases, 538 PreSpawn cases, 2,616 HitFor
cases including synchronous reentry, and 1,792 ClearAggro cases, with zero
mismatches within each stated service/branch scope. Unsupported player/online
HitFor tails and full Kill remain explicit boundaries.

A new native composition bridge routes owned state-machine calls through
PreSpawn and eighteen instruction-proven empty methods. Its 548-check audit
verifies actual PreSpawn-to-Idle transition, a nested interactive callback,
wrapping elapsed advanced once, sixty-five metadata/owner guards and forty-three
required nonempty method failures. This bridge is native architecture over the
recovered bodies; scene animation/physics/Revive services remain explicit test
providers in that audit.

The actual monster target pipeline runs original EnemySpotted Lua through the
source event router and controller/path kernels. The separate new ClearAggro
suite executes original monster OutSight using genuine ClearAggro instead of
the historical test-only wrapper suffix, including nested same-VM delivery and
actual finalization. OnDeAggro/Stop scene bodies, failed FindPath, sound and
other declared host backends remain fixtures. These tests establish component
composition, not complete original enemy AI or whole-frame parity.

`native-clear-aggro-spawn-enemy-hit-source-build-inspection.json` binds successful
repository and Android Studio builds to the frozen 411-input capture
`.local-inputs/native-clear-aggro-spawn-enemy-hit-source-build-capture.zip`, SHA256
`d9819e904cd273d442dac9e71e601ae7a0677368d818a0e31b07bdb367b562ae`.
Both ABIs expose the new functions; sixteen ELF64 libraries retain 16 KiB
alignment and the same 263 prototype assets. Repository APK SHA256 is
`1581c8a405e96e62c2b65f83c4a5b1f52eee69687a012480a0722f9b1e833c83`;
Studio APK SHA256 is
`727bc1ba48f0c5da0e2616ac3e7221c8334b4988d9920ec86f5276c3160c7cbb`.
Live validation for these source builds is NOT_RUN. The latest fully
five-suite emulator-tested checkpoint remains aef5c9f4, and its frozen build
and validation reports are unchanged. Native-source additions have not yet
replaced the live prototype enemy controller.

Next integration dependencies include an owned player scene record, authored
DesignSettings ownership, genuine spawn-delay/Spawn behavior, full Kill/event
delivery and live scene controller/navigation/animation services. Full campaign,
inventory, original UI/audio/saves, all-game assets and physical ARM64 validation
remain unfinished.

## Latest tested checkpoint: retained player scene backing d57c6e01

The latest checkpoint is now `dh2-native-player-scene-d57c6e01.apk` (26,212,515
bytes). This supersedes the latest-APK statements in the historical sections.
The player record uses the actual KnightPlayerBase gameplay property/life
backing, existing identity 0x100000001 and current native actor position. It
joins the eleven stable monster records; all twelve retain their identity and
backing through GL context recreation. Original monster Init sessions can
resolve the player through the native Lua object bridge. Full original live
player/enemy AI remains unfinished.

`../../level-world/reports/character-player-scene-main-linked-host-audit.json`
records 36 passing actual main CMake audits with zero sanitizer findings,
including 453 checks for actual player target/life/position ownership and Lua
close finalization. Repository and Android Studio builds passed for ARM64 and
x86_64. `native-player-scene-d57c6e01-build-inspection.json` binds the immutable
411-input source/build capture, compiled libraries and 263 prototype assets.

`native-player-scene-d57c6e01-checkpoint-validation.json` binds five passing
suites to the same installed APK: initialization/scene records, movement,
rotation/pause/resume, Prince animation bank, and stationary/moving combat.
The API 37 x86_64 emulator is verified; physical ARM64 testing is not. This is
one bundled prototype APK, not yet the complete game with all its assets.

The live prototype enemy controller remains. The next integration work is
object-position queries, owned DesignSettings, recovered Spawn/Kill behavior,
and genuine live event/controller/navigation/animation services. Independent
worker proofs for DesignSettings, Spawn and Kill do not establish their central
integration or live APK execution. See
`../../level-world/reference/character-player-scene/NOTES.md` for this checkpoint.

## Latest tested checkpoint: owned target inputs and positions 9112998a

`dh2-native-owned-target-9112998a.apk` (25,371,034 bytes) supersedes the latest-APK
statements above. The retained object registry now routes original GetPosition
0x38e700 to its genuine spatial getter, returning each object's current raw
three-coordinate point. Android read-only probes execute this actual object
method for player and monster records in every private monster VM, including
after GL recreation. They do not supply an original AI event or move an actor.

The Android world now owns the complete first 43-field DesignSettings table,
its names/schema and the actual authored EnemySpottedAggro word 0x41200000. Three
original cache assets were added; the backing remains stable after rotation.
Central CMake now includes original-derived Spawn selection, permission/body,
and Kill/Ctrl_Kill routines. Their required scene/loot/XP/quest/death backends
are not all implemented or connected to live enemy behavior.

`../../level-world/reports/character-owned-target-spawn-kill-main-linked-host-audit.json`
binds 42 passing actual central suites with zero sanitizer findings. The new
object-method composition has 448 checks; the actual original monster target
pipeline using owned settings and a KnightPlayerBase target has 463 checks.
Both repository and Studio ARM64/x86_64 builds passed. The 421-input immutable
capture and required exports are checked in
`native-owned-target-9112998a-build-inspection.json`; 266 prototype assets and
16 ELF64 libraries retain 16 KiB alignment without the original ARM32 engine.

`native-owned-target-9112998a-checkpoint-validation.json` binds five passing
emulator suites to the same installed APK: original Init/position/settings,
movement, lifecycle, Prince bank and stationary/moving combat. Portrait and
landscape screenshots show the textured Crypt/player with prototype controls.
No physical ARM64 phone is currently attached or verified.

The prototype enemy controller still runs. Next integration includes the
separately frozen owned Spawn adapter, AI OnDied/SetDead and CancelSneaking,
with real owned scene/timer/animation/skill backings. Preserve original authored
and inherited initialization: the current DACT actor conversion drops part of
the ai_state and spawn properties. Full game systems, campaign, original
UI/audio/saves, all-game assets and full rendering fidelity remain unfinished.
See `../../level-world/reference/character-owned-target-integration/NOTES.md`.

## Source integration after 911: owned Spawn, AI death and sneaking

The current central world source additionally compiles the separately frozen
`character_spawn_owner_extensions`, `character_ai_death` and
`character_cancel_sneaking` modules. The tested 9112998a checkpoint and its
42-suite build capture remain unchanged and are still the latest live proof.

`../../level-world/reports/character-spawn-owner-ai-death-sneaking-main-linked-host-audit.json`
binds 45 passing actual CMake suites, source/corpus/compile records and pre/post
library hashes, with zero sanitizer findings. The world hash is now
`f90819b3d86eb716d9fc1e4bc9a2d5d5dd32c535f3e5677335df1f4797979695`;
runtime/data remain
`3f1886b22c2230e6ddda5c75fae248d0a710b75034eac3144489f9992b7f9945` and
`13c9499376f16fdddf0639987a7cc735b12d57a14c68b1f1ba23694a5ab35569`.

The owned Spawn adapter has 83,930 checks including genuine transitions/timers,
PreSpawn→Spawn→Idle and nested event/notification delivery. AI death has 161,578
checks and 896 failure prefixes; its genuine event2/debug/target composition
stops at the required SM_SetDeadState backend before touching timers.
CancelSneaking has 34,930 checks, 976 original cases and 152 actual-cache cases;
full buff deletion and skill VM ownership remain required where selected.
These are real component compositions with declared scene/backend fixtures,
not complete live enemy behavior.

Both Android projects built both ABIs. The immutable 427-input source capture
`.local-inputs/native-spawn-owner-ai-death-sneaking-build-capture.zip` has SHA256
`8b7d723ac7811ba87bafaaa5a474e538d36c052ca791ab716d7209deabc2967c`.
`native-spawn-owner-ai-death-sneaking-source-build-inspection.json` verifies
captured audited inputs and new exports with the same 266 prototype assets and
16 aligned ELF64 libraries. Repository APK hash is
`54eea334dd1191ec27fa215c5cf8c16a29b609b9915b1457160e9b9b78d8d653`;
Studio APK hash is
`1e14312af93ec4de8911fcb88346e13a159c4fe38d9c3c07f5d7c853a3ad2061`.
Live validation for these newer source builds is NOT_RUN.

The live-owner readiness investigation uses actual shipping MGP XML from the
authorized cache ZIP. The `.local-inputs/world/crypt01/crypt01.dwld` input is a
port-produced descriptor, not that original XML. Five monsters explicitly
author ai_state=Idle; six omit it. Original string descriptor defaults are
empty and source GetPreSetAIState maps empty to Idle3. Shipping LoadTemplate is
an assertion-policy routine; it does not apply a Monster template resource.
Preserve authored versus omitted values and original property reset/load order
when extending actor packaging and constructing the real live state owners.
Current parallel recovery is addressing death-state selection and owned skill
tables. Original scene/runtime/controller integration and full-game work remain.

## Current source integration: death selection and owned SkillsTable

The central native build now also includes frozen `character_dead_select` and
`skill_tables`. The existing 9112998a checkpoint, its immutable capture and five
passing emulator suites remain the latest tested live checkpoint.

`../../level-world/reports/character-dead-select-skill-tables-main-linked-host-audit.json`
binds **47 passing actual central CMake suites**, the original/O2 gold reports,
current compiler records, input hashes and before/after DSO hashes, with zero
sanitizer findings. World SHA256 is
`5096be2bb4df26c9772af69b89fb07e140d25f1ef6926b073cd762a902f50b5a`;
data is `ac2e2f7c4b568db0d4bfee2ae6101aa38fef376fd6f8105739b2db76b65b056d`;
runtime remains `3f1886b22c2230e6ddda5c75fae248d0a710b75034eac3144489f9992b7f9945`.

Death selection replays 2,400 original cases and 40 synchronous reentries;
186,572 host checks include 14 genuine StateOwner/animation-table/constant/stance
compositions and 1,384 failure prefixes. It preserves source current-state
clearing and death animation selection. Required downstream Dead behavior scene
services still reject explicitly when unavailable.

The complete skill owner retains all 36 ordered lists, 127 full Skill records,
both name blocks and schemas. Its 40,212 central host checks include 14,868
malformed/ownership guards and a genuine CancelSneaking call using retained
actual-cache backing. Original/O2 evidence covers the bounded parsers and
lookup routines; C++ immutable ownership is separately sanitizer-proved.
Whole skill VM/buff behavior is not established by this loader.

Both repository and Android Studio ARM64/x86_64 builds passed.
`native-dead-select-skill-tables-source-build-inspection.json` binds the new
431-input immutable capture `.local-inputs/native-dead-select-skill-tables-build-capture.zip`
(SHA256 `861cca50762c1b1297623c69f9d75a5ce027548e967419c67695a93e4e59100f`),
actual compiler inputs, new exports, 266 prototype assets and sixteen ELF64
libraries with 16 KiB alignment. Repository APK SHA256 is
`0a5bcc44511031e62609e1bdec081255ad4f8e3fcd9dd402d393c4b17efbf04d`;
Studio APK is `37ca0437f4dc850a9b32319b6f3c9b1c95aa1789885d0175dffeb6cea45e95fb`.
These source-build APKs have live validation **NOT_RUN** and are not presented
as newer tested gameplay checkpoints.

Active follow-up work preserves actual authored initialization separately from
descriptor defaults, builds a retained skill-to-character adapter, and records
the original per-monster animation registration occurrences needed by live
playback/state ownership. The current live prototype enemy controller remains
until those native owners and mandatory scene services are connected. Full
campaign, inventory, quests, original UI/audio/saves, all-game assets, rendering
parity and physical ARM64 validation remain unfinished.

## Current source milestone: retained monster playback and initialization

The central world now includes owned `character_sneaking_tables`,
`actor_initialization` and `character_animation_instance` modules. The latest
fully live-tested checkpoint remains **9112998a**; the renderer has not yet
enabled these per-monster owners or replaced its prototype controller.

`../../level-world/reports/character-owned-monster-animation-initialization-main-linked-host-audit.json`
records **53 passing actual main CMake suites**, with zero sanitizer findings.
The actual four original-cache monster animation banks run 720 native frames
across independent per-character CPU playback owners. Repeated registration
occurrences and first resource-identity lookup are preserved. Ghost's template
retains the signed -133ms start. Genuine owned Idle Focus selects the native
sequence and flags0x2380; its outer Character notification remains a declared
test fixture, so full live AI/event/physics integration is not claimed.

The CAI1 port sidecar preserves actual shipping MGP attribute presence, requested
template, source member hashes and descriptor-resolved initialization. All11
presets resolve3, preserving five authored Idle and six absent/empty strings.
Its 4,780 checks include 4,724 atomic rejections and direct-XML projection
comparison; it is not a full shipping XML/factory implementation. The retained
skill adapter supplies full ordered 36-list/127-row views and payload pinning,
with 9,211 checks. Full Buff/skill VM providers remain explicit.

World DSO SHA256 is `a519e8cd20de2c0cf41c6791a1d48ab63c6d1afc2a09c7266387110481440870`;
data/runtime remain `ac2e2f7c4b568db0d4bfee2ae6101aa38fef376fd6f8105739b2db76b65b056d`
and `3f1886b22c2230e6ddda5c75fae248d0a710b75034eac3144489f9992b7f9945`.
Both Android projects built both ABIs.
`native-owned-monster-animation-initialization-source-build-inspection.json`
binds the new437-input capture (SHA256
`321bf331f3a7f7a57c331856e60da489ab6a687531e5dfab5a56993499969dac`),
required compiled modules/exports, unchanged266 packaged prototype assets and
16aligned ELF64 libraries. Repository APK SHA256 is
`267ed95502e9b0766341491464f658c41d6ccd7bd4f5c5eb074bcec25c7deb04`;
Studio APK is `b6b8577a13a2e14c5982a259cc0f6ec7ca5f8a2b4b4feb203a81a2841189f8c4`.
These source builds have live validation **NOT_RUN**. Original monster banks
and CAI1 are currently staged/host-tested, not yet additional bundled assets.

Next work is genuine state-change/animation-end callback delivery and independently
computed asset digests, followed by live retained state-owner/playback startup.
See `../../level-world/reference/character-animation-instance/NOTES.md` for
ownership contracts, scope and the required live integration order.

## Current source milestone: packaged monster resources and native callbacks

The latest fully validated live checkpoint remains **9112998a**. The current
source build is newer, but its five-suite emulator checkpoint proof is incomplete.

`../../level-world/reports/character-monster-backings-callbacks-main-linked-host-audit.json`
records **56 passing actual central CMake suites**, zero sanitizer findings and
unchanged source/input/library hashes during execution. It adds original-derived
state-change/animation-end relays and actual private-Lua callback composition,
plus native SHA256 checks of complete animation resource bytes. World DSO hash
is `6ecf806974095c9ed4b3620483e2714cceb50d1a94b34f80d3ecffa513efc0a8`;
Scene DSO is `08b26f5c9f913919fd76e054e1f2aa2dc5ff4d0ac073cd7171ef08f96c975411`.

Both Android projects build ARM64 and x86_64. The immutable443-input capture
`.local-inputs/native-monster-backings-callbacks-build-capture.zip` has SHA256
`72d5aa93c34afcbf5835ad7111a13de2de618629f2821d5b708bf1d6f44eeefa`.
`native-monster-backings-callbacks-source-build-inspection.json` passes actual
compiler/export checks and validates **342 packaged prototype assets**, sixteen
aligned ELF64 libraries and no original ARM32 library. Repository APK SHA256 is
`77f1e1cd67c8fe656c95c89f2c9535e7ae37d432c86bd9f9773d96e12e31961e`;
Studio APK is `5e6d806f83dae12d11c2f977c6cc1abf10a095a44cace72ec4884935802f8349`.

The renderer retains full skill backing, independently hashed CAI1/DACT bytes
and shared resources for all four Crypt monster banks, with eleven private CPU
animation instances. These are **prepared, inactive playback owners**; the
prototype enemy controller remains. Full original FSM/AI/physics integration
is not claimed by these resource owners or callback modules.

The repository APK is currently installed on emulator-5554. Initial live checks
loaded all eleven CPU owners and matched authored startup state. The first test
lost the ADB transport during rotation; preserved artifacts are in
`.local-inputs/player-scene-live-77f1e1cd`. A second fresh launch timed out in
ActivityManager; artifacts are in the separate `player-scene-live-77f1e1cd-retry1`
folder. Post-timeout logs show eventual eleven-monster initialization and frame
submission, alongside broad Android UI/Binder delays. Rotation retention and
the cause of the slowdown remain unverified. Neither attempt is a passing
checkpoint, and no new checkpoint APK is advertised as validated.

Next work connects one genuine character state owner to script startup and
animation callbacks, supplies source monster bounds/body inputs and recovers
required FX services. Full campaign, original UI/audio/saves, all-game assets
and physical ARM64 testing remain unfinished.

## Latest validated checkpoint: retained monster backings 77f1e1cd

`native-monster-backings-77f1e1cd-checkpoint-validation.json` binds all five
passing emulator suites to the exact frozen APK and its original 443-input
source capture and 56-suite host audit. The checkpoint adds 76 packaged inputs
over 9112998a: four monster animation banks, their animation resources, full
skill backing and authored initialization sidecar. There are now 342 prototype
assets inside the APK. Eleven separate native CPU animation owners and their
private script/object records survive rotation. Native SHA256 verifies the
actual initialization and animation bytes. These CPU owners are prepared;
the original live monster FSM has not replaced the prototype controller.

The development emulator was rebooted after broad UI/Binder/graphics delays.
Render retry3 verifies actual textured portrait and landscape viewport pixels,
not just a frame-submission log. Movement, lifecycle, bank and combat suites
all pass on the exact 77f1e1cd bytes. Combat retry1 preserves its real touch
inputs, movement thresholds and destination checks. The earlier zero-motion
pulse ran only two movement frames during a GPU stall; source floor and decor
probes found a clear route. The harness now allows at most two insufficient
movement pulses before failure, using longer real-touch input rather than
changing game position or accepting zero displacement. Earlier failed folders
and their reports remain preserved.

## Newer source-only milestone: Idle update and original NPC bodies

`../../level-world/reports/character-npc-body-idle-main-linked-host-audit-r3.json`
passes 58 actual central CMake suites with zero sanitizer findings. Source Idle
update, marker/skin bounds and original body creation requests are integrated.
All eleven genuine Box2D bodies are created and destroyed in the native audit.
The NPC producer fixture comparison preserves geometry, pose, flags, body
requests and all pointer aliasing, normalizing only process-local addresses.
It establishes the bounded factory-scene/static-body composition, not live
monster navigation or a full Character factory.

Both Android projects built both ABIs. The 447-input immutable capture
`.local-inputs/native-idle-npc-body-build-capture.zip` has SHA256
`3af5c9f88618ab5124a142d1ea1e0a07fc75d9c458637a1d896f619af10d8750`.
The repository source-build APK is 8f253733; Studio is a38c1506. Their build
inspection passes, but live validation is NOT_RUN. Emulator tests of 77f1e1cd
do not validate these newer APKs.

The next engine gate is one original monster running startup, animation,
AI/state events, shared-frame timing, collision and movement together, followed
by attack/death and required effects. Remaining engine services include effect
factory/particle/material animation, sound, navigation and timer integration,
broader original rendering checks, and physical ARM64 verification. Campaign,
inventory, quests, original UI, saves and all-game assets remain additional
game work. No credible completion percentage or date is established by suite
counts alone.

## Newer source-only milestone: retained Idle events and FX preload

`../../level-world/reports/character-idle-events-fx-preload-main-linked-host-audit.json`
passes **60 actual central CMake suites**, zero sanitizer findings. The new
Idle event adapter composes eleven real private monster Init sessions with
four animation banks, 2,420 separate scene/animator phase pairs, 362 animation
events, 68 genuine commons End callbacks and 215 real debug switch queries.
It borrows one FSM/animation owner and shared caller RNG; it adds no second
controller or elapsed-time increment. Controller/startup gate producers,
navigation and full Character frame execution remain outside this composition.

The effect preload module passes 1,000 original/O2 cases and 46,798 linked host
checks, including real missing-file Debug providers and repeated FX77 queueing.
It reconstructs registration, ordered leaf queue and debug-module ownership.
It does not create or play FX: particle channel28, material channel86, scene
factory, callbacks/pooling and GPU applicators remain required.

World DSO SHA256 is
`479cb68037efc15e63ce530fa9f73d5a55df8a26cd00d28dee7c63d9ce31222c`.
Both repository and Android Studio build ARM64 and x86_64 successfully. Their
452-input immutable capture has SHA256
`e5307f053f1c2f01d501b2ff3de4738ded2b7f592b061fade40b574da90d875b`.
`native-idle-events-fx-preload-source-build-inspection.json` passes actual
compiler-input/export checks, 342 prototype assets and sixteen 16KiB-aligned
ELF64 libraries. Repository APK is b43715b7; Studio APK is fcc64812. Both have
live validation **NOT_RUN**. Latest fully emulator-validated checkpoint remains
**77f1e1cd**; these new source modules are not active live monster controllers.
