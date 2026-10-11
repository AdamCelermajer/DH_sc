# world batch fidelity audit (2026-10-11)

## Progress
- last_address_done: 0x4bbd50 (all gameplay-consequent groups reviewed; remaining table/serialization boilerplate grouped as n/a)
- groups_done: GameObject spawn probability/conditions/interaction radii; SpawnGroupManager/SpawnSpot/SpawnPoint; Trigger/TriggerZone/TriggerObject/TriggerPlate/Zone/ZoneEx; TriggerTrap/Timer/Projectile traps; CheckpointZone; Door; Destructible/Openable containers; Liftable (no authored data); RoomZone/ObjectManager (skimmed); PO* proxies; Arrays/Structs readers; CRoomAttributes (network); Dragable/Dropable (menu UI)
- next: none (audit complete). Possible follow-up: ExitLevel zone, RoomZone activation, TimerTrap/ProjectileTrap Lua data, GameObject::createBindings Lua API (script batch)

## Method note
The port's `port/level-world` canonical_* / *_v83 / *_v108 kernels are line-by-line reconstructions of the IDA code with every engine dependency injected as a "service". Where I compared them against pseudocode (spawn group update, spawn probability, Zone IsInside/IsTouching, checkpoint save, Trigger Activate/CanActivate, Destructible and Openable container logic) they are faithful, including quirks. The big risk is therefore NOT kernel arithmetic but **wiring**: `port/windows-foundation/main.cpp` (the shipped `dh-foundation.exe`) does not instantiate almost any of these kernels. Verified by grep: no non-test source under `port/windows-foundation` references SpawnGroup, SpawnSpot, TimerTrap/ProjectileTrap/TriggerTrap, TriggerPlate, CheckpointZone, Liftable, Door runtime, or the Openable/Destructible session binders from `main.cpp`; the only linked `features/interactions` sources are the world-item drop/pickup ones (`CMakeLists.txt:709-712`). These gaps are mostly already tracked in `docs/BUGS-AND-IMPLEMENTATION.md` as open (I003 chest/pot enrollment, I009 intro/spawn, I021 transitions/checkpoints, I022 triggers, I024 checkpoint revive), so "missing" below means "not live in the exe", not "wrong code".

Authored data used for impact counts: `.local-inputs/assets-extra/android/data/3d/modules/**/*.mgp` (whole game; the recovered/assets/source-data path in the brief does not exist in this checkout).

## Summary
Status counts (grouped rows in mapping table): faithful 14, divergent 3, partial 4, missing 8, unclear 2, n/a 9, intentional-adaptation 0.

Top 5 risks:
1. **Population policy drops enemies with `spawn_prob < 100`** instead of rolling (GameObject::CheckSpawnProbability). Whole game: ~350 authored objects (297 at 90, 26 at 50, 13 at 30...). Swamp: 15 objects.
2. **Condition handling in `main.cpp` population policy** treats `activate_cond`/`deactivate_cond` strings not passed on the CLI as "unknown, do not instantiate", including the literal `"Invalid"` which the original treats as "no condition = true". Swamp loses ~8 `Invalid` + 8 `IsBefore_Swamp_Escape` + `GameStartOnly` + `NoFollowers1` objects.
3. **CheckpointZone auto-checkpoint missing** (106 zones). Only manual F5 save exists.
4. **Doors (114), TriggerZone/TriggerObject/ExitLevel (194/43/136), traps (41, crypt), SpawnPoint entry scripts (161)** are not live: no closed-door collision, no authored trigger scripts, no level exit, no entry-point script.
5. **Containers (1059 destructible, 230 openable)** are not live in the exe (kernels are faithful; binder exists but "no main enrollment claim").

## Findings

