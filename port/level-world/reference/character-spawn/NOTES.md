# Character Limbus and Spawn source slice

This host implementation covers the source-owned Limbus/Spawn state boundary
used by the two directly authored surprise Crypt ghosts. It does not emulate
the game AI, discover when the Crypt trigger fires, or implement revival and
group policy beyond explicit caller facts.

## Recovered native path

The pinned original is `libDungeonHunter2.so` (source manifests under
`recovered/native/assembly/libDungeonHunter2.so`). Addresses below are ELF
addresses:

| Function | ELF address | Recovered source and observed behavior |
|---|---:|---|
| `Script_SpawnCharacter::Execute` | `0x45f400` | [`Script_SpawnCharacter-1a3476c33649-001.asm`](../../../../recovered/native/assembly/libDungeonHunter2.so/Script_SpawnCharacter-1a3476c33649-001.asm) resolves one named object, then calls `SM_SetSpawnState(false,false)`; it does not allocate a new Character. |
| `CharStateMachine::SM_SetSpawnState` | `0x3c2734` | [`CharStateMachine-9e67f9b0cab6-001.asm`](../../../../recovered/native/assembly/libDungeonHunter2.so/CharStateMachine-9e67f9b0cab6-001.asm) selects state ID `1` for the `false,false` request used above. |
| `CharStateMachine::_SetState` | `0x3c1938` | Calls old state blur, installs the target state, clears elapsed state time on a changed ID, calls new focus, then emits event `0x1d` with the previous ID. Same-state requests still run blur/focus. |
| `CSLimbus::OnBlur` / `OnFocus` / `OnInit` | `0x3c2be4` / `0x3c2e58` / `0x3c7cc0` | [`CSLimbus-f65fc268875e-001.asm`](../../../../recovered/native/assembly/libDungeonHunter2.so/CSLimbus-f65fc268875e-001.asm) restores the Limbus actor and registers timer event `0x2f`; focus may schedule it only under the source respawn gate and clears aggro. Update and event handlers are no-ops. |
| `CSSpawn::OnBlur` / `OnFocus` / `OnEvent` / `OnInit` | `0x3c2f7c` / `0x3c35ec` / `0x3c0b04` / `0x3c7d0c` | [`CSSpawn-6f640697797f-001.asm`](../../../../recovered/native/assembly/libDungeonHunter2.so/CSSpawn-6f640697797f-001.asm) is the source for fallback body initialization, exact animation-table field `CharAnimTable+0x80`, AI target reset, sneak cancellation, fade call, and registration of completion event `0x22 -> Idle(3)`. |
| `CharStateMachine::RegisterState` | `0x3c7318` | The character constructor registers state IDs `0..19`; the source registration API and bounded factory's Spawn-registration check are linked by this function. |
| `VisualObject::StartFadeIn` / `UpdateFadeIn` | `0x470ce4` / `0x470cec` | Both are one-instruction return stubs in [`VisualObject-79dd69531261-001.asm`](../../../../recovered/native/assembly/libDungeonHunter2.so/VisualObject-79dd69531261-001.asm). The property value remains an opaque raw argument; the port does not invent an alpha ramp. |
| `GameObject::SetVisible(bool)` | `0x38b0f0` | Character vptr `+0x40` resolves to this 32 byte function. False clears current visibility; true restores ObjectBase enabled byte `+0x8a` into current visibility `+0x80`, then optionally calls VisualObject::SyncVisibility. |

`CSSpawn::OnFocus` reads the `Spawn` member at `CharAnimTable` offset `0x80`
and requests that exact sequence. This path does not add a stance to the
sequence. For the two selected ghost records the decoded sequence is `213`.
The actual packaged `Crypt_Ghost` Spawn clip (clip 714) has no
`is_interactive` event. It completes with event `0x22`; the source then blurs
Spawn and constructs the physical body as the fallback before entering Idle.
The host fixture also checks the separate source `is_interactive` event path,
but that case is not evidence that clip 714 emits it. The interactive flag
`0x2000` is copied only from the prior-state-17 branch; it controls body
construction, not visibility.

## Implemented code and validation

The logical Character/FSM projection is in [`character_state.hpp`](../../character_state.hpp)
and [`character_state.cpp`](../../character_state.cpp). Exact-name lookup and
atomic miss/duplicate handling are in [`character_factory.hpp`](../../character_factory.hpp)
and [`character_factory.cpp`](../../character_factory.cpp). The focused host
test is [`character_spawn.cpp`](../../tests/character_spawn.cpp); run it with:

```powershell
python port/level-world/tests/run_character_spawn_host.py
```

It compiles only those actual sources, then verifies ordered Limbus-to-Spawn,
both the conditional interactive event and the actual completion-time blur
fallback body creation, `0x22` completion to Idle, source no-op update,
same-state `_SetState`, synchronous completion callback, and lookup atomicity.

## Explicit unsupported behavior

