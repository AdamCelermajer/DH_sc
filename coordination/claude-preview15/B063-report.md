# B063 report: looting by walking onto the item (no key)

User bug: "looting please make it by walking on it with the intended logic, not E". Preview 15 required E (PC adaptation from P14) while an item was targeted.

## Evidence
### Video (Part 1, 640x360, looked at frame sequences)
- 205-222 s (chest items, `v/seq*.png`, 4-5 fps): hero walks with the joystick only; green "Imbued Armor" then white "Useless Mace" appear as pickup status lines (fixed HUD place, fade) and the inventory tutorial sheet starts right after. No pickup button visible.
- 285-296 s (Bogwomp kill, `v/pot.png`): hero keeps running, a "Potion" status line appears, then the tutorial about carrying 12 potions. Again no button.
- Not observable: touches (the touch overlay is not drawn), pickup sound, pickup FX at this resolution. The ground-item moment where the hero walks onto an item is not isolatable. Direct observation: pickups happen while the hero is only moving. Inference: nothing else is pressed.

### IDA (libDungeonHunter2.so pseudocode-all.c / assembly-functions.asm)
- ItemTable word 3 PickUpType = 1 ("MoveOn") for ALL 1322 rows (unit test histogram `1=1322`), so no row is Automatic (`_DoAutoPickupHack` 0x3ec474 never fires in Act 1 data) and no row is Interact-only.
- `ItemObject::GetInteractionType` 0x3ebeb4 (asm): always returns -1.
- `POItem::onCollisionBegins` 0x4702a8 -> `ItemObject::OnCollisionBegins` 0x3ec048: when the type is -1 and `CharStateMachine::SM_IsMoving(0)` holds, the Character is stored at item+0x2E4 (this+185). `GameObject::Update` 0x38cbe8 (and TriggerObject::Update 0x3994a8): if +0x2E4 is set, call vtable+152 (`Interact`, with the stored character in R1) and clear it. This is the walk-over: contact begin while moving -> Interact on the next update. No key, no action button.
- Fallback on mobile: HUDControls::Update 0x41a780: attack button with an object of interest (`Character::UpdateObjectOfInterest` 0x3abb9c, OOI_Distance, type -1) -> `v2Controller::Cmd_UseOOI` -> `Character::UseOOI` 0x3ad614. PC has no pickup key by user rule.
- `ItemObject::Interact` 0x3ed144 gates (already implemented in `interact_world_item_v1`): looted, owner (item+956), owner-protect timer (+952 for the dropping player), local player, AutoTransmute option, potion capacity (`GetNumPotions < +936`), inventory full. Success: status line `<font color>name</font>` (this is the label seen in the video), `cinematic_Tuto_menuInvSheet` tutorial for the first non-gold item, TransferInventoryTo, stat 223 (+trophy at 300), pickup sound, `loot_orb_fx`.

## Root cause
`main.cpp` ran `interact_world_item_v1` only from the P14 PC adaptation "interact key (E) while targeted" (and `--pickup-frame`); contact only set the target/label. The recovered original trigger (contact begin while moving) was never wired. Latent second bug found while testing inventory-full in the EXE: the localized `GAMEPLAYMENUS_INVENTORY_FULL` text carries `<font color=...>` markup, which the single-line HUD text check rejects, ending the run with "Foundation error: World item status" (a crash exit 1).

## Change
- `features/loot/world_drop_rules_v1.{hpp,cpp}`: new `WorldItemContactTrackerV1::begin_contacts(store, playerPos, playerMoving)`. Sensor box +-225 (existing adaptation). Returns, nearest first, items in contact for which no attempt was made during this contact, only while the player is moving (SM_IsMoving gate). One attempt per contact (rejected items are not retried until the player leaves the box and re-enters). Items the player dropped (owner set) that appear under the player start as attempted, so drop + walk away does not re-collect. Also covers items sliding into a walking player.
- `main.cpp`: the E key no longer picks up (Space/E stays the context button for chests/NPCs). Each frame the tracker's items run the existing `runWorldItemPickup(id,"walkover")` (same gates, status line, sound event, store retirement). Tracker cleared on reload / menu return. `--pickup-frame` stays as a scripted test hook. New test hook `--move-segment FROM:TO:X,Y`. Inventory-full text: font markup and control characters stripped.
- PC adaptation, labelled: original needs contact BEGIN while moving; when the player stands still while an item lands under them (common: the item scatters 150-350 units toward the killer, inside the 225 box), the original had the action button as fallback. PC has none, so the first moving frame inside the box also collects (once per contact).

## Tests
- `world_drop_rules_v1` ctest PASS (new `walkover_contact` block): outside box (400 and 226 units) = no contact; edge 224 = contact once; no retrigger while inside; second item entering; idle contact does nothing until moving; one attempt per contact; leave and re-enter = retry (nearest first); store cleared / retired item; item landing on a walking player; clear() for session rebind; player-dropped item: no collect when walking away, collected on real re-entry.
- Full ctest (ctest.exe run outside the build script's PATH): 2 failures, `session_skill_binding` (known worktree failure) and `winmm_pump_priority_v1` (exit 0xc0000135 = missing DLL in my shell PATH, unrelated to loot; not re-run with the toolchain PATH).

## Real EXE before / after (quiet runs, Preview 15 assets, seed 1234 lizard kill, scratch `.local-inputs/claude-preview15/fix063/runs/`)
- BEFORE (Preview 15 EXE, `base_idle/before.log`): `World item target frame=148 ... ClothGloves01`, no pickup line to frame 300 without E.
- AFTER, idle (`base_idle/after3.log`): the item is targeted at 148, hero never moves, no pickup (idle gate).
- AFTER, walking (`walk_*/after3.log`, `cap1234`): `World item pickup frame=181 item=1 id=ClothGloves01 reason=walkover outcome=0 picked=1 stacks=4->5 store=0`; frame `cap1234/cap.png` shows the "Imbued Bracers" status line.
- Other types: seed 16 `World item pickup frame=182 ... GoldStack01 ... gold=0->9` and `Potion0 ... stacks=4->5` (both from one contact), seed 14 `Staff01 stacks=4->5`.
- Inventory full (`full1234`, 100 extra bag items): `outcome=4 picked=0 stacks=104->104 store=1` at 181, the item stays, retry only after leaving and re-entering (second line at 215); frame `full1234/cap.png` shows the plain "Inventory full" line and the item label. Before the markup fix this run died with the HUD text error.
- Pickup sound: `World item sound item=1 event=pickup source=154 status=no_audio_runtime` (event dispatched; quiet runs have no audio runtime).

## Remaining gaps / unknowns
- No video frame of a hero walking onto a ground item with a visible item; evidence is the pickup status lines while only the joystick moves plus the IDA path.
- Contact test is "player point inside the item's +-225 box" (P14 adaptation), not Box2D body overlap (character body radius unknown).
- "Walk past outside the radius" and "potion capacity" are unit-level; the scripted EXE fight cannot leave the box before the kill. Potion over capacity: not exercised in the EXE.
- `loot_orb_fx` at the character and the inventory tutorial script (`cinematic_Tuto_menuInvSheet`) on first pickup are not implemented here (pre-existing gap, not part of B063).
- Item in flight: unit-tested only (slide-in), not captured.

## Package files required
None (code only; no new assets).
