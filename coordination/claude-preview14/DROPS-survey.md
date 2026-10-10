# DROPS survey (Preview 14, read-only phase)

Scope: ground item drops and gold, world presentation, pickup, inventory insertion, persistence, audio.
No file under port/, docs/ or tools/ was edited. Scratch: `.local-inputs/claude-preview14/drops/`
(frames, quiet run args/logs/captures under `runs/`). EXE used: `.local-inputs/windows-source-clock-v19-preview-13-rc2/dh-foundation.exe`.

## A. EXISTING

Status key: source-only (not compiled into EXE) / component-tested (unit tests) / integrated (called from main.cpp) / verified in EXE.

| Item | File | Purpose | Status |
|---|---|---|---|
| Death reward chain | `port/windows-foundation/features/loot/runtime_session_death_rewards_v1.{hpp,cpp}`, `runtime_death_rewards_v1.{hpp,cpp}` | Kill -> loot roll -> publish into world store; XP. | integrated (main.cpp ~L1999-2026; compiled in `foundation_runtime_loot`) |
| World item store | `features/loot/runtime_world_item_adapter_v1.{hpp,cpp}` | `RuntimeWorldItemAdapterV1`: one in-memory `std::map` of world items; `publish_death_drop`, `publish_source_object_drop`, `inspect`, `render_items` (data only), `pickup` (transaction into `CharacterState::inventory` or gold via `stage_source_add_gold`). | integrated for publish only (main.cpp L2016 constructs it; L2831 logs `store=`). `pickup`, `render_items`: component-tested only. |
| Pickup interaction | `features/loot/runtime_world_item_interaction_v1.{hpp,cpp}` | `RuntimeWorldItemInteractionV1::dispatch` / `dispatch_live_player`: checks local player, then calls adapter pickup. | component-tested only. No call site in main.cpp (grep: none). |
| Death drop pickup session test | `features/loot/runtime_death_drop_pickup_session_v1_tests.cpp`, `reports/runtime-death-drop-pickup-session-v1.json` | Feature-owned test PASS. Its own report says "production main/GUI integration and visual playback remain unverified". | component-tested |
| Original loot bridge | `features/loot/original_loot*.{hpp,cpp}`, `loot_creation` (game-data `loot_*_v8.cpp`) | Source table selection, quantities, powers, gold value. | component-tested |
| Source-owned loot tables | `features/loot/runtime_loot_source_owner_v1.*` | Holds `LootTablesV2` snapshot used by the store. | integrated (main.cpp L1852 binds menu loot owner) |
| World item consumer (renderer projection) | `features/interactions/session_world_item_consumer_v1.{hpp,cpp}` | Presentation rows (name, icon, source visual, model URI `data/3D/GameObjects/itemdrops.bdae`), `selected interaction` -> `dispatch_live_player`. | source-only: `features/interactions/` has **no entry in any CMakeLists.txt or cmake/**. |
| Item drop renderer | `features/interactions/source_world_item_drop_render_v1.{hpp,cpp}`, `source_world_item_drop_material_v1.*` | Loads Potion0 static subtree and GoldStack01 skinned rest pose from itemdrops.bdae; prepare/submit draw packets; no animation, no bob/spin, no object store. | source-only (not compiled, not called). |
| Container drop path | `features/interactions/session_container_modern_drop_v1.*` | Chest/urn drops through the same store (future chests depend on it). | source-only |
| Equipment/inventory owners | `character_state.hpp` (`InventoryItem{instance_id,definition_id,quantity}`, `gold`, `inventory`), `features/inventory/inventory_feature.hpp` (`Presenter::pickup`), `source_instance_resolver` | Owners for pickup targets. | integrated |
| Save | `game_save.*`, `save_store.*` | No world-item section. GameSave has no inventory/gold fields in the header grep. | none for world items |
| HUD | `features/generic_skills/pc_gameplay_hud_v1.cpp` | "5 Potion: N" text only. | integrated |

Verification in the EXE (this survey): item presentation **not drawn at all**; pickup **never called**; store **never drained** (see E/G logs). Details in section C.

Existing statements to correct: `reports/feature-loot.json` says "runtime_integration: caller must supply live ... providers" (accurate). `docs/BUGS-AND-IMPLEMENTATION.md` I003/I016 and `docs/FEATURE-ACCEPTANCE-TRACKER` A13 ("Loot drop/pickup, potions and item/equipment sounds: Not done") match this survey.

## B. ORIGINAL BEHAVIOUR

Sources: IDA export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/*.c` (functions.jsonl addresses). Reference video: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`. NOTE: the file is 640x360, 30 fps, not 720p.

### B1. Spawn, placement, scatter (IDA)
- `ItemManager::Spawn` 0x3eacd0: per-visual pool (`ItemAudioVisual` id of the item) of **5 slots, round robin**; a live slot is DeSpawned and reused. Sets position = dropper position, `SetDestination` = scatter point, calls `InitAgain`, activates. So the item is a moving GameObject that travels to its landing point (movement speed = `ItemObject+944`, read by `GetSpeed` 0x3ebc04; the value's initialisation was not found).
- `ItemObject::_GetRandomDropPos` 0x3ec668: if a killer exists, landing point = victim + normalized(killer->victim) * (150 + Random) plus lateral offset `Random - 150`; otherwise XY offset `Random - 250`. Random bounds are not resolved here (`Random::GetRandom` ranges unknown).
- `ItemObject::DropAndAwardLoot` 0x3ec8a0: for each item, spawn, then `_DoAutoPickupHack`.
- `ItemObject::_DoAutoPickupHack` 0x3ec474: if the item's `PickUpType` (ItemInstance::GetPickUpType) == constant `"Automatic"` (PyDataConstants), calls the interaction virtual (slot +152) with the killer at once. So auto pickup is data-driven, not radius-driven.
- `ItemObject::InitAgain` 0x3ec0f0: moves the ItemInstance into the object's own `ItemInventory` (this+884); reads ItemAudioVisualTable row (20-byte rows): `+4` -> drop sound (+474), `+8` -> pickup sound (+475); applies `ItemInstance::GetColor` (rarity colour) to the item visual; `VoxSoundManager::Play3D` drop sound at the item position; creates a `PhysicalObject` (flags 0x40) = trigger/sensor; `ShowGlow` (stub 0x3ebca4).
- `ItemObject::InitOnce` 0x3ece80: loads `data/3D/GameObjects/itemdrops.bdae`; the row's `Visual` string selects the node.
- `ItemObject::Update` 0x3ebee4: follows tooltip, decrements a 16-bit timer at +476 by frame dt (pickup protection window, see B3).

### B2. World model, icon, rarity, FX data
- Row table `loot_audiovisual_pyarraynames.bin` (in package `assets/data/`): names = Axe, Bag, Belt, BootCloth, BootLeather, BootPlate, ChestCloth, ChestLeather, ChestPlate, DLCItem, GloveCloth, GloveLeather, GlovePlate, **GoldStack1/2/3**, Helm, Knife, LongSword, Mace, **Potion**, Ring, Shield, Staff, Sword, TwoHandedAxe, TwoHandedMace, TwoHandedSword, Warhammer. Columns: `AudioDrop`, `AudioPickup`, `Visual` (struct names file). Loader exists: `port/game-data/loot_audiovisual_v8.*` (`LootAudioVisualRowV8`). Decoded visual values per row not extracted in this survey.
- Icons: the item's ItemTable `IconName` (already in `RuntimeWorldItemRenderV1::exact_icon_name`). Ground icon not used by original (world item is 3D).
- Rarity: `ItemInstance::GetColor` / `GetFontDef` -> `Arrays::FontPalette` colour; name text uses `#%06X`. Observed colours in video: green (magic-type "Imbued Armor"), white ("Useless Mace"). Full rarity-to-RGB table not extracted.
- Animated FX: `"loot_orb_fx"` (AnimatedEffectTable) played at the **character** position on pickup (`VisualFXManager::PlayAnimFXSet`).

### B3. Collision, tooltip, pickup trigger (IDA)
- `ItemObject::OnCollisionBegins` 0x3ec048: sensor contact by a Character does NOT pick up. If the local player's current object-of-interest is this item, it stores the player at +241*4 and calls `ShowTooltip`. (Matches the port header note.)
- `ItemObject::ShowTooltip` 0x3ebd5c: SWF tooltip `_text_itemname.text` = item name, text colour = item rarity colour, positioned at item+352 (above the item), `linkToPlayer` callback.
- `ItemObject::Interact` 0x3ed144 (0x8ec bytes, the actual pickup). Gates in order:
  1. return if `HasBeenLooted`.
  2. caller Character handle must be valid; if tooltip owner (+239*4) is set it must equal the caller.
  3. if item owner player index (+480) equals caller's player index and the window timer (+476) > 0: reject (owner protection window after a drop).
  4. Character virtual +40 gate (alive/can-interact; identity not named in export).
  5. item branch on field +104 of the item row:
     - if +104 == -1: no inventory-full check; goes to the common success path (target = Character+892 inventory).
     - else: if `NumPowers < Application option "AutoTransmute"`: auto-transmute (`Character::INV_TransmuteItem`), status `ITEMS_AUTO_TRANSMUTE`, item never enters inventory. Else if `IsInventoryFull(Character+892)`: status `GAMEPLAYMENUS_INVENTORY_FULL`, nothing taken. Else success path.
  6. potions (type 14): success path only if `GetNumPotions < Character+936` (potion capacity byte); otherwise the block is skipped and the item is not taken (stays on the ground; pseudocode-derived, see G).
  7. success: status message `<font color="#RRGGBB">name</font>` to local player; if not gold (type 13), not online, and tutorial flag set: `StartScript("cinematic_Tuto_menuInvSheet")` (this is the first-pickup inventory sheet tutorial); `TransferInventoryTo(item -> char inventory)`; `PROPS_AddInt(Character+1376, 223)` pickup counter; at >= 300 unlock trophy `picked_up_300_drops`.
  8. after success: 3D pickup sound (`AudioPickup` at item position), destroy tooltip, `HideGlow`, clear OOI, `loot_orb_fx` at character, `ItemManager::DeSpawn`, `PlayerStatManager::IncrementStat`.
- Pickup is therefore: sensor contact -> becomes target/tooltip -> an explicit Interact on that exact item (or `Automatic` PickUpType at spawn). No radius query was found in these functions. The input that calls Interact on PC was NOT resolved in this survey.

### B4. Persistence (IDA)
- No `ItemObject`/`ItemManager` save/load/serialize function appears in `functions.jsonl` (searched: save/load/serial/stream/persist/restore). Inference: original ground items are NOT saved (session only). Not verified against the level/checkpoint save code.

### B5. Reference video (`itemdrop-*.jpg`, extracted at 4 fps and 6 fps; LOOK)
Observed (640x360 source, coarse):
- ~205-207 s: tutorial caption "Keep an eye out for Epic Chests, which contain superior equipment." (frames in `.local-inputs/claude-preview14/drops/frames/sheet-205-221.jpg`).
- ~208-210.5 s: an open green-trimmed chest sits on the floor by the player; a bright white-yellow glow flare lies on the floor at the chest base (near the player's feet). No visible model at this resolution.
- ~210.5-211.5 s: green label **"Imbued Armor"** appears above the drop point (fade in/out), with a gold sparkle burst near 211.5 s.
- ~211.8-212.5 s: white label **"Useless Mace"** appears over a second drop point (`zoom-210-5-212.jpg`, `pickup-212-216.jpg`).
- ~212.5-222 s: video cuts to Stats, then Inventory and equipment sheets (the tutorial sheet); "Imbued Armor" (green) is in the inventory list, "Useless Mace" is in the right-hand weapon list; "Total Gold 0". (Inference: this is the cinematic_Tuto_menuInvSheet from B3-7.)
- NOT observed: the pickup key/click; the arc/bounce (too small at 4 fps); the gold pile model/sound; the pickup SFX.
- Gold: no gold drop in the 205-222 window; gold timing not captured. Chest sequence `reference-video/dh2-act1/chest-sequence-205-213.jpg` covers the same moment.

## C. GAPS (with evidence)

1. **Nothing is drawn for a dropped item.** `main.cpp` has no call to `render_items`, `SourceWorldItemDropRenderV1::load/prepare/submit`, or `SessionWorldItemConsumerV1` (grep). The renderer files are not in CMake. Evidence: captures `.local-inputs/claude-preview14/drops/runs/r160/cap.png` and `r200/cap.png` (rc2 EXE, seeded run where a lizard drop was published at frame 148) show no item, glow, beam or label at the death site.
2. **Pickup never runs.** No call site for `RuntimeWorldItemInteractionV1::dispatch(_live_player)` or `RuntimeWorldItemAdapterV1::pickup` in main.cpp/CMake. The live store is never drained: `store=` only grows within a run (`runs/r160/run.log` store=1 at frame 148; one existing log reaches store=3 at frame 2461 in `claude-preview13/verify-final/logs/`), and no run logs a pickup. The game has no way to move a dropped item into `CharacterState::inventory`.
3. **Gold is never awarded from a drop** (same cause; `GoldStack` path exists in `pickup` only).
4. **Input/trigger missing**: no PC trigger (no proximity, no OOI/tooltip state, no interact key).
5. **No label**: no tooltip/name text over the ground item (original: B3-2).
6. **No drop motion**: the adapter places the item exactly at the victim position (`publish_at_position`, `victim.transform.position`); original scatters 150-250 units and travels to the landing point (B1).
7. **No pool cap / no PickUpType Automatic** handling in the port (original: 5-slot pool per visual, auto pickup on spawn when `Automatic`).
8. **No AutoTransmute, no inventory-full message, no potion-capacity rule** in the port (`grep AutoTransmute` in port: none). Original rules in B3-5/6.
9. **No persistence**: store is in memory only; a checkpoint reload recreates an empty store (no duplicates, but drops vanish). Original persistence unknown (B4).
10. **Audio**: cue names exist in `audio-assets/data/sounds.xml` (`DropGold` uid 151, `PickupGold` 155, `PickupPotion` 153, `PickupArmor` 152, `PickupWeapon` 154, `DropPotion` 149, `DropArmor` 148, `DropSword` 150, `PotionDrink` 147, `ChestOpen` in the SDD name table), but **the WAV files are missing**: `sfx_drop_*.wav`, `sfx_pickup_*.wav`, `sfx_potion_drink.wav` are not in `audio-assets/data/sounds/`; a search of all of `.local-inputs` found none. No play call site exists for item cues.
11. **Stale claim risk**: `runtime-death-drop-pickup-session-v1.json` says helper PASS but "not integrated / not visually verified". Keep it that way until the EXE shows the item.

### C2. Quiet batch results (rc2 EXE, `quiet_run.ps1`, parallel, silent)

| Run | Args | Result |
|---|---|---|
| `runs/k200`, `runs/k300` | from `claude-preview13/verify-final/wip/kxp-200/300.args`, rc2 assets | exit 0. Lizard `7118915781085668844` killed at frame 143: `Source death reward frame=143 ... xp=8 level=1 spawned=0 store=0 suppressed=0`. No drop this seed. No "world item" or "pickup" lines. |
| `runs/r160`, `runs/r200` | `claude-preview13/rng-check/R/run.args` with frames 160 / 200, rc2 assets, captures | exit 0. `Source death reward frame=148 victim=7118915781085668844 state=2 xpRecipients=1 xp=8 level=1 spawned=1 store=1 suppressed=0`. Captures show no item visual. No other drop/pickup log lines. |
| Existing logs (`verify-final/logs/*.out`, `rng-check/*`) | - | 25 logs contain `Source death reward`. Outcomes: spawned=0 in most; spawned=1 store=1 at frames 148, 225, 229, 289; spawned=2 at 1258, 1930; store up to 3 (frame 2461). Store never decreases. |

Log tags to use for future checks: `Source death reward frame=... spawned=N store=M` (main.cpp L2828-2831). There is no log line for render, pickup or item position yet.

Note on the lizard's loot: spawn count depends on the seeded loot roll; the same lizard gave 0 or 1 items across seeds.

## D. DESIGN (minimal, reusable)

### D1. Data model
- Keep `RuntimeWorldItemAdapterV1` as the single world-item store (already created in main.cpp L2016). Each entry already carries identity, authored item, quantity, source position, `inventory_instance_id`, and `RuntimeWorldItemRecordV1` (loot table, entry, gold value).
- Add to the entry (new fields, not persisted yet): `landing_position` (scatter), `visual_row` (LootAudioVisual row index), `pickup_sound`/`drop_sound` ids, `owner_player` + `owner_timer_ms` (B3-3), `rarity_color` (ItemInstance::GetColor), `pickup_allowed_flags` (none needed).
- Scatter: compute landing point with the B1 formula using the existing RNG owner (`combatSession->world()->random_state()` is the seed owner used by the death path); pool cap 5 per visual row inside the adapter.

### D2. Lifecycle (entity)
- Publish (existing) -> Spawn (set position = victim, destination = landing point, move at item speed) -> Idle (sensor, glow) -> Target (player within sensor or OOI) -> Interact (gates B3-2..6) -> Collected (erase from store, mutate CharacterState in one transaction, play pickup SFX, FX at player) or Rejected (stays; status message).
- Auto pickup: if `PickUpType == "Automatic"`, Interact is called at spawn with the killer (B1).

### D3. Rendering through the existing renderer
- Use the already-written `SourceWorldItemDropRenderV1` (features/interactions) as the renderer projection: `load(assets, *worldItems, lootAudioVisual.borrow(), services, out, error)`, then each frame `prepare(frame)` and `submit()`.
- Services: `material` callback = existing material upload for itemdrops.bdae (Potion0 static, GoldStack01 skinned rest pose), `submit` = the current RenderQueue used by actors.
- Needs: a `LootAudioVisualV8::Borrow` from the menu/loot owner (`menuSourceOwner.loot_owner`); verify it exposes the audiovisual snapshot.
- Limit to Potion0 and GoldStack01 first (what the bank proves); other rows: fallback = no draw + diagnostic (no invented model).
- Label: draw `item name` text above the item only when it is the current target (B3-2), with rarity colour, using the existing combat-text overlay path (`Combat text ...` already exists).

### D4. Pickup trigger (PC adaptation, to be labelled as such)
- Per frame in the gameplay update (main.cpp around the `combatSession->update` block, ~L2814-2822): compute the nearest world item within a sensor radius of the player (PC adaptation: original uses physics sensor contact + OOI; state the radius as an input adaptation). Set it as the target (tooltip). On the interact input (PC key/click chosen by the user) call `RuntimeWorldItemInteractionV1::dispatch_live_player(player, is_local, actor, request, services, *worldItems, receipt, error)`. Auto pickup for `Automatic`.
- Services: `resolve_character_state` -> `sharedCharacter` (same as death rewards, main.cpp L2007-2012).

### D5. Inventory insertion (same owners as equipment)
- Use `CharacterState` (`sharedCharacter`) and `features/inventory/inventory_feature.hpp` `Presenter::pickup` (already used by the adapter). Equipment auto-equip reads the same `inventory`. Gold: `stage_source_add_gold` (INT32 limit baseline).
- Add the original gates (B3-5/6): AutoTransmute option, inventory-full (`character_collection_limit`), potion capacity; status message via the existing status message path.

### D6. Persistence
- Proposed new versioned section in the level/checkpoint save: `world_items` = list of {identity, item_id/definition, quantity, position, landing_position, loot_table, entry, gold value, inventory_instance_id}. Picked items are erased in the same transaction that updates `CharacterState` (no duplicate on reload). Load: rebuild store before first update, reusing identities.
- Older saves: missing section = empty store (no migration of existing saves).
- Open: whether original keeps ground items across checkpoints (G2). Until decided, make it a flag so both behaviours can be tested.

### D7. Audio hooks
- Cue IDs: from the ItemAudioVisual row (`AudioDrop` on spawn, `AudioPickup` on collect) and fixed cues from sounds.xml (DropGold, PickupGold, PickupPotion, PickupArmor, PickupWeapon, PotionDrink).
- Playback: `audio_gameplay_runtime_v42` `submit_plain_source` / `audio_campaign_bridge_v46` submit path exist for plain/actual plays; add a 3D or plain submit at item position. Missing WAVs: keep silent and report; do not synthesize a fallback (A13 rule).

### D8. Integration hooks (main.cpp / CMake)
- main.cpp, after `bindDeathRewards();` (~L2025): create `SourceWorldItemDropRenderV1` (load), store `std::unique_ptr`.
- main.cpp, gameplay update block (~L2814-2834, after `deathRewards.after_update`): nearest-item target + pickup call + SFX submit.
- main.cpp, render submit section (where combat actors are submitted; search `RenderQueue` submit in the gameplay draw): `renderer->prepare(frame)` / `submit()`.
- main.cpp, save write/read near the existing `GameSave` path (exact anchor to be chosen by the save owner).
- CMakeLists.txt: new `foundation_runtime_item_presentation` static library with `features/interactions/source_world_item_drop_render_v1.cpp`, `source_world_item_drop_material_v1.cpp`, `session_world_item_consumer_v1.cpp`; link `foundation_runtime_loot`, `foundation_runtime_equipment_menu`, and the renderer target; link into `dh-foundation` (near L593-607 `foundation_runtime_loot`).

## E. WORK BREAKDOWN

Dependency order. Sizes: S < 2h, M < half day, L > half day.

1. **T1 (M) Pickup and inventory in the live game.** Wire pickup: nearest target + interact input (PC adaptation) + auto pickup; call `dispatch_live_player` with `sharedCharacter`; add the B3-5/6 gates; status message. Tests: unit tests for gates (inventory full keeps item, potion capacity keeps item, AutoTransmute converts, duplicate pickup no effect, owner window rejects). Quiet verifier: rng-check/R seed (drop at 148) with an auto-pickup or scripted interact; expect `store` 1 -> 0 and inventory/gold +1 in logs and in the saved gameplay save.
2. **T2 (M) Visual projection.** Compile the `features/interactions` renderer, load, prepare/submit per frame for Potion0/GoldStack01. Tests: loader unit tests (exists in source), then quiet captures at frames 150/160/200 (run `r160` args) must show the item mesh at the death site. Compare with reference frame ~211 s.
3. **T3 (S) Target label.** Name text above the target item with rarity colour; capture test with a magic item (label green like "Imbued Armor" in video).
4. **T4 (S/M) Scatter and pool.** Landing point via B1 formula, 5-slot pool per visual row, deterministic under the existing RNG. Test: multiple drops in one kill differ in landing point; sixth drop of one visual recycles the oldest.
5. **T5 (M) Persistence.** Versioned `world_items` section; save on checkpoint; restore on load; never duplicate picked items. Test: drop -> save -> reload -> one item at same place -> pick -> save -> reload -> none.
6. **T6 (S, blocked on assets) Audio.** Wire cue submits; blocked on WAVs (G3).

## F. DEPENDENCIES / CONFLICTS

- **Equipment**: pickup writes `CharacterState::inventory` through `inventory::Presenter`; equipment auto-equip and inventory UI read the same list. Pickup must not break `source_instance_resolver` stable IDs (refresh after transfer, see loot README).
- **Skills/stats**: pickup counter `PROPS 223` (Character+1376) and potion capacity (Character+936) are stats; the skill/stat stream must keep these fields.
- **Quests**: original `ItemObject` calls quest hooks (`_GetNextPlayerQuestRRId`, `_OnItemCollected`); a quest item pickup event must be emitted after the item enters the inventory (quests stream).
- **Faery**: none directly.
- **Main menu metadata**: the save slot metadata (map/act/difficulty) reads the same save section; the new `world_items` section must not change slot metadata.
- **Shared files**: `main.cpp` (gameplay update, draw, save anchors), `CMakeLists.txt`, `GameSave`/`SaveStore` (save format). Chests/urns (`session_container_modern_drop_v1`) use the same store and renderer; keep them in the same change set or behind the same API.

## G. OPEN QUESTIONS (cannot be decided from evidence here)

1. **PC pickup input.** The original is sensor contact -> target -> Interact. The video shows no key/click. Which PC input (key, click on item, or auto-walk-over)? Proposed default: nearest-target + interact key (PC adaptation).
2. **Persistence across checkpoints/reloads.** No ItemObject save function found in the IDA export. Need the level/checkpoint save IDA search to decide if ground items survive a reload.
3. **Missing item WAVs.** `sfx_pickup_*.wav`, `sfx_drop_*.wav`, `sfx_potion_drink.wav` are absent from every local input. Accept silence until the original assets are supplied, or is there another source?
4. **Potion over capacity.** Pseudocode says the potion is not taken and stays on the ground. Confirm by video or by another IDA read of the `+104`/`+936` fields.
5. **`+104 == -1` branch.** The full-inventory check is skipped for these rows; need the field's meaning to confirm (AutoTransmute vs. unlimited).
6. **Rarity colours** (`Arrays::FontPalette` index per `ItemInstance::GetFontDef`): RGB table needed for the label.
7. **Drop motion.** The video is too coarse for the arc. Need a higher-resolution source or the `ItemObject` speed value (`+944`) and the GoldStack pose clip before choosing an arc.
8. **Gold drop visuals.** No gold pile captured in the reference window; GoldStack01 model is only partially proven (rest pose).