- Trigger activation and the `GhostAmbushHallway` trigger/command table.
- Autonomous AI, target selection, combat decisions, and AI event production.
- Respawn eligibility computation, group membership/update computation,
  saved Limbus position/rotation producers, and complete revive/physical body
  ownership. These are adapter services/facts, not guessed in this kernel.
- PreSpawn focus and arbitrary paths entering Spawn from states outside this
  bounded actor fixture.
- Timed visual fading: original `StartFadeIn` and `UpdateFadeIn` are stubs.
- General enable-condition/serialized visibility ownership and all visual
  synchronization branches. The current Native adapter implements the
  selected fresh Ghosts' enabled-byte visibility and retains it across reload.

## Authored Faery factory gate (2026-10-10)

`Level::_LoadProcess` calls `Level::PlaceFaeryAndFollowers(this, nullptr)` at
load case `0x25`; the retained call site is in
[`spawn_callers.txt`](../../../../outputs/item-manager-source-trace/spawn_callers.txt).
The source constructor order is in
[`character_constructor_owner_v1.hpp`](../../character_constructor_owner_v1.hpp).
No complete Android component-provider bundle currently reaches that factory:

| Constructor component | Existing Android owner found | Why it cannot fill a Faery factory slot |
|---|---|---|
| GameObject | `NativeCryptRoomRuntime::manager_projection` | It is a separate projection; factory `Record` must own the registered Character/GameObject identity. |
| Controllable | Character command/input adapters | They dispatch commands; no per-Character `v2Controllable` owner or attached source controller exists. |
| Inventory | `FreshInventoryOwnedV4` | Its only Android runtime instance is Prince's Character owner; it requires borrowed loot tables/RNG and is not the source `ItemInventory` constructor object. |
| Timers / state machine | `Coordinator` | The factory now owns one Coordinator and publishes it to both ordered slots; the current authored Faery still owns a separate Coordinator and is not factory-connected. |
| AI | `NativeCharAIProjection` | A Faery CharAI projection exists, but it is manually associated and not published in a factory-created Character. |
| Animator | Character animation scheduler and packaged Faery clips | These load renderer playback assets; no Faery `CharAnimator` constructor owner is bound. |
| Properties | `AuthoredCryptFaeryOwner::properties` | The source property prefix exists but is not attached through the factory's canonical Properties slot. |
| Controller | `ObjectActor::direct_controller` | This is the port navigation path controller, not the source Character Controller component. |

The existing `character_runtime_factory_v1::Record` already owns constructor,
InitPost/InitFinal barriers, Faery script session, POFaerie body and the
Character+0x420 link. The next exact production edge is therefore the first
missing source component provider, `Component::controllable` at constructor
callsite `0x3a9368`; after that, the same owner must supply the ordered
Inventory-through-Controller components before factory creation can publish a
Faery. Do not feed the authored MGP transform to placement: the source placement
kernel computes position from the chosen Player's live look vector and target
position. Until these owners exist on the same `Record`, the retained Faery
roster prefix is not a live Faery Character and placement must stay gated.


### Constructor provider trace (2026-10-10)

At `Character::Character` `0x3a9340`, callsite `0x3a9368` invokes the real
44-byte `v2Controllable::v2Controllable()` (`0x404db8`): it installs the
source vptr and clears its controller pointer. `0x3a9378` invokes
`ItemInventory::ItemInventory()` (`0x3ff330`), which constructs the inventory's
bucket vectors; this is not `FreshInventoryOwnedV4`'s Save-bound owner. Later,
`0x3a9788` allocates 16 bytes for the default controller record, writes its
vtable/controllable backpointer, and `0x3a97c4` calls
`v2Controllable::SetController()` (`0x404e10`). There is no distinct controller
constructor call in this path.

The missing provider boundaries are concrete: Controllable is the source
`v2Controllable` at `Character+0x374` (constructor `0x404db8`; its `+4`
controller pointer is `Character+0x378`); Inventory is the source
`ItemInventory` at `Character+0x37c` (constructor `0x3ff330`). The default
controller is a separately allocated 16-byte object at `0x3a9788`; its vtable
comes through a global table and it is attached by `SetController` at
`0x3a97c4`. Android input/command code is callback dispatch, `PathController`
owns nav state, and the only live `FreshInventoryOwnedV4` is Prince's
`NativePrinceCombat::inventory`, constructed with a LootTables borrow, random
service and Prince PropertyState. No Android `character_runtime_factory_v1`
component provider exists yet (the app only supplies its CharacterList roster
adapter). None supplies Faery owners/lifetimes on its factory Record.
The factory now constructs one native `Coordinator` at source `CharTimers` callsite
`0x3a9394` and publishes that same object to the later state-machine slot.
This reuses the existing source-backed 20-slot timer and FSM kernels without a
second store. The authored Faery still uses its own Coordinator outside the
factory, so it must be moved into the factory Record when that path is joined;
do not instantiate both for one Faery.

### Placement and UI selection transaction (IDA, 2026-10-10)

