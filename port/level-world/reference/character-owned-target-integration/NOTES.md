# Owned settings, live object positions and Spawn/Kill integration

This stage adds a genuine object GetPosition method to the retained Character
registry, owns the actual first DesignSettings table in the Android world, and
centrally compiles the recovered Spawn selection/permission/body and Kill
coordinators. Complete original scene AI and game completion remain unfinished.

## Object position source and lifetime

Both source object catalogs already register GetPosition at 0x38e700. The
complete 52-byte body reads raw GameObject+160/+164/+168 and pushes three
numbers; it ignores its Arguments contents. Source instructions are captured
in `../character-spatial-bindings/reference/original-functions.asm:258`.
The unchanged spatial getter has 850 original/O2 position cases in the
historical 2,746-case spatial differential corpus.

`character_script_objects.cpp` now resolves the method's captured native
identity and retains that Character record through the call. It delegates to
the existing spatial getter with the record's current point. It does not use
the session owner's position, dereference an opaque identity, rescale values,
or claim unsupported projectile/GameObject identities.

`tests/character_object_position.cpp` has 448 checks with a real original
monster Init session and cache-backed player/monster presets. It verifies the
three returns, live target coordinates, ignored extra arguments, signed-zero,
NaN/infinity values, and finalizer lifetime. A real Lua argument metamethod
mutates both position and receiver `_this`: the bridge retains the identity
captured before projection but reads the position after projection.

The renderer's explicitly labelled read-only development probe observes player
and monster object positions through each actual private VM. Its temporary Lua
function is removed after the call. It does not move actors, set targets or
replace an original AI callback. Eleven position markers per load represent
both object reads in each monster session.

## Actual owned design settings and target composition

`game-data/design_settings.hpp/.cpp` decode and retain all 43 scalar fields,
names and schema of the actual first DesignSettings table. The three original
cache streams are now bundled inside the APK. The verified `Default` row's
EnemySpottedAggro field 11/runtime+0x30 has word 0x41200000 (float 10). No default
threat is synthesized. Difficulty/debug suffix tables and complete Application
registration remain separate work.

WorldScriptContext owns and borrows the settings snapshot. Its threat pointer
stays valid through VM/world lifetime and GL recreation. New logs bind the
43 fields, consumed prefixes, authored word and retained backing address.

`tests/character_owned_target_pipeline.cpp` has 463 checks. It destroys the
settings owner/input bytes while retaining the borrow, then executes actual
original monster EnemySpotted, native targeting and the controller/path prefix
against a cache-backed KnightPlayerBase record. The target's GetPosition sees
subsequent changes and remains callable in a Lua close finalizer. Required FSM,
FindPath failure and downstream controller effects remain the historical
pipeline's explicit service fixtures; this is not a complete live AI proof.

## Central native proof

`../../reports/character-owned-target-spawn-kill-main-linked-host-audit.json`
binds 42 actual main CMake suites, compiler records, source/corpus inputs and
pre/post DSO/executable hashes. Address/undefined/leak sanitizer findings are 0.
World/runtime/data hashes respectively:

- 73a0cf6bcfdec32c8bddb8723744c668311845119b2f789f5ef4f79ba114af4e
- 3f1886b22c2230e6ddda5c75fae248d0a710b75034eac3144489f9992b7f9945
- 13c9499376f16fdddf0639987a7cc735b12d57a14c68b1f1ba23694a5ab35569

The central suites now include 3,456 Spawn selections plus 512 permissions,
5,458 Spawn body cases/60,104 requests, 2,200 Kill records plus 22 synchronous
reentry cases/484,688 checks, and 385 settings records/97 tables/33,484 word
comparisons. Frozen original/O2 reports and their source/corpus hashes are
bound by the runner. Scene spawning providers, full loot/XP/quest/death-event
backends, and original live AI are explicitly incomplete.

## Android checkpoint

`../../../android-native/build/checkpoints/dh2-native-owned-target-9112998a.apk`
is 25,371,034 bytes, SHA256
`9112998a6ea67fdf366795bb3204d6bf7e1279bbc4be107bf27a57b24fbb18d9`.
Both repository and linked Android Studio builds passed for ARM64/x86_64.
The APK contains 266 prototype assets and 16 ELF64 libraries with 16 KiB alignment;
the original ARM32 engine is absent.

Immutable build capture `.local-inputs/native-owned-target-build-capture.zip`
contains 421 compiler-recorded source/build inputs, SHA256
`3e0d1acf259a510bff7ac7a146d80205f00e45ea40c5e116796f8930c95e6cd1`.
`../../../android-native/reports/native-owned-target-9112998a-build-inspection.json` matches those inputs against
the audited sources and checks the new exports in both packaged ABIs.

`../../../android-native/reports/native-owned-target-9112998a-checkpoint-validation.json` binds five passing
emulator suites to this same installed APK: Init/object positions/settings,
movement, lifecycle, Prince animation bank, and stationary/moving combat.
The settings backing and all player/monster records survive rotation.
Portrait/landscape screenshots were visually reviewed. The only current ADB
device is API 37 x86_64 emulator-5554; physical ARM64 testing is unverified.

Full campaign, inventory/skills/loot, quests, original UI/audio/saves, all-game
asset packaging and full rendering fidelity remain. The prototype enemy
controller still runs; adding compiled kernels is not their full live wiring.
Before replacing it, preserve original authored/inherited ai_state and spawn
properties: the current DACT actor conversion drops part of that information.
Do not invent state 17 or a shared spawn policy for all eleven monsters.