### F1. Spawn-probability roll replaced by hard exclusion (high)
- IDA `GameObject::CheckSpawnProbability` 0x38bd64: if handle is a player → -2. Else, if cached roll (field 156) == -1, roll `Random::GetRandom` (0..99, offline channel 0), store. `roll >= spawn_prob (field 157)` -> object is hidden (vcall +64 with 0), `ObjectBase::Delete`, byte130=0, `ObjectManager::MarkForDeletion`. Otherwise keep (-2). Called from GameObject::InitPost 0x38be5c and every derived InitPost/InitFinal (Door 0x3e7da8, TriggerTrap 0x39dee0, Container...).
- Port kernel `port/level-world/game_object_spawn_probability_v1.cpp:3-25` is faithful (uses `dh2_loot_v2_random(random,99)`, same -1/-2 caching, same delete path). But the live population path does not call it: `port/windows-foundation/main.cpp:1107` `it=a.properties.find("spawn_prob"); if(it!=end && it->second!="100"&&it->second!="100.0") return PopulationDecision::unknown;` and `actor_population.cpp:~125` skips `unknown` actors.
- Consequence: Characters (and any object) with spawn_prob 90/80/70/60/50/30 never appear (original: appear with probability prob%). Act1 swamp: 6x50, 4x80, 3x30, 2x70; darkwood 4; whole game 297 at 90 (castle). Fewer enemies, wrong encounter density; RNG stream not consumed per object either.
- Confidence high. Fix: in policy return `include` after rolling `Random(0..99) < prob` with the shared source RNG (use the kernel), caching the roll per object so reload/restore is stable.

### F2. activate_cond / deactivate_cond semantics wrong in live population policy (high)
- IDA `ConditionData::Init` 0x33eb28: empty name, name == "Invalid", or a name not in `Arrays::v2Conditions` -> no ConditionList created; `ConditionData::IsTrue` 0x33e5f8 returns 1 when the list is null. `ObjectBase::TestEnableCondition` 0x33e6d4: enable = (min_difficulty <= level difficulty) && ConditionData(140).IsTrue(); `TestDisableCondition` 0x33e61c: disable only if IsTrue()==false of the deactivate list (null list -> returns early, not disabled). Both re-evaluated at runtime (`SetEnable`), not only at load.
- Port: `main.cpp:1099-1105`: any non-empty `activate_cond` not in the CLI `--condition-active` set -> `unknown` (not instantiated); any non-empty `deactivate_cond` not in `--condition-inactive` -> `unknown`. Shipped `swamp.args` only passes `--condition-active RENE_FOLLOW`.
- Consequence: `"Invalid"` (8 activate + 1 deactivate in swamp alone) objects are dropped though the original always enables them; `GameStartOnly`, `NoFollowers1`, `IsBefore_Swamp_Escape` (8), `IsBefore_Swamp_KillWitch`, `EDWARD_FOLLOW` etc. are never evaluated against quest state. Also no runtime re-evaluation when quest state changes (objects never appear/disappear later).
- Confidence high (IDA clear; data counts from swamp .mgp). Fix: evaluate through the quest/v2Conditions evaluator (ConditionList::Eval), treat Invalid/empty/unknown as true, re-test on state change.

### F3. CheckpointZone has no live effect (high, documented open I021/I024)
- IDA `CheckpointZone::OnCollisionBegins` 0x395f04: for a Character that is the local player, if zone position != player's `checkpoint1468` (epsilon compare), call `Level::CheckpointSave(level, zonePos)` (stores checkpoint pos, then SG_SaveCheckpoint + LevelSavegame::SaveCheckPoint when save phase > 33, player alive); records player in set so repeat entries still re-test position.
- Port kernel `port/level-world/zone_collision_runtime_v83.cpp:50-100` (`checkpoint_collision_begin_v83`, `level_checkpoint_save_v83`) is faithful, including epsilon 0x38d1b717 and "stores happen before save failure". Not referenced by `main.cpp`; the exe only has manual F5/F9 (`main.cpp:3011-3056`) and `features/campaign/runtime_source_checkpoint_v1.cpp` (transition-keyed saves, no zone).
- Consequence: 106 authored checkpoint zones do nothing; death/revive cannot restore to last checkpoint. Confidence high. Fix: create zone bodies from `CheckpointZone` placements (dimensions field) and call the kernel on player contact begin.

