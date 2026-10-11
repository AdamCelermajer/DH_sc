# Residual triage: functions NOT matched by partition.py (2026-10-11)

## Progress
last_address_done: n/a (family-level triage, complete)
groups_done: enumeration (residual_enum.py), categorisation (residual_categorize.py), pseudocode spot checks of flagged families, port greps
next: none for this task; the "Needs audit" list below is the hand-off. The flagged functions themselves are triaged, not audited.

Method: `py -I residual_enum.py <functions.jsonl> partition.py fam.json` re-applies partition.py's exact class/free-fn regexes,
then re-keys the unmatched functions by normalized family (return types and template args stripped);
`py -I residual_categorize.py fam.json N` buckets them. 31,725 functions total, 4,702 matched, 27,023 residual
(scripts live next to this file).

## Partition leaks found (gameplay code that partition.py misses)
1. Return-type or template prefix hides the class: `void CharAI::SetScript<AIS*>` (6), `void PropertyMap::AddProperty<T>` (10),
   `ObjectiveTemplate_InteractWith/KillCharacter/KillEnemies<...>` (92 fns, 0x47a898-0x47f634),
   `int Objective_EventReceiver::HasEnemyOfTypeLoaded<>` (2). partition only anchors `^Class::`.
2. Free functions with no `::` fall through unless they match the narrow `F` regexes: all combat formulas `CF_*`, `GetLevelScaledXP`,
   and most `Native*` (only Inv/Item/Merchant/Equip/Loot/Gold/Sell/Buy/Quest/Dialog/Stat/Skill/Faery/Char/Attr/Touch/Interact are caught).
3. Struct names not in the lists (`Structs::Skill`, `Loot`, `WeaponRef`, ...): read/finalize only, data schema.

## Category table
| Category | Fns | Bytes | Verdict |
|---|---:|---:|---|
| G0 combat formulas: `CF_SetCombatants`, `CF__CalcDamage/Crit/Block/MissOrDodge/Stun/Slow/Fear/Push/Hurt`, `CF_CalcDotDamage` | 11 | 4,328 | GAMEPLAY, needs audit (highest risk) |
| G1 input: v2Controller/Controllable/MixedController/GamepadController/HudController/EmuController, Gamepad, InputManager, TouchScreen*, Accelerometer, Ev* | 275 | 30,544 | mixed. v2Controller::Cmd_* are gating plus net-forward wrappers (needs audit, small). v2GamepadController::Update (0x406c2c, 7360 B) has no port equivalent (adaptation candidate). Rest n/a |
| G2 HUD/zoom/minimap: HUDControls, HUDMinimap, InfoHUDManager, ZoomHandler, MenuMinimap, MenuDebugHUD, PlayerLightTweaker | 124 | 35,716 | display/layout only. Read InfoHUDManager::FastUpdate, HUDControls::OnEvent, ZoomHandler::onEvent: no gameplay state mutation. n/a |
| G3 gameswf `Native*` callbacks not caught by partition F regex | 154 | 67,824 | partly gameplay: see A4-A9. Remainder (UI, online, IPod, leaderboard, save-slot lists, strings) n/a |
| G4 quest objective templates, `Objective_EventReceiver`, `GetObjectiveImplInstance`, `GetRewardImplInstance<Reward_*>`, `GetQuestFunctor`, `Script_CharacterState_Callback`, `IsNonDebugQuest*`, `GetDialogMsgListById`, `ParsePlayerName` | 119 | 13,236 | GAMEPLAY, needs audit |
| G5 factories `GetNewState<CS*>`, `GetNewInstance<ObjectBase>`, `CharAI::SetScript<AIS*>` | 70 | 4,320 | small: id-to-class mapping and which AI script drives player/monster/faery. Fold into ai/states batches |
| G6 `PropertyMap::AddProperty<T>` | 10 | 1,336 | property registry; defaults come from (batched) callers. Low |
| G7 free fns: `GetLevelScaledXP` 0x3bd918, `TrophyCheckArmorSet*` x9, `playerClassAs*`, `IsPlayableClassID`, `UpdateSavedValues`, `AssignSaveSlotToPlayer`, `TempHackScriptIDAreStrings` | 16 | 3,136 | GetLevelScaledXP needs audit; trophy checks achievement-only; rest low |
| G8 TrophyManager/TrophyData/AchievementMsg/GameCenter | 54 | 6,888 | achievement popup + save only. n/a |
| G9 ScriptCmd leaf structs (97 script commands: ctor/read/finalize/`GetNewScriptCmdDataInstance`) | 495 | 34,512 | data schema/boilerplate; executors live in matched Script_*/ScriptManager. n/a (verify cmd-id table only) |
| D1 data schema `Structs::*::read/finalize`, `Arrays::*::GetMemberIDByString` | 1,158 | 147,972 | deserialisers; port reads recovered data. Low/n/a |
| N1 network/multiplayer/online/social/Bluetooth/GLX/QE_* net msgs/CMsg* | 2,135 | 330,556 | n/a (multiplayer). Note CMsgControllerAction: the original mirrors controller commands as net messages, see A5 |
| E1 glitch engine (scene, video, material, animators, nodes) | 8,554 | 1,444,668 | n/a |
| E2 std/STL template instantiations | 3,498 | 611,314 | n/a |
| E3 gameswf/RenderFX/GameSWFUtils/FSCommand tokenizer | 2,483 | 522,508 | n/a (runtime). FSCommand handlers are the Native* free fns, see G3 |
| E4 vox audio, SoundEmitter | 1,079 | 157,988 | n/a |
| E5 boost | 55 | 5,564 | n/a |
| E6 Box2D (b2*) | 318 | 97,068 | handled: port/physics-backend vendors official Box2D 2.0.1 (63 of 65 files byte-identical; README documents the only two edits: b2_maxProxies 2048 / pairs 16384 and allocator header; `b2_version` {2,0,1} checked against the ELF; ARM64 differential tests). Residual risk is caller-side settings (iterations, gravity, filters) in the matched PhysicalWorld/PO* batch. n/a |
| E7 Lua core + parser/libs | 294 | 46,548 | n/a (LuaScript/LuaManager are in the quests batch) |
| E8 freetype/png/jpeg/zlib/vorbis/TinyXML | 1,497 | 476,396 | n/a |
| E9 libc/GL stubs, global ctors (`global constructor keyed to`), `__tcf_N` static dtors, `sub_` | 1,670 | 187,554 | n/a |
| E10 single-function lib/engine free fns (ft, jpeg, mpc, glu, lua parser, zlib, ...) | 1,039 | 256,012 | n/a |
| X1 PyDataArrays (table registry loader 0x4aa21c), VisualFXManager, SimpleTypeProperty, sfc, misc | 303 | 72,812 | VisualFXManager is anim-FX playback only (damage is CF__CalcDamage). PyDataArrays::Load is the table-file list. n/a |
| P1 platform/core glue: Application, StringManager, Console, DebugSwitches, FileSystem, Menu*/GS* states, MenuManager, OnScreenKeyboard, Device, scene nodes | 1,483 | 263,064 | n/a (MenuCharacterSelect is UI) |
| Unclassified residue (anon-namespace glitch, new/delete, tiny interfaces) | 129 | 80,780 | n/a |
| **Total** | **27,023** | **4,902,644** | |

