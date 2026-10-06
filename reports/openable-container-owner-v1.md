# OpenableContainer source receiver handoff

The new `port/level-world/openable_container_owner_v1` implements class-specific
constructor fields, original record decoding, InitPost/InitFinal ordering,
Spawn, key unlocking, inherited Interact, DoOpen, timeline completion, and
authored event delivery. It does not yet implement the outer Interact's level
host asynchronous event and achievement/property218 tail, nor canonical base
GameObject construction. `interact_base` is explicitly the inherited receiver;
caller must first deliver the actual outer unlocking/host event sequence.

Original source capture is in `reference/openable-container-v1/`:
Openable C1 3a1cb0, Container C2 3a0788, InitPost 39f910/3a1b64,
InitFinal39fc98, Interact3a0b38/3a1904, DoOpen3a0a98,
event3a0c64 and completion39f44c. Factory340da4 passes GO_ID7;
factory catalog position25 is not that GO_ID.

Actual cache resources `game_objects_pyarray.bin`, names, schema and dictionary
were extracted without modification from the original cache zip. The unique
OpenableContainers group spans array bytes3031..5006 inclusive: count68,
68 source records, next group count1. Names group5 has exactly68 names.
Struct schema group6 gives Effect/InteractSound/KeepSolid/Loot/Script/
TrapChance/TrapType/Visual. Original record reader4fdb90 establishes wire
order int,int,bool,int,length+string,int,int,int; no guessed fields.
`actual-openable-rows.json` captures all actual values. Runtime table loader
accepts isolated original groups with their count prefixes and requires exact
consumption; a generic whole-table registration owner must supply that slice.
No production fixed byte offset has been added.

Swamp_Normal_Chest: Effect−1, InteractSound33, KeepSolid1, Loot227,
Script empty, TrapChance0, TrapType−1, Visual47. Other exact Swamp values are
in the JSON. The five selected SWAMP declaration/template bindings still
require the loader's actual registered-property/defaults producer.
Actual dictionary visual47 is `data/3D/GameObjects/go_chest_swamp.bdae`,
48 is `go_chest_swamp_big.bdae`, and49 is `go_chest_swamp_rotten.bdae`.

Bindings:

- Factory retains `OpenableContainerFieldsV1` beside the ONE canonical
  GameObject base. Registry handle/room/name/archetype/across-room fields must
  borrow that base; this module does not invent an independent handle.
- InitPost source spawn roll must come from the same original World/RNG.
  Visual47 is the actual dictionary row, not a manually chosen mesh.
  GameObject InitPost, MeetCondition, callback registration, mesh-box and
  source script loading are reached required providers.
- Timeline plays prespawn, spawn, idle in source fallback order. State0
  waits for Spawn; state1 completion selects idle/state2. Opening selects
  activate/state3; authored `opened` calls DoOpen; completion selects
  idleactive/state4. No synthetic opened event or loot-on-end has been added.
- SetState manipulates actual scene+11c bit400; completion outside state1/3
  clears bit200 if the timeline is inactive. Missing scene services fail.
- virtual+2c is GameObject::Update38cbe8, not an invented interaction callback.
- InitFinal calls actual base, updates only outside states3/4, evaluates the
  actual condition and constructs/attaches PODecor via PhysicalWorld.
- Key unlock uses SAME actor ItemInventory FindItem, signed short quantity,
  and original RemoveItemByID(key). key_consume=false returns false even when
  quantity is sufficient. State3/4 are source-unlocked.
- DoOpen invokes ItemObject::DropLootTable(table, SAME chest, SAME opener,
  fixedPowerCount, false), then optional source Lua OnOpen(opener). Actual
  world ItemObject factory/placement and original shared LootRNG remain
  mandatory. Gear add_item would skip this drop lifecycle and is not used.
- Opening plays actual Vox Play3D sound after animation/DoOpen. There is no
  successful silent fixture in the production owner.

Validation: aarch64 Android24 strict C++17 Wall/Wextra/Werror syntax PASS for
production+focused test. Focused test covers state/event ordering, reached
missing-drop failure, no duplicate interaction and original key consumption
predicate with explicitly declared service fixtures. Host execution has NOT
passed: WSL immediately reports Wsl/Service/E_UNEXPECTED. No build/install or
live chest claim. Existing shared/root sources unchanged.

Original ARM predicate oracle PASS81 cases over extreme signed states/key IDs
and disabled-byte values. Receipt `reference/openable-container-v1/predicate-oracle.json`
records exact ELF hash and methods. This verifies arithmetic/branch semantics;
it does not replace the pending compiled whole-owner host execution.