### F4. Doors not live (high)
- IDA `Door::InitPost` 0x3e7da8 creates a static PhysicalObject (filter group 2, collides with all) when "door_collision" byte933 is set; `Door::Closed` 0x3e7598 enables its filter and calls `PFWorld::FlagFloorAsDeadEnd(true)` (pathfinding treats door footprint as blocked); `Door::Opened` disables; `Door::Open/Close` 0x3e786c/0x3e763c play "open"/"close" timeline, set state 3/2, block re-entry via byte940 until the timeline callback; Arrays::Doors sounds (+12 close, +16 open) played 3D.
- Port: kernel `canonical_door_v27.cpp` and `features/progression_barriers/source_door_commands.*` exist (script command receivers only). `main.cpp` has no door handling. 114 Door objects in data (swamp 12, darkwood 15, crypt 30...).
- Consequence: closed doors do not block the player/enemies (unless geometry itself blocks) and no AI dead-end flagging; scripted open/close does nothing. Confidence medium-high (cannot prove module mesh does not already block). Fix: instantiate Door owner per authored Door, wire filter + PF dead-end + script commands.

### F5. TriggerZone / TriggerObject / TriggerPlate / ExitLevel / SpawnPoint scripts not live (high, documented I009/I022/I021)
- IDA TriggerZone::Update 0x39b458: skipped if player flag 5248 set; gated by `Trigger::CanActivate` (0x3987ac: `count>=0 && count<=activated || timer>0` blocks; ignores object enable byte138), MeetCondition; counts contacting players (`GetNumPlayerActivating`); enter edge starts script (only if not already running) and `Trigger::Activate` (0x398794: `activated++; timer=delay`) when no move-out script; exit edge starts move-out script then Activate; multiplayer variants with all-player scripts. TriggerObject::Interact 0x399af0 runs OnTrigger Lua, plays sound, starts script (or alt script if flag), raises quest `TriggerOn`. TriggerPlate::Update 0x39a668 compares valid contacts against required count (field 1844; mode 2 = num players), edge up: Activate, quest event, sound, "down" anim, script; edge down: sound, "up" anim, release script.
- Port kernels (`canonical_trigger_zone_v22.cpp:38-74`, `canonical_trigger_object_v28.cpp`) are faithful (Activate sets timer=delay at line 44). Live path is only the CLI diagnostic `OriginalCampaignRuntime::trigger_contact` (`original_campaign_runtime.cpp`), which is not driven by player/actor physics contacts in `main.cpp`.
- Divergences inside that diagnostic path (`original_campaign_runtime.cpp` trigger_contact): (a) delay is set only on the *enter* edge; the original sets `timer=delay` on every `Trigger::Activate`, including the move-out activation. (b) `script_all_player`, `script_all_player_move_out`, `effect_one_player`, `is_door_closed` hard-fail ("requires additional source provider"); 178 objects carry those fields.
- 194 TriggerZone, 43 TriggerObject, 136 TriggerZoneExitLevel (level exit; IDA 0x39c33c 820 bytes), 161 SpawnPoint (entry points: PlaceObject 0x3ea22c sets position, rotation and starts the `script` id = enterLocation_* script) in data; none run in the exe, so intro triggers, ambushes, level exits, enter-location scripts do not happen. Confidence high on missing; medium on divergence (a).

