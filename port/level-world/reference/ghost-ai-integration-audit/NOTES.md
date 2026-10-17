# Crypt Ghost source AI integration audit

This is a read-only source map for the two direct `Crypt_Ghost` actors in the
Crypt ambush. It answers which original AI the cache selects, how the first
enemy callback is reached, which native Lua values its methods accept, and
what is still missing from the Android actor path. It does not claim a rebuilt
or running autonomous Ghost AI.

## The selected AI is the external `monster` script

The direct Crypt records select CharacterProperties rows 35 and 37
(`Crypt_Ghost` and `Crypt_Ghost_RE`). Both rows resolve to AI row 68,
`Ugly_Dog`, with `Type=4`, `Script="monster"`, `MeleeRadius=120`,
`ViewRadius=1500`, and `ViewRadiusNoAggro=1500`. Both use animation table 24
and model 44. The `ai-provenance.json` input set and actor provenance are pinned
to cache SHA-256
`3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679`.
The cache's `recovered/scripts/original/data/scripts/ai/monster.luac` is
readable Lua source text despite its `.luac` name; it is 7,393 bytes with
SHA-256 `84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d`.

`CharAI::SetScriptByName` (`0x3ceeb0`) routes names with the `__...__`
convention to built-in AIS classes. Exact `__monster__` selects `AISMonster`;
plain `monster` selects `AISExternal` and retains the script filename. The
normal `StepCreateScript` (`0x3cf04c`) sets the scripted flag after selection.
Therefore the built-in `AISMonster` C++ callbacks are not the original AI
class for these Ghost records. The script lifecycle loads `_commons` and then
`monster`, runs the bind/set-character stages, initializes the script, and
publishes the pending AIS as active. Existing lifecycle kernels reconstruct
source wrappers, but their allocation, Lua and ownership services are still
external.

## Enemy spotting and the source script callback

On the original update path, `Character::Update` enters `CharAI::Update`.
After source pause/controller/owner gates, `CharAI::Update` calls
`_UpdateTarget`, `_UpdateMaster`, `_UpdateAggro`, and then the active AIS update.
`_UpdateAggro` (`0x3cf3f0`) builds candidates through the original
`ObjectSearcher::TargetList`/`Search` path and raises source Character event
9 for an enemy candidate. Event 9 crosses `Character::RaiseEvent`
(`0x3a4d5c`) and `CharAI::RaiseAIEvent` (`0x3cbb34`).
When the aggro scan finds no enemy, its separate normal-convergence path tests
the pointer at `CharAI+0x40`; if nonzero, it raises event `0xc` with that
pointer as the payload. This is not event 12 with a null payload.

`CharAI::OnEnemySpotted(Character*)` (`0x3d14b4`) applies its own owner,
group and FSM gates. When it reaches the active AIS virtual at address-point
offset `+0x34`, the `AISExternal` vtable routes to
`AISExternal::OnEnemySpotted(Character*)` (`0x3dd2f4`). That wrapper converts
the target into the source script Value and invokes Lua method
`OnEnemySpotted`. The loaded script's `AddToVFTable` registers this key to
`monster_OnEnemySpotted(enemy)`, whose body is:

1. Only when `HasTarget()` is false, call both `SetTarget(enemy)` and
   `HeadTo(enemy)`. An already-targeting Ghost does not call `HeadTo` in this
   handler.

The source `OnTargetOutOfRange()` path is separate: CharAI slot `+0x50`
reaches `AISExternal::OnTargetOutOfRange()` (`0x3dcde8`), which invokes the Lua
handler only when that handler was registered. The script checks
`GetState()==AIStates.Idle && !HasPath()` before calling `MoveTo(GetTarget())`.

## Exact Lua value and method contract

The registration caller trace confirms the native bindings are exposed in
`Character::createBindings` / `GameObject::createBindings`, under both the
global-function and object-method registrations, with callback context
`this`. It records these source targets:

| Lua name | Original wrapper | Source result/argument contract used by `monster` |
|---|---|---|
| `HasTarget` | `Character::_HasTarget`, `0x3b6f50` | Boolean from `Character+0x408 != 0` |
| `GetTarget` | `Character::_GetTarget`, `0x3b6c7c` | Pushes target as `UserData` |
| `SetTarget` | `Character::_SetTarget`, `0x3b8f38` | One GameObject argument; accepts source Value type 2 or 7; calls `CharAI::AI_SetTarget(target,false)` |
| `HeadTo` | `Character::_HeadTo`, `0x3ba71c` | One GameObject argument (type 2 or 7) calls controller `Cmd_MoveTo(GameObject*)`; three numbers select the coordinate overload |
| `MoveTo` | `Character::_MoveTo`, `0x3bada8` | Same one-object `Cmd_MoveTo(GameObject*)` overload; three numbers select the coordinate overload |
| `GetState` | `Character::_GetState`, `0x3b6d78` | Source FSM state returned as a Lua number |
| `HasPath` | `GameObject::_HasPath`, `0x38e98c` | Boolean path-list query |

