# Audit batch: stats (2026-10-11)

## Progress
- last_address_done: 0x4f7b14 (batch complete at the depth below)
- groups_done: CharProperties getters/setters/Recalc/ClassFunc evaluators/gear loaders/buffs/DoT/slow, CharTimers, Native Stats/Skills natives, PropertyMap, PyDataConstants, DesignSettings/GameDifficulty/StatAutoAssign readers, per-monster *Properties structs, table boilerplate
- next: nothing mandatory. Optional depth: Character::IncStat*/ResetStats (outside batch), CharPropTextField::Update (UI text), caller of CharTimers::Update dt/pause flag in the port.

## 1. Summary

Status counts (by group, see table): faithful 15, divergent 1, partial 1, missing 1, intentional-adaptation 1, unclear 4, n/a 9.

The core stat engine is the most carefully reconstructed part of the port: `port/game-data/{class_tables,properties,item_gear_properties_v5,combat*}.cpp`
and `port/level-world/{character_buffs,character_timers,character_slow_reaction_v1,character_dot_attack}.cpp`
were differentially tested against the original ARM code (see `port/game-data/README.md`), and my independent read of the
IDA pseudocode agrees on every branch I compared (type-bit precedence in RecalcProperty, ClassFunc quirks, gear
stat/power jump tables incl. left-hand offsets, AddBuff stacking, AddDot, DebuffSlow, CharTimers). The defects found are in
glue/integration, not in the primitives.

Top risks:
1. HIGH: XP gain bonus is read from the wrong property (201 Special_Merchant_Price instead of 200 Special_Bonus_To_XP).
2. MEDIUM: paid stat+skill respec (NativeStatsReset, cost StatsReset_GoldCostPerLevel * Level) has no port equivalent.
3. LOW/unclear: stat-training recalculation keeps the *base* Level instead of reloading it from the character row.
4. LOW/unclear: BonusCritRating / GetIntWithBonus display paths (character panel) not located in the port.
5. LOW: speed multipliers use 1/256 exactly vs original 0.0039062f (about 1e-6 relative; no gameplay effect).

## 2. Findings

### F1 (HIGH, divergent): XP modifier reads Special_Merchant_Price (201) instead of Special_Bonus_To_XP (200)
- IDA 0x3de7ec `CharProperties::PROPS_GetModifiedXP(int)`: pseudocode `((*((_DWORD*)this+878) + 25600) / 100 * a2) >> 8`.
  Assembly (assembly-functions.asm @003de7ec): `LDR R2,[R0,#0xDB8]` = this+3512. The resolved sheet is at this+2708 and
  `_GetProperty` reads `sheet + 4 + 4*idx` (0x3dedb4; m_dataOffsets = 4*idx per port README), so idx = (3512-2712)/4 = **200**.
  Cross-check: the sibling getters read +2896/2900/2904 = idx 46/47/48 (Speed_Modifier_Walk/Rotation/Attack), and the port's own attack-speed code uses 48.
  `port/game-data/reference/properties/property-rules.json` names 200 = `Special_Bonus_To_XP`, 201 = `Special_Merchant_Price` (0x3de814 PROPS_GetModifiedPrice reads +0xDBC = 201).
  Callers: Character::GiveXP at 0x3bf60c (applied via PROPS_Add(33)) and 0x3bfca4 (floating combat text).
- Port: `port/level-world/player_progression_v1.cpp:90` and `:126` (`progression_modified_xp_v1(raw, p->resolved[201])`),
  `port/level-world/character_progression_world_v23.cpp:76`, `port/windows-foundation/features/loot/runtime_death_rewards_v1.cpp:216`
  (`sheet.resolved[201]`). The arithmetic itself (`(bonus+0x6400)/100` first, then `*raw`, `>>8`) at `player_progression_v1.cpp:75-78` is correct;
  only the index is wrong. Unit tests (`port/level-world/tests/player_progression_v1_host.cpp:16,35`) feed bonus/raw from a gold file or reuse `resolved[201]`, so they cannot catch it.