The presumption holds for about 26,800 functions (~4.78 MB). It is false for roughly 230 functions (G0 plus parts of G1/G3/G4/G5/G7),
most importantly the combat-formula layer, which sits between three matched Character functions
(`F_GOAttack` 0x3b061c, `F_ApplyResult` 0x3b10b4, `_F_CalculateResult` 0x3b2638).

## Needs audit (ordered by risk)
| # | Address / name | Size | Why gameplay | Where the port handles it (grep evidence) |
|---|---|---:|---|---|
| A1 | `CF__CalcDamage` 0x3b1fb8 (1652 B), `CF__CalcMissOrDodge` 0x3b1df0, `CF__CalcCrit` 0x3b0868, `CF__CalcBlock` 0x3b0918, `CF_CalcDotDamage` 0x3b09c4, `CF__CalcSlow` 0x3b0ad4, `CF__CalcFear` 0x3b0c64, `CF__CalcPush` 0x3b0d78, `CF__CalcHurt` 0x3b0e8c, `CF__CalcStun` 0x3b0fa0, `CF_SetCombatants` 0x3b059c | 4.3 KB | Core damage, hit, crit, block and status-chance math. Not in any batch; the character batches will only see calls into it | port/level-world/character_combat_result_v6.cpp (miss_dodge; crit roll threshold 0x6200; status(); mask bits), character_dot_attack.cpp/.hpp (cites CF__CalcDamage 0x3b1fb8 and CF_SetCombatants), reference/character-dot-attack/NOTES.md. No port file cites CalcCrit/Block/Slow/Fear/Push/Hurt/Stun/MissOrDodge by name or address, and docs/act1-ida-audit has no CF_ hit. Must be verified function by function against v6 |
| A2 | `GetLevelScaledXP` 0x3bd918 | 328 B | XP scaled by level difference: DesignSettingsTable +140/+144/+148/+152/+156 (per-level slopes, min/max percent, diff clamp), `(int)slope*diff+100` clamped, /100 | port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp:260 `progression_scaled_xp_v1`; port/level-world/tests/player_progression_v1_original.py (ARM instruction replica of GetLevelScaledXP). Probably faithful; confirm field offsets and int truncation |
| A3 | `ObjectiveTemplate_KillCharacter/KillEnemies/InteractWith<...>` handleEvent/Compile (92 fns, 0x47a898-0x47f634), `Objective_EventReceiver::HasEnemyOfTypeLoaded<>` 0x47c7f4/0x47cc20, `GetObjectiveImplInstance<>` x13, `GetRewardImplInstance<Reward_XP/Gold/Loot/ConsumeLoot/CharacterProps>` 0x482d80-0x482ef0, `GetQuestFunctor` 0x440080, `Script_CharacterState_Callback` 0x4594a8, `IsNonDebugQuest*` 0x43b4dc-0x43b53c | 13 KB | Quest completion counting (kill/clear by template vs prop id, enemy-of-type-loaded gating), reward kinds, which quests are shown | port/level-world/native_quest_runtime_v76.cpp/.hpp ("thirteen authored objective types", RewardList stop-on-false per features/quests/CONTRACT.md); features/quests/source_quest_service_binding.cpp (GetQuestFunctor). No name hits for HasEnemyOfTypeLoaded, IsNonDebugQuest, ObjectiveTemplate |
| A4 | HUD input Native*: `NativeUsePotion` 0x43d2c8, `NativeHUDSkill` 0x43e734, `NativeHUDSpell` 0x43cabc, `NativeMoveCharacter` 0x43e830, `NativeHUDInteract` 0x43afb8, `NativePinchZoom` 0x43d6f8, `NativeLockZoom/UnlockZoom/SetLockZoom` | ~1.3 KB | Input-to-action mapping. Potion needs `HasPotion` and (hp% or mp% below full) then `Cmd_UsePotion` and "PotionDrink" sound. Skill buttons: `CTRLIsAllowed`, `SG_GetSkillInSlot(slot)` (-1 ignored), BeginSkill then EndSkill in the same call. Spell: BeginCast+EndCast. Joystick: angle to direction (1,-1) normalized rotated by (90-angle), `Cmd_HeadTowards` | PC rebinding: features/platform_input/semantic_input.hpp (Control enum joystick/attack/skill1-3/spell/potion, keys 1-3, 4, 5), live_dispatch.cpp, features/inventory/runtime_session_potion_use_v1.cpp, features/generic_skills/pc_skill_input_binding_v1_tests.cpp. grep for `CTRLIsAllowed`/`ctrl_allowed` found nothing (controls-allowed gate unverified); potion fullness threshold not compared |
| A5 | `v2Controller::Cmd_*` 0x4051a8-0x405b04 (Cmd_Attack 428 B, Cmd_UseOOI 332, Cmd_BeginSkill 216, Cmd_EndSkill 192, Cmd_DropObject 92) and `s_blocked` | 2.7 KB | Gate `this[9] or (!s_blocked and !this[8])`; BeginSkill requires `AI_IsSkillUsable`; Attack saves and restores previous target when already attacking; UseOOI skips merchants and interaction result 1, falls back to character+5284 target; online path sends CMsgControllerAction | features/platform_input/live_dispatch.cpp (blocked), features/combat/object_of_interest_owner_v1.cpp, auto_target_marker_v1.cpp, target_selection_runtime.cpp, original_controller_commands.cpp/.hpp (name suggests a direct Cmd_* port). Confirm gate order and target restore |
| A6 | `NativeSelectClassSpec` 0x43d178, `NativeGetPossibleClassSpec` 0x449b10 | ~1 KB | Class specialisation: new class = sel + 1 + current class, then `LoadPropertiesForClassSelect`, `SG_SetPlayerClass`, `AI_ReloadSkills`, `SG_Save` | grep for SelectClassSpec / LoadPropertiesForClassSelect / SetPlayerClass finds only frontend art/presentation files. Possibly missing from gameplay; check whether specs are reachable |
| A7 | `NativeSwapEquipment` 0x442198 | 272 B | `SwapEquipmentSet`, `UpdateGearsProperties`, `INV_CheckItemsRequirements`, `INV_UpdateSkin`, `ValidateHPMP` ordering | features/equipment/equipment_adapter.cpp and character_state.cpp match swap/equipment_set grep; verify ordering and HP/MP clamp |
| A8 | State-transition Native*: `NativeReviveAllPlayers` 0x43b1e4, `NativeSetLevelPause` 0x43ab40 (Level+424), `NativeGoToZone` 0x4422b8 (QuickSave when level state 38, `SG_SaveAllPlayer`, index -1 becomes 0), `NativeSetCurrentDifficulty` 0x43cd68, `NativeSetCurrentQuest` 0x43d3b4, `NativeSaveGame` 0x43dc08, `NativeStartGame` 0x43e0d0, `NativeLockCharacter/UnlockCharacter` 0x43eac0/0x43ea0c, `NativeSkipScript` 0x43ac90, `NativeReloadSkills` 0x43dbb8, `NativeHUDSetActiveFaery/GetActiveFaery/GetIsFaeryUnlocked` 0x43ee40/0x44a820/0x43ed58 (ChangeFaery then `PlaceFaeryAndFollowers`) | ~4 KB | Death-to-revive fade flow, pause flag, zone travel and save semantics, difficulty choice, faery switching | features/persistence/death_restart_v1.hpp, features/pause_ui, level-world/application_pause_save_v1.cpp, level-world/canonical_exit_zone_v29.cpp, features/faery_menu/character_state_faery_v1.hpp, features/scene_flow, level-world/character_menu_live_reload_connection_v1.cpp. No hits for SetLevelPause or GoToZone. Needs per-function compare |
| A9 | Stat/tooltip Native*: `NativeGetPlayerStats` 0x446afc (11.5 KB), `NativeGetPlayerHUDInfos` 0x44e5cc, `NativeGetSkillDetails` 0x445a10, `NativeGetBuffs` 0x44b604, `NativeGetMonsterHUDHP` 0x43cc40 (100*hp/max of target at char+5284), `NativeGetNumPotions` 0x44996c, `NativeGetQuestIDsInRange/Details`, `NativeGetWorldMapLocations/MapIcons` | ~24 KB | Display-only, but compute derived numbers shown to the player (damage/armor/crit text, skill tooltip values, buff durations). Wrong formulas would be player-visible, not mechanical | features/character_menu, features/skill_ui, features/status_effects, features/quests/character_quest_page_v1.cpp. UI-numbers pass, lower priority |
| A10 | `CharAI::SetScript<AISExternal/AISMonster/AISFaery/AISDefault/AISPlayerIPhone/AISPlayer>` 0x3ccaf4-0x3cd11c; `GetNewState<CS*>` x20 from 0x3c0454; `GetNewInstance<ObjectBase>` x31 from 0x340800 | ~4 KB | Which AI script drives which actor (AISPlayerIPhone vs AISPlayer is the touch auto-target script) and object-type id to class mapping | features/combat/auto_target_marker_v1.cpp, features/enemy_ai/native_monster_endpoint.cpp; grep SetScript / AISPlayer in code finds nothing. Fold into ai/states batches |
| A11 | `v2GamepadController::Update` 0x406c2c (7.3 KB), `v2EmuController` 0x405f48, `Gamepad::UpdateFrame` 0x34d124, `GamepadTweaker::TranslateButton` 0x404710 | ~11 KB | Gamepad/emulated input mapping (camera-relative stick to HeadTowards, skill buttons, potion, zoom). Port is keyboard/mouse/touch-HUD; no gamepad in port | intentional-adaptation candidate; confirm it is documented |
| A12 | `II_Item_sortbyname/value/buyback`, `operator<(ItemInventory::Item)` 0x3fd1dc-0x3fda1c | ~100 B | Inventory sort order (player-visible) | features/equipment/equipment_adapter.cpp (SortBy hit). Low |
| A13 | `TrophyCheckArmorSet{Decay,Phantasm,Shadow,Marksman,Angelic,Rage,Archmage,Jester,Lordly}` 0x3fc6e8-0x3fce48 | 2.3 KB | Achievement-only (equipped item-id lists). No stat effect found | port only has the quest-all-complete trophy hook (features/quests/source_quest_service_binding.cpp:145). Low/optional |
| A14 | `PropertyMap::AddProperty<T>` x10 (0x33e4ac...), `Structs::Skill/Loot/WeaponRef/AIProps/Faery/Merchant/CharAnim::read` | ~35 KB | Matters only if the port parses binary data itself instead of recovered data; likely n/a | low |

Checked and cleared (no gameplay consequence): TrophyManager (achievement popup/save only), VisualFXManager/AnimatedFX (cosmetic FX
playback), HUDControls (layout customization), InfoHUDManager::FastUpdate (bar frames from hp/mp/xp percent, potion count text),
ZoomHandler (map-page pinch zoom via MenuCharMenu_Map), TouchScreenBase::ProcessEvents (queue to EventManager::Raise, no timing logic
beyond debug switches), HUDMinimap (visited-room icon rendering), b2* (vendored byte-identical Box2D 2.0.1).

Caveat: category counts come from regex bucketing of the family keys. E-categories were verified by sampling and by an address-range
scan of 0x317000-0x530000 (the game-code range; 843 non-engine families were listed and read), not function by function. Port greps
were by name/keyword only; absence of a hit means "not found by that name", not "not implemented".
