# Canonical Character/Player candidate V60 — source-only handoff

Production source is written, but has not been compiled or executed. The RAM
hold prohibits compiler, WSL, test and emulator launches. Existing historical
V49/V50 runtime receipts do not validate this successor.

## Construction and ownership

`canonical_character_candidate_v60.cpp` accepts the original `Character` and
`Player` catalog aliases only with factory address `0x340800`. Both use the same
Character C1. The original C1 at `0x3aa1b4` calls ItemInventory C2 `0x3ff330` on
Character+`0x37c` at `0x3aa1e8..0x3aa1ec`. The candidate owns one inventory and
moves that exact owner to Gear; it does not clone an existing Crypt actor or
construct another inventory when adoption is requested.

The candidate retains the same sheets, life, ScriptCharacterObject, native FSM,
controller, position fields, visual scene and handle. Actual IsPlayer uses AI
type when nonzero, otherwise the source `PlayerCharacter` name prefix through
`loot_character_is_player_v47`; the catalog name alone is not a player test.

The source InitializePlayerSavegame endpoint constructs one default Save, stores
it into the sole Character14e8 slot, then calls SetCharacter. This follows
`0x3b36b0..0x3b36e0`. Replay, replacement and an already-bound Save fail. The new
`character_candidate_save_v60.cpp` regression records those cases but is unrun.

## Callable root transport

Include `renderer_character_candidate_v60.inc` after the World definition and
V59 profile include. Retain its owner outside its own factory callback. Call
`connect` after actual source candidate C1 and before the first loading tick.
It uses the status agent's once-only `bind_source_campaign_class_factory_v60`.

Original PlayerManager spawn must return this same canonical receiver. Its
initialize_save endpoint calls `initialize_player_save(id)`. The actual PM
record664 and its source setter/receipt, immutable selected profile, actual
Application file storage and jobs owner feed `profile_before_gear`. The latter
uses the same Save/SaveLoad and source14e8 backing. No PM660/664 store is inferred
from a world handle or class default. `move_c1_inventory_to_gear` installs the
V59 profile hook and moves the existing inventory exactly once.

For destruction, release actual body/PF, remove manager/world discoverability,
and call `removed_after_unpublication`. Transferred Gear/Skill teardown requires
the supplied real release endpoint before visual destruction. The raw
`inventory37c` observation must not outlive the moved Gear owner.

## Generic player continuation (source implementation, awaiting integration validation)

`character_generic_initpost_owner_v61.cpp` now owns generic Character/Player
control flow and is called by the canonical receiver. Actual ELF symbols correct
the earlier notes: `_InitEquipment` is `0x3b395c`, invoked at `0x3b54e0` after
SG_Load(4) and positive PM lookup. `0x3a41a0` is AddMultiplayerHighlight;
`0x3bb950` is SG_SetGameDifficulty. Thus preparation/restoration is followed by
conditional grants, gear reset, quest sync, LoadBase/LoadGears/recalc, skill slots,
gold/capacity stores, marker construction and highlight. Gear exposes
`prepare_restore_v60` and `finish_initial_grants_v60`; legacy initialize composes
both. Preparation never publishes readiness; grants are once-only and
unavailable during restore callbacks. The transport has
`prepare_equipment_at_load4` and `finish_equipment_at_init_equipment`, checking
the same Gear/inventory/sheets. Pure gear reset/load and raw gold/capacity stores
now use that same owner. Remaining source selectors and services still require
actual PM, Level difficulty, skills, quest-sync, marker/highlight and body/script
endpoints. The actual-cache
profile-window test now records premature/reentrant finish, phase replay,
readiness and failure-prefix assertions; these remain uncompiled and unexecuted.

Other required actual producers are App FileManager/SavegameJobs (currently not
published by candidate App V5), real PlayerInfo664 selection/setter, player model
selection, body/PF/script/remaining InitPost endpoints and ordered teardown.
NPC InitPost composes the existing NPC owner and explicit remaining endpoints;
this packet is not full Swamp Stage14 acceptance.

## Link closure and shared changes

New production TUs: `port/level-world/canonical_character_candidate_v60.cpp`
and `port/level-world/character_generic_initpost_owner_v61.cpp`.
It depends on existing V6 visual, V7 position, V47 source fields/IsPlayer, V59
bootstrap, source properties/AI/model/animation/loot owners. The V59 five-TU
closure is listed in `character-profile-bootstrap-v59-source-handoff.md`.

Narrow existing changes: read-only FreshInventory `random_v60`; Gear input
`constructed_inventory_v60` and move-adoption in initialize; V47 exact fresh
Save association method. Gear input layout changes require coherent rebuilding
all consumers. No root CMake, model_renderer or native_app change is made here.