The AI callback is registered with one target object argument; the out-of-range
callback has no explicit arguments. The original AISExternal callback creates a
source script Value from the Character pointer before calling Lua. On the
port side, `dh2_script_vm_bind_source_values` converts a Lua object table's
`_this` field to source Value type 7. Do not convert target identity to a
number. Passing a light-userdata/type-2 identity is accepted by the two
object-taking wrappers above, but it is a narrower value than the original
object table and must not be advertised as full wrapper compatibility.

The separate registration manifests under `port/script-runtime/reference/game-bindings/registration` and `character-registration` identify original binding callers. Registration evidence alone does not execute callbacks or prove a live Lua invocation. The readable script and the bounded external-session host tests establish the two callback bodies and object-identity transport.

## Current Android actor status and ownership

`model_renderer.cpp` stores each loaded source actor in `ObjectGroup::instances`
and gives a gated Ghost a shared `SpawnOwner`. `SpawnOwner::bind` connects the
source FSM Coordinator, owner facts and native service callback. The owner
keeps a raw `ObjectActor*`; world teardown nulls it, and world initialization
rebinds it. It is valid only while the owning actor slot stays at a stable
address. Any future resize/reallocation of `ObjectGroup::instances` must first
detach or rebind that owner pointer. Activity/scene restoration copies actor
snapshots, so the copied owner must not be serviced until the new actor is
rebound.

The gated actor currently has source Spawn/Idle animation and body services,
but its `idle_common_update` service is a diagnostic counter/no-op. The draw
loop advances the gated Coordinator's timers/state, then calls `update_enemy`;
`update_enemy` returns immediately for every `gated_spawn` actor. There is no
actor-owned AISExternal, no per-Ghost Lua state, no `_commons`/`monster`
execution, and no original `CharAI::Update` -> `_UpdateAggro` -> event-9
dispatch wired to the direct Ghost pair. The controller/path/physics plumbing
for a full source target pursuit is also not bound to these Ghosts.

The un-gated `update_enemy` path is an acknowledged manual approximation: it
compares the actor to the Prince directly, records `combat_target` fields and
selects coarse events. It bypasses the original TargetList candidate ordering,
filters, CharAI state gates and script object wrappers; it is not evidence that
the gated Crypt Ghosts have their original autonomous AI.

## Smallest faithful integration slice

Bind the already reconstructed script selector/lifecycle to a per-actor
AISExternal session owned by the stable SpawnOwner. Load the actual
`_commons` and `monster` bytes and preserve script globals per AIS instance;
`monster` stores `saved_X`, `saved_Y`, `buff` and flee flags as script state.
Connect source `CharAI::Update` with its source-ordered target/master/aggro
services. `_UpdateAggro` constructs `TargetList(owner, 0x7fffffff, 2, 1)` and
searches with the selected AI view radius and a full-circle `2Ï€` cone. Its
filter-2/all-relationship query is not the existing melee caller's
flags-1/filter-0 query, so `character_target_search` must not be reused as-is.
Use the separate aggro query adapter and bind its borrowed registry/services to
live Character identities and source properties; do not invent an owner/player
enemy gate. In this caller, non-character objects fail filter 2. The
`0x7fffffff` sentinel accepts every Character relationship only after the
source `owner+0x1314 >= candidate+0x1310` comparison, and bypasses the
narrower dead/enemy/player/group tests. Search still applies its visibility,
virtual eligibility, `IsInteractive`, range and closest-order rules. `_UpdateAggro`
classifies popped results as friend/neutral/enemy and dispatches event 7/8/9 in
that candidate order. Then route event 9 through source CharAI gates into the
active external AIS and provide the actual object identity for the script's
SetTarget/HeadTo calls. A successful `HeadTo(GameObject*)` must invoke the
native controller's target-object route, not teleport or set a numeric
destination.

The first meaningful runtime acceptance check should begin with the authored
Crypt `GhostAmbush01` spawning the two exact ghosts and completing Spawn. Move
the Prince into the original TargetList query region. Verify the source event-9
candidate/order, each ghost's active AIS `OnEnemySpotted` callback, script
HasTarget/SetTarget/HeadTo calls, and actual controller path/root movement
toward the target. Then cross the source out-of-range boundary with the target
idle/no-path gates satisfied and verify `MoveTo(GetTarget())`. Run two ghosts
with distinct target/script state, and exercise scene restoration to ensure no
stale owner, VM, target or callback pointer survives. Until this succeeds, keep
full AI, attack, death and respawn claims scoped separately.

## Evidence index

The readable script and cache provenance are identified above. Pinned original
function manifests for script selection/lifecycle, enemy spotting, source
TargetList search, and the external `monster` session are maintained beside
their respective port modules under `port/level-world/reference/`. The
original hashes bind instruction ranges to the recovered ARM32 library; they
do not mean those functions are completely rebuilt or Android-integrated.

## Later source checkpoint

The six acquisition/event/external-session units now compile and export from the native Android library for ARM64/x86_64. The new Crypt APK `4998613b47f7df4312aca75ffd543349fa3ef58f01758f1203ee4dddc7fc487b` passes the original spawn regression, but its gated actors still lack the live AI/controller composition described above. Compiled code is not a live-pursuit claim. See `docs/SOURCE-AI-ROOM-CHECKPOINT-2026-10-04.md`.