- Consequence: every XP award ignores Special_Bonus_To_XP and instead scales by the merchant-price property (which the original never uses for XP; PROPS_GetModifiedPrice has no callers).
  The property is live in data: `.local-inputs/assets-extra/android/data/pydata/character_classes_pyarray.bin` contains four
  ClassFunc type-1 (Linear) formulas with destination 200 (found by raw word scan: `{200,1,0,19,768}`, `{200,1,0,19,256}` = per-level bonus from Level, and two `{200,1,-666,172,384}` = from SnS_Level).
  Those bonuses never reach XP; any source of property 201 would wrongly inflate XP.
- Confidence: high on the index (assembly offset + property names + sibling cross-check); medium on which in-game classes are visibly affected (I did not decode which class rows own those formulas).
- Fix: use index 200 in all four sites; add tests that set resolved[200] and assert the XP delta and that setting 201 changes nothing.

### F2 (MEDIUM, missing): paid stat/skill respec (NativeStatsReset)
- IDA 0x43e494 `NativeStatsReset(fn_call)`: args (playerIndex:int, queryOnly:bool). cost = `PyDataConstants::getConstant("CharacterDesign","StatsReset_GoldCostPerLevel") * Character::GetLevel()`;
  if not queryOnly: `Character::ResetStats()`, `Character::ResetSkills()`, `ItemInventory::AddGold(-cost)`; returns cost to Flash either way.
- Port: no occurrence of StatsReset/GoldCostPerLevel/ResetStats/ResetSkills in `port/level-world` or `port/windows-foundation`. Only the per-visit staged spend/refund exists
  (`port/windows-foundation/features/character_menu/stat_training_v1.{hpp,cpp}`), which implements NativeStatsAssignPoint (0x43eb7c) and
  NativeStatsRemoveAssign (0x43d43c -> ResetStatsChange) and is a documented PC adaptation (multiple spends per visit; header comment lines 12-19, POINTS-report.md in coordination/claude-preview15).
- Consequence: the player cannot pay gold to reset all stat and skill points (if the ported menu shows the button it does nothing or is free).
- Confidence: medium (absence by grep only; the Flash menu button wiring was not inspected). Fix: add a reset service that charges constant*level, zeroes saved props 149..152 and refunds Stat_Points (148) / Skill_Points (157), with the query-only mode for the price label.

### F3 (LOW, unclear): stat-training recalculation keeps current base Level
- IDA UpdateBaseProperties 0x3e087c = ResetBaseProperties + LoadBaseProperties (memcpy of the whole 224-word row, `_LoadFromCharTable` 0x3df250) + RecalcProperties(1).
- Port `stat_training_v1.cpp:98-103` copies the row then restores the previous base Level ("profile projection rule"). Whether the original loses or keeps Level depends on whether Level lives in
  base or saved (type 32) at runtime; not verified. If level-up writes saved, both behaviours coincide. Needs a check of Character::IncStat* / LevelUp (outside batch).

### F4 (LOW, unclear): character-panel derived values (GetIntWithBonus / BonusCritRating)
- IDA 0x3df980 PROPS_GetIntWithBonus adds >>8 of GetBonusAttackRating to prop 50 and GetBonusDamage(main/off) to props 79/80 and 81/82; GetBonusCritRating (0x3df7c4) = prop(64+weaponClass) of the equipped main/off item; exposed to Lua (_GetPropBonusCritRating) and used in the panel text.
- Port combat uses equivalent attack/damage bonus helpers (`game-data/combat.cpp:15-19`, `game-data/combat_result.cpp:17`), faithful. I did not find a port equivalent for the crit-rating bonus (64+class) or the panel's "with bonus" integers
  (Windows `features/character_menu/menu_stats.cpp` not compared). Display-only unless panel numbers differ; needs a targeted compare.

### F5 (LOW, divergent but benign): speed multiplier constant
- IDA uses `0.0039062` (float) * 0.01 + 1.0 floored at 0 (Walk 0x3de6c4, Rotation 0x3de708, Attack 0x3de74c, Opacity 0x3de790 clamped to [0,1]).
- Port `level-world/move_state.cpp:6` and `character_state.cpp:137-141` use 1/256 exactly. Difference about 1.3e-6 relative; below the 0.0001 speed-change tolerance in `character_state.cpp:133`. No gameplay effect.
- Opacity_Modifier (prop 49) getter has no port consumer (no `resolved[49]`); cosmetic.