### F6. Traps not implemented beyond data (medium; crypt only)
- IDA TriggerTrap::Update 0x39eedc: ZoneEx::Update, for each victim in the contact set runs Lua `OnDamageRoll(victim)` (data/scripts/objects/trap.luac / swampking_trap.luac) or, with no script, `Character::F_GOAttack(this, victim, GetDamagerId (Arrays::TriggerTraps row +4), GetHurtChance (row +8))` then `F_ApplyResult`; TransferVictims 0x39f168 moves touching objects that pass `CanTrigger` (Lua `OnTrigger` returning false vetoes) into the victim set; Activate plays "activate" anim and sound. Animation event "damage"/callbacks via `__EventCallback` -> `OnAnimEvent`. TimerTrap/ProjectileTrap/ExplosiveTrap are summoned by script (`_SummonTimerTrap` 0x390c50, `_SummonTriggerTrap` 0x391670), with `SpecificUpdate` timers (0x39d688, 0x39cdc4).
- Port: `canonical_trigger_trap_v37.cpp:27` `update()` just forwards to an injected `whole_update` service that is not implemented anywhere (grep for `OnDamageRoll` finds no port code). Timer/Projectile/Explosive traps: no port equivalent found.
- Consequence: no trap damage/timing in the exe; authored TriggerTrap only in crypt (41). Confidence high (missing); damage numbers live in Arrays::TriggerTraps and compiled Lua, unverified.

### F7. SpawnGroupManager has no live caller (medium; no authored consumers found in campaign data)
- IDA `SpawnGroupManager::update` 0x3e9238: per group timer (ms) decremented by `GetDt`; at <=0 reset to group delay (+8), pick weighted entry (weights +8 summed, `GetRandom(sum)`, subtract loop), skip if entry count (+12) is zero; build candidate spot list (if group flag +4 set: exclude spots that are zonable AND in-zone (byte750) AND NOT enabled (byte752)); empty -> timer = 1000 ms and skip; else spawn `count` Characters, each at a uniformly random remaining spot (spot removed after use), named `Spawn_%05u`, `Character::InitSpawned(charId, spotPos)`, `SpawnSpot::PlaceObject`. No cap on living spawns in this function.
- Port `spawn_group_manager_v108.cpp:41-76` matches every step (timer before sum/roll, weight loop, 1000 ms retry, per-spot erase, name format, call order). Faithful.
- Data: no `SpawnSpot` gametype in any module .mgp (0 of 5759 objects), so campaign levels do not currently use it; likely used by procedural/hub levels. Windows: never instantiated (`actor_population.cpp:140` skips SpawnPoint). Severity medium/low; confidence medium that nothing in Act 1 needs it.

### F8. Spawn view radius / spawn_delay / skip_checkpoint population gating not in live path (unclear, medium)
- Authored: `spawn_view_radius` 241 objects, `spawn_delay` 120, `skip_checkpoint` 175, `ai_state` 2708. The properties are declared in the level-world kernels (`canonical_property_declarations_v1.inc`, `actor_initialization.cpp`) and `spawn_delay` is used in `windows-foundation/original_actor_lifecycle.cpp`, but `spawn_view_radius` and `skip_checkpoint` have no reader under `windows-foundation`. The consuming original functions (Character spawn / Level::CheckpointSave) are in other batches; I did not trace them. Needs confirmation from the character/level audits.

### F9. Culling and room activation not modelled (unclear)
- IDA `ObjectBase::TestCullingBeforeUpdate` 0x33de90 (object AABB vs 6 camera frustum planes, state byte134 0->1->2) and `RoomZone::Update/Activate/DeActivate` 0x396e9c/0x396d24/0x396918, `ObjectManager::UpdateRooms/AddRoomObjects` gate which objects update. The exe keeps all population actors enabled at once. Gameplay consequence (enemies in far rooms idle/not aggro) is plausible but I did not trace AI gating; marking unclear. Kernels exist (`canonical_room_zone_v3.cpp`, `room_zone_runtime_v104.hpp`, not wired).

