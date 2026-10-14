# attacker:GetProp

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetProp` | bindFunction and/or Method (see registration ledger) | `Character::_GetProp` | `Character::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:578 |

Address-identified callback: `Character::_GetProp` at `0x003b9d8c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c`, unified lines 135419-135516.
Recovered declaration: `void __fastcall Character::_GetProp( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: Number tag 3 converted getUInteger, <=223; unit/scale Property index; return is raw signed word, no division by 256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:70`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:91`.
- Lua argument 2: Optional boolean or pointer; unit/scale true -> global temp sheet; false/other type -> owner cached; nonnull pointer -> external sheet. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:70`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:91`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Default reads owner cached sheet +4084. Boolean true selects CharProperties::s_temp. Nonnull pointer selects external PROPS_GetFromSheet. Does not recalculate or modify property. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:77`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:70`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:91`.
**Gates:**
- PROVEN: Missing/non-number first, converted index >223, or explicit null pointer second emits nothing. getUInteger reads first argument more than once. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:85`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:1`.
**Return:**
- PROVEN: One integer/raw property word on accepted read.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** Reusable callback preserves current/temp/external selection and unsigned conversion in ordinary valid sheet domain. Port validates service identity/alignment instead of dereferencing arbitrary original 32-bit pointers.
- Source evidence: `port/level-world/character_property_bindings.cpp`:29.
- Source evidence: `port/level-world/character_property_bindings.cpp`:49.