### Verified faithful (with citations)
- RecalcProperty 0x3dfe60 type-bit precedence (4 additive, 2 override, 1 priority, 32 saved+base, 16 base-only, else keep) and AddProperty "replace if equal-to-default" semantics: `port/game-data/properties.cpp` + README "Property resolution and mutation"; 119k-case differential.
- ClassFunc evaluators 0x3e2bcc-0x3e3014 and `_LoadClass` 0x3e2e20: type 0 group (up to 3 sub-classes, -1 skipped), 1 linear (`base + k*(src>>8)`, base -666 = current destination; source via RecalcProperty for normal sheets, resolved sheet for buffs), 2 MinMax, 4 add-other-prop, 5 scale `(a*b)>>8`, 6 scale-for-buff then FALLS THROUGH into type 7 AddValue (quirk), 7 add, 8 ApplyGroupOnlyIf = no-op that RETURNS from the class (stops later formulas), 9 set-OID, 3 ignored. Port: `port/game-data/class_tables.hpp:16-23` comments, `class_tables.cpp`, README "Class formulas".
- `_LoadGearStats` 0x3e3154 / `_LoadGearPower` 0x3e32b4: compared all 49 power types and the item types, including left-hand offsets (types 7-9 -> +3, 10-19 and 28 -> +2) and type 9 using SET not ADD: `port/game-data/item_gear_properties_v5.cpp:11-34`. All matched.
- PROPS_AddBuff 0x3e232c (stack capacity default 128 when <=0, replace lowest-strength then shortest-time-left instance, timer start with event 0x36, FX frame, ResetSheet, sheet[172]=strength<<8 and resolved[172]), PROPS_DelBuff, BuffExpired, RemoveAllBuffs: `port/level-world/character_buffs.cpp:15-47`.
- PROPS_AddDot 0x3e2720 (class/FX id = index("AUTO_DOT_01_FIRE")+element, names dot_normal..dot_air, single stack, buff sheet[127+element]=damage then RecalcProperty): `character_buffs.cpp:44`. HandleDots 0x3df3f0 (elements -1..4, props 127..132, >0 and not dead): `character_timer_effects.cpp:49-56`.
- PROPS_DebuffSlow 0x3e2a5c (non-boss, duration!=0, class "Debuff_Slow", FX AUTO_DEBUFF_SLOW, ApplyClassToSheet then recalc 48,47,46): `character_slow_reaction_v1.cpp:7-27`.
- CharTimers Start/Stop/Pause/Resume/StopAll/TimeLeft/FindSlot/Update: `character_timers.cpp:17-64`. Update semantics (accumulate dt; while elapsed>=duration: repeat==0 deactivates WITHOUT subtracting, repeat!=0 subtracts and decrements positive repeat; event -1 becomes 0x29; skipped when the global pause flag is set) match.
- PROPS_GetBonusDamage/GetBonusAttackRating (two-hander +91, dual +90, attack +51+class and +58 dual): `game-data/combat.cpp:15-19`, `combat_result.cpp:17`. MagicToHit base 19200 = 0x4b00 in `combat_result.cpp:36-38`.
- PropertyMap LoadDefault/LoadOverridesFromXML/SetProperty (absent attribute -> default, numeric parse zeroes first): prior audit LW-0109/0110 (`docs/act1-ida-audit/checklist.csv:645-646`) and `level-world/canonical_property_map_v1.cpp:128-152`; the `_templateName` handling is a documented "modern compatibility correction" there.

## 3. Mapping table