### F10. Containers faithful but not live; one source quirk worth knowing (medium, documented I003)
- Destructible: IDA 0x3a0da0/0x3a11dc: breakable takes `animationFrameCount-3` hits (InitPost sets fields 444/445); each Interact decrements, plays staged frame `total-(remaining)` and a 3D sound; final hit raises quest `DestroyGameObject`, Container::Interact (loot, state 3/4), increments CharProperty 217 and unlocks trophy "destroy_200_breakables" at >199 for the local player. Port `canonical_destructible_container_v16.cpp` (`init_post`, `interact`, stage count at line `count>3 -> stages=remaining=count-3`) and `features/interactions/session_destructible_interaction_v1.cpp:134-135,264-267` match.
- Openable: IDA 0x3a1904/0x3a18bc/0x3a1860: unlocked if not locked; else needs key item with `qty <= stack` AND **`key_consume` flag set**, then `TryConsuming`; with key_consume=0 the chest can never be unlocked. Port `openable_container_owner_v1.cpp:111-118` reproduces this (`!f_.key_consume -> locked`). Faithful. Trophy "open_100_chests" at property 218 > 99.
- The exe does not instantiate either (only ground drops are live; `main.cpp:2314-2396` world drops). 1059 destructibles / 230 openables authored. Confidence high on missing; kernels not re-verified for Container::Interact/loot (items batch).

### F11. Interaction radius / nearby predicates (low, faithful where present)
- IDA GetInteractionRadius 0x38ad7c = min(width, depth of relative AABB)/2 (floats 81-86, x/y only); IsNearby 0x38ac0c/0x38b300 expands the absolute AABB by `r*100` on each axis; IsTouching is inclusive AABB overlap; GetInteractionSpot = "interaction_position" node else target position.
- Port: `level-world/character_target_providers.cpp:47` `dh2_gameobject_interaction_radius`, `world_click_target_v1.cpp:38-47`, `windows-foundation/features/interactions/original_interactions.cpp:in_interaction_range` (distance - melee radius - target radius <= padding (80) or 0 for type 8) and zone `touching` all follow the source. Not re-derived the AI padding constant (80); defers to Character batch.

## Mapping table