**Port comparison:** Reusable callback preserves current/temp/external selection and unsigned conversion in ordinary valid sheet domain. Port validates service identity/alignment instead of dereferencing arbitrary original 32-bit pointers.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Cached property 172=512, temp=768; GetProp(172) 512. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c`:77.
2. **PROVEN:** Given: Same state; GetProp(172,true) 768. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c`:70.
3. **PROVEN:** Given: External sheet property 172=1024; GetProp(172,pointer) 1024. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c`:91.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **58** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:112` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Rating_Attack"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Rating_Attack\")","literal_tokens":["\"CharacterProperties\"","\"Rating_Attack\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:122` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Spell_Rating_Attack"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Spell_Rating_Attack\")","literal_tokens":["\"CharacterProperties\"","\"Spell_Rating_Attack\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:125` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Spell_Rating_Attack_Bonus_Fire") + element)` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Spell_Rating_Attack_Bonus_Fire\") + element","literal_tokens":["\"CharacterProperties\"","\"Spell_Rating_Attack_Bonus_Fire\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:216` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Rating_Critical"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Rating_Critical\")","literal_tokens":["\"CharacterProperties\"","\"Rating_Critical\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:221` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Spell_Rating_Critical"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Spell_Rating_Critical\")","literal_tokens":["\"CharacterProperties\"","\"Spell_Rating_Critical\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:252` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Hurt_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Hurt_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"Hurt_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:254` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:261` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "SnS_Hurt_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"SnS_Hurt_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"SnS_Hurt_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:263` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:291` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Push_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Push_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"Push_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:293` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:301` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "SnS_Push_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"SnS_Push_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"SnS_Push_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:303` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:331` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Stun_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Stun_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"Stun_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:333` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:341` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "SnS_Stun_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"SnS_Stun_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"SnS_Stun_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:343` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:370` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Fear_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Fear_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"Fear_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:372` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:380` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "SnS_Fear_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"SnS_Fear_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"SnS_Fear_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:382` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:409` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Slow_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Slow_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"Slow_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:411` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:420` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "SnS_Slow_Target_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"SnS_Slow_Target_Chance\")","literal_tokens":["\"CharacterProperties\"","\"SnS_Slow_Target_Chance\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:422` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:446` | outside scoped roots | `attacker:GetProp(CP_SnS_Dot_Duration)` | `[{"index":1,"expression":"CP_SnS_Dot_Duration","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:446` | outside scoped roots | `attacker:GetProp(CP_Dot_Duration)` | `[{"index":1,"expression":"CP_Dot_Duration","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:451` | outside scoped roots | `attacker:GetProp(CP_Dot_Min_Damage)` | `[{"index":1,"expression":"CP_Dot_Min_Damage","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:451` | outside scoped roots | `attacker:GetProp(CP_SnS_Dot_Min_Damage)` | `[{"index":1,"expression":"CP_SnS_Dot_Min_Damage","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:452` | outside scoped roots | `attacker:GetProp(CP_SnS_Dot_Max_Damage)` | `[{"index":1,"expression":"CP_SnS_Dot_Max_Damage","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:452` | outside scoped roots | `attacker:GetProp(CP_Dot_Max_Damage)` | `[{"index":1,"expression":"CP_Dot_Max_Damage","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:460` | outside scoped roots | `attacker:GetProp(CP_SnS_Dot_Elemental_Type)` | `[{"index":1,"expression":"CP_SnS_Dot_Elemental_Type","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:503` | outside scoped roots | `attacker:GetProp(CP_Damage_Min_Off_Hand)` | `[{"index":1,"expression":"CP_Damage_Min_Off_Hand","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:504` | outside scoped roots | `attacker:GetProp(CP_Damage_Max_Off_Hand)` | `[{"index":1,"expression":"CP_Damage_Max_Off_Hand","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:505` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Elemental_Type_Off_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Elemental_Type_Off_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Elemental_Type_Off_Hand\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:507` | outside scoped roots | `attacker:GetProp(CP_Damage_Min_Main_Hand)` | `[{"index":1,"expression":"CP_Damage_Min_Main_Hand","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:508` | outside scoped roots | `attacker:GetProp(CP_Damage_Max_Main_Hand)` | `[{"index":1,"expression":"CP_Damage_Max_Main_Hand","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:509` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Elemental_Type_Main_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Elemental_Type_Main_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Elemental_Type_Main_Hand\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:516` | outside scoped roots | `attacker:GetProp(CP_SnS_MinDamage)` | `[{"index":1,"expression":"CP_SnS_MinDamage","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:517` | outside scoped roots | `attacker:GetProp(CP_SnS_MaxDamage)` | `[{"index":1,"expression":"CP_SnS_MaxDamage","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:531` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Bonus_Stunned"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Bonus_Stunned\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Bonus_Stunned\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:535` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Special_Sneak"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Special_Sneak\")","literal_tokens":["\"CharacterProperties\"","\"Special_Sneak\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:536` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Bonus_Sneaking"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Bonus_Sneaking\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Bonus_Sneaking\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:542` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Bonus_Combo"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Bonus_Combo\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Bonus_Combo\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:574` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Leech_HP"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Leech_HP\")","literal_tokens":["\"CharacterProperties\"","\"Leech_HP\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:575` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Leech_MP"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Leech_MP\")","literal_tokens":["\"CharacterProperties\"","\"Leech_MP\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:582` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Elemental_Min_Off_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Elemental_Min_Off_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Elemental_Min_Off_Hand\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:583` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Elemental_Max_Off_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Elemental_Max_Off_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Elemental_Max_Off_Hand\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:585` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Elemental_Min_Main_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Elemental_Min_Main_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Elemental_Min_Main_Hand\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:586` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Elemental_Max_Main_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Elemental_Max_Main_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Elemental_Max_Main_Hand\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:658` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "SnS_MinDamage"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"SnS_MinDamage\")","literal_tokens":["\"CharacterProperties\"","\"SnS_MinDamage\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:659` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "SnS_MaxDamage"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"SnS_MaxDamage\")","literal_tokens":["\"CharacterProperties\"","\"SnS_MaxDamage\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:661` | outside scoped roots | `attacker:GetProp(CP_Damage_Min_Main_Hand)` | `[{"index":1,"expression":"CP_Damage_Min_Main_Hand","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:662` | outside scoped roots | `attacker:GetProp(CP_Damage_Max_Main_Hand)` | `[{"index":1,"expression":"CP_Damage_Max_Main_Hand","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:663` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Damage_Elemental_Type_Main_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Elemental_Type_Main_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Elemental_Type_Main_Hand\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:680` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Spell_Damage_Bonus_Fire") + element)` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Spell_Damage_Bonus_Fire\") + element","literal_tokens":["\"CharacterProperties\"","\"Spell_Damage_Bonus_Fire\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/level/combat_formulas.luac:750` | outside scoped roots | `attacker:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:24` | player_skill_lua | `attacker:GetProp(GetPyStruct("CharacterProperties", "Slow_Resist_Chance"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Slow_Resist_Chance\")","literal_tokens":["\"CharacterProperties\"","\"Slow_Resist_Chance\""]}]` |

## Related natives

`Character::_GetProp`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
