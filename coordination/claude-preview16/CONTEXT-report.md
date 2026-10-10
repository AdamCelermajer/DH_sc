# CONTEXT report (PC context button, interaction priority, walk-over pickup)

Status: IN PROGRESS. Section 1 (IDA decode) is complete. Sections 2-5 are being implemented.

## 1. Evidence (IDA: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`, ARM32)

Vtable note: `_ZTV` raw words start with offset-to-top and typeinfo, then the virtuals. The brief's "vt+N" is the raw word at 8+N.
Verified: ItemObject raw 152 = GetInteractionType 0x3ebeb4 (vt+144), raw 160 = Interact 0x3ed144 (vt+152).
GameObject::Update disassembly (0x38cc6c-0x38cc84): `LDR R1,[R4,#0x2E4]; ... LDR PC,[R3,#0x98]` = vt+152(this, item+740).

### 1.1 OOI refresh: Character::UpdateObjectOfInterest 0x3abb9c (called from Character::Update, pseudocode ~126682)
- State: OOI at Character+0x14a4; 500 ms timer at +0x14aa (WORD); cached candidate type at +0x14a8 (-1 when none); candidate flag cache +0x14ac; changed flag +0x14ad.
- Every call: the stored OOI is validated with vt+140 (cleared when invalid or when OOI+129 is set). Timer -= dt; return while > 0.
- On expiry: timer = 500, cached type = -1, flag cache = 0, OOI = 0, then the query:
  - `TargetList(this, mask 89, mode 1)`, centre `GameObject::GetTargetPosition(this)`, radius `CharacterDesign.OOI_Distance` (200),
    arc 2*pi (`1086918619` = 0x40C90FDB, no cone), sorter `TargetSorter::_sortFrontal` (0x38d5b4).
- Search (`TargetList::Search` 0x4a2f34): skip the owner; candidate needs byte +138, gates vt+196/+750/+752 and vt+136(candidate, owner);
  `d = |c - p| (3D) - radius(vt+148) - MeleeRadius(owner AI)`, must be <= 200. Each survivor is queued as
  `TargetInfo { obj +0, d +4, angle +8 = |angle(c-p, owner look)|, flag +12 = candidate is a Character }`.
- Characters pass `_IsCharacterValid` (0x4a1ab8), mask 89 = 0x59: 0x01 enemy (AI_IsEnemy, not dead-ish), 0x08 friend whose SM state != 16 (owner SM != 15), 0x10 merchant, 0x40 flag +762.
- GameObjects pass `_IsGameObjectValid` (0x4a1950) in mode 1: `vt+144(obj, owner) != -1`. Items return -1, so they never enter the queue.
- Queue order (`_sortFrontal`, comparator 0x38d5b4): FLAG first (all Characters), then ascending angle (most frontal first). Distance is NOT used to rank.

Candidate loop (pseudocode 126139-126178), per popped candidate c (not the owner):
1. If OOI is already set and c is NOT a Character and `vt+144(c, owner) != 1`: skip.
2. OOI := c; cached type := `vt+144(c, owner)`; flag cache := isCharacter(c).
3. Accept (stop) if c is a Character; or type is 0 or 2; or (type 1 and `c+956 == owner`).
4. Otherwise continue. At exhaustion the OOI is the LAST ASSIGNED candidate, even if it was never accepted, and the cached type is its type.