| IDA address | IDA function (or group) | port location(s) | status | note |
|---|---|---|---|---|
| 0x33dcc8-0x33dce8 etc | ObjectBase Is*/getters/ctors/dtors/Deserialize/Handle/ObjectHandle | level-world/canonical_gameobject_base_owner_v1.cpp | n/a | boilerplate |
| 0x33de90 | ObjectBase::TestCullingBeforeUpdate | level-world canonical_object_update_v102.cpp (not wired) | unclear | F9 |
| 0x33e61c/0x33e6d4 | ObjectBase::TestDisable/EnableCondition | main.cpp:1099-1105 (static CLI sets) | divergent | F2 |
| 0x33ec0c | ObjectBase::InitPost | level-world/game_object_initialization_owner_v1.cpp | faithful (kernel) | min_difficulty all "Invalid" in data |
| 0x33f014/0x33f15c | ObjectBase DeclareProperties / ctors | canonical_property_declarations_v1.inc | faithful | property names match (activate_cond, deactivate_cond, spawn_prob...) |
| 0x340274-0x340be0 | ObjectManager network (DoRemoteUpdate, IsPacketValid, AssignObjectNetworkId, LoadGameObjectNetData, sRead/WritePacketData, AskNetResend, Process*Packet, IsRemotePlayerOfMember, CanSendUpdate, NetworkInitLevel...) | none | n/a | multiplayer only |
| 0x34163c/0x3427a0/0x345fbc/0x345954 | ObjectManager UpdateRooms/AddRoomObjects/DelRoomObjects/HandleNoRoomObjects | canonical_object_manager_v1.cpp, room_zone_runtime_v104.hpp (not wired) | unclear | F9 |
| 0x3419a8/0x347100 | ObjectManager fog colour (GetDynamicFogColorAtPos, InitModulesFogColor) | not found in windows-foundation | unclear | visual only; not traced |
| 0x3432f8/0x3496b8/0x348ea4/0x349240/0x34a620 | ObjectManager MarkForDeletion/Flush/Remove/FakeRemove/Update | level-world canonical_object_update_v102.cpp, canonical_object_fake_remove_v106.cpp | partial | kernels only; exe uses its own actor registry |
| 0x34b270-0x34b868 | ObjectManager Add/GetNewObject/Spawn/LoadFromXML/GetObjectByName/ByType | level-world canonical_object_factory_v1.cpp; windows source_world_objects.cpp, actor_definitions.cpp | partial | exe loads only Character/Decor-class definitions, named lookup in source_world_objects.cpp (faithful ambiguity rejection) |
| 0x34bc30-0x34c7f4 | PhysicalWorld ctor/update/clear/load/ShouldCollide/contact callbacks | physics-backend, windows features/physics/runtime_session_contact_v1.cpp | partial | contact filtering outside this audit's depth; ShouldCollide filter masks not compared |
| 0x3884fc-0x389938 | ColBox/Floor/Decor/AnimatedDecor InitPost/LoadFloorMap/props | level-world canonical_decor_v15.cpp, canonical_animated_decor_v23.cpp, source_module_floors.cpp | faithful (kernel) | floors live via SourceModuleFloors |
| 0x38aac8-0x38b6d4 | GameObject AABB/IsNearby/IsTouching/IsInteractiveEx/GetInteractionRadius/Spot/IsPlayerNearby/GetNumPlayerTouching | character_target_providers.cpp:47, world_click_target_v1.cpp, original_interactions.cpp | faithful | F11 |
| 0x38ae2c | GameObject::UpdateIdleSound | engine-audio (idle_sound) | unclear | audio, not traced |
| 0x38bd64 | GameObject::CheckSpawnProbability | game_object_spawn_probability_v1.cpp (kernel) vs main.cpp:1107 | divergent | F1 |
| 0x38be5c/0x38cd48 | GameObject::InitPost/InitFinal (scale/rotation, AABB, visual load) | game_object_initialization_owner_v1.cpp, actor_initialization.cpp | faithful (kernel) | |
| 0x38c600-0x38c790 | Zoning Enable/Disable/Entered/Exited | zone_startup_v76.cpp | partial | not wired |
| 0x38cbe8/0x393710-0x3949b0 | GameObject Update/rotation/path/subobjects/collisions | windows actor_movement.cpp, original_actor_*.cpp | partial | belongs to character/movement batches |
| 0x38d7ec-0x393444 | GameObject Lua bindings (_TargetList*, _DealDamages, _Summon*, _IsInRange, _PlayFX...) | script-runtime, level-loader/script_command_receivers_v59.cpp | unclear | script-runtime batch; not audited here |
| 0x39594c/0x395f04 | CheckpointZone ctor/OnCollisionBegins | zone_collision_runtime_v83.cpp:50-100 | missing (kernel faithful) | F3 |
| 0x3964cc-0x396e9c | RoomZone HasInside/Add/Remove/Activate/DeActivate/Update | canonical_room_zone_v3.cpp | missing | F9 |
| 0x3970c8 | Zone::IsInside | zone_collision_runtime_v83.cpp:20-40 | faithful (kernel) | unused XY vector/radius reproduced as dead code; colzone mesh branch needs ray provider |
| 0x397594-0x398200 | Zone Init/props, ZoneEx Update/collision begin/end | zone_ex_contacts_v90.hpp, canonical_zone_physical_v82.cpp | faithful (kernel) / missing live | |
| 0x398838-0x398fd4 | Trigger Update/props/net | canonical_trigger_zone_v22.cpp | faithful (kernel) | net portions n/a |
| 0x3993bc-0x399ff8 | TriggerObject TestInteractiveCond/Update/Interact/InitPost | canonical_trigger_object_v28.cpp | missing live | F5 |
| 0x39a510-0x39b0bc | TriggerPlate IsObjectActivating/Update/InitPost | none found | missing | F5; no TriggerPlate in data |
| 0x39b2a0-0x39c0a8 | TriggerZone SafeStartScriptOnlyOnce/Update/Marker/InitPost | canonical_trigger_zone_v22.cpp; original_campaign_runtime.cpp trigger_contact (diag) | partial | F5 divergence (a), (b) |
| 0x39c33c-0x39ca40 | TriggerZoneExitLevel Update/props | canonical_exit_zone_v29.cpp | missing live | F5 |
| 0x39cb2c-0x39d904 | ProjectileTrap, TimerTrap | none | missing | F6 |
| 0x39dbc0-0x39f168 | TriggerTrap (Activate, Update, TransferVictims, CanTrigger, GetDamagerId/HurtChance) | canonical_trigger_trap_v37.cpp (stub update) | missing | F6 |
| 0x39fc2c/0x3b4010 | PODecor / POCharacter ctor clones | canonical_podecor_body_v49.cpp, original_actor_physical.* | n/a | proxy ctors |
| 0x3a0da0-0x3a1508 | DestructibleContainer | canonical_destructible_container_v16.cpp; session_destructible_interaction_v1.cpp | faithful (kernel) / missing live | F10 |
| 0x3a177c-0x3a1e8c | OpenableContainer | openable_container_owner_v1.cpp, openable_container_interaction_v2.cpp | faithful (kernel) / missing live | F10 |
| 0x3e74c8-0x3e8990 | Door | canonical_door_v27.cpp; source_door_commands.cpp | missing live | F4 |
| 0x3e8b3c-0x3ea730 | SpawnGroupManager (update, Ins/DelSpawn, GetGroup, GetInstance) | spawn_group_manager_v108.cpp | faithful (kernel) / missing live | F7 |
| 0x3ea22c-0x3eaa98 | SpawnPoint (PlaceObject, props) / SpawnSpot (InitFinal, props) | canonical_spawn_point_v15.cpp, canonical_spawn_spot_v108.cpp; actor_population.cpp:140 skips | missing live | F5/F7 |
| 0x3ee468-0x3eefec | LiftableObject Pickup/Drop/Release/Update | none | missing | 0 authored LiftableObject in data; low |
| 0x412098-0x413240 | Dropable/Dragable (menu drag UI) | engine-ui | n/a | UI, not world |
| 0x46e6bc-0x46f754 | PhysicalObject (filters, pin, velocity, ctor, draw) | physics-backend, original_actor_collision_filter.cpp | partial | not compared in depth |
| 0x46fb4c-0x470840 | POCharacter/POItem/POZone collision tests | runtime_session_contact_v1.cpp, original_trigger_contacts.cpp | partial | POItem contact -> pickup in main.cpp:3351 (B063); POZone not wired |
| 0x4770d0-0x478520 | AnchorBase/AnchorForward/AnchorGroup (camera anchors) | camera_anchor_owner_v75.cpp, original_actor_camera_anchor.cpp | faithful (per prior camera audit; not re-verified) | camera batch |
| 0x4a4b70-0x4bbd50 | Arrays::<table>::finalize/readNames/read | game-data readers, level-loader/game_object_arrays_owner_v81.cpp | n/a | table deserialization |
| 0x4ed35c-0x4ff1cc | Structs::{TriggerPlate,TriggerTrap,TriggerObject,TimerTrap,ProjectileTrap,OpenableContainer,Door,DestructibleContainer,LiftableObject}::read | level-world/*_data_v16.cpp, openable_container_data_connection_v11.cpp | faithful (assumed) | field layouts not byte-compared; Door/Destructible/Openable connections exist |
| 0x817dbc-0x819094 | CRoomAttributes (online room attributes) | none | n/a | multiplayer |
