# P14 DROPS report (continuation round 3, branch p14/drops)

Scope: world item drops and gold from enemy deaths: render, target label, pickup (PC key while targeted), inventory/gold insertion, no duplicates on reload or menu return, public `drop_item_to_world`. Audio: silent (WAVs missing).

## What changed

Commits on `p14/drops` (after WIP snapshots ed0c4306, 56ec6ce3, 087cb6cc):
- `e67dac9b` (this round):
  - `main.cpp` world-item update block rewritten around a `runWorldItemPickup(id, reason)` lambda, used by the E key, `--pickup-frame` (scripted) and automatic pickup. Automatic pickup (`PickUpType == Automatic`, ItemObject::_DoAutoPickupHack) runs for the local player each frame. Log line now carries `reason=` and `store=`.
  - Potion capacity bug fixed: property 194 is q8 fixed point (x256). The WIP wiring used the raw value (3072 for capacity 12). New helper `potion_capacity_from_property_v1` in `features/loot/world_drop_rules_v1.{hpp,cpp}`, covered by an added check in `world_drop_rules_v1_tests.cpp`.
- Earlier WIP (reviewed, kept): `world_drop_rules_v1.{hpp,cpp}` (scatter, 5-slot pool, sensor target, Interact gates, `drop_item_to_world`), `features/interactions/world_drop_runtime_v1.*` (renderer owner), `source_world_item_drop_render_v1` / `_material_v1` (itemdrops.bdae packets, external effect loader), `runtime_world_item_adapter_v1` (advance/clear/pool), main.cpp target label, status line, reload/menu-return clear, CMake entries.

Review result of the WIP: builds; the rules file, the Interact gates and the tests match the survey (B3). Only the potion capacity scale and the missing automatic pickup were wrong.

## Tests run (real output)

- `p14_build.ps1 -Name drops -Test` build: `build exit 0`.
- ctest (`build-drops`, after e67dac9b): `99% tests passed, 1 tests failed out of 103`; `world_drop_rules_v1` Passed. The one failure is `session_skill_binding`, the known worktree environment failure (ignored per brief).
- Focused unit test `world_drop_rules_v1` covers: scatter draw order and ranges (150..349 along, +-150 lateral, +-250 without killer), 5-slot pool with recycling, sensor target, gold pickup (+7 resolved value, item retired, no second loot), potion stacking and capacity (take while potions < capacity), inventory-full (100 slots, equippable only), owner window, `drop_item_to_world` atomicity, potion capacity q8 helper.

## Evidence (quiet runs, EXE `build-drops/dh-foundation.exe`, assets = Preview 13 + one package file, see below)

Logs quoted from `.local-inputs/claude-preview14/drops/b-a1..a7/*/run.log`:
- Kill at frame 148 (seed 1234): `Source death reward frame=148 victim=7118915781085668844 ... spawned=1 store=1`, then `World item target frame=148 item=1 id=ClothGloves01 qty=1 position=-6963.67,937.362,255`, `World item draws frame=148 count=1 store=1`.
- Pickup, seed 16 (`b-a4/p16`, `b-a2`): `World item pickup frame=190 item=1 id=GoldStack01 reason=scripted outcome=0 picked=1 gold=0->9 stacks=4->4 store=1`; then `World item pickup frame=200 item=2 id=Potion0 ... gold=9->9 stacks=4->5 store=0`.
- Equippable pickup, seed 14 (`b-a4/p14`): `World item pickup frame=190 item=1 id=Staff01 reason=scripted outcome=0 picked=1 gold=0->0 stacks=4->5 store=0`.
- Reload at frame 300 (`b-a6/rlp16`): `Content unloaded and reloaded at frame=300`; no `World item draws` after it; the store was empty (no duplicates).
- Inventory page (`b-a7/inv16`, equipment page at 230): gold shows 9 and `Potions: 1`.

Images (LOOKED AT):
- `b-a1/r160/diff.png`: pixel diff against the rc2 capture of the same seed (`runs/r160/cap.ppm`). Changed pixels cluster at the item's label anchor, between the player and the lizard. Identity of the small mesh is NOT confirmed at this resolution (it reads as a red-brown shape).
- `b-a1/r200/cap.crop.png`: white label "Imbued Bracers" over the target (zero-power colour).
- `b-a2/p150/cap.png`: after pickup, the status line "Imbued Bracers" at top centre; the item is gone from the ground.
- `b-a3/s16/cap.crop.png`: "9 GOLD" label over the gold pile; a red potion item beside it.
- `b-a6/rlp16/cap.png`: after reload, no ground items or labels; enemies reset.
- `b-a7/inv16/cap.png`: equipment page with gold 9 and Potions: 1.