Consequences:
- Any valid Character in range wins: the most frontal one, whatever its type (enemy 8, friendly NPC 3, ...). Characters sort first and are accepted immediately.
- Only when no Character qualifies: the most frontal GameObject with type != -1 is assigned. It is accepted only for type 0 (OpenableContainer) or 2. Any other first GameObject (DestructibleContainer 8, LiftableObject 4/5) stays the OOI and blocks later non-type-1 objects, so a chest behind a barrel loses.
- Type 1 comes only from data-driven `TriggerObject::GetInteractionType` (0x399330, row word from `Arrays::TriggerObjects`). No Character or item returns 1.
- So enemy vs chest in range: the enemy wins (the user's wish). This is a RESULT of the Character-first queue order, not an explicit enemy-first rule.
- Correction to B004 (`object_of_interest_owner_v1` "nearest eligible"): the source ranks by frontal angle, not distance, and uses 3D distance with radius and melee terms.

### 1.2 Type sources (GetInteractionType, vt+144)
| class | function | return |
|---|---|---|
| GameObject (default) | 0x38ad74 | -1 |
| OpenableContainer | 0x3a16cc | 0 |
| DestructibleContainer | 0x3a0d60 | 8 |
| Character | 0x3a47e8 | 8 if `AI_IsEnemy(this.ai, a2)` and not `vt40(a2)`; -1 if summoned or `vt52(this)`; 8 if monster; else 3 (friendly NPC) |
| TriggerObject | 0x399330 | data row (TriggerObjects table) |
| ItemObject | 0x3ebeb4 | -1 always (no action button) |
| LiftableObject | 0x3ee440 | 4 if unheld; 5 if held by a2; else -1 |

### 1.3 Marker FX by type (Character::Update local-player block, pseudocode ~126735-126790; FX creation ~132100)
- The marker FX array (`Character+0x13b4`, 9 entries, 0x24 bytes) is filled from AnimatedEffectTable from `target_circle_00_chest` consecutively:
  0 chest, 1 item, 2 lever, 3 npc, 4-7 UNUSED, 8 attack. Revive (10) is the sentinel `&byte_9[1]` in SM_SetInteractState.
  The names are confirmed in `port/game-data/reference/effects-tables/original-reader-projection.json`.
- Marker target (local player only): last target `+0x40c` (Character+1036) if set and not self, else the OOI. The FX shown is FX[type].
  For type 1 it also calls `ItemObject::OnCollisionBegins(target, player)` (reachable only if a type-1 object is the target).

### 1.4 Action button dispatch (HUD)
- Press/release: `HUDControls::OnEvent` 0x418d28. RenderFX event type 4 sets `HUDControls+9 = 1`; types 6/7 set it back to 0. It is a LEVEL (held) flag.
- `HUDControls::Update` 0x41a780 runs every frame. If `+9`: if the player's OOI (+0x14a4) != 0, call `v2Controller::Cmd_UseOOI(ctrl, null)` (0x4057fc); else clear `player+1043` and call `Cmd_Attack(ctrl, null)` (0x405b04).
- `Cmd_UseOOI` (offline branch) -> `Character::Ctrl_UseOOI` 0x3ad690 -> `Character::UseOOI` 0x3ad614: if OOI != 0 and `Character+1032` (current attack target) is null and SM is idle or moving, then `CharAI::AI_SetTarget(ai, OOI, 0)` and `+1042 = 1`. There is no type dispatch here.
- Type dispatch is in `CharAI::AI_InteractWith` 0x3cff34: `type = vt+144(target, owner)`; if type is -1 or -2, stop; else `SM_SetInteractState(sm, type)`.
  `SM_SetInteractState` 0x3c64ac: type 8 -> attack state; sentinel 10 -> revive; 0..N -> that CharAnimTable interaction plus `RaiseStateEvent(50003)` (CSInteract).
- `Character::Interact` 0x3a4d78 (friendly NPC talk): raises the TalkToNPC quest event, `SM_SetInteractState(3)`, then RaiseEvent(5).
- `Cmd_Attack(null)` (0x405b04): melee on the existing AI target (`AI_DoMeleeAttack`), i.e. held auto-attack.

### 1.5 HUD action icon
- `MenuManager::Update` 0x42eab4 (`reference/authored-gameplay-hud-v1/action-icon-source.md`): reads the signed cached type at Character+0x14a8. If the type is negative or above 10, the icon is 5. Otherwise icon = table 0x8c9f28 = [0,1,2,3,5,5,6,7,5,5,4][type]. The cache starts at -1 and the callback runs only when the icon changes.
- `FillActionIcon(icon)` drives `controls.controls.btn_interact` (ID 374, bottom right; `btimg` frames 0..11). Attack artwork is `btn_interact` (authored HUD handoff, line 59).
- Resulting icons: chest (0) -> icon 0; friendly NPC (3) -> icon 3 (talk); enemy (8) or no OOI (-1) -> icon 5 (sword/attack); type 10 -> icon 4.
- The PC HUD has NO action button today (`features/generic_skills/pc_gameplay_hud_v1` draws skills 1-3, Faery and potion only).

### 1.6 Item pickup chain (answers the user's question)
- Contact: `POItem::onCollisionBegins` 0x4702a8 (physics begin-contact; the partner Character is resolved from its handle) calls `ItemObject::OnCollisionBegins` 0x3ec048 with (item, character). If the item is not `HasBeenLooted` and `vt+144(item, character) == -1`: when `SM_IsMoving(character)` then `item+740 = character`. Otherwise nothing is stored.
- The tooltip branch (`item == character OOI`) is unreachable for items, because items never enter the OOI (1.1).
- Consumer: `GameObject::Update` 0x38cbe8: `if (this+740) { vt+152(this, this+740); this+740 = 0; }`. vt+152 = `ItemObject::Interact` 0x3ed144. Gates: `HasBeenLooted` returns; the toucher must resolve to a Character; owner protection window (`item+476 > 0` for the owner) returns; `vt+40(toucher)` (alive) returns; then the inventory transfer.
- Automatic pickup: `ItemObject::_DoAutoPickupHack` 0x3ec474, called right after `ItemManager::Spawn` in the drop path (0x3ec8a0/0x3ec974 area). If `PickUpType == "Automatic"` and a character is given, `vt+152(item, character)` runs at once.
- Conclusion: walk-over pickup is real. It fires when a contact BEGINS while the character is moving (SM_IsMoving). Standing still on an item does not pick it up; leaving and re-entering while moving does. Non-automatic items have no action button (type -1 and never in the OOI), so there is no button path and E is not a source input for pickup.
- Shape: the source contact is the physics begin-contact of the item body; its exact shape is not decoded here (`ItemObject::SetRelativeAABB` scales the AABB by 1.5). The port approximates it with the existing AABB sensor box (`world_item_sensor_half_extent_v1` = 225).

## 2. Expected behaviour (preserved rules)
- Space (PC adaptation of the held action button) is a single context button.
  - Press edge, OOI present: UseOOI (AI target = OOI, subject to the original gates). Type decides the outcome: enemy -> attack; chest -> open (containers agent); NPC -> talk.
  - Press edge, no OOI: Cmd_Attack(null), i.e. attack the current/last target.
  - Held (not an edge): combat-type OOI (enemy) keeps the attack path; no OOI -> Cmd_Attack(null). A non-combat OOI (chest, NPC) is NOT activated by a hold. This is the PC adaptation, so chests never open by accident.
- Action icon: from the cached type via the MenuManager table (1.5).
- Walk-over pickup: an item is taken when a contact begins while the character is moving. The E key is removed from pickup.
