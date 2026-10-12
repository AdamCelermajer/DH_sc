# Original object initialization: isolated native candidate

The loader now contains `object_initialization_v1.hpp/.cpp`, a resumable native
port of the original `ObjectManager::InitPost` phase dispatcher. It is linked
only by this worktree's standalone loader CMake. Its service interface is an
internal review candidate, not an agreed gameplay factory or save ABI.

The first-return traces and final list memberships match the actual original
ARM routine across 16 fixtures, 182 calls and 269 observed service events.
Host and ASan/UBSan comparisons pass. Six additional checks cover explicit
service failures, latched failure without callback replay, independently
interleaved candidates, and stable repeated completion. The library also
compiles for Android arm64 and x86_64. No new APK was installed: the visible
`DH2_Loader_API37:5590` still runs the map-inspection preview, with mobs/chests
pending. This milestone does not satisfy SWAMP object-rendering acceptance.

## Original evidence and ordering

The original ELF has SHA-256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
Captured symbol-bounded instructions are in
`reference/object-loading-lifecycle/0034552c.asm` (936 bytes at `0x34552c`),
with associated `GetNewObject`, `Spawn`, `Level::_LoadFromXML`, `Add` and
manager-constructor captures. Receipts bind the original input, oracle helper,
fixture code and captured files by hash.

The reached sequence is:

1. Phase 0 resets both module-list and object-map cursors and enters phase 1.
2. Phase 1 expands one module per call. It uses the live module queue, so a
   callback-appended module is reached. The source also advances its object
   tree cursor on these calls. When the module cursor is already exhausted at
   entry, it enters phase 2, resets the object cursor, and immediately enters 3.
3. Phase 3 walks the integer-keyed registry in sorted order. It constructs a
   handle, resolves with `GetObject(false)`, and skips initialization if null.
   Otherwise it resolves with `true`, calls virtual `+0x1c` (`InitPost`), resolves
   again with `true`, and calls `TestEnableCondition(false)`. The re-resolution
   belongs to the service contract; the dispatcher must not cache a pointer
   across class initialization.
4. When phase 3 ends, phase 4 resets the object cursor and clears manager lists
   in order `+0x2c`, `+0x44`, `+0x34`.
5. Phase 4 uses the raw registry pointer, including an initially invalid handle
   fixture whose raw object remains present. A null raw pointer is skipped.
   An ASCII case-insensitive `RoomZone` type match appends to the room list
   `+0x24` and calls `RoomZone::InitObjectList`. Other objects call virtual
   `+0x38` (`IsUpdatable`), and true results append to `+0x2c`.
   After those callbacks it reads membership fields; `+0x44` receives an object
   when `(byte_ac == 0 && word_a8 != 0) || (byte_d0 == 0 && word_cc != 0)`.
   Their gameplay semantics remain unresolved. `+0x34` remains empty here.
6. Exhausting phase 4 enters phase 5 and returns true.

The port preserves integer-keyed order rather than treating source-declaration
indices as IDs. Fixture keys are synthetic and are never assigned to cache
objects. The oracle now includes negative/unsorted keys, balanced tree topology,
live module appends, module-time registry insertion before/after existing keys,
all 32 updatable/membership-flag combinations, pre-existing lists and fields
changed by InitPost/RoomZone callbacks. There is no class implementation or
condition evaluator hidden in the dispatcher.

## Adapter choices and limits

Original cursors are function-static and guarded; complete oracle runs begin
with the guards initialized. The portable candidate stores cursors per candidate
so separately owned levels can be interleaved. This is an adapter choice, not
evidence that the original routine supports reentrant/concurrent managers.
The map and module storage still require sequential owner access.

Object and handle tokens borrow from the service owner; they do not create or
release engine objects. Callbacks may insert registry objects and append modules,
but must preserve the source manager's valid-node/list invariants: erasing a
current node or removing/reordering the queue is not a supported contract.
The compared domain is the reached phase-0-to-first-completion sequence with
valid registry/module state. This is not a general validator for corrupted
source manager memory or post-completion original calls.

Service booleans mean that the operation was performed successfully. In
particular, `test_enable_condition` must not return the object's enabled state
as an execution-status flag. A condition that evaluates false is a legitimate
gameplay result, not a loader failure. The owner stores/effects that result;
the dispatcher preserves the original unconditional call and its force flag.

