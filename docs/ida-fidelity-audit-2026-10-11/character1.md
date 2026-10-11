# character1 audit: Character:: methods 0x3a2e1c-0x3abe98 (145 functions)

## Progress
- last_address_done: 0x3abe98 (whole batch triaged; deep reads on HitFor, Kill, Revive, Update, UpdateObjectOfInterest, UpdateStateFX, GetRespawnDelay/CanRespawn, INV_DoesMeetRequirements, INV_TransmuteItem, EquipAllSlotsAuto, GetAnimStance, GetRotationSpeed, combo/attack-delay)
- groups_done: gameplay set listed above; trivial getters/ctors/dtors/thunks/net/serialize grouped in the table
- next: optional deeper pass on Draw 0x3a84f4, INV_UpdateSkin 0x3a999c, GetInteractionType/IsInteractive consumers, HitFor integer-vs-float damage rounding, Interact merchant stock fill

## Method note (important context)
The Windows executable (`port/windows-foundation/CMakeLists.txt`) links only a subset of `port/level-world`. Many very literal
IDA transliterations (level-world/character_kill*.cpp, character_death_owner_v56, character_revive_owner_v1, character_deferred_queue,
character_update_startup, character_hit_player_v116, character_hit_player_tail_v7, canonical_character_death_v84, and
android-native/.../source_campaign_character_frame_v111.cpp) are NOT linked into the Windows build (grep of
windows-foundation/CMakeLists.txt and features/*/CMakeLists.txt; only character_hit_v6.cpp, character_skill_application_v6.cpp,
character_combat_result_v6.cpp etc. are linked). Windows melee damage goes through combat_system.cpp / actor_combat_runtime.cpp /
actor_state.cpp, and deaths go through features/loot/runtime_death_rewards_v1.cpp. Findings judge the Windows-linked behavior; where logic
only exists in unlinked code I say so.

## Summary
Status counts (grouped rows counted once): faithful 11, partial 7, divergent 1, missing 5, unclear 5, n/a about 16 groups, intentional-adaptation 0.

Top risks:
1. Character::Kill side effects missing in Windows: quest kill events (KillXEnemies/ClearEnemies/KillEnemyTemplate/ClearEnemyTemplate), aggro-list kill credit (KillCount props 23/24), player death counter (prop 25), kill/death trophies.
2. HitFor tail missing: low-HP warning SFX, cinematic_Tuto_potionUse tutorial trigger (HP<=50%, difficulty 0), power_50/100/150/200dam and power_onehit trophies, damage suppression when the main player is dead.
3. UpdateObjectOfInterest simplified to "nearest eligible enemy"; original scans a 2pi arc with the frontal-sort priority queue and includes interactables, and records an OOI type.
4. Respawn (GetRespawnDelay/CanRespawn/Revive) has no Windows implementation; lifecycle hard-fails if a respawning actor enters Limbus.
5. UpdateStateFX (stun/fear/slow status VFX) is not present in the Windows build (only android-native).

## Findings

### F1 (high, confidence medium) Kill: quest kill-objective events, kill credit and death counters not produced
- IDA Character::Kill 0x3a5b18. Non-player victim: always raises async quest events via PyDataConstants "v2QuestObjectiveType" "KillXEnemies" and "ClearEnemies" (victim type = short at +0x13c4 (idx 2532), object id +0x64); when template id (short idx 2533) != -1 also "KillEnemyTemplate" and "ClearEnemyTemplate". Iterates the AI aggro list: RaiseEvent(owner,4,victim), clears aggressor's target if it was the victim, PROPS_AddInt(23,1) KillCount and (24,1) MPLevelKillCount, trophies killed_100/500/1000/2000 for local player; DistributeXP if the killer is in the list. Player victim (a3==0): PROPS_AddInt(25,1) death count, trophies quest_died_10/50/100_times, returns before loot/XP.
- Port: Windows has only features/loot/runtime_death_rewards_v1.cpp:300-452 (DropLoot table + distribute_xp at :238; players skipped at :331). No producer of the four quest events, no prop 23/24/25 increments, no trophies in Windows-linked code (grep for KillXEnemies/ClearEnemies/killed_100 hits only port/level-world/character_kill.cpp and its tests, which are not linked).
- Consequence: quest objectives of kill/clear types cannot progress from kills; kill/death counters (menu stats, trophies) stay 0. TalkToNPC objective delivery does exist (features/quests/npc_talk_objective_integration_test.cpp).
- Confidence: medium (absence by grep across Windows-linked sources; recommend a runtime test killing a quest target).
- Fix: in the death consumer raise the four objective events with victim type/template ids, apply the aggro-list credit and the player death counter/trophies.

### F2 (high, confidence medium) HitFor tail: low-HP sound, potion tutorial, damage trophies, dead-player suppression
- IDA Character::HitFor 0x3a8bc4. After HP change, if player and HP <= MaxHP/2 (int division): offline, difficulty 0, flag byte (dword_99F778+45) set -> StartScript "cinematic_Tuto_potionUse" and clear flag; if +0x1448 (armed) -> clear and play "sfx_mc_low_hp" (self is main player) or "sfx_friend_low_hp". Else if player, not armed and HP >= 0.75*MaxHP (float) -> re-arm. Trophies by (damage>>8): >49 power_50dam, >99 power_100dam, >149 power_150dam, >199 power_200dam (cascading); power_onehit if victim was at full HP, died, and boss/miniboss. Damage is only subtracted if a main player exists and is not dead (offline).
- Port: Windows melee path combat_system.cpp:172-210 and actor_state.cpp:76-86 only clamp and subtract health; no sound/tutorial/trophy/dead-main-player gate. Faithful versions exist in level-world/character_hit_player_tail_v7.cpp and character_hit_player_v116.cpp (not linked); character_hit_v6.cpp:35-104 (linked) is the nonplayer branch only.
- Consequence: no low-HP warning cue, no first-time potion tutorial, no power trophies; while the player is dead other actors can still damage each other.
- Fix: route Windows damage through (or emulate) the v116 player tail; thresholds use raw 8.8 HP.

### F3 (medium, confidence medium) UpdateObjectOfInterest reduced to nearest eligible enemy
- IDA 0x3abb9c: every 500 ms (short +0x14aa reset 500), TargetList(characterFlags 89, objectType 1) searched at GetTargetPosition, radius CharacterDesign.OOI_Distance, angle 0x40C90FDB (2pi), ordered by ObjectSearcher::TargetSorter::_sortFrontal (entries with flag bit0 first, then by float at +8). First entry becomes the OOI; type from virtual +144 (8 enemy, 3 interact, 1 pickup) stored at +0x14a8; a type-1 pickup is kept only when owned by this character.
- Port: features/combat/object_of_interest_owner_v1.hpp:6-40, object_of_interest_world_v1.cpp:12-29: nearest eligible enemy by XY distance, tie by lower id. 200 and 500 ms constants correct (:23-26). No frontal sort, no non-enemy objects, no OOI type.
- Consequence: target marker/auto-target priority differs with multiple enemies nearby; no OOI type for action icon. NPC/chest interaction handled by other features so interaction works, but priority may differ. Confidence medium (sorter score field meaning not fully decoded).
- Fix: port TargetList::Search + _sortFrontal ordering and OOI type output.

### F4 (medium, confidence high that it is absent) Monster respawn not bound in Windows
- IDA GetRespawnDelay 0x3a4bac: byte@5249 ? 0 : 1000*(prop11>>8). CanRespawn 0x3a5248: false if byte@5249 or prop11<=0, else GroupInfo::CanRespawn (or true). CanUpdate 0x3a52a4 keeps dead characters updating only while CanRespawn.
- Port: windows-foundation/original_actor_lifecycle.cpp:45 returns error "Original Limbus respawn timer/network owner not bound" when can_respawn && respawn_delay_ms>0 on Limbus entry. Formula logic exists in level-world/character_ai_groups_v87.cpp:129 and android-native, not in Windows.
- Consequence: authored respawning spawners fail or never respawn. Act1 impact unknown (RespawnTime prop 11).
- Fix: bind a respawn timer (1000*(prop11>>8) ms), GroupInfo::CanRespawn, Revive.

### F5 (medium, confidence low) Revive 0x3a59ac not verified in Windows
- IDA: if dead raise event 3; low-HP armed byte=1; dead=0; _InitHpMp (full HP/MP); clear +280; online resets; optional InitPhysicalObject; if player, set position to floor height at current XY; UpdateAllSkills.
- Port: level-world/character_revive_owner_v1.cpp (unlinked); Windows relies on lifecycle "revive" effect callback (original_actor_lifecycle.cpp:32,37). unclear whether main.cpp provider restores full HP/MP. Missing evidence: the OriginalLifecycle services implementation for operation revive.

### F6 (low, confidence high) UpdateStateFX status VFX absent on Windows
- IDA 0x3a4470, every 8th frame: priority stun (kind 4, needs flag 0x800, DesignSettings fx +128), scared (kind 3, flag 0x400, +116), slowed (kind 1 when PROPS_GetWalkSpeed < 1.0, +124); grabs/drops looping AnimatedFX.
- Port: only android-native/.../source_campaign_character_frame_v111.cpp:83-94. Windows features/status_effects has no state-FX attach. Cosmetic.

### F7 (low, confidence medium) HitFor unit handling
- IDA subtracts a2 (raw 8.8 int) from HP raw (prop 36), kills at HP<=0. Port actor_state.cpp:76 uses float health (raw/256 via combat_session.cpp:1341) and float damage; sub-1/256 fractions may differ. Clamp-to-zero equivalent. Whether float damage is floored to raw first is unclear (see character_combat_result_v6).

### F8 (low, confidence medium) AI script concurrency cap
- IDA Update 0x3abe98: offline non-boss monsters load AI scripts into the s_concurrentAI map; if it holds >8 entries (>24 in level 29) the oldest is killed and unloaded first.
- Port: faithful in level-world/character_deferred_queue.cpp:95-99 (24/8 correct) but not linked into Windows. Windows enemy AI is features/enemy_ai. Gameplay impact unclear.

## Mapping table
| IDA address | IDA function / group | port location(s) | status | note |
|---|---|---|---|---|
| 0x3a8bc4 | Character::HitFor | level-world/character_hit_v6.cpp:35-104 (nonplayer, linked), character_hit_player_v116.cpp (unlinked); windows combat_system.cpp:172-210, actor_state.cpp:76-86 | partial | HP subtraction+kill faithful; tail/trophies/SFX/dead-player gate missing (F2, F7) |
| 0x3a5b18 | Character::Kill | features/loot/runtime_death_rewards_v1.cpp:300-452; level-world/character_kill*.cpp (unlinked) | partial | loot+XP only (F1) |
| 0x3a59ac | Character::Revive | level-world/character_revive_owner_v1.cpp (unlinked); original_actor_lifecycle.cpp:32,37 | unclear | F5 |
| 0x3a5ae4 | DropLoot | features/loot/runtime_death_rewards_v1.cpp:376-447 | faithful | killer prop 195 value bonus used; not audited deep |
| 0x3a4bac, 0x3a5248 | GetRespawnDelay, CanRespawn | original_actor_lifecycle.cpp:45 | missing | F4 |
| 0x3a52a4 | CanUpdate | actor frame (not traced) | unclear | culling/dead-update gate |
| 0x3abe98 | Character::Update | per-frame runtime (windows); level-world/character_update_*.cpp unlinked | partial | order timers/AI/SM/animator fine; F8; debug switches n/a; scrolling combat text gold/xp not traced |
| 0x3abb9c | UpdateObjectOfInterest | features/combat/object_of_interest_*.cpp | divergent | F3 |
| 0x3a469c | UpdateSpotTarget | not found in Windows | missing | ground spot target FX, cosmetic |
| 0x3a4470 | UpdateStateFX | android-native only | missing | F6 |
| 0x3a40b0, 0x3a40e4, 0x3a5974, 0x3a598c | DisableSelfFX/StateFX, Disabled/Enabled | level-world/canonical_character_death_v84.cpp:112; lifecycle set_enabled | n/a | FX teardown |
| 0x3a4930 | INV_DoesMeetRequirements | level-world/player_equipment_render_owner_v1.cpp:91 (props 19,149-152 base only) | faithful | matches IDA (no Prereq bonus); online/remote bypass kept |
| 0x3a9d10 | INV_CheckItemsRequirements | player_equipment_render_owner_v1.cpp:92-93 | faithful | unequip failing slots, recalc, recurse, skin, ValidateHPMP |
| (context) ItemInstance::IsEquippableBy 0x3fa330 | equip-time check | features/equipment/equipment_adapter.cpp:23-50 | faithful | base+Prereq >= req<<8, class by Item[34] |
| 0x3a4a3c, 0x3a4bec | INV_TransmuteItem x2 | engine-ui/character_menu_item_actions_v1.cpp:16-42 | faithful | mult*((v<<8)*(prop197+256)>>8)>>16 min 1; qty>1 decrement else remove; gold; prop213 +1; gear_transmute at 300 |
| 0x3a4c14 | EquipAllSlotsAuto | features/equipment/equipment_adapter.cpp:76-81 (auto_all) | unclear | order (unequip all, equip n-1..0, refill except slots 1,2) not verified in body |
| 0x3a9ee8, 0x3a9eb0, 0x3a9f30, 0x3a9f6c, 0x3a9fa8 (+thunks) | EquipSlotAuto/UnEquip*/Equip* | equipment_adapter.cpp:76-81 | partial | IDA EquipItemAuto skips INV_CheckItemsRequirements; not verified |
| 0x3a999c | INV_UpdateSkin | features/equipment visual | n/a | visual |
| 0x3a53e0 | GetAnimStance | level-world/character_stance.cpp:2-14 | faithful | staff 3, bow 4, dual 2, two-hand 1, no main-hand 5; >=count -> 0 |
| 0x3a372c | GetRotationSpeed | level-world/move_state.cpp:17-20 | faithful | flag 0x20 => -1.0 |
| 0x3a3438 | GetAttackDelay | windows combat_session.cpp:1382-1383 | faithful | AITable attack_delay ms -> cooldown |
| 0x3a346c, 0x3a34f0 | HasComboAttack, GetNumComboAttack | level-world/character_combat_queries.cpp:25-40; combat_session.cpp:441 | partial | HasCombo faithful; NumCombo (AnimTable +8) usage not verified |
| 0x3a4cd0, 0x3a4d3c (+thunks) | CanRangeAttack x2 | character_combat_queries.cpp:20-24; combat_session.cpp:441-445 | faithful | range item: weapon-range agent. prop32 != -1 => ranged, min prop30>>8, max prop31>>8 |
| 0x3a47e8, 0x3a4870 | GetInteractionType, IsInteractive | features/interactions/original_interactions.cpp | unclear | enemy 8 / NPC 3 rule not traced |
| 0x3a4d78 | Interact | features/interactions/npc_interact.cpp | partial | TalkToNPC event; merchant stock fill from MerchantTable, merchant/cleaner menus not traced |
| 0x3a5184 | GetImmunityTimer | features/status_effects | unclear | lazy per-damage-type TimerUtil |
| 0x3a2e1c-0x3a3064, 0x3a30ac-0x3a3254, 0x3a3294-0x3a3438, 0x3a3724, 0x3a374c, 0x3a3754 | trivial predicates/getters (Is*, GetChar*Id, GetFXFootprint/Blood, opacity, interaction radius, obstacle) | game-data tables | n/a | one-line getters |
| 0x3a2e9c | GetFXScale | FX_Scale*0.01 | unclear | not traced |
| 0x3a4398 | SetRelativeAABB | windows original_actor_bounds.cpp:50 | partial | Collision_Scale*0.01 per axis assumed |
| 0x3a36e4 | IsZonable | - | n/a | zone condition |
| 0x3a3fe4 | CheckForAllQuestsTrophy | none in Windows | missing | trophy only |
| 0x3a4068, 0x3a41a0 | Add/RemoveMultiplayerHighlight | - | n/a | multiplayer |
| 0x3a4344 | UpdateAIPointers | level-world/character_update_pointers_v105.cpp (unlinked) | n/a | |
| 0x3a5784-0x3a58f4 | GetPreSetAIState, foot positions, SetInitialPosition | original_actor_lifecycle.cpp:15-32 | faithful | PreSpawn/Limbus preset handled |
| 0x3a54b4, 0x3a54d4, 0x3a56a4 | UniqueAnimSetId/ModelName/ClassName | data lookups | n/a | |
| 0x3a9db4 | ReloadSkills | level-world/character_menu_live_reload_connection_v1.cpp:24 | partial | recalc path; spec-time menu callback not traced |
| 0x3a5990 | HasPotion | features/inventory | n/a | trivial |
| 0x3a2f48, 0x3ab674 | Has/AddCharOIDToCache | none | n/a | multiplayer net-ID cache |
| 0x3a6208-0x3a7f10 | Serialize/Deserialize/NetStruct/PopulateOutgoing/InterpretIncoming | net/save | n/a | |
| 0x3a66f8 | Clean | level-world/character_clean_v123.hpp | n/a | teardown |
| 0x3a799c, 0x3a7b24 | ClearConcurrentAI, UnLoadScriptProcess | level-world/character_deferred_queue.cpp (unlinked) | partial | F8 |
| 0x3a7c3c-0x3a7dfc | ~Character + thunks | - | n/a | dtors |
| 0x3a9340, 0x3aa1b4 | Character ctors | actor construction | n/a | |
| 0x3a84f4 | Character::Draw | renderer | n/a | not audited |
| 0x3a9fe4 (+thunk 0x3a9fdc) | DeclareProperties | - | n/a | |
| 0x3a4d5c | RaiseEvent | - | n/a | event dispatch |
