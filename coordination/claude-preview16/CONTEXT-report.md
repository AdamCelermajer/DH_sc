# CONTEXT report (PC context button, interaction priority, walk-over pickup)

Status: IMPLEMENTED AND ISOLATED-TESTED; WALK-OVER PICKUP VERIFIED IN THE QUIET EXE. Not verified in the EXE: chest/enemy priority with a real chest, HUD action button drawing. Section 1 = IDA decode. Sections 3-5 = implementation, quiet evidence, gaps.

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
- The marker FX array (`Character+0x1494` = slot 1317, 9 entries, 0x24 bytes) is filled from AnimatedEffectTable from `target_circle_00_chest` consecutively:
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

## 3. Implementation (branch p16/context, commits dea95d41, 50567aca, 375a2c26, 90a78171)
- `features/combat/object_of_interest_owner_v1.{hpp,cpp}` (extended, B004 owner): source candidate loop. Candidate fields: is_character, position, radius, interaction_type, type1_targets_owner, eligible. Queue = eligible and in range (3D centre distance - radius - owner melee radius <= 200), Characters first, then ascending angle to the owner's look vector. `select_object_of_interest_v1` is the pure loop; `update` keeps the 500 ms timer, per-frame validity, the cached type (`interaction_type()`) and `changed()`. B004's nearest-distance rule is replaced by the source frontal-angle rule.
- `features/combat/object_of_interest_world_v1.{hpp,cpp}`: adapter. Owner look = (-sin h, cos h, 0) from rotation[2] (ActorMovement::root_world_delta convention). Enemy characters are type 8. Optional `InteractableRegistryV1` adds objects. Main call site unchanged except the registry argument.
- `features/interactions/interactable_registry_v1.{hpp,cpp}`: the interaction-type provider interface. Each class registers `interaction_type(viewer)` (and optional `type1_targets_viewer`); the registry builds candidates per viewer. Chests, barrels, NPC objects, triggers register here (containers/NPC agents). Registry is empty in the EXE today.
- `features/combat/context_button_v1.{hpp,cpp}`: `decide_context_button_v1` (Space press edge and held rules, section 2) and `action_button_icon_v1` (MenuManager table).
- `features/loot/world_item_contact_v1.{hpp,cpp}`: `WorldItemContactTrackerV1` (contact begins while moving is stored; consumed on the next update; one per contact; unavailable items dropped).
- `main.cpp` hunks (anchors: include block after object_of_interest_world_v1; member block after objectOfInterest; OOI tick; applyPlayerFrameControls input block; pickup block):
  1. Context block before the gameplayInput attack OR: suppresses held attack when the decision says so; on the press edge with an admitted UseOOI it calls `set_source_target(player, OOI, false)` for actor OOIs and logs `Context button frame=...`.
  2. Action icon: `Action icon frame=... icon=... type=...` printed on change.
  3. Pickup: E (`uiInput.actions.interact`) no longer picks up. Walk-over via the contact tracker with the item sensor box (225) as contact. `--pickup-frame` kept as a scripted test hook only.
  4. Test options: `--move-segment start:end:x,y,z` (repeatable), `--move-from-frame`.
- `CMakeLists.txt`: one commented `target_sources` line after the B004 OOI line.
- Isolated tests: `port/windows-foundation/features/combat/run_context_p16_tests.ps1` (OOI priority matrix 24 cases, range/timer/validity, marker; context press-edge and icon 17 cases; contact tracker 7 cases; registry 6 cases). All PASS.
- Build: `p14_build.ps1 -Name p16context` build exit 0.

## 4. Quiet verification (integrated EXE build `DH_wt/build-p16context/dh-foundation.exe`, quiet_run.ps1, hidden desktop)
Base: P15 drop args (`verify-p14feat/drops-verify/drop/run.args`, seed 1234, swamp), Space held 30..160 (kill at frame 148).
- Context button: `Context button frame=30 ooi=7118915781085668844 type=8 use=1 actor=1` (press edge on a lizard OOI). `Action icon frame=0 icon=5 type=8`. No context lines on the held frames.
- Kill and drop still work: `Source death reward frame=148 ... spawned=1 store=1`, `World item target frame=148 ... ClothGloves01`.
- Contact diagnostics (`World item contacts frame=... count=... moving=...`):
  - Standing still in the sensor box at 148 (`count=1 moving=0`): no pickup (`still`, `stand` runs).
  - Exit while moving, no return (`xexit`, `px`, `nx`, `py`, `ny`, `xback`): no pickup.
  - Exit then re-enter while moving: `yback` contact frame 282 moving=1 -> `World item pickup frame=283 item=1 id=ClothGloves01 reason=walk-over outcome=0 picked=1 ... stacks=4->5 store=0`; `nxback` 299 -> pickup 300; `nyback` 292 -> pickup 293.
- Logs: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview16/walk4/{yback,nxback,nyback,xexit}/run.log`; control `walk/still`, `walk2/stand`.
- Not verified: the chest/enemy priority in the real EXE (no chest in this scene; containers agent must register); the HUD action icon is not drawn; the held-vs-press behaviour over a non-combat OOI in the EXE (no such OOI present); visual check of the marker ring after the owner change (captures exist, not reviewed).

## 5. Gaps, uncertainties, and corrections
- B004 correction: the source ranks by frontal angle, not nearest distance. The earlier claim "nearest eligible within 200 (horizontal)" was wrong. Range is 3D and measured from the melee edge.
- Two OOI producers now exist: this owner, and `level-world/character_object_interest_v106.*` (services form of the same function, not wired in the EXE). Their semantics match (flag-first, pop order, type-1 owner check). They should be merged into one owner later.
- Friendly NPC eligibility (mask 0x8: friend and not in the interacting state) is not implemented. Only enemies are admitted from Characters (type 8). NPC talk (type 3) needs the NPC agent to register.
- Candidate radius = `PlayableActorWorld::target_radius` (melee reach). The source vt+148 is not decoded; the mapping is unverified.
- Per-frame validity (vt+140) is approximated by "still in the eligible candidate list". The `OOI+129` clear bit is not decoded.
- `set_source_target(..., false)` is assumed to equal `AI_SetTarget(ai, OOI, 0)`. Not verified against the combat session side effects.
- Type 1: no Character or ItemObject returns it. TriggerObject data and the `+956 == owner` item-owner clause (V106 `item_owner3bc`) suggest a data-driven trigger/item case. Unresolved; no provider registers type 1.
- Walk-over contact uses the item sensor AABB (225 half-extent, existing port approximation). The source is the physics begin-contact of the item body; its shape is not decoded.
- PC adaptation (explicit, user decision): Space is press-edge for non-combat OOIs (chest, NPC). Held Space never activates them and suppresses the attack over them. Enemy OOIs keep the source held behaviour.
- HUD action button: state is computed and logged (frame 0..11 mapping). NOT drawn. The authored `btn_interact` (ID 374, `btimg` frames) exists in `engine-ui/authored_gameplay_hud_v1`, which is not wired into the PC HUD. Needs the HUD owner to draw it; positions from the authored layout, not the PC skill layout.
- Chests (containers agent): no registration yet. `interactables` is empty, so no chest or barrel can become the OOI in the EXE until they register.
- Scripted tests: `--move-segment` and `--move-from-frame` are test hooks; `--pickup-frame` stays a scripted hook for the DROPS batches.
- Commits are local on `p16/context`; not pushed (per brief).