| IDA address | IDA function (or group) | port location(s) | status | note |
|---|---|---|---|---|
| 0x3de6c4/708/74c | CharProperties::PROPS_GetWalkSpeed/RotationSpeed/AttackSpeed | level-world/move_state.cpp:6-14; character_state.cpp:137 | faithful | F5 constant 1/256 vs 0.0039062 |
| 0x3de790 | PROPS_GetOpacity | none | unclear | no consumer found; cosmetic |
| 0x3de7ec | PROPS_GetModifiedXP | level-world/player_progression_v1.cpp:75,90,126; character_progression_world_v23.cpp:76; features/loot/runtime_death_rewards_v1.cpp:216 | divergent | F1 wrong property index (201 vs 200) |
| 0x3de814 | PROPS_GetModifiedPrice | none | n/a | no callers in the original (dead code) |
| 0x3deca0/dedb4/deed8/def10/def34 | _SetProperty/_GetProperty/_GetType/_GetDefault/_ResetProperties | game-data/properties.cpp; dh2_property_* | faithful | out-of-range get returns -1; type default 16 |
| 0x3df114/df140 | _IsPropertySet/_AddProperty | properties.cpp; item_gear_properties_v5.cpp:10 | faithful | add replaces when equal to default |
| 0x3df250, 0x3df2a4, 0x3e087c, 0x3e0854 | _LoadFromCharTable/LoadBaseProperties/UpdateBaseProperties/LoadPropertiesForClassSelect | properties.cpp (reset_properties); features/character_menu/stat_training_v1.cpp:98 | partial | F3 base Level kept |
| 0x3dfe60 | RecalcProperty | game-data/properties.cpp (recalc_properties) | faithful | differential-tested |
| 0x3e0810 | RecalcProperties | properties.cpp recalc_properties_with_class | faithful | applies class id from base prop 26 first |
| 0x3e0614/0708/07a0 (+0798/0808) | PROPS_SetToSheet/Add/Set/AddInt/SetInt | game-data/properties.hpp:34-37 (dh2_property_*) | faithful | type bit 32 saved / bit 8 runtime |
| 0x3df314/3b8 | PROPS_ApplyClassToSheet / PROPS_ApplyClass | class_tables.hpp dh2_class_apply / apply_class_uncached | faithful | |
| 0x3e2bcc-0x3e3014, 0x3e2e20 | _ApplyGroupOnlyIf/_SetOID/_AddValue/_ScaleWithPercentage(ForBuff)/_AddOtherProp/_MinMax/_LinearWithPropPlusBaseValue/_Group/_LoadClass | game-data/class_tables.cpp (dh2_class_formula, dh2_class_apply) | faithful | quirks (type 6 fallthrough, type 8 stops class) preserved |
| 0x3e3154 | _LoadGearStats | game-data/item_gear_properties_v5.cpp:20-26 | faithful | |
| 0x3e32b4 | _LoadGearPower | item_gear_properties_v5.cpp:12-34 | faithful | type 9 is Set |
| 0x3df480 | LoadGearsProperties / UpdateGearsProperties | item_gear_properties_v5; equipment owners in level-world | faithful | per-slot loop incl. left-hand flag; owner wiring not re-read |
| 0x3df6e0, 0x3df980 | PROPS_GetInt / PROPS_GetIntWithBonus | not located | unclear | F4 |
| 0x3df7c4 | PROPS_GetBonusCritRating | not located | unclear | F4 |
| 0x3df81c, 0x3df8ac | PROPS_GetBonusAttackRating / PROPS_GetBonusDamage | game-data/combat_result.cpp:17; combat.cpp:15 | faithful | |
| 0x3df718, 0x3df76c | PROPS_GetMagicDamageBonus / GetMagicToHit | game-data/combat.cpp:57; combat_result.cpp:36-38 | faithful | faery element selects props 166+e / 159+e, +19200 base |
| 0x3dfd18 | PROPS_IsBonus | UI tint only | n/a | |
| 0x3e232c, 0x3e101c, 0x3e123c, 0x3e0af8 | PROPS_AddBuff/DelBuff/BuffExpired/RemoveAllBuffs | level-world/character_buffs.cpp | faithful | |
| 0x3e2720, 0x3df3f0 | PROPS_AddDot / HandleDots | character_buffs.cpp:44; character_timer_effects.cpp:49-56; character_dot_attack.cpp | faithful | |
| 0x3e2a5c | PROPS_DebuffSlow | level-world/character_slow_reaction_v1.cpp | faithful | |
| 0x3db288-0x3dbe24 | CharTimers (Start/Stop/Pause/Resume/StopAll/TimeLeft/FindSlot/Update/SetCharacter) | level-world/character_timers.cpp | faithful | caller dt/pause-flag wiring not re-read |
| 0x3af76c, 0x3bd130, 0x3b55d4 | PROPS_Get clones / PROPS_GetFromSheet | dh2_property_resolve | faithful | |
| 0x3e1374, 0x3e16e0, 0x3e1758, 0x3df2b0 | LoadPropNames/GetNameFromPropID/GetPropIDFromName/UnLoadPropNames | property-rules.json; script name lookups | n/a | name tables |
| CharProperties ctors/dtors/Reset*/DBG_*, BuffDecl, std::map/vector/deque helpers | boilerplate | n/a | n/a | |
| 0x43eb7c, 0x43d43c | NativeStatsAssignPoint / NativeStatsRemoveAssign | features/character_menu/stat_training_v1.cpp | intentional-adaptation | multi-spend per visit, documented in stat_training_v1.hpp:12-19 |
| 0x43e494 | NativeStatsReset (paid respec) | none | missing | F2 |
| 0x43d548, 0x43ec98, 0x4425a0 | NativeSkillsTrainSkill / GetSkillPointsLeft / GetEquipedSkillsIDs | features/generic_skills/runtime_skill_progression_v1.cpp, runtime_skill_session_training_v1.cpp | unclear | not compared in depth (Skill_Points = prop 157, same index in port) |
| 0x4c3708, 0x4c4bdc, 0x4c4b08, 0x4c540c | PyDataConstants Load/getConstant/getConstantName/reload | level-world/character_design_services.cpp (design->lookup) | faithful | original returns 0 for a missing constant; not re-verified in port lookup |
| 0x510b4c-0x51419c | PropertyMap (defaults, overrides, template, clone, init) | level-world/canonical_property_map_v1.cpp | faithful | prior audit LW-0109/0110; `_templateName` is a documented compat correction |
| 0x4b3cd0, 0x4b7638, 0x4ee0d0, 0x4a901c/90b8 | DesignSettings read/readNames/finalize (43 fields, 172-byte rows) | game-data/design_settings.cpp | faithful | field kind table at design_settings.cpp:40 |
| 0x4f23e4 | GameDifficulty::read (5 ints) | game-data (name-ID tables) | n/a | Arrays::GameDifficulties members are never field-read in the original (only size/names), no formulas to port |
| 0x4b41f4, 0x4dce90, 0x4ef520, 0x4a961c/96b8 | StatAutoAssign scheme/target/list readers | game-data generic table readers | n/a | auto-assign scheme use in Windows menu not verified |
| 0x4c57b4..0x4f7b10 (31 per-monster *Properties structs: dtor/finalize/read) | Structs::*Properties | n/a | n/a | read()/finalize() are 4-byte empty stubs; monster fields come from PropertyMap XML + CharacterTable rows |
| Arrays::Class/Character/Stat/Skill/Faery/AI/Difficulty/DesignSettings tables read/finalize/skip | table boilerplate | game-data/*_tables.cpp | n/a | structural readers, host-audited per README |
| 0x379084-0x379e10 | PlayerStatManager (multiplayer ranking) | none | n/a | network/leaderboard stats; Reset/Update/Increase are no-op stubs |
| 0x41f8f0, 0x422aa8 | CharPropTextField::Update | engine-ui / character menu text | unclear | UI text binding not compared |
| 0x318178-0x3191b0 | UserProperties | n/a | n/a | key=value parser |

## 4. Scope and limits
- I read the pseudocode for every gameplay-consequent CharProperties/CharTimers function and the Native Stats/Skills callbacks, and the assembly for PROPS_GetModifiedXP. I did not re-run the port's differential tests; "faithful" rests on my own read of pseudocode vs port source plus the port's published oracle evidence.
- Not covered (outside this batch's functions): level-up/XP curve tables (Max_XP formulas live in class data), Character::IncStat*/ResetStats, monster level/difficulty scaling in Character::InitPost.
