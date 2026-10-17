# Crypt GhostAmbush01 script execution mapping

This slice reconstructs the selected original Crypt Wait/Spawn program and its
bounded native session composition. `original-functions.json` hashes seven
complete original ELF ranges. A complete evidence range does **not** mean that
the complete original function body or its external services are rebuilt.

## Original data and source boundaries

The owned native PyData decoder consumes all 15 common scripts and all 25
`007_crypt_01` scripts. The manifest pins the four original table hashes and
the original `crypt_straight_c_ns_01.mgp` hash. `GhostAmbush01` is local index 2,
global script ID 17, and its original program is:

| Command | Original byte offset | Argument |
|---|---:|---|
| Wait | 138 | 250 ms |
| SpawnCharacter | 146 | `_prim_Monster_SURPRISE_01` |
| Wait | 179 | 75 ms |
| SpawnCharacter | 187 | `_prim_Monster_SURPRISE_02` |

The MGP defines the direct pair as `Character`, `ai_state=Limbus`,
`auto_spawn=0`, `charpropsname=Crypt_Ghost`, and its ordinary TriggerZone as
`script=GhostAmbush01`, count 1, delay 0. These records differ from
`GhostAmbushHallway` and its four weighted-template ambushers in
`crypt_straight_ns_01.mgp`. `decoded-original-scripts.json` records both paths
without conflating them. Trigger geometry/contact evidence is maintained
separately in `../crypt-spawn-trigger`.

## Wait dispatch happens on manager passes

ARM `ScriptManager::ExecuteScript` at ELF `0x45c16c` calls IsBlocking through
virtual slot +0xc at `0x45c328`. If the result is true, it calls Update through
slot +4 at `0x45c34c` and returns at `0x45c350`. It does not check the changed
elapsed value again in that pass. The original `_ZTV11Script_Wait` independently
maps these slots; Execute is slot +8.

`Script_Wait::Execute` resets elapsed to zero and reads the decoded duration.
A positive new Wait is then checked and updated with that same pass's
Application dt. It blocks that task for the rest of the pass even if the dt
exceeds its target. An existing Wait checks its prior elapsed value; an update
that reaches the target remains blocked until the following manager pass.
The next Wait resets elapsed, discarding previous-wait overshoot.

Therefore authored arguments are not promises of exact wall-clock dispatch:

| Actual fixture schedule | First Spawn dispatch | Second Spawn dispatch |
|---|---:|---:|
| Initial advance0; 249+1 ms, separate advance0; 74+1 ms, separate advance0 | 250 ms | 325 ms |
| Initial advance0 followed only by ordinary 25 ms manager frames | 275 ms | 350 ms |

The host tests verify both schedules. The SWAMP regression also checks
target-reaching versus subsequent passes, fixed 25 ms frames, initial
target-exceeding dt and no overshoot carry. Its ordinary-frame dispatches are
525/2025/4025 ms after initial advance0; its zero-delta boundary fixture retains
clock values 500/2000/4000 ms by using additional manager passes.

The original elapsed arithmetic uses signed int addition. The exposed bounded
scheduler saturates uint32 elapsed addition at extreme dt instead of emulating
signed overflow. Ordinary game-frame dt is the reconstructed path.

## Level phase ordering

The original Level::Update ELF range begins at `0x3f82d8`. On the normal path,
`0x3f8994` calls ScriptManager::ExecuteAllScripts (`0x45c37c`), then branches
back to `0x3f8474`. Later `0x3f84c8` calls PhysicalWorld::update (`0x34bd08`),
and `0x3f84e8` calls ObjectManager::Update (`0x34a620`). Script execution is
gated by the local-player scripted flag read at `0x3f8468`.

The bounded Native Crypt composition advances the session before its physics
and actor update. Its trigger producer consumes the updated player absolute
GameObject AABB after the actor update. A contact-started task therefore runs
on the following level frame. Full ObjectManager enumeration order and all
player/scripted/online policy producers remain outside this integration;
GhostAmbush01 itself never locks or marks the player.

The Ghidra pseudocode export in this repository is rebased by `+0x10000`.
Manifest addresses and annotated assembly use original ELF addresses.

## Synchronous Spawn and session ownership

Original Script_SpawnCharacter::Execute (`0x45f400`) looks up an existing
ObjectHandle, converts it to Character, and synchronously requests
`SM_SetSpawnState(false,false)`. It does not allocate an actor. The current
factory performs a unique exact-name request on the two loaded actors.
Original ObjectManager lookup also applies a caller module-ID filter; repeated
modules require that producer before this adapter can be generalized.

`SpawnServices` invokes the bound native service before diagnostic shadow
state mutation and command-PC advancement. A zero callback result records a
lookup miss; a negative result stops the session. Unsupported commands in the
selected program are rejected before activation, so the generic diagnostic
scheduler's no-op handling cannot silently advance this live flow.

SpawnSession owns decoded common/level storage and the runtime that borrows
it. Candidate load is atomic; a stable heap-owned implementation retains task
pointers and callback context. Callback reentry into advance/load/activation
is rejected, clear retains borrowed storage while dispatch is active, and
exceptional service dispatch makes the session terminal. Current host tests
exercise these paths and real decoded commands through CharacterFactory and
the bounded Limbus/Spawn service projection.

Activity/context restore retains the session, elapsed waits, activation count
and edge state while actor bodies and graphics resources are recreated.
Native `load_scene(..., preserve_level_session=true)` keeps the session during
the internal Prince scene rebuild. Public model preview uses false and
disposes it. World-load failure clears native body owners and session/restore
state explicitly. This lifecycle fix was inspected in source; Android
reload/resume evidence must be attached separately to a tested APK identity.

## Verification and remaining work

The manifest's source and test paths identify the exact components. Both
actual-cache host suites passed with `-Wall -Wextra -Werror`:

```powershell
python port/level-world/tests/run_crypt_spawn_script_session_host.py --cache <absolute-cache/files> --compiler <g++.exe>
python port/script-runtime/tests/run_host.py --cache <absolute-cache/files>
```

The SWAMP runner uses CXX to choose its compiler and validates nine original
modules/148 object records. Crypt tests cover decoded 15+25 tables, the two
timing schedules, synchronous factory ordering, case-sensitive misses,
callback miss/failure, unsupported/malformed atomic replacement, reentry and
owned storage disposal.

This evidence does not establish full campaign playability, all ScriptManager
commands, arbitrary skip/cutscene policy, level save serialization, complete
Character ownership, autonomous combat/AI, broad revival or the weighted
Hallway path. Native code presence and host passes do not establish rendered
Android gameplay; device tests must identify the exact tested build.