`NativeHUDSetActiveFaery` (`0x43ee40`) calls `Character::ChangeFaery`
(`0x3ae99c`) first, then calls `Level::PlaceFaeryAndFollowers` (`0x3f0898`)
when a Level exists. `ChangeFaery` writes the active difficulty's Save ID,
runs `CharAI::UpdateAllSkills`, then—if the player has a linked Faery at
Character+`0x420`—resolves that Faery's model name, replaces its VisualObject,
and re-adds its animation set. Placement walks the ObjectManager Character
list; each Faery/follower is positioned at Player target position plus look
vector and force-updated. Followers disable zoning. Faeries are linked into
each Player GameObject at `+0x420`; placement temporarily sets their CharAI
master, reads the master's current Save Faery ID, invokes `ChangeFaery` on the
chosen Player, and restores the previous master. These operations are
source-ordered; the Android `FaerySelectionTransaction` wrapper is a
fail-closed port gate, not evidence the original callback is atomic.

IDA resolves that model lookup: `Character::GetCharModelName` (`0x3a54d4`)
first rejects source `ModelId == -1`; for a Faery it reads its CharAI master at
Character+`0x418`, asks that master's `SG_GetCurrentFaerieId(-1)`, calls
`Character::GetCharFaery` using the Faery's own property list, then maps
FaeryTable `ModelFile` through ModelDict. Placement sets this same CharAI
master before the UI-triggered `ChangeFaery`, so the model choice is driven by
the canonical master's Save and Faery's source `FaeryList`, not a renderer
override. The portable `character_faery_selection::select` kernel already
implements the table selection/fallback through the exact ModelFile row. The
remaining Android provider must resolve its inputs from one factory Faery's
Properties, its same-record AI master and that master's current Save, then
pass the model-dictionary result into the VisualObject/animation lifecycle.

The canonical Save selection and skill update already exist through
`PlayerSavegameV1`/`player_faery_change_v1` and the retained Player skills VM.
`character_faery_links_v1::Owner` is the one factory-Record owner for links,
and `character_faery_level_registry_v1::Owner` validates the same Character,
visual, physical and AISFaery graph before source placement. Neither is wired
to the current authored Crypt Faery prefix. `faery-asset-provenance.json`
confirms the five models and Celest animation clips are staged, but explicitly
marks runtime binding open; model assets are not a live VisualObject owner.
The general `game_object_visual_attachment_v1` kernel requires a renderer-owned
VisualObject candidate and root SceneNode. Current Crypt `ObjectActor` stores
its Character identity as an opaque `visual_object_2d8` token; the Faery prefix
has no VisualObject, SceneNode or animator owner to pass to that kernel.
Keep the UI selection gated until one factory Record owns the Faery
constructor/components, renderer/animation lifecycle, and Level placement
services. Do not update Save alone or invent transforms to bridge the gap.

The exact production edge is `Character::InitPost` (`0x3b4d60`) ->
`GameObject::InitPost` (`0x38be5c`) -> `GameObject::LoadVisualObject`
(`0x394eb0`) -> forced `GameObject::SetVisualObject` (`0x394d34`). That call
allocates a source `VisualObject`, whose constructor loads a scene root through
`AssetManager::loadSceneNode`; it attaches the resulting root back to the same
GameObject. `Character::ChangeFaery` repeats that replacement and then rebuilds
the source animation-set controller on the new visual. Android's BDAE resource
renderer can parse model/clip BDAEs into a portable scene graph, but its current
`ObjectGroup` owns the mutable `Resource`/GPU batches and its `ObjectActor`
separately owns transform/scheduler state; neither lives in the Faery factory
Record. The BDAE graph is not an Irrlicht `SceneNode`, and current group loading
is driven by DACT actor records with ordinary `Idle/Walk/Attack/Died` states.
The source-shaped contract for a safe port visual is therefore: one factory
Record owns a stable per-Character visual facade and animation state; it borrows
an immutable cached model/clip resource whose lifetime is retained across room
reload; changing Faery prepares the new resource/animation binding, swaps only
after validation, and retires the old binding with that Record. Rendering must
consume the Record's live transform/visibility and the selected Faery clip
state. This is a possible renderer adaptation, not an implemented provider.
A model-name callback alone would only fill the source string before the
mandatory load edge, so it cannot safely complete InitPost or enable Faery
placement.

## Subsequent source correction

The earlier vtable interpretation used a full-table offset without its `+8`
address point. `IsUpdatable` is at vptr offset `+0x38`; these state calls use
`SetVisible` at `+0x40`. Limbus focus hides; Limbus/PreSpawn blur restore enabled
visibility. Limbus blur resets its controller lock byte, not an auxiliary
Limbus object. Its role3 group branch queries every other member and selects
status2 when **any** other member is in Limbus; empty/self-only selects0.
The separate GroupInfo respawn predicate checks **all** members. The logical
SpawnFacts names now preserve these distinct meanings without a layout change.

The bounded SpawnFacts timer fields remain pre-resolved observations. Full
Limbus Focus performs two live delay reads, with mode/hosting gates between
them; the separate `character_limbus_respawn` producer preserves that behavior.
The published f811 artifact predates this source correction.