Missing/failed services latch an explicit error with the partial candidate state
intact and never replay callbacks on later polls. Stable completion is another
adapter guard; the original is only compared through its first true return.
Unwind, cleanup, factory destruction, restoration, event binding and publication
of the active level remain integration work. The dispatcher does not claim
rollback of a class callback that already changed its own state.

## Factory registry and cache inventory

`tools/inspect_original_object_factories.py` recovers all 33 original factory
entries from the actual `GetNewObject` table at `0x95c800`.
`reports/original-object-factories.json` retains each original type spelling,
factory address and symbol. Dispatch is case-sensitive. Module/Block and
Character/Player share callbacks, but their authored type names remain distinct.
Original registration is not proof of native factory support: every entry is
explicitly marked `not_integrated`.

The receipt inventories 2,171 raw cache XML-family documents once each, not
selected-level occurrences or active objects. It reuses 1,664 SHA-matched actual
original first-root parser results; nine retain nonzero original XML errors and
partial trees. The other 507 documents use standard XML solely for inventory.
It does not repair files or claim that an original caller accepts partial trees.

The inventory contains 18 registered raw declaration types plus `link`.
All 518 `link` declarations are tagged GameObject: 503 in MGX and 15 in nine MGP
documents. The MGX link behavior has separate procedural evidence. The MGP
occurrences now have executed entry-point evidence: original lookup returns
a null handle and the XML caller returns without applying properties. See
`OBJECT-ENTRY-HANDOFF.md` and `reports/object-factory-dispatch-original.json`.
This does not establish which selected levels reach those files/elements or
their file-error policy. They remain explicit retained declarations, not
supported runtime objects. The summarizer prints their exact URIs.

## Requested main-session agreement

Before any shared integration, agree the minimal retained-declaration boundary
and the owner of required service operations:

- Original case-sensitive factory lookup; template application, registered
  defaults and ordered attribute overrides; raw authored type/name/auroraID.
- Runtime-owned integer registry keys and lifetime-checked ObjectHandles.
- Actual per-class InitPost, IsGameObject, IsUpdatable, RoomZone initialization,
  and condition evaluation. Existing Character/NPC physics is part of this,
  but does not supply all 33 types, default/template services or orchestration.
- Visual/physics attachment, script/event references and restoration/release
  services, preserving SWAMP and SWAMP_02 as separate level identities.

The earlier `INTERFACE-PROPOSAL.md` still awaits acceptance. Main owns class
behavior, AI, combat/skills/animation, loot/quests, condition evaluation and
persistent state. Loader owns preparation and source-backed call ordering.
The `+0x20` slot is IsGameObject, not InitPre; LevelConfig alone has the early
InitPost call in LoadFromXML. Keep that creation-time ordering separate from
the phase-3 calls above.

## Reproduce

From the isolated worktree
`C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc`:

```bat
C:\Users\adamc\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe port\level-loader\tests\object_initialization_original.py --engine C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so --dependency-root C:\Users\adamc\.codex\worktrees\generic-level-loader\dependencies --out port\level-loader\reports\object-initialization-original.json
wsl.exe -d Ubuntu -- cmake -S /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader -B /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml
wsl.exe -d Ubuntu -- cmake --build /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml --target dh2_loader_object_initialization_probe -j 4
wsl.exe -d Ubuntu -- python3 /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/tests/object_initialization_host.py --original /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/object-initialization-original.json --probe /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/host-xml/dh2_loader_object_initialization_probe --out /mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reports/object-initialization-host.json
```

The sanitizer build uses `host-sanitizers` with
`CMAKE_CXX_FLAGS=-fsanitize=address,undefined`; substitute that build path in
the build and comparison commands, writing `object-initialization-sanitizers.json`.
The Android compile commands use SDK CMake 3.22.1 to build target
`dh2_level_loader` in the private `build/android-arm64` and `build/android-x86_64`
directories. These compile checks are not new emulator runtime evidence.
`reports/object-initialization-checkpoint.json` verifies current source/receipt
hashes and separately records the still-incomplete gameplay/full-loader gates.