## Package files required

Files needed at runtime beyond Preview 13's assets:
1. `data/gfx/effects/GL_Diffuse_L1_VC_iPhone.bdae` (the code requests this exact name; the catalog lookup is case-insensitive). 18792 bytes, SHA256 `10c64054906caf1683f3669becf415fe20196491ac5f182881cb69705d080bdc`. Source: `.local-inputs/interactions-source-material-cache-v1/assets/data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae` (original cache, same SHA256; copies in `candidate-data-gfx-effects/`). Preview 13 has no `data/gfx/effects/` directory.

Staged asset folder used for runs: `.local-inputs/claude-preview14/drops/assets-overlay` = the Preview 13 assets folder (1288 files, identical names and sizes, checked with find -printf) plus item 1 above. Nothing was written into the Preview packages.

Not needed (none found in the local inputs): pickup/drop WAVs (`sfx_pickup_*`, `sfx_drop_*`, `sfx_potion_drink.wav`). The stream stays silent and does not substitute.

## Needs from schema v4

- Nothing persisted yet. World items are session-only: the store is cleared when the session is bound (reload, F5/F9, new session) and on return to the main menu. No `world_items` section. If the schema stream wants persistence, the record fields are listed in `runtime_world_item_adapter_v1.hpp` (identity, item id, quantity, source position, landing position, loot table/entry, gold value, owner).

## Verifier script

`coordination/claude-preview14/DROPS-verify.ps1` (in this branch). Usage:
`powershell -NoProfile -File DROPS-verify.ps1 -Exe <abs build-drops/dh-foundation.exe> -Assets <abs assets dir> -Out <abs empty scratch dir> -Runner <abs port/windows-foundation/tools/quiet_run.ps1>`

It builds five quiet jobs from `claude-preview13/rng-check/R/run.args` (drop; gold+potion pickup; staff pickup; reload at 300; inventory page) and prints PASS/FAIL. Last run: 10 PASS, 0 FAIL.

## Open risks and what is not verified

- Pickup key: the scripted run uses `--pickup-frame`, which calls the same `uiInput.actions.interact` path as E. The E key itself was not pressed in the EXE (no scripted key option). Pickup key is E (PC adaptation, labelled).
- Automatic pickup: no `reason=automatic` line was seen in any run. Drop rows checked: ClothGloves01, Staff01, GoldStack01, Potion0. Whether any ItemTable row is `PickUpType == Automatic` is unverified.
- Rarity colour: every world item uses the zero-power row (white). The original shows magic items in green ("Imbued Armor" in the reference video). World records do not carry the power count yet, so magic drops are white (wrong colour). Open.
- Drop motion: the adapter scatters the landing point and advances items toward it (`advance`), but the per-frame travel was not checked visually. The original arc is not reproduced.
- Ground glow: not implemented. The original `ItemObject::ShowGlow` (0x3ebca4) is a stub in the IDA export, so no glow is drawn.
- Gold pile model: drawn via GoldStack01 (packets resolved), but the pile was not isolated in a clean capture. The "9 GOLD" label is verified.
- Mesh identity: the drawn mesh for ClothGloves01 is a small red-brown shape; the Preview 13 itemdrops.bdae visual mapping was not checked against the reference video.
- AutoTransmute: option not bound (0), so the branch is disabled. The auto-transmute branch returns `auto_transmute_unavailable` and the item stays. Unit tests only.
- Inventory-full (100 slots) and potion capacity from the EXE: unit tests only. The potion capacity scale bug was found by reading the q8 rule, and is fixed with a test.
- Menu return clears the store in code; not exercised in the EXE.
- Quests hook (`_OnItemCollected`), chest/urn drops (`session_container_modern_drop_v1`): not done.
- Audio: silent (no playback calls added). WAVs missing.
- Persistence across checkpoints: unknown in the original (no ItemObject save function found in the IDA export). The port clears ground items on reload.
