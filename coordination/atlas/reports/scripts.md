# Script to engine map

Static map of original Lua script sites to engine bindings and current port evidence. Work status describes source mapping; readiness is context-qualified and does not imply integrated gameplay verification.

## Inventory

- Android Lua files: **219** (`lua-source`: 219, `lua-bytecode`: 0).
- iOS Lua files: **0**.
- Source lines: **27037**; call expressions: **7982**; engine candidates: **6672** (288 unique candidate rows).
- Script argument references inventoried: **4783**.
- Available Android/iOS/original-cache assets can inform reference resolution. Native bindings here are mapped from the Android IDA export only; this report makes no new iOS native claim.

## Boss and custom script inventory

Readiness is `ported / engine call sites`; PARTIAL and MISSING are listed separately.

| Script | Engine calls | Ported | Partial | Missing | Readiness | Work status |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| .local-inputs/assets-extra/android/data/scripts/ai/assassin1.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/assassin2.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/assassin3.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/assassindemon.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/balrog.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/darkqueen1.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac | 148 | 42 | 87 | 19 | 28.4% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/darkqueen2.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/edwardboss.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac | 79 | 34 | 39 | 6 | 43.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/emissary.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/flaminite.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac | 135 | 45 | 71 | 19 | 33.3% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/flaminiteed.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/madruk.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac | 19 | 9 | 10 | 0 | 47.4% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac | 103 | 27 | 75 | 1 | 26.2% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac | 20 | 10 | 10 | 0 | 50.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/madruk_torso.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac | 78 | 15 | 62 | 1 | 19.2% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac | 5 | 1 | 4 | 0 | 20.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac | 31 | 16 | 13 | 2 | 51.6% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/rene.luac | 51 | 21 | 28 | 2 | 41.2% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac | 31 | 11 | 20 | 0 | 35.5% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/roottroll.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac | 93 | 37 | 50 | 6 | 39.8% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac | 8 | 4 | 4 | 0 | 50.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac | 8 | 4 | 4 | 0 | 50.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac | 124 | 42 | 74 | 8 | 33.9% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac | 8 | 4 | 4 | 0 | 50.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac | 129 | 38 | 78 | 13 | 29.5% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac | 126 | 42 | 77 | 7 | 33.3% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac | 185 | 184 | 0 | 1 | 99.5% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/winddragon.luac | 2 | 0 | 2 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac | 132 | 37 | 88 | 7 | 28.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac | 6 | 4 | 2 | 0 | 66.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/ai/witch.luac | 83 | 82 | 0 | 1 | 98.8% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/objects/swampking_trap.luac | 6 | 0 | 6 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac | 27 | 2 | 19 | 6 | 7.4% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac | 13 | 0 | 9 | 4 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac | 22 | 4 | 13 | 5 | 18.2% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac | 22 | 4 | 13 | 5 | 18.2% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac | 28 | 0 | 28 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac | 17 | 0 | 13 | 4 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac | 26 | 0 | 16 | 10 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac | 18 | 0 | 11 | 7 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac | 23 | 0 | 17 | 6 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac | 13 | 0 | 7 | 6 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac | 4 | 0 | 4 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac | 21 | 3 | 13 | 5 | 14.3% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac | 16 | 0 | 12 | 4 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac | 24 | 3 | 14 | 7 | 12.5% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac | 17 | 0 | 13 | 4 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac | 18 | 1 | 13 | 4 | 5.6% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac | 22 | 2 | 13 | 7 | 9.1% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac | 12 | 0 | 8 | 4 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac | 70 | 4 | 50 | 16 | 5.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac | 4 | 0 | 4 | 0 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac | 7 | 1 | 6 | 0 | 14.3% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac | 44 | 9 | 29 | 6 | 20.5% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac | 7 | 1 | 6 | 0 | 14.3% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac | 32 | 11 | 21 | 0 | 34.4% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac | 26 | 9 | 16 | 1 | 34.6% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac | 33 | 7 | 20 | 6 | 21.2% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac | 22 | 7 | 15 | 0 | 31.8% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac | 7 | 1 | 6 | 0 | 14.3% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac | 19 | 0 | 13 | 6 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac | 48 | 10 | 27 | 11 | 20.8% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac | 48 | 10 | 27 | 11 | 20.8% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac | 61 | 5 | 38 | 18 | 8.2% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac | 13 | 0 | 9 | 4 | 0.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac | 68 | 6 | 45 | 17 | 8.8% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac | 50 | 4 | 29 | 17 | 8.0% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac | 46 | 4 | 25 | 17 | 8.7% | UNRESOLVED |
| .local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac | 24 | 4 | 15 | 5 | 16.7% | UNRESOLVED |

## Ranked engine functions

Ranked by direct and method call sites. PORTED requires callback plus owner binding evidence; PARTIAL is scoped or provider-limited; MISSING has no corresponding behavior in scanned production code.

| Function | Call sites | Scripts | Native handler(s) | Port status |
| --- | ---: | ---: | --- | --- |
| Trace | 725 | 103 | LuaScript::_Trace | PARTIAL |
| GetPyCst | 608 | 117 | LuaScript::_GetPyCst | PARTIAL |
| GetPyStruct | 442 | 141 | LuaScript::_GetPyStruct | PARTIAL |
| GetPyOID | 428 | 162 | LuaScript::_GetPyOID | PARTIAL |
| AddToVFTable | 256 | 31 | LuaScript::_AddToVFTable | PORTED |
| IsTargetListEmpty | 233 | 99 | GameObject::_IsTargetListEmpty, GameObject::_IsTargetListEmpty | MISSING |
| ToFixed | 221 | 110 | LuaScript::_ToFixed | PORTED |
| FromFixed | 215 | 101 | LuaScript::_FromFixed | PORTED |
| GetProp | 195 | 93 | Character::_GetProp, Character::_GetProp | PORTED |
| GetTargetListTop | 157 | 99 | GameObject::_GetTargetListTop, GameObject::_GetTargetListTop | PARTIAL |
| TargetListSearch | 140 | 99 | GameObject::_TargetListSearch, GameObject::_TargetListSearch | PARTIAL |
| SetProp | 137 | 90 | Character::_SetProp, Character::_SetProp | PARTIAL |
| SetTargetListSorting | 130 | 99 | GameObject::_SetTargetListSorting, GameObject::_SetTargetListSorting | MISSING |
| SetTargetListCharacterFilter | 128 | 99 | GameObject::_SetTargetListCharacterFilter, GameObject::_SetTargetListCharacterFilter | MISSING |
| SetTargetListObjectFilter | 127 | 98 | GameObject::_SetTargetListObjectFilter, GameObject::_SetTargetListObjectFilter | MISSING |
| DoSkill | 116 | 21 | Character::_DoSkill, Character::_DoSkill | PARTIAL |
| LookAt | 106 | 70 | Character::_LookAt, Character::_LookAt | PARTIAL |
| GetTarget | 99 | 24 | Character::_GetTarget, Character::_GetTarget | PORTED |
| ApplyPropClass | 85 | 74 | Character::_ApplyPropClass, Character::_ApplyPropClass | PARTIAL |
| PopTargetList | 83 | 68 | GameObject::_PopTargetList, GameObject::_PopTargetList | MISSING |
| ClearTarget | 73 | 48 | Character::_ClearTarget, Character::_ClearTarget | PORTED |
| GetRand | 68 | 36 | GameObject::_Rand, GameObject::_Rand | PARTIAL |
| RemoveBuff | 64 | 42 | Character::_RemoveBuff, Character::_RemoveBuff | PARTIAL |
| MoveTo | 61 | 22 | Character::_MoveTo, Character::_MoveTo | PARTIAL |
| Stop | 59 | 23 | Character::_Stop, Character::_Stop | PARTIAL |
| attacker:GetProp | 58 | 2 | Character::_GetProp, Character::_GetProp | PORTED |
| HasTarget | 57 | 24 | Character::_HasTarget, Character::_HasTarget | PORTED |
| ApplyBuff | 54 | 41 | Character::_ApplyPropClass, Character::_ApplyPropClass | PARTIAL |
| CreateBuff | 54 | 41 | Character::_CreateBuff, Character::_CreateBuff | PARTIAL |
| Include | 49 | 28 | LuaScript::_Include | PARTIAL |
| GetState | 42 | 20 | Character::_GetState, Character::_GetState | PORTED |
| PlaySound | 41 | 29 | LuaScript::_PlaySound | PARTIAL |
| HasMana | 40 | 40 | Character::_HasMana, Character::_HasMana | PARTIAL |
| UseMana | 40 | 40 | Character::_UseMana, Character::_UseMana | PARTIAL |
| SetTarget | 38 | 20 | Character::_SetTarget, Character::_SetTarget | PORTED |
| GetSelf | 37 | 25 | GameObject::_GetSelf | MISSING |
| PlayFX | 36 | 29 | GameObject::_PlayFX, GameObject::_PlayFX | PARTIAL |
| defender:GetProp | 33 | 1 | Character::_GetProp, Character::_GetProp | PORTED |
| HasPath | 32 | 12 | GameObject::_HasPath, GameObject::_HasPath | PARTIAL |
| target:PlayFX | 31 | 26 | GameObject::_PlayFX, GameObject::_PlayFX | PARTIAL |
| AddAggro | 30 | 15 | Character::_AddAggro, Character::_AddAggro | PARTIAL |
| Attack | 30 | 11 | Character::_Attack, Character::_Attack | PARTIAL |
| ClearAggro | 30 | 15 | Character::_ClearAggro, Character::_ClearAggro | PARTIAL |
| RegisterAnim | 29 | 8 | Character::_RegisterAnim, Character::_RegisterAnim | PORTED |
| Summon | 29 | 13 | GameObject::_Summon, GameObject::_Summon | PARTIAL |
| target:GetName | 28 | 20 | GameObject::_GetName, GameObject::_GetName | PARTIAL |
| GetDistanceBetween | 25 | 16 | GameObject::_IsInRange | PARTIAL |
| PlayAnim | 21 | 8 | Character::_PlayAnim, Character::_PlayAnim | PARTIAL |
| PlayMusic | 20 | 11 | LuaScript::_PlayMusic | PARTIAL |
| GetName | 19 | 13 | GameObject::_GetName, GameObject::_GetName | PARTIAL |
| MulFixed | 18 | 6 | LuaScript::_MulFixed | PORTED |
| SetInt | 18 | 4 | LuaScript::_SetInt | PARTIAL |
| CallPyScript | 17 | 12 | LuaScript::_CallPyScript | PARTIAL |
| GetID | 17 | 17 | GameObject::_GetID, GameObject::_GetID | PORTED |
| GetPosition | 16 | 11 | GameObject::_GetPosition, GameObject::_GetPosition | PORTED |
| GetPropHP | 16 | 11 | Character::_GetPropHP, Character::_GetPropHP | PARTIAL |
| PlaySound3D | 16 | 5 | unresolved | MISSING |
| SetLevel | 16 | 16 | Character::_SetLevel, Character::_SetLevel | PORTED |
| GetCurrentSpellInfo | 15 | 15 | Character::_GetCurrentSpellInfo, Character::_GetCurrentSpellInfo | MISSING |
| GetHostPlayerDifficulty | 15 | 15 | LuaScript::_GetHostPlayerDifficulty | PARTIAL |
| GetHostPlayerLevel | 15 | 15 | LuaScript::_GetHostPlayerLevel | PARTIAL |
| SpellCombatRoll | 15 | 5 | Character::_SpellCombatRoll, Character::_SpellCombatRoll | PARTIAL |
| target:ApplyBuff | 15 | 15 | Character::_ApplyPropClass, Character::_ApplyPropClass | PARTIAL |
| target:CreateBuff | 15 | 15 | Character::_CreateBuff, Character::_CreateBuff | PARTIAL |
| GetCurrentLevelRange | 14 | 14 | LuaScript::_GetCurrentLevelRange | PARTIAL |
| TargetListBackup | 14 | 11 | GameObject::_TargetListBackup, GameObject::_TargetListBackup | MISSING |
| WarpTo | 14 | 6 | Character::_WarpTo, Character::_WarpTo | PARTIAL |
| defender:GetID | 10 | 10 | GameObject::_GetID, GameObject::_GetID | PORTED |
| hack:GetPosition | 10 | 6 | GameObject::_GetPosition, GameObject::_GetPosition | PORTED |
| SetIsTargetable | 10 | 4 | Character::_SetIsTargetable, Character::_SetIsTargetable | PARTIAL |
| GetTargetListSize | 9 | 9 | GameObject::_GetTargetListSize, GameObject::_GetTargetListSize | MISSING |
| HasMaster | 9 | 6 | Character::_HasMaster, Character::_HasMaster | PARTIAL |
| StartTimer | 9 | 3 | Character::_StartTimer, Character::_StartTimer | PORTED |
| target:IsCharacter | 9 | 9 | GameObject::_IsCharacter, GameObject::_IsCharacter | PARTIAL |
| target:IsPlayer | 9 | 9 | GameObject::_IsPlayer, GameObject::_IsPlayer | PARTIAL |
| attacker:GetID | 8 | 8 | GameObject::_GetID, GameObject::_GetID | PORTED |
| HasBow | 8 | 6 | Character::_HasBow, Character::_HasBow | MISSING |
| player:GetID | 8 | 4 | GameObject::_GetID, GameObject::_GetID | PORTED |
| SetMaster | 8 | 7 | Character::_SetMaster, Character::_SetMaster | PARTIAL |
| Flee | 7 | 4 | Character::_Flee, Character::_Flee | MISSING |
| Kill | 7 | 5 | Character::_Kill, Character::_Kill | PARTIAL |
| object:GetID | 7 | 4 | GameObject::_GetID, GameObject::_GetID | PORTED |
| SetBuffProp | 7 | 5 | Character::_SetProp, Character::_SetProp | MISSING |
| ClearProps | 6 | 4 | Character::_ClearProps, Character::_ClearProps | PARTIAL |
| GetEquippedFaeryElement | 6 | 2 | Character::_GetEquippedFaeryElement, Character::_GetEquippedFaeryElement | MISSING |
| LookAtTarget | 6 | 6 | Character::_LookAtTarget, Character::_LookAtTarget | PARTIAL |
| object:GetState | 6 | 6 | Character::_GetState, Character::_GetState | PORTED |
| actor:PlayFX | 5 | 5 | GameObject::_PlayFX, GameObject::_PlayFX | PARTIAL |
| attacker:TargetInMeleeRange | 5 | 5 | Character::_TargetInMeleeRange, Character::_TargetInMeleeRange | MISSING |
| EnableCollisions | 5 | 4 | GameObject::_EnableCollisions, GameObject::_EnableCollisions | MISSING |
| GetCurrentEquippedFaeryLevel | 5 | 1 | Character::_GetCurrentEquippedFaeryLevel, Character::_GetCurrentEquippedFaeryLevel | MISSING |
| GetInt | 5 | 5 | LuaScript::_GetInt | PARTIAL |
| GetMaster | 5 | 1 | Character::_GetMaster, Character::_GetMaster | MISSING |
| HeadTo | 5 | 4 | Character::_HeadTo, Character::_HeadTo | MISSING |
| MarkAsFlying | 5 | 5 | GameObject::_MarkAsFlying, GameObject::_MarkAsFlying | MISSING |
| Rand | 5 | 1 | LuaScript::_Rand | PARTIAL |
| SetActorPosition | 5 | 4 | GameObject::_SetPosition, GameObject::_SetPosition | PARTIAL |
| target:GetID | 5 | 5 | GameObject::_GetID, GameObject::_GetID | PORTED |
| target:SetStunState | 5 | 5 | Character::_SetStunState, Character::_SetStunState | MISSING |
| victim:GetID | 5 | 2 | GameObject::_GetID, GameObject::_GetID | PORTED |

## Boss/custom script call detail

Calls preserve original source line and argument expressions, including symbolic values.

### assassin1.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin1.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassin1_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly               = false; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting   = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills         = false;    -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack  = false;    -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:18` (authored assignment): <code>MELEE_RANGE                  = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:19` (authored assignment): <code>RANGED_RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob     = 70; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob    = 50; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:27` (authored assignment): <code>ATTACK_DELAY             = 500; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:28` (authored assignment): <code>SEARCH_DELAY             = 3000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY          = 8000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                   = "AssassinFloodedCastleMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                  = "GothicusCastleHubAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin1_settings.luac:34` (authored assignment): <code>MUSIC_RESUME             = false;</code>

### assassin2.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin2.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassin2_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly               = false; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting   = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills         = false;    -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack  = false;    -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:18` (authored assignment): <code>MELEE_RANGE                  = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:19` (authored assignment): <code>RANGED_RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob     = 70; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob    = 50; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:27` (authored assignment): <code>ATTACK_DELAY             = 500; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:28` (authored assignment): <code>SEARCH_DELAY             = 3000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY          = 8000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                   = "AssassinFloodedCastleMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                  = "LighthouseTempleAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac:34` (authored assignment): <code>MUSIC_RESUME             = false;</code>

### assassin3.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin3.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassin3_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly               = false; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting   = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills         = false;    -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack  = false;    -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:18` (authored assignment): <code>MELEE_RANGE                  = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:19` (authored assignment): <code>RANGED_RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob     = 70; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob    = 50; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:27` (authored assignment): <code>ATTACK_DELAY             = 500; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:28` (authored assignment): <code>SEARCH_DELAY             = 3000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY          = 8000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                   = "AssassinAbbeyRuinsMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                  = "AbbeyRuinsOneAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac:34` (authored assignment): <code>MUSIC_RESUME             = false;</code>

### assassindemon.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassindemon.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassindemon_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly               = true; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting   = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills         = false;    -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack  = false;    -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:18` (authored assignment): <code>MELEE_RANGE                  = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:19` (authored assignment): <code>RANGED_RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob     = 70; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob    = 50; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:27` (authored assignment): <code>ATTACK_DELAY             = 3000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:28` (authored assignment): <code>SEARCH_DELAY             = 3000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY          = 8000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                   = ""; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                  = "DarkDungeonTempleAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac:34` (authored assignment): <code>MUSIC_RESUME             = false;</code>

### balrog.luac — .local-inputs/assets-extra/android/data/scripts/ai/balrog.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### balrog_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly              = false; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting  = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills            = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack = true; -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:18` (authored assignment): <code>MELEE_RANGE                 = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:19` (authored assignment): <code>RANGED_RANGE                = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob        = -1; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob   = -1; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:27` (authored assignment): <code>ATTACK_DELAY                = 3000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:28` (authored assignment): <code>SEARCH_DELAY                = 6000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY         = 5000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                  = "GiantBalrogMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                 = "WaterTempleCaveAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac:34` (authored assignment): <code>MUSIC_RESUME                = false;</code>

### darkqueen1.luac — .local-inputs/assets-extra/android/data/scripts/ai/darkqueen1.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### darkqueen1_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac

Relevant calls: 85; source lines: 533; engine readiness: 42/148.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 56 | `DoSkill` | `1=2` | global | PARTIAL |
| 58 | `DoSkill` | `1=1` | global | PARTIAL |
| 63 | `DoSkill` | `1=1` | global | PARTIAL |
| 67 | `DoSkill` | `1=1` | global | PARTIAL |
| 70 | `DoSkill` | `1=4` | global | PARTIAL |
| 72 | `DoSkill` | `1=3` | global | PARTIAL |
| 89 | `DoSkill` | `1=2` | global | PARTIAL |
| 91 | `DoSkill` | `1=0` | global | PARTIAL |
| 96 | `DoSkill` | `1=0` | global | PARTIAL |
| 100 | `DoSkill` | `1=0` | global | PARTIAL |
| 103 | `DoSkill` | `1=2` | global | PARTIAL |
| 105 | `DoSkill` | `1=3` | global | PARTIAL |
| 117 | `GetPosition` | `` | global | PORTED |
| 121 | `PlayFX` | `1=TELEPORT_FX; 2=x; 3=y; 4=z` | global | PARTIAL |
| 122 | `PlaySound` | `1="sfx_teleport"; 2=false; 3=0; 4=false` | global | PARTIAL |
| 125 | `WarpTo` | `1=0.0; 2=MOVE_RANGE; 3=0.0; 4=true` | global | PARTIAL |
| 130 | `WarpTo` | `1=0.0; 2=-MOVE_RANGE; 3=0.0; 4=true` | global | PARTIAL |
| 135 | `WarpTo` | `1=MOVE_RANGE; 2=0.0; 3=0.0; 4=true` | global | PARTIAL |
| 140 | `WarpTo` | `1=-MOVE_RANGE; 2=0.0; 3=0.0; 4=true` | global | PARTIAL |
| 148 | `GetPosition` | `` | global | PORTED |
| 149 | `PlayFX` | `1=TELEPORT_FX; 2=x1; 3=y1; 4=z1` | global | PARTIAL |
| 163 | `HasTarget` | `` | global | PORTED |
| 164 | `GetTarget` | `` | global | PORTED |
| 165 | `ClearTarget` | `` | global | PORTED |
| 168 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 169 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 170 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 172 | `TargetListSearch` | `1=RANGED_RANGE` | global | PARTIAL |
| 173 | `TargetListBackup` | `` | global | MISSING |
| 175 | `GetTargetListSize` | `` | global | MISSING |
| 177 | `IsTargetListEmpty` | `` | global | MISSING |
| 179 | `GetTargetListTop` | `` | global | PARTIAL |
| 180 | `SetTarget` | `1=target` | global | PORTED |
| 188 | `GetTargetListTop` | `` | global | PARTIAL |
| 190 | `target:HasMaster` | `` | object_method | PARTIAL |
| 193 | `target:GetMaster` | `` | object_method | MISSING |
| 199 | `PopTargetList` | `` | global | MISSING |
| 200 | `IsTargetListEmpty` | `` | global | MISSING |
| 212 | `obj:GetPropHP` | `` | object_method | PARTIAL |
| 246 | `SetTarget` | `1=myTarget` | global | PORTED |
| 259 | `ClearTarget` | `` | global | PORTED |
| 261 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 262 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 263 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 265 | `TargetListSearch` | `1=MELEE_RANGE` | global | PARTIAL |
| 267 | `IsTargetListEmpty` | `` | global | MISSING |
| 268 | `GetTargetListTop` | `` | global | PARTIAL |
| 270 | `SetTarget` | `1=target` | global | PORTED |
| 280 | `ClearTarget` | `` | global | PORTED |
| 282 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 283 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 284 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 286 | `TargetListSearch` | `1=RANGED_RANGE` | global | PARTIAL |
| 291 | `IsTargetListEmpty` | `` | global | MISSING |
| 293 | `GetTargetListTop` | `` | global | PARTIAL |
| 294 | `SetTarget` | `1=target` | global | PORTED |
| 299 | `GetTargetListTop` | `` | global | PARTIAL |
| 302 | `target:GetName` | `` | object_method | PARTIAL |
| 305 | `target:GetName` | `` | object_method | PARTIAL |
| 312 | `PopTargetList` | `` | global | MISSING |
| 313 | `IsTargetListEmpty` | `` | global | MISSING |
| 315 | `SetTarget` | `1=closestTarget` | global | PORTED |
| 379 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=isAttackReady` | imported_script_function | PARTIAL |
| 380 | `StartTimerCB` | `1=MOVE_DELAY; 2=true; 3=DoTeleport` | imported_script_function | PARTIAL |
| 381 | `StartTimerCB` | `1=RANGE_ATTACK_DELAY; 2=true; 3=isRangeAttackReady` | imported_script_function | PARTIAL |
| 384 | `StartTimerCB` | `1=SEARCH_DELAY; 2=true; 3=searchForEfficientTarget` | imported_script_function | PARTIAL |
| 388 | `StartTimerCB` | `1=SP_ATTACK_DELAY; 2=true; 3=Do_SpecialAttack` | imported_script_function | PARTIAL |
| 399 | `PlayMusic` | `1=LEVEL_MUSIC; 2=1000` | global | PARTIAL |
| 408 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 412 | `StopTimerCB` | `1=timer_teleport_flag` | imported_script_function | PARTIAL |
| 416 | `StopTimerCB` | `1=timer_rangeattack_flag` | imported_script_function | PARTIAL |
| 420 | `StopTimerCB` | `1=timed_spotTargeting` | imported_script_function | PARTIAL |
| 424 | `StopTimerCB` | `1=timed_specialAttack` | imported_script_function | PARTIAL |
| 450 | `PlayMusic` | `1=BOSS_MUSIC; 2=500` | global | PARTIAL |
| 456 | `HasTarget` | `` | global | PORTED |
| 457 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 467 | `GetTarget` | `` | global | PORTED |
| 468 | `ClearTarget` | `` | global | PORTED |
| 484 | `HasTarget` | `` | global | PORTED |
| 485 | `GetTarget` | `` | global | PORTED |
| 487 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 488 | `GetTarget` | `` | global | PORTED |
| 500 | `GetTarget` | `` | global | PORTED |
| 513 | `GetTarget` | `` | global | PORTED |
| 529 | `GetTarget` | `` | global | PORTED |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:2` (authored assignment, condition/gate): <code>local attack_flag                = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:3` (authored assignment): <code>local range_attack_flag          = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:4` (authored assignment, condition/gate): <code>local musicStarted_flag      = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:5` (authored assignment): <code>local sp_attack_flag         = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:8` (authored assignment): <code>local STATE_SKILL                = GetPyCst("AIStates", "Skill");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:9` (authored assignment): <code>local STATE_IDLE             = GetPyCst("AIStates", "Idle");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:10` (authored assignment): <code>local TELEPORT_FX               = GetPyOID("AnimatedEffectTable", "spell_dh2_prince_mage_stone_escape_magefx");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:11` (authored assignment): <code>local MOVE_RANGE                = 650.0;        -- Teleportation range</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:12` (authored assignment): <code>local MOVE_DELAY                = 14000.0;      -- Delay between teleports</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:13` (authored assignment): <code>local RANGE_ATTACK_DELAY        =  4000.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:31` (authored assignment, condition/gate): <code>attack_flag             = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:32` (authored assignment): <code>range_attack_flag       = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:33` (authored assignment, condition/gate): <code>musicStarted_flag       = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:34` (authored assignment): <code>sp_attack_flag          = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:38` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:42` (authored assignment): <code>range_attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:46` (authored assignment): <code>sp_attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:50` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:52` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:53` (condition/gate): <code>if (range_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:54` (condition/gate): <code>if (sp_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:55` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:56` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:58` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:60` (authored assignment): <code>sp_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:62` (condition/gate): <code>if (firstRangedSkill_prob == -1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:63` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:66` (condition/gate): <code>if (rnd_roll &lt; firstRangedSkill_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:67` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:69` (condition/gate): <code>if (not timed_specialAttack) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:70` (skill/timer/script action): <code>DoSkill(4);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:72` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:77` (authored assignment): <code>range_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:83` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:85` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:86` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:87` (condition/gate): <code>if ( sp_attack_flag ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:88` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:89` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:91` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:93` (authored assignment): <code>sp_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:95` (condition/gate): <code>if (firstMeleeSkill_prob == -1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:96` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:99` (condition/gate): <code>if (rnd_roll &lt; firstMeleeSkill_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:100` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:102` (condition/gate): <code>if (not timed_specialAttack) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:103` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:105` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:110` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:160` (authored assignment): <code>local t_targetList     = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:161` (authored assignment): <code>local t_rndTargetList  = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:163` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:175` (authored assignment): <code>local listSize = GetTargetListSize();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:177` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:179` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:188` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:190` (condition/gate): <code>if (target:HasMaster()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:206` (authored assignment): <code>local lowerMaxHP           = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:207` (authored assignment): <code>local lowerMaxHPtarget = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:208` (authored assignment): <code>local lowerCurrHP          = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:209` (authored assignment): <code>local lowerCurrHPtarget    = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:214` (condition/gate): <code>if (lowerMaxHPtarget == 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:215` (authored assignment): <code>lowerMaxHP = HPMax;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:216` (authored assignment): <code>lowerMaxHPtarget = obj;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:218` (condition/gate): <code>if (HPMax &lt; lowerMaxHP) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:219` (authored assignment): <code>lowerMaxHP = HPMax;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:220` (authored assignment): <code>lowerMaxHPtarget = obj;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:224` (condition/gate): <code>if (lowerCurrHPtarget == 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:225` (authored assignment): <code>lowerCurrHP = HPCurr;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:226` (authored assignment): <code>lowerCurrHPtarget = obj;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:228` (condition/gate): <code>if (HPCurr &lt; lowerCurrHP) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:229` (authored assignment): <code>lowerCurrHP = HPCurr;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:230` (authored assignment): <code>lowerCurrHPtarget = obj;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:239` (authored assignment): <code>local listCount = #t_rndTargetList;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:240` (authored assignment): <code>local targetlistCount = #t_targetList;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:244` (authored assignment): <code>local myTarget = t_rndTargetList[GetRand(1, #t_rndTargetList)];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:252` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:267` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:268` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:274` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:288` (authored assignment): <code>local closestTarget    = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:291` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:293` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:299` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:301` (condition/gate): <code>if (closestTarget == 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:302` (authored assignment): <code>closestDist = FromFixed(GetDistanceBetween(GetName(), target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:303` (authored assignment): <code>closestTarget = target;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:305` (authored assignment): <code>local distance = FromFixed(GetDistanceBetween(GetName(), target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:308` (authored assignment): <code>closestTarget = target;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:320` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:332` (callback or registration): <code>function darkqueen1_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:360` (condition/gate): <code>if (timer_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:361` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:363` (condition/gate): <code>if (timer_teleport_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:364` (authored assignment): <code>timer_teleport_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:366` (condition/gate): <code>if (timer_rangeattack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:367` (authored assignment): <code>timer_rangeattack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:369` (condition/gate): <code>if (timer_spot_targeting ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:370` (authored assignment): <code>timer_spot_targeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:372` (condition/gate): <code>if (timer_sp_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:373` (authored assignment): <code>timer_sp_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:379` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:380` (authored assignment, skill/timer/script action): <code>timer_teleport_flag = StartTimerCB(MOVE_DELAY, true, DoTeleport);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:381` (authored assignment, skill/timer/script action): <code>timer_rangeattack_flag = StartTimerCB(RANGE_ATTACK_DELAY, true, isRangeAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:383` (condition/gate): <code>if (timed_spotTargeting) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:384` (authored assignment, skill/timer/script action): <code>timer_spot_targeting = StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:387` (condition/gate): <code>if (timed_specialAttack) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:388` (authored assignment, skill/timer/script action): <code>timer_sp_attack_flag = StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:391` (callback or registration): <code>AddToVFTable("OnInitFinal", "darkqueen1_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:397` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:398` (condition/gate): <code>if (LEVEL_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:401` (skill/timer/script action): <code>CallPyScript("ResumeLevelMusic");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:403` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:407` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:408` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:409` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:411` (condition/gate): <code>if (timer_teleport_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:412` (skill/timer/script action): <code>StopTimerCB(timer_teleport_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:413` (authored assignment): <code>timer_teleport_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:415` (condition/gate): <code>if (timer_rangeattack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:416` (skill/timer/script action): <code>StopTimerCB(timer_rangeattack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:417` (authored assignment): <code>timer_rangeattack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:419` (condition/gate): <code>if (timed_spotTargeting ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:420` (skill/timer/script action): <code>StopTimerCB(timed_spotTargeting);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:421` (authored assignment): <code>timed_spotTargeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:423` (condition/gate): <code>if (timed_specialAttack ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:424` (skill/timer/script action): <code>StopTimerCB(timed_specialAttack);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:425` (authored assignment): <code>timed_specialAttack = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:431` (callback or registration): <code>function darkqueen1_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:434` (callback or registration): <code>AddToVFTable("OnDied", "darkqueen1_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:438` (callback or registration): <code>function darkqueen1_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:441` (callback or registration): <code>AddToVFTable("OnTerminate", "darkqueen1_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:445` (callback or registration): <code>function darkqueen1_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:446` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:447` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:448` (condition/gate): <code>if (not musicStarted_flag and state == STATE_IDLE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:449` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:456` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:460` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "darkqueen1_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:465` (callback or registration): <code>function darkqueen1_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:469` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:471` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "darkqueen1_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:476` (callback or registration): <code>function darkqueen1_OnTargetInSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:478` (callback or registration): <code>AddToVFTable("OnTargetInSight", "darkqueen1_OnTargetInSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:483` (callback or registration): <code>function darkqueen1_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:484` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:491` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "darkqueen1_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:498` (callback or registration): <code>function darkqueen1_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:499` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:500` (authored assignment): <code>local target = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:506` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "darkqueen1_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:508` (callback or registration): <code>function darkqueen1_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:509` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:516` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "darkqueen1_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:524` (callback or registration): <code>function darkqueen1_OnTargetInMeleeRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:525` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:532` (callback or registration): <code>AddToVFTable("OnTargetInMeleeRange", "darkqueen1_OnTargetInMeleeRange");</code>

### darkqueen1_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly                  = true; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting          = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills                = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack         = false;    -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:18` (authored assignment): <code>MELEE_RANGE                 = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:19` (authored assignment): <code>RANGED_RANGE                = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob        = -1; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob       = -1; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:27` (authored assignment): <code>ATTACK_DELAY                = 3000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:28` (authored assignment): <code>SEARCH_DELAY                = 3000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY             = 8000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                  = "DarkQueenOneMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                 = "DarkWoodHubAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac:34` (authored assignment): <code>MUSIC_RESUME                = false;</code>

### darkqueen2.luac — .local-inputs/assets-extra/android/data/scripts/ai/darkqueen2.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### darkqueen2_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly              = false; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting  = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills            = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack = true; -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:18` (authored assignment): <code>MELEE_RANGE                 = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:19` (authored assignment): <code>RANGED_RANGE                = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob        = 70; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob   = 50; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:27` (authored assignment): <code>ATTACK_DELAY                = 3000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:28` (authored assignment): <code>SEARCH_DELAY                = 3000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY         = 8000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                  = "DarkQueenTwoMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                 = "MachineRoomsAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac:34` (authored assignment): <code>MUSIC_RESUME                = false;</code>

### edwardboss.luac — .local-inputs/assets-extra/android/data/scripts/ai/edwardboss.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### edwardboss_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac

Relevant calls: 45; source lines: 356; engine readiness: 34/79.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 41 | `StopTimerCB` | `1=timer_spot_targeting` | imported_script_function | PARTIAL |
| 45 | `StopTimerCB` | `1=timer_sp_attack_flag` | imported_script_function | PARTIAL |
| 49 | `StopTimerCB` | `1=timer_teleport_flag` | imported_script_function | PARTIAL |
| 53 | `StopTimerCB` | `1=timer_rangeattack_flag` | imported_script_function | PARTIAL |
| 83 | `DoSkill` | `1=1` | global | PARTIAL |
| 85 | `DoSkill` | `1=3` | global | PARTIAL |
| 103 | `DoSkill` | `1=0` | global | PARTIAL |
| 105 | `DoSkill` | `1=3` | global | PARTIAL |
| 115 | `DoSkill` | `1=4` | global | PARTIAL |
| 120 | `DoSkill` | `1=2` | global | PARTIAL |
| 131 | `HasTarget` | `` | global | PORTED |
| 132 | `GetTarget` | `` | global | PORTED |
| 133 | `ClearTarget` | `` | global | PORTED |
| 136 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 137 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 138 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 140 | `TargetListSearch` | `1=RANGED_RANGE` | global | PARTIAL |
| 145 | `IsTargetListEmpty` | `` | global | MISSING |
| 147 | `GetTargetListTop` | `` | global | PARTIAL |
| 149 | `target:IsPlayer` | `` | object_method | PARTIAL |
| 154 | `SetTarget` | `1=target` | global | PORTED |
| 159 | `PopTargetList` | `` | global | MISSING |
| 161 | `IsTargetListEmpty` | `` | global | MISSING |
| 213 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=isAttackReady` | imported_script_function | PARTIAL |
| 214 | `StartTimerCB` | `1=MOVE_DELAY; 2=true; 3=DoTeleport` | imported_script_function | PARTIAL |
| 215 | `StartTimerCB` | `1=RANGE_ATTACK_DELAY; 2=true; 3=isRangeAttackReady` | imported_script_function | PARTIAL |
| 226 | `PlayMusic` | `1=LEVEL_MUSIC; 2=1000` | global | PARTIAL |
| 234 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 238 | `StopTimerCB` | `1=timer_spot_targeting` | imported_script_function | PARTIAL |
| 242 | `StopTimerCB` | `1=timer_sp_attack_flag` | imported_script_function | PARTIAL |
| 246 | `StopTimerCB` | `1=timer_teleport_flag` | imported_script_function | PARTIAL |
| 250 | `StopTimerCB` | `1=timer_rangeattack_flag` | imported_script_function | PARTIAL |
| 275 | `PlayMusic` | `1=BOSS_MUSIC; 2=500` | global | PARTIAL |
| 279 | `HasTarget` | `` | global | PORTED |
| 280 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 290 | `GetTarget` | `` | global | PORTED |
| 291 | `ClearTarget` | `` | global | PORTED |
| 307 | `HasTarget` | `` | global | PORTED |
| 308 | `GetTarget` | `` | global | PORTED |
| 310 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 311 | `GetTarget` | `` | global | PORTED |
| 323 | `GetTarget` | `` | global | PORTED |
| 336 | `GetTarget` | `` | global | PORTED |
| 352 | `GetTarget` | `` | global | PORTED |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:2` (authored assignment, condition/gate): <code>local attack_flag                = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:3` (authored assignment): <code>local range_attack_flag          = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:4` (authored assignment, condition/gate): <code>local musicStarted_flag      = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:5` (authored assignment): <code>local sp_attack_flag         = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:8` (authored assignment): <code>local STATE_SKILL                = GetPyCst("AIStates", "Skill");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:9` (authored assignment): <code>local STATE_IDLE             = GetPyCst("AIStates", "Idle");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:10` (authored assignment): <code>local TELEPORT_FX               = GetPyOID("AnimatedEffectTable", "spell_dh2_prince_mage_stone_escape_magefx");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:11` (authored assignment): <code>local MOVE_RANGE                = 900.0;        -- Teleportation range</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:12` (authored assignment): <code>local MOVE_DELAY                = 10000.0;      -- Delay between teleports</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:13` (authored assignment): <code>local RANGE_ATTACK_DELAY        =  4000.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:31` (authored assignment, condition/gate): <code>attack_flag         = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:32` (authored assignment): <code>range_attack_flag   = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:33` (authored assignment, condition/gate): <code>musicStarted_flag   = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:34` (authored assignment): <code>sp_attack_flag      = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:36` (condition/gate): <code>if (timer_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:37` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:38` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:40` (condition/gate): <code>if (timer_spot_targeting ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:41` (skill/timer/script action): <code>StopTimerCB(timer_spot_targeting);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:42` (authored assignment): <code>timer_spot_targeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:44` (condition/gate): <code>if (timer_sp_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:45` (skill/timer/script action): <code>StopTimerCB(timer_sp_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:46` (authored assignment): <code>timer_sp_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:48` (condition/gate): <code>if (timer_teleport_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:49` (skill/timer/script action): <code>StopTimerCB(timer_teleport_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:50` (authored assignment): <code>timer_teleport_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:52` (condition/gate): <code>if (timer_rangeattack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:53` (skill/timer/script action): <code>StopTimerCB(timer_rangeattack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:54` (authored assignment): <code>timer_rangeattack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:60` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:65` (authored assignment): <code>range_attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:70` (authored assignment): <code>sp_attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:75` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:77` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:78` (condition/gate): <code>if (range_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:82` (condition/gate): <code>if (rnd_roll &lt; firstRangedSkill_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:83` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:85` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:88` (authored assignment): <code>range_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:95` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:97` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:98` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:102` (condition/gate): <code>if (rnd_roll &lt; firstMeleeSkill_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:103` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:105` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:108` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:115` (skill/timer/script action): <code>DoSkill(4);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:120` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:128` (authored assignment): <code>local t_targetList     = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:129` (authored assignment): <code>local t_rndTargetList  = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:131` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:142` (authored assignment): <code>local target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:143` (authored assignment): <code>local targetChosen = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:145` (condition/gate): <code>if (not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:147` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:149` (condition/gate): <code>if (target:IsPlayer()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:150` (authored assignment): <code>targetChosen = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:153` (condition/gate): <code>if (targetChosen) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:170` (callback or registration): <code>function edwardboss_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:213` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:214` (authored assignment, skill/timer/script action): <code>timer_teleport_flag = StartTimerCB(MOVE_DELAY, true, DoTeleport);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:215` (authored assignment, skill/timer/script action): <code>timer_rangeattack_flag = StartTimerCB(RANGE_ATTACK_DELAY, true, isRangeAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:219` (callback or registration): <code>AddToVFTable("OnInitFinal", "edwardboss_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:223` (callback or registration): <code>local function edwardboss_OnEndGameCleanUp()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:224` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:225` (condition/gate): <code>if (LEVEL_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:228` (skill/timer/script action): <code>CallPyScript("ResumeLevelMusic");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:230` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:233` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:234` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:235` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:237` (condition/gate): <code>if (timer_spot_targeting ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:238` (skill/timer/script action): <code>StopTimerCB(timer_spot_targeting);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:239` (authored assignment): <code>timer_spot_targeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:241` (condition/gate): <code>if (timer_sp_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:242` (skill/timer/script action): <code>StopTimerCB(timer_sp_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:243` (authored assignment): <code>timer_sp_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:245` (condition/gate): <code>if (timer_teleport_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:246` (skill/timer/script action): <code>StopTimerCB(timer_teleport_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:247` (authored assignment): <code>timer_teleport_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:249` (condition/gate): <code>if (timer_rangeattack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:250` (skill/timer/script action): <code>StopTimerCB(timer_rangeattack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:251` (authored assignment): <code>timer_rangeattack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:257` (callback or registration): <code>function edwardboss_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:260` (callback or registration): <code>AddToVFTable("OnDied", "edwardboss_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:264` (callback or registration): <code>function edwardboss_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:267` (callback or registration): <code>AddToVFTable("OnTerminate", "edwardboss_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:270` (callback or registration): <code>function edwardboss_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:271` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:272` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:273` (condition/gate): <code>if (not musicStarted_flag and state == STATE_IDLE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:274` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:279` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:283` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "edwardboss_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:288` (callback or registration): <code>function edwardboss_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:292` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:294` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "edwardboss_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:299` (callback or registration): <code>function edwardboss_OnTargetInSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:301` (callback or registration): <code>AddToVFTable("OnTargetInSight", "edwardboss_OnTargetInSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:306` (callback or registration): <code>function edwardboss_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:307` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:314` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "edwardboss_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:321` (callback or registration): <code>function edwardboss_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:322` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:323` (authored assignment): <code>local target = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:329` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "edwardboss_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:331` (callback or registration): <code>function edwardboss_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:332` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:339` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "edwardboss_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:347` (callback or registration): <code>function edwardboss_OnTargetInMeleeRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:348` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_core.luac:355` (callback or registration): <code>AddToVFTable("OnTargetInMeleeRange", "edwardboss_OnTargetInMeleeRange");</code>

### edwardboss_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly                  = true; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting          = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills                = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack         = true; -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:18` (authored assignment): <code>MELEE_RANGE                 = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:19` (authored assignment): <code>RANGED_RANGE                = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob        = 85; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob       = 90; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:27` (authored assignment): <code>ATTACK_DELAY                = 3000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:28` (authored assignment): <code>SEARCH_DELAY                = 6000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY             = 8500; -- Timer special attack delay. (old value was 15000)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                  = "GiantBalrogMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                 = "WaterTempleCaveAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac:34` (authored assignment): <code>MUSIC_RESUME                = false;</code>

### emissary.luac — .local-inputs/assets-extra/android/data/scripts/ai/emissary.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### emissary_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly                = false; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting    = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills          = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack   = false;    -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:18` (authored assignment): <code>MELEE_RANGE                   = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:19` (authored assignment): <code>RANGED_RANGE              = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob      = 70; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob = 50; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:27` (authored assignment): <code>ATTACK_DELAY              = 3000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:28` (authored assignment): <code>SEARCH_DELAY              = 4000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY           = 8000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                    = "TwoHeadedDragonMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                   = "LighthouseTempleAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac:34` (authored assignment): <code>MUSIC_RESUME              = false;</code>

### flaminite.luac — .local-inputs/assets-extra/android/data/scripts/ai/flaminite.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### flaminite_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac

Relevant calls: 74; source lines: 493; engine readiness: 45/135.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 31 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 35 | `StopTimerCB` | `1=timer_spot_targeting` | imported_script_function | PARTIAL |
| 39 | `StopTimerCB` | `1=timer_sp_attack_flag` | imported_script_function | PARTIAL |
| 51 | `DoSkill` | `1=2` | global | PARTIAL |
| 67 | `PlayAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate"); 2=nil; 3=true` | global | PARTIAL |
| 70 | `DoSkill` | `1=0` | global | PARTIAL |
| 75 | `DoSkill` | `1=1` | global | PARTIAL |
| 91 | `PlayAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate"); 2=nil; 3=true` | global | PARTIAL |
| 94 | `DoSkill` | `1=0` | global | PARTIAL |
| 99 | `DoSkill` | `1=0` | global | PARTIAL |
| 119 | `HasTarget` | `` | global | PORTED |
| 120 | `GetTarget` | `` | global | PORTED |
| 121 | `ClearTarget` | `` | global | PORTED |
| 124 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 125 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 126 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 128 | `TargetListSearch` | `1=RANGED_RANGE` | global | PARTIAL |
| 129 | `TargetListBackup` | `` | global | MISSING |
| 131 | `GetTargetListSize` | `` | global | MISSING |
| 133 | `IsTargetListEmpty` | `` | global | MISSING |
| 135 | `GetTargetListTop` | `` | global | PARTIAL |
| 136 | `SetTarget` | `1=target` | global | PORTED |
| 144 | `GetTargetListTop` | `` | global | PARTIAL |
| 146 | `target:HasMaster` | `` | object_method | PARTIAL |
| 149 | `target:GetMaster` | `` | object_method | MISSING |
| 155 | `PopTargetList` | `` | global | MISSING |
| 156 | `IsTargetListEmpty` | `` | global | MISSING |
| 168 | `obj:GetPropHP` | `` | object_method | PARTIAL |
| 202 | `SetTarget` | `1=myTarget` | global | PORTED |
| 215 | `ClearTarget` | `` | global | PORTED |
| 217 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 218 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 219 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 221 | `TargetListSearch` | `1=MELEE_RANGE` | global | PARTIAL |
| 223 | `IsTargetListEmpty` | `` | global | MISSING |
| 224 | `GetTargetListTop` | `` | global | PARTIAL |
| 226 | `SetTarget` | `1=target` | global | PORTED |
| 236 | `ClearTarget` | `` | global | PORTED |
| 238 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 239 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 240 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 242 | `TargetListSearch` | `1=RANGED_RANGE` | global | PARTIAL |
| 247 | `IsTargetListEmpty` | `` | global | MISSING |
| 249 | `GetTargetListTop` | `` | global | PARTIAL |
| 250 | `SetTarget` | `1=target` | global | PORTED |
| 255 | `GetTargetListTop` | `` | global | PARTIAL |
| 258 | `target:GetName` | `` | object_method | PARTIAL |
| 261 | `target:GetName` | `` | object_method | PARTIAL |
| 268 | `PopTargetList` | `` | global | MISSING |
| 269 | `IsTargetListEmpty` | `` | global | MISSING |
| 271 | `SetTarget` | `1=closestTarget` | global | PORTED |
| 289 | `RegisterAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate")` | global | PORTED |
| 334 | `RegisterAnims` | `` | script_function | PORTED |
| 336 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=isAttackReady` | imported_script_function | PARTIAL |
| 339 | `StartTimerCB` | `1=SEARCH_DELAY; 2=true; 3=searchForEfficientTarget` | imported_script_function | PARTIAL |
| 344 | `StartTimerCB` | `1=SP_ATTACK_DELAY; 2=true; 3=Do_SpecialAttack` | imported_script_function | PARTIAL |
| 354 | `PlayMusic` | `1=LEVEL_MUSIC; 2=1000` | global | PARTIAL |
| 363 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 367 | `StopTimerCB` | `1=timer_spot_targeting` | imported_script_function | PARTIAL |
| 371 | `StopTimerCB` | `1=timer_sp_attack_flag` | imported_script_function | PARTIAL |
| 397 | `PlayMusic` | `1=BOSS_MUSIC; 2=500` | global | PARTIAL |
| 403 | `HasTarget` | `` | global | PORTED |
| 404 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 414 | `GetTarget` | `` | global | PORTED |
| 415 | `ClearTarget` | `` | global | PORTED |
| 431 | `HasTarget` | `` | global | PORTED |
| 432 | `GetTarget` | `` | global | PORTED |
| 434 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 435 | `GetTarget` | `` | global | PORTED |
| 450 | `GetTarget` | `` | global | PORTED |
| 452 | `GetTarget` | `` | global | PORTED |
| 462 | `GetTarget` | `` | global | PORTED |
| 478 | `GetTarget` | `` | global | PORTED |
| 489 | `StartTimerCB` | `1=500; 2=false; 3=performingSpecialAttack` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:2` (authored assignment, condition/gate): <code>local attack_flag             = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:3` (authored assignment, condition/gate): <code>local musicStarted_flag       = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:4` (authored assignment): <code>local sp_attack_flag          = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:8` (authored assignment): <code>local STATE_SKILL             = GetPyCst("AIStates", "Skill");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:9` (authored assignment): <code>local STATE_IDLE              = GetPyCst("AIStates", "Idle");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:12` (authored assignment): <code>local timer_attack_flag          = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:13` (authored assignment): <code>local timer_spot_targeting       = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:14` (authored assignment): <code>local timer_sp_attack_flag       = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:25` (authored assignment, condition/gate): <code>attack_flag          = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:26` (authored assignment, condition/gate): <code>musicStarted_flag    = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:27` (authored assignment): <code>sp_attack_flag       = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:30` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:31` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:32` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:34` (condition/gate): <code>if (timer_spot_targeting) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:35` (skill/timer/script action): <code>StopTimerCB(timer_spot_targeting);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:36` (authored assignment): <code>timer_spot_targeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:38` (condition/gate): <code>if (timer_sp_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:39` (skill/timer/script action): <code>StopTimerCB(timer_sp_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:40` (authored assignment): <code>timer_sp_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:46` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:51` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:56` (authored assignment): <code>sp_attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:61` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:63` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:64` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:65` (condition/gate): <code>if (sp_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:66` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:70` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:72` (authored assignment): <code>sp_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:74` (condition/gate): <code>if (firstRangedSkill_prob == -1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:75` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:78` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:85` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:87` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:88` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:89` (condition/gate): <code>if ( sp_attack_flag ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:90` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:94` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:96` (authored assignment): <code>sp_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:98` (condition/gate): <code>if (firstMeleeSkill_prob == -1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:99` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:102` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:116` (authored assignment): <code>local t_targetList      = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:117` (authored assignment): <code>local t_rndTargetList   = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:119` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:131` (authored assignment): <code>local listSize = GetTargetListSize();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:133` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:135` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:144` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:146` (condition/gate): <code>if (target:HasMaster()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:162` (authored assignment): <code>local lowerMaxHP            = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:163` (authored assignment): <code>local lowerMaxHPtarget  = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:164` (authored assignment): <code>local lowerCurrHP           = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:165` (authored assignment): <code>local lowerCurrHPtarget = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:170` (condition/gate): <code>if (lowerMaxHPtarget == 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:171` (authored assignment): <code>lowerMaxHP = HPMax;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:172` (authored assignment): <code>lowerMaxHPtarget = obj;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:174` (condition/gate): <code>if (HPMax &lt; lowerMaxHP) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:175` (authored assignment): <code>lowerMaxHP = HPMax;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:176` (authored assignment): <code>lowerMaxHPtarget = obj;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:180` (condition/gate): <code>if (lowerCurrHPtarget == 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:181` (authored assignment): <code>lowerCurrHP = HPCurr;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:182` (authored assignment): <code>lowerCurrHPtarget = obj;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:184` (condition/gate): <code>if (HPCurr &lt; lowerCurrHP) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:185` (authored assignment): <code>lowerCurrHP = HPCurr;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:186` (authored assignment): <code>lowerCurrHPtarget = obj;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:195` (authored assignment): <code>local listCount = #t_rndTargetList;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:196` (authored assignment): <code>local targetlistCount = #t_targetList;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:200` (authored assignment): <code>local myTarget = t_rndTargetList[GetRand(1, #t_rndTargetList)];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:208` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:223` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:224` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:230` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:244` (authored assignment): <code>local closestTarget = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:247` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:249` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:255` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:257` (condition/gate): <code>if (closestTarget == 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:258` (authored assignment): <code>closestDist = FromFixed(GetDistanceBetween(GetName(), target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:259` (authored assignment): <code>closestTarget = target;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:261` (authored assignment): <code>local distance = FromFixed(GetDistanceBetween(GetName(), target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:264` (authored assignment): <code>closestTarget = target;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:276` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:288` (callback or registration): <code>function RegisterAnims()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:289` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "root_troll_intimidate"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:293` (callback or registration): <code>function myBoss_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:336` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:338` (condition/gate): <code>if (timed_spotTargeting) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:339` (authored assignment, skill/timer/script action): <code>timer_spot_targeting = StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:342` (condition/gate): <code>if (timed_specialAttack) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:344` (authored assignment, skill/timer/script action): <code>timer_sp_attack_flag = StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:347` (callback or registration): <code>AddToVFTable("OnInitFinal", "myBoss_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:351` (callback or registration): <code>local function myBoss_OnEndGameCleanUp()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:352` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:353` (condition/gate): <code>if (LEVEL_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:356` (skill/timer/script action): <code>CallPyScript("ResumeLevelMusic");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:358` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:362` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:363` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:364` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:366` (condition/gate): <code>if (timer_spot_targeting) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:367` (skill/timer/script action): <code>StopTimerCB(timer_spot_targeting);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:368` (authored assignment): <code>timer_spot_targeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:370` (condition/gate): <code>if (timer_sp_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:371` (skill/timer/script action): <code>StopTimerCB(timer_sp_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:372` (authored assignment): <code>timer_sp_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:378` (callback or registration): <code>function myBoss_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:381` (callback or registration): <code>AddToVFTable("OnDied", "myBoss_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:385` (callback or registration): <code>function myBoss_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:388` (callback or registration): <code>AddToVFTable("OnTerminate", "myBoss_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:392` (callback or registration): <code>function myBoss_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:393` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:394` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:395` (condition/gate): <code>if ((not musicStarted_flag) and state == STATE_IDLE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:396` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:403` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:407` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "myBoss_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:412` (callback or registration): <code>function myBoss_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:416` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:418` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "myBoss_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:423` (callback or registration): <code>function myBoss_OnTargetInSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:425` (callback or registration): <code>AddToVFTable("OnTargetInSight", "myBoss_OnTargetInSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:430` (callback or registration): <code>function myBoss_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:431` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:438` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "myBoss_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:445` (callback or registration): <code>function myBoss_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:446` (condition/gate): <code>if (isUsingSkills and not isMeleeOnly) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:455` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "myBoss_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:457` (callback or registration): <code>function myBoss_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:458` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:465` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "myBoss_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:473` (callback or registration): <code>function myBoss_OnTargetInMeleeRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:474` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:481` (callback or registration): <code>AddToVFTable("OnTargetInMeleeRange", "myBoss_OnTargetInMeleeRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:487` (callback or registration): <code>function myBoss_OnEndOfAnim()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:489` (skill/timer/script action): <code>StartTimerCB( 500, false, performingSpecialAttack );</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:493` (callback or registration): <code>AddToVFTable("OnEndOfAnim", "myBoss_OnEndOfAnim");</code>

### flaminite_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly                   = true; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting           = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills             = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack          = true; -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:18` (authored assignment): <code>MELEE_RANGE                  = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:19` (authored assignment): <code>RANGED_RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob     = -1; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob        = -1; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:27` (authored assignment): <code>ATTACK_DELAY             = 4000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY              = 15000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                   = "FireSpritesMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                  = ""; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac:34` (authored assignment): <code>MUSIC_RESUME             = true;</code>

### flaminiteed.luac — .local-inputs/assets-extra/android/data/scripts/ai/flaminiteed.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### flaminiteed_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly             = true; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting = true; -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills           = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack    = false;    -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:18` (authored assignment): <code>MELEE_RANGE                    = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:19` (authored assignment): <code>RANGED_RANGE               = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob       = 50; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob  = -1; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:27` (authored assignment): <code>ATTACK_DELAY               = 2000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:28` (authored assignment): <code>SEARCH_DELAY               = 6000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY            = 5000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                 = ""; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                    = ""; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac:34` (authored assignment): <code>MUSIC_RESUME               = true;</code>

### madruk.luac — .local-inputs/assets-extra/android/data/scripts/ai/madruk.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### madruk_buffer.luac — .local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac

Relevant calls: 7; source lines: 98; engine readiness: 9/19.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 24 | `friend:GetName` | `` | object_method | PARTIAL |
| 24 | `g_master:GetName` | `` | object_method | PARTIAL |
| 38 | `WarpBehind` | `1=g_master` | global | PARTIAL |
| 45 | `DoSkill` | `1=0` | global | PARTIAL |
| 71 | `StartTimerCB` | `1=TIMER_DELAY; 2=false; 3=Do_Buff` | imported_script_function | PARTIAL |
| 79 | `StopTimerCB` | `1=g_heal_timer` | imported_script_function | PARTIAL |
| 88 | `StopTimerCB` | `1=g_heal_timer` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:11` (authored assignment): <code>local TIMER_DELAY     = 3250;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:12` (authored assignment): <code>local skill_check     = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:13` (authored assignment): <code>local g_heal_timer = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:15` (callback or registration): <code>function madruk_buffer_OnFriendSpotted(friend)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:29` (callback or registration): <code>AddToVFTable("OnFriendSpotted", "madruk_buffer_OnFriendSpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:37` (callback or registration): <code>function madruk_buffer_OnMasterOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:40` (callback or registration): <code>AddToVFTable("OnMasterOutOfSight", "madruk_buffer_OnMasterOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:45` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:46` (authored assignment): <code>skill_check = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:47` (authored assignment): <code>g_heal_timer = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:51` (callback or registration): <code>function madruk_buffer_OnMasterOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:54` (callback or registration): <code>AddToVFTable("OnMasterOutOfRange", "madruk_buffer_OnMasterOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:57` (callback or registration): <code>function madruk_buffer_OnMasterInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:59` (callback or registration): <code>AddToVFTable("OnMasterInRangedRange", "madruk_buffer_OnMasterInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:62` (callback or registration): <code>function madruk_buffer_OnMasterInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:64` (callback or registration): <code>AddToVFTable("OnMasterInCloseRange", "madruk_buffer_OnMasterInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:67` (callback or registration): <code>function madruk_buffer_OnMasterInMeleeRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:68` (condition/gate): <code>if(skill_check == true) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:70` (authored assignment): <code>skill_check = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:71` (authored assignment, skill/timer/script action): <code>g_heal_timer = StartTimerCB(TIMER_DELAY, false, Do_Buff);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:74` (callback or registration): <code>AddToVFTable("OnMasterInMeleeRange", "madruk_buffer_OnMasterInMeleeRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:77` (callback or registration): <code>function madruk_buffer_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:78` (condition/gate): <code>if (g_heal_timer ~= -1 and g_heal_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:79` (skill/timer/script action): <code>StopTimerCB(g_heal_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:80` (authored assignment): <code>g_heal_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:83` (callback or registration): <code>AddToVFTable("OnDied", "madruk_buffer_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:86` (callback or registration): <code>function madruk_buffer_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:87` (condition/gate): <code>if (g_heal_timer ~= -1 and g_heal_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:88` (skill/timer/script action): <code>StopTimerCB(g_heal_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:89` (authored assignment): <code>g_heal_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:92` (callback or registration): <code>AddToVFTable("OnTerminate", "madruk_buffer_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:95` (callback or registration): <code>function madruk_buffer_OnMasterDied()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_buffer.luac:98` (callback or registration): <code>AddToVFTable("OnMasterDied", "madruk_buffer_OnMasterDied");</code>

### madruk_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac

Relevant calls: 27; source lines: 441; engine readiness: 27/103.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 66 | `StopTimerCB` | `1=timer_summonHealer_flag` | imported_script_function | PARTIAL |
| 70 | `StopTimerCB` | `1=timer_summonBuffer_flag` | imported_script_function | PARTIAL |
| 74 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 78 | `StopTimerCB` | `1=timer_helper_flag` | imported_script_function | PARTIAL |
| 84 | `object:GetState` | `` | object_method | PORTED |
| 180 | `DoSkill` | `1=2` | global | PARTIAL |
| 182 | `DoSkill` | `1=3` | global | PARTIAL |
| 191 | `DoSkill` | `1=0` | global | PARTIAL |
| 193 | `DoSkill` | `1=1` | global | PARTIAL |
| 218 | `StartTimerCB` | `1=RESPAWN_DELAY; 2=false; 3=SummonHelpers; 4="Healer"` | imported_script_function | PARTIAL |
| 226 | `StartTimerCB` | `1=RESPAWN_DELAY; 2=false; 3=SummonHelpers; 4="Buffer"` | imported_script_function | PARTIAL |
| 296 | `PlayMusic` | `1="HellKingTwoLoopMusic"; 2=500` | global | PARTIAL |
| 299 | `HasTarget` | `` | global | PORTED |
| 301 | `SetTarget` | `1=enemy` | global | PORTED |
| 325 | `GetTarget` | `` | global | PORTED |
| 348 | `GetTarget` | `` | global | PORTED |
| 364 | `DoSkill` | `1=0` | global | PARTIAL |
| 389 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=isAttackReady` | imported_script_function | PARTIAL |
| 390 | `StartTimerCB` | `1=HELPER_CHECK_DELAY; 2=true; 3=isHelpersAlive` | imported_script_function | PARTIAL |
| 400 | `HasTarget` | `` | global | PORTED |
| 401 | `GetTarget` | `` | global | PORTED |
| 402 | `ClearTarget` | `` | global | PORTED |
| 420 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 421 | `StopTimerCB` | `1=timer_helper_flag` | imported_script_function | PARTIAL |
| 422 | `StopTimerCB` | `1=timer_summonBuffer_flag` | imported_script_function | PARTIAL |
| 423 | `StopTimerCB` | `1=timer_summonHealer_flag` | imported_script_function | PARTIAL |
| 432 | `PlaySound` | `1="HellKingThreeIntroMusic"; 2=false; 3=0; 4=false` | global | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:2` (authored assignment, condition/gate): <code>local attack_flag            = true; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:3` (authored assignment, condition/gate): <code>local first_melee_flag       = true; -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:4` (authored assignment, condition/gate): <code>local summon_flag            = true; -- checks if the character's helpers can be summoned.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:5` (authored assignment, condition/gate): <code>local musicStarted_flag      = false; -- checks if the boss music has started or not.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:6` (authored assignment, condition/gate): <code>local canUseDyingAttack      = true; -- check if the boss can perform his dying attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:14` (authored assignment): <code>local meleeAttack_prob      = 50; -- chances in PCT to perform a melee attack when in melee range.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:15` (authored assignment): <code>local meleeAtkInRanged_prob = 30; -- chances in PCT to perform a melee attack in ranged range.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:16` (authored assignment): <code>local rangedAttack_prob     = 50; -- chances in PCT to perform a ranged attack in ranged range.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:17` (authored assignment): <code>local normalAttack_prob     = 70; -- chances in PCT to perform a regular attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:21` (authored assignment): <code>local ATTACK_DELAY          = 2000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:22` (authored assignment): <code>local RESPAWN_DELAY         = 17000; -- Timer for character's helpers respawns.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:23` (authored assignment, condition/gate): <code>local HELPER_CHECK_DELAY    = 5000; -- Timer to check if helpers are still alive.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:30` (authored assignment): <code>local helpers_array = { Healer, Buffer };</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:37` (authored assignment, condition/gate): <code>local allowed                   = false;    -- check if the NPc is really spawned before allowing it to attack and move</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:41` (authored assignment, condition/gate): <code>attack_flag             = true; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:42` (authored assignment, condition/gate): <code>first_melee_flag        = true; -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:43` (authored assignment, condition/gate): <code>summon_flag             = true; -- checks if the character's helpers can be summoned.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:44` (authored assignment, condition/gate): <code>musicStarted_flag       = false; -- checks if the boss music has started or not.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:45` (authored assignment, condition/gate): <code>canUseDyingAttack       = true; -- check if the boss can perform his dying attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:54` (authored assignment): <code>meleeAttack_prob = 50; -- chances in PCT to perform a melee attack when in melee range.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:55` (authored assignment): <code>meleeAtkInRanged_prob = 30; -- chances in PCT to perform a melee attack in ranged range.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:56` (authored assignment): <code>rangedAttack_prob = 50; -- chances in PCT to perform a ranged attack in ranged range.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:57` (authored assignment): <code>normalAttack_prob = 70; -- chances in PCT to perform a regular attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:61` (authored assignment): <code>ATTACK_DELAY = 2000; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:62` (authored assignment): <code>RESPAWN_DELAY = 15000; -- Timer for character's helpers respawns.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:63` (authored assignment, condition/gate): <code>HELPER_CHECK_DELAY = 5000; -- Timer to check if helpers are still alive.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:64` (authored assignment, condition/gate): <code>allowed                 = false;    -- check if the NPc is really spawned before allowing it to attack and move</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:65` (condition/gate): <code>if (timer_summonHealer_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:66` (skill/timer/script action): <code>StopTimerCB(timer_summonHealer_flag)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:67` (authored assignment): <code>timer_summonHealer_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:69` (condition/gate): <code>if (timer_summonBuffer_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:70` (skill/timer/script action): <code>StopTimerCB(timer_summonBuffer_flag)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:71` (authored assignment): <code>timer_summonBuffer_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:73` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:74` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:75` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:77` (condition/gate): <code>if (timer_helper_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:78` (skill/timer/script action): <code>StopTimerCB(timer_helper_flag)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:79` (authored assignment): <code>timer_helper_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:84` (authored assignment): <code>local state = object:GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:85` (condition/gate): <code>if( state == GetPyCst("AIStates", "Dead")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:86` (condition/gate): <code>if (traces) then Trace("--&gt; State: Dead"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:89` (condition/gate): <code>if( state == GetPyCst("AIStates", "Idle")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:90` (condition/gate): <code>if (traces) then Trace("--&gt; State: Idle"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:93` (condition/gate): <code>if( state == GetPyCst("AIStates", "Limbus")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:94` (condition/gate): <code>if (traces) then Trace("--&gt; State: Limbus"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:97` (condition/gate): <code>if( state == GetPyCst("AIStates", "Spawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:98` (condition/gate): <code>if (traces) then Trace("--&gt; State: Spawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:101` (condition/gate): <code>if( state == GetPyCst("AIStates", "Despawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:102` (condition/gate): <code>if (traces) then Trace("--&gt; State: Despawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:105` (condition/gate): <code>if( state == GetPyCst("AIStates", "Move")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:106` (condition/gate): <code>if (traces) then Trace("--&gt; State: Move"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:109` (condition/gate): <code>if( state == GetPyCst("AIStates", "Attack")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:110` (condition/gate): <code>if (traces) then Trace("--&gt; State: Attack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:113` (condition/gate): <code>if( state == GetPyCst("AIStates", "Skill")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:114` (condition/gate): <code>if (traces) then Trace("--&gt; State: Skill"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:117` (condition/gate): <code>if( state == GetPyCst("AIStates", "Cast")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:118` (condition/gate): <code>if (traces) then Trace("--&gt; State: Cast"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:121` (condition/gate): <code>if( state == GetPyCst("AIStates", "Scared")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:122` (condition/gate): <code>if (traces) then Trace("--&gt; State: Scared"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:125` (condition/gate): <code>if( state == GetPyCst("AIStates", "Stunned")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:126` (condition/gate): <code>if (traces) then Trace("--&gt; State: Stunned"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:129` (condition/gate): <code>if( state == GetPyCst("AIStates", "KnockedBack")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:130` (condition/gate): <code>if (traces) then Trace("--&gt; State: KnockedBack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:134` (condition/gate): <code>if( state == GetPyCst("AIStates", "Injured")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:135` (condition/gate): <code>if (traces) then Trace("--&gt; State: Injured"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:138` (condition/gate): <code>if( state == GetPyCst("AIStates", "Interact")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:139` (condition/gate): <code>if (traces) then Trace("--&gt; State: Interact"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:142` (condition/gate): <code>if( state == GetPyCst("AIStates", "Anim")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:143` (condition/gate): <code>if (traces) then Trace("--&gt; State: Anim"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:146` (condition/gate): <code>if( state == GetPyCst("AIStates", "Reviving")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:147` (condition/gate): <code>if (traces) then Trace("--&gt; State: Reviving"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:150` (condition/gate): <code>if( state == GetPyCst("AIStates", "Revived")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:151` (condition/gate): <code>if (traces) then Trace("--&gt; State: Revived"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:154` (condition/gate): <code>if( state == GetPyCst("AIStates", "PreSpawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:155` (condition/gate): <code>if (traces) then Trace("--&gt; State: PreSpawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:162` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:171` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:175` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:177` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:179` (condition/gate): <code>if (prob_pct &lt; normalAttack_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:180` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:182` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:187` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:188` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:190` (condition/gate): <code>if (prob_pct &lt; normalAttack_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:191` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:193` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:198` (condition/gate): <code>if (helperType == "ALL") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:199` (authored assignment): <code>helpers_array.Healer = Summon(g_healer_oid, false, -460.0, -460.0, 0.0, true);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:200` (authored assignment): <code>helpers_array.Buffer = Summon(g_buffer_oid, false, 460.0, -460.0, 0.0, true);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:204` (authored assignment): <code>helpers_array.Healer = Summon(g_healer_oid, false, -460.0, -460.0, 0.0, true);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:207` (authored assignment): <code>helpers_array.Buffer = Summon(g_buffer_oid, false, 460.0, -460.0, 0.0, true);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:214` (condition/gate): <code>if (helpers_array.Healer ~= "NONE" and helpers_array.Healer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:215` (condition/gate): <code>if (helpers_array.Healer:GetHP() &lt;= 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:217` (authored assignment): <code>helpers_array.Healer = "NONE";</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:218` (authored assignment, skill/timer/script action): <code>timer_summonHealer_flag = StartTimerCB(RESPAWN_DELAY, false, SummonHelpers, "Healer");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:222` (condition/gate): <code>if (helpers_array.Buffer ~= "NONE" and helpers_array.Buffer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:223` (condition/gate): <code>if (helpers_array.Buffer:GetHP() &lt;= 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:225` (authored assignment): <code>helpers_array.Buffer = "NONE";</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:226` (authored assignment, skill/timer/script action): <code>timer_summonBuffer_flag = StartTimerCB(RESPAWN_DELAY, false, SummonHelpers, "Buffer");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:239` (callback or registration): <code>function madruk_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:282` (callback or registration): <code>AddToVFTable("OnInit", "madruk_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:287` (callback or registration): <code>function madruk_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:289` (condition/gate): <code>if (summon_flag == true) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:290` (authored assignment): <code>summon_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:294` (condition/gate): <code>if (not musicStarted_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:295` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:299` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:305` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "madruk_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:310` (callback or registration): <code>function madruk_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:316` (condition/gate): <code>if(state == GetPyCst("AIStates", "Idle")</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:321` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:322` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:324` (condition/gate): <code>if(prob_pct &lt; meleeAtkInRanged_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:332` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "madruk_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:337` (callback or registration): <code>function madruk_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:339` (condition/gate): <code>if(state == GetPyCst("AIStates", "Idle")</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:344` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:345` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:347` (condition/gate): <code>if(prob_pct &lt; meleeAtkInRanged_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:349` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:356` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "madruk_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:361` (callback or registration): <code>function madruk_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:363` (condition/gate): <code>if (first_melee_flag == true and attack_flag == true) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:364` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:371` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "madruk_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:376` (callback or registration): <code>function madruk_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:380` (condition/gate): <code>if(HPPercentCurr &lt;= 50) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:382` (skill/timer/script action): <code>CallPyScript("end_MadrukPhase2");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:386` (authored assignment): <code>local check = getAIState(GetSelf(), false);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:389` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:390` (authored assignment, skill/timer/script action): <code>timer_helper_flag = StartTimerCB(HELPER_CHECK_DELAY, true, isHelpersAlive);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:395` (callback or registration): <code>AddToVFTable("OnUpdate", "madruk_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:398` (callback or registration): <code>function madruk_OnTargetDied()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:400` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:405` (callback or registration): <code>AddToVFTable("OnTargetDied", "madruk_OnTargetDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:408` (callback or registration): <code>local function madruk_OnEndGameCleanUp()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:409` (condition/gate): <code>if ((helpers_array.Healer ~= nil) and (helpers_array.Healer ~= "NONE")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:412` (authored assignment): <code>helpers_array.Healer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:414` (condition/gate): <code>if ((helpers_array.Buffer ~= nil) and (helpers_array.Buffer ~= "NONE")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:417` (authored assignment): <code>helpers_array.Buffer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:420` (skill/timer/script action, condition/gate): <code>if (timer_attack_flag) then      StopTimerCB(timer_attack_flag); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:421` (skill/timer/script action, condition/gate): <code>if (timer_helper_flag) then      StopTimerCB(timer_helper_flag); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:422` (skill/timer/script action, condition/gate): <code>if (timer_summonBuffer_flag) then    StopTimerCB(timer_summonBuffer_flag); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:423` (skill/timer/script action, condition/gate): <code>if (timer_summonHealer_flag) then    StopTimerCB(timer_summonHealer_flag); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:427` (callback or registration): <code>function madruk_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:429` (skill/timer/script action): <code>CallPyScript("end_MadrukPhase2");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:435` (callback or registration): <code>AddToVFTable("OnDied", "madruk_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:438` (callback or registration): <code>function madruk_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:441` (callback or registration): <code>AddToVFTable("OnTerminate", "madruk_OnTerminate");</code>

### madruk_healer.luac — .local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac

Relevant calls: 7; source lines: 109; engine readiness: 10/20.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 31 | `friend:GetName` | `` | object_method | PARTIAL |
| 31 | `g_master:GetName` | `` | object_method | PARTIAL |
| 45 | `WarpBehind` | `1=g_master` | global | PARTIAL |
| 52 | `DoSkill` | `1=0` | global | PARTIAL |
| 82 | `StartTimerCB` | `1=TIMER_DELAY; 2=false; 3=Do_Healing` | imported_script_function | PARTIAL |
| 90 | `StopTimerCB` | `1=g_heal_timer` | imported_script_function | PARTIAL |
| 99 | `StopTimerCB` | `1=g_heal_timer` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:11` (authored assignment): <code>local TIMER_DELAY     = 9000;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:12` (authored assignment): <code>local skill_check     = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:13` (authored assignment): <code>local g_heal_timer        = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:17` (authored assignment): <code>skill_check       = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:18` (authored assignment): <code>g_heal_timer  = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:22` (callback or registration): <code>function madruk_healer_OnFriendSpotted(friend)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:36` (callback or registration): <code>AddToVFTable("OnFriendSpotted", "madruk_healer_OnFriendSpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:44` (callback or registration): <code>function madruk_healer_OnMasterOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:47` (callback or registration): <code>AddToVFTable("OnMasterOutOfSight", "madruk_healer_OnMasterOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:52` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:53` (authored assignment): <code>skill_check = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:54` (authored assignment): <code>g_heal_timer = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:57` (callback or registration): <code>function madruk_healer_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:60` (callback or registration): <code>AddToVFTable("OnInitFinal", "madruk_healer_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:62` (callback or registration): <code>function madruk_healer_OnMasterOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:65` (callback or registration): <code>AddToVFTable("OnMasterOutOfRange", "madruk_healer_OnMasterOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:68` (callback or registration): <code>function madruk_healer_OnMasterInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:70` (callback or registration): <code>AddToVFTable("OnMasterInRangedRange", "madruk_healer_OnMasterInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:73` (callback or registration): <code>function madruk_healer_OnMasterInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:75` (callback or registration): <code>AddToVFTable("OnMasterInCloseRange", "madruk_healer_OnMasterInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:78` (callback or registration): <code>function madruk_healer_OnMasterInMeleeRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:79` (condition/gate): <code>if(skill_check == true) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:81` (authored assignment): <code>skill_check = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:82` (authored assignment, skill/timer/script action): <code>g_heal_timer = StartTimerCB(TIMER_DELAY, false, Do_Healing);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:85` (callback or registration): <code>AddToVFTable("OnMasterInMeleeRange", "madruk_healer_OnMasterInMeleeRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:88` (callback or registration): <code>function madruk_healer_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:89` (condition/gate): <code>if (g_heal_timer ~= -1 and g_heal_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:90` (skill/timer/script action): <code>StopTimerCB(g_heal_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:91` (authored assignment): <code>g_heal_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:94` (callback or registration): <code>AddToVFTable("OnDied", "madruk_healer_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:97` (callback or registration): <code>function madruk_healer_OnTerminate(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:98` (condition/gate): <code>if (g_heal_timer ~= -1 and g_heal_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:99` (skill/timer/script action): <code>StopTimerCB(g_heal_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:100` (authored assignment): <code>g_heal_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:103` (callback or registration): <code>AddToVFTable("OnTerminate", "madruk_healer_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:106` (callback or registration): <code>function madruk_healer_OnMasterDied()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_healer.luac:109` (callback or registration): <code>AddToVFTable("OnMasterDied", "madruk_healer_OnMasterDied");</code>

### madruk_torso.luac — .local-inputs/assets-extra/android/data/scripts/ai/madruk_torso.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### madruk_torso_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac

Relevant calls: 13; source lines: 313; engine readiness: 15/78.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 69 | `object:GetState` | `` | object_method | PORTED |
| 179 | `DoSkill` | `1=1` | global | PARTIAL |
| 190 | `DoSkill` | `1=2` | global | PARTIAL |
| 198 | `DoSkill` | `1=0` | global | PARTIAL |
| 218 | `PlayMusic` | `1=MUSIC; 2=500` | global | PARTIAL |
| 220 | `HasTarget` | `` | global | PORTED |
| 222 | `SetTarget` | `1=enemy` | global | PORTED |
| 233 | `GetTarget` | `` | global | PORTED |
| 244 | `GetTarget` | `` | global | PORTED |
| 289 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=Attack` | imported_script_function | PARTIAL |
| 301 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 303 | `PlayMusic` | `1="DarkDungeonTempleAmbientMusic"; 2=500` | global | PARTIAL |
| 310 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:3` (authored assignment): <code>local drainAttack_probModifier = 40;       -- chances in PCT that can be applied as a probability bonus.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:4` (authored assignment): <code>local drainAttack_pct          = 30;       -- Drain attack activation on health %</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:8` (authored assignment, condition/gate): <code>local hp_flag                  = false;    -- checks if the hp has been initialized to 50%.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:9` (authored assignment, condition/gate): <code>local range                        = false;    -- checks if the character is in range to attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:10` (authored assignment, condition/gate): <code>local musicStarted_flag           = false;    -- checks if the boss music has started or not.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:11` (authored assignment, condition/gate): <code>local allowed                 = false;    -- check if the NPc is really spawned before allowing it to attack and move</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:14` (authored assignment): <code>local MUSIC                       = "HellKingThreeLoopMusic";</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:17` (authored assignment): <code>local ATTACK_DELAY                = 2000;     -- Timer attack delay. ! DO NOT TOUCH will affect all the other attacks timing</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:18` (authored assignment): <code>local ATTACK_LASER                = 5;        -- Multiplier delay from the standard attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:19` (authored assignment): <code>local WAIT_LASER              = 2;        -- Turns to wait before attacking again when itinitating the laser attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:20` (authored assignment): <code>local ATTACK_DRAIN                = 3;        -- Multiplier delay from the standard attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:21` (authored assignment): <code>local WAIT_DRAIN              = 4;        -- Turns to wait before attacking again when initiating the drain attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:24` (authored assignment): <code>local timer_attack_flag           = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:25` (authored assignment): <code>local counter_attack1             = 0;        -- count to check before doint the attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:26` (authored assignment): <code>local counter_attack2         = 0;        -- count to check before doint the attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:27` (authored assignment): <code>local wait                        = 0;        -- Count turn to wait before doing the std attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:36` (authored assignment): <code>drainAttack_probModifier  = 40; -- chances in PCT that can be applied as a probability bonus.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:37` (authored assignment): <code>drainAttack_pct               = 30; -- Drain attack activation on health %</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:41` (authored assignment, condition/gate): <code>hp_flag                   = false;    -- checks if the hp has been initialized to 50%.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:42` (authored assignment, condition/gate): <code>range                     = false;    -- checks if the character is in range to attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:43` (authored assignment, condition/gate): <code>musicStarted_flag         = false;    -- checks if the boss music has started or not.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:44` (authored assignment, condition/gate): <code>allowed                       = false;    -- check if the NPc is really spawned before allowing it to attack and move</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:47` (authored assignment): <code>MUSIC                     = "HellKingThreeLoopMusic";</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:49` (authored assignment): <code>ATTACK_DELAY  = 1500;     -- Timer attack delay used for the primary melee attack. ! DO NOT TOUCH will affect all the other attacks timing</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:50` (authored assignment): <code>ATTACK_LASER  = 5;        -- Multiplier delay from the standard attack (6 = 9 sec)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:51` (authored assignment): <code>WAIT_LASER        = 2;        -- Turns to wait before attacking again when itinitating the laser attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:52` (authored assignment): <code>ATTACK_DRAIN  = 4;        -- Multiplier delay from the standard attack     (4 = 6 sec)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:53` (authored assignment): <code>WAIT_DRAIN        = 4;        -- Turns to wait before attacking again when initiating the drain attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:56` (authored assignment): <code>counter_attack1       = 0;    -- count to check before doint the attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:57` (authored assignment): <code>counter_attack2       = 0;    -- count to check before doint the attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:58` (authored assignment): <code>wait                      = 0;  -- Count turn to wait before doing the std attack</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:60` (authored assignment): <code>timer_attack_flag         = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:69` (authored assignment): <code>local state = object:GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:70` (condition/gate): <code>if( state == GetPyCst("AIStates", "Dead")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:71` (condition/gate): <code>if (traces) then Trace("--&gt; State: Dead"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:74` (condition/gate): <code>if( state == GetPyCst("AIStates", "Idle")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:75` (condition/gate): <code>if (traces) then Trace("--&gt; State: Idle"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:78` (condition/gate): <code>if( state == GetPyCst("AIStates", "Limbus")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:79` (condition/gate): <code>if (traces) then Trace("--&gt; State: Limbus"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:82` (condition/gate): <code>if( state == GetPyCst("AIStates", "Spawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:83` (condition/gate): <code>if (traces) then Trace("--&gt; State: Spawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:86` (condition/gate): <code>if( state == GetPyCst("AIStates", "Despawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:87` (condition/gate): <code>if (traces) then Trace("--&gt; State: Despawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:90` (condition/gate): <code>if( state == GetPyCst("AIStates", "Move")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:91` (condition/gate): <code>if (traces) then Trace("--&gt; State: Move"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:94` (condition/gate): <code>if( state == GetPyCst("AIStates", "Attack")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:95` (condition/gate): <code>if (traces) then Trace("--&gt; State: Attack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:98` (condition/gate): <code>if( state == GetPyCst("AIStates", "Skill")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:99` (condition/gate): <code>if (traces) then Trace("--&gt; State: Skill"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:102` (condition/gate): <code>if( state == GetPyCst("AIStates", "Cast")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:103` (condition/gate): <code>if (traces) then Trace("--&gt; State: Cast"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:106` (condition/gate): <code>if( state == GetPyCst("AIStates", "Scared")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:107` (condition/gate): <code>if (traces) then Trace("--&gt; State: Scared"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:110` (condition/gate): <code>if( state == GetPyCst("AIStates", "Stunned")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:111` (condition/gate): <code>if (traces) then Trace("--&gt; State: Stunned"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:114` (condition/gate): <code>if( state == GetPyCst("AIStates", "KnockedBack")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:115` (condition/gate): <code>if (traces) then Trace("--&gt; State: KnockedBack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:119` (condition/gate): <code>if( state == GetPyCst("AIStates", "Injured")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:120` (condition/gate): <code>if (traces) then Trace("--&gt; State: Injured"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:123` (condition/gate): <code>if( state == GetPyCst("AIStates", "Interact")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:124` (condition/gate): <code>if (traces) then Trace("--&gt; State: Interact"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:127` (condition/gate): <code>if( state == GetPyCst("AIStates", "Anim")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:128` (condition/gate): <code>if (traces) then Trace("--&gt; State: Anim"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:131` (condition/gate): <code>if( state == GetPyCst("AIStates", "Reviving")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:132` (condition/gate): <code>if (traces) then Trace("--&gt; State: Reviving"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:135` (condition/gate): <code>if( state == GetPyCst("AIStates", "Revived")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:136` (condition/gate): <code>if (traces) then Trace("--&gt; State: Revived"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:139` (condition/gate): <code>if( state == GetPyCst("AIStates", "PreSpawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:140` (condition/gate): <code>if (traces) then Trace("--&gt; State: PreSpawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:159` (condition/gate): <code>if (HPPercentCurr &lt; drainAttack_pct) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:171` (authored assignment): <code>counter_attack1 = counter_attack1 + 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:172` (authored assignment): <code>counter_attack2 = counter_attack2 + 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:174` (condition/gate): <code>if (traces) then Trace ("Waiting before starting another attack"..wait); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:177` (condition/gate): <code>if (counter_attack1 &gt;= ATTACK_LASER) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:179` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:180` (authored assignment): <code>counter_attack1 = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:181` (authored assignment): <code>wait = WAIT_LASER; -- estimated attack duration</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:182` (condition/gate): <code>if (traces) then Trace("Madruk decide to do a Laser attack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:184` (condition/gate): <code>if (counter_attack2 &gt;= ATTACK_DRAIN) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:186` (authored assignment): <code>counter_attack2 = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:187` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:188` (condition/gate): <code>if (traces) then Trace ("Is "..prob_pct.." &lt; "..drainAttack_probModifier.."?"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:189` (condition/gate): <code>if (Do_HealthCheck() or (prob_pct &lt; drainAttack_probModifier)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:190` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:191` (authored assignment): <code>wait = WAIT_DRAIN; -- estimated attack duration</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:192` (condition/gate): <code>if (traces) then Trace("Madruk decide to do a Drain attack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:196` (condition/gate): <code>if (traces) then Trace ("States of combat: counter 1:"..counter_attack1.." Counter 2:"..counter_attack2.." wait is:"..wait); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:197` (condition/gate): <code>if (range and wait==0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:198` (skill/timer/script action): <code>DoSkill(0); -- Bite</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:199` (condition/gate): <code>if (traces) then Trace("Madruk decide to do a bite attack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:206` (callback or registration): <code>function madrukTorso_OnInitFinal()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:209` (callback or registration): <code>AddToVFTable("OnInitFinal", "madrukTorso_OnInitFinal");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:214` (callback or registration): <code>function madrukTorso_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:216` (condition/gate): <code>if (not musicStarted_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:217` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:220` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:226` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "madrukTorso_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:231` (callback or registration): <code>function madrukTorso_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:234` (authored assignment): <code>range = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:237` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "madrukTorso_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:242` (callback or registration): <code>function madrukTorso_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:245` (authored assignment): <code>range = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:248` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "madrukTorso_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:253` (callback or registration): <code>function madrukTorso_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:254` (authored assignment): <code>range = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:256` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "madrukTorso_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:261` (callback or registration): <code>function madrukTorso_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:264` (condition/gate): <code>if(hp_flag == false ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:267` (authored assignment): <code>local HPAmount = (HPMax / 2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:270` (authored assignment): <code>hp_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:275` (condition/gate): <code>if (HPPercentCurr == 100 and hp_flag == true) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:278` (authored assignment): <code>local HPAmount = (HPMax / 2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:281` (authored assignment): <code>hp_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:285` (authored assignment): <code>local check = getAIState(GetSelf(), false);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:288` (condition/gate): <code>if (timer_attack_flag==-1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:289` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, Attack);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:294` (callback or registration): <code>AddToVFTable("OnUpdate", "madrukTorso_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:299` (callback or registration): <code>function madrukTorso_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:300` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:301` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:305` (callback or registration): <code>AddToVFTable("OnDied", "madrukTorso_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:308` (callback or registration): <code>function madrukTorso_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:309` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:310` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/madruk_torso_core.luac:313` (callback or registration): <code>AddToVFTable("OnTerminate", "madrukTorso_OnTerminate");</code>

### miniboss.luac — .local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac

Relevant calls: 0; source lines: 22; engine readiness: 1/5.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac:11` (callback or registration): <code>function myOnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac:20` (callback or registration): <code>AddToVFTable("OnInit", "myOnInit");</code>

### miniboss_state.luac — .local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac

Relevant calls: 9; source lines: 98; engine readiness: 16/31.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 48 | `StartTimer` | `1=5000` | global | PORTED |
| 51 | `StartTimer` | `1=5000` | global | PORTED |
| 53 | `GetTarget` | `` | global | PORTED |
| 64 | `StartTimer` | `1=GetRand(3000, 7000)` | global | PORTED |
| 67 | `DoSkill` | `1=0` | global | PARTIAL |
| 68 | `StartTimer` | `1=GetRand(3000, 7000)` | global | PORTED |
| 70 | `GetTarget` | `` | global | PORTED |
| 88 | `GetTarget` | `` | global | PORTED |
| 93 | `RegisterActorAnim` | `1=GetPyOID("AnimDict", "balrog_idle_02")` | global | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:19` (callback or registration): <code>AddToVFTable("OnTimer",         "StateBasicOnTimer");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:20` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "StateBasicInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:21` (callback or registration): <code>AddToVFTable("OnTargetInMeleeRange", "StateBasicInMeleeRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:22` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "StateBasicOnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:25` (authored assignment): <code>StateBasicTimerID = -2;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:36` (callback or registration): <code>function StateBasicOnTimer(timerId)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:38` (condition/gate): <code>if (timerId == StateBasicTimerID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:39` (authored assignment): <code>StateBasicTimerID = -1;      -- Timer expires</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:44` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:46` (condition/gate): <code>if(CanAttackInMelee()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:47` (condition/gate): <code>if(StateBasicTimerID == -2) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:48` (authored assignment, skill/timer/script action): <code>StateBasicTimerID = StartTimer(5000);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:49` (condition/gate): <code>else if(StateBasicTimerID == -1 and state == GetPyCst("AIStates", "Idle") ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:51` (authored assignment, skill/timer/script action): <code>StateBasicTimerID = StartTimer(5000);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:60` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:62` (condition/gate): <code>if(CanAttackFromRange()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:63` (condition/gate): <code>if(StateBasicTimerID == -2) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:64` (authored assignment, skill/timer/script action): <code>StateBasicTimerID = StartTimer(GetRand(3000, 7000));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:66` (condition/gate): <code>if(StateBasicTimerID == -1 and state == GetPyCst("AIStates", "Idle") ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:67` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:68` (authored assignment, skill/timer/script action): <code>StateBasicTimerID = StartTimer(GetRand(3000, 7000));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:75` (callback or registration): <code>function StateBasicOnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:76` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:78` (condition/gate): <code>if(   state == GetPyCst("AIStates", "Idle")</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:87` (authored assignment): <code>StateBasicTimerID = -2;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:92` (callback or registration): <code>function StateBasicOnInitFinal()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:93` (callback or registration): <code>RegisterActorAnim(GetPyOID("AnimDict", "balrog_idle_02"));</code>

### rene.luac — .local-inputs/assets-extra/android/data/scripts/ai/rene.luac

Relevant calls: 9; source lines: 251; engine readiness: 21/51.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 70 | `SetTarget` | `1=t` | global | PORTED |
| 78 | `ClearTarget` | `` | global | PORTED |
| 86 | `WarpTo` | `1=g_master` | global | PARTIAL |
| 149 | `GetTarget` | `` | global | PORTED |
| 173 | `DropTarget` | `` | local_function | PORTED |
| 200 | `StartTimerCB` | `1=200; 2=false; 3=CaughtUp` | imported_script_function | PARTIAL |
| 213 | `GrabTarget` | `1=enemy` | local_function | PORTED |
| 220 | `DropTarget` | `` | local_function | PORTED |
| 226 | `DropTarget` | `` | local_function | PORTED |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:19` (authored assignment): <code>local IDLE_DELAY       = 8000;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:20` (authored assignment): <code>local CATCH_DELAY      = 500;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:21` (authored assignment): <code>local WARP_DELAY       = 2000;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:23` (authored assignment): <code>local CST_IDLE         = GetPyCst("AIStates", "Idle");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:24` (authored assignment): <code>local CST_MOVE         = GetPyCst("AIStates", "Move");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:28` (authored assignment): <code>local g_target             = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:33` (authored assignment): <code>local g_catchup_timer  = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:40` (authored assignment): <code>g_target           = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:45` (authored assignment): <code>g_catchup_timer        = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:52` (callback or registration): <code>function rene_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:64` (callback or registration): <code>AddToVFTable("OnInitFinal", "rene_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:68` (condition/gate): <code>if(not g_is_far and not g_is_catching_up and not g_target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:69` (authored assignment): <code>g_target = t;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:76` (condition/gate): <code>if(g_target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:77` (authored assignment): <code>g_target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:85` (condition/gate): <code>if (traces) then Trace("[RENE] Couldn't find a path to our master... extreme fallback solution for now is... TELEPORTATION!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:121` (authored assignment): <code>g_catchup_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:136` (callback or registration): <code>function rene_OnUpdate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:149` (authored assignment): <code>g_target = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:151` (callback or registration): <code>AddToVFTable("OnUpdate", "rene_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:159` (callback or registration): <code>function rene_OnFriendSpotted(friend)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:165` (callback or registration): <code>AddToVFTable("OnFriendSpotted", "rene_OnFriendSpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:168` (callback or registration): <code>function rene_OnMasterOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:175` (callback or registration): <code>AddToVFTable("OnMasterOutOfSight", "rene_OnMasterOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:178` (callback or registration): <code>function rene_OnMasterInSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:179` (condition/gate): <code>if(not g_target and not g_is_catching_up) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:183` (callback or registration): <code>AddToVFTable("OnMasterInSight", "rene_OnMasterInSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:187` (callback or registration): <code>function rene_OnMasterInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:189` (condition/gate): <code>if(not g_target and not g_is_catching_up) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:193` (callback or registration): <code>AddToVFTable("OnMasterInRangedRange", "rene_OnMasterInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:196` (callback or registration): <code>function rene_OnMasterInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:199` (condition/gate): <code>if (not g_catchup_timer and g_is_catching_up) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:200` (authored assignment, skill/timer/script action): <code>g_catchup_timer = StartTimerCB(200, false, CaughtUp);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:203` (callback or registration): <code>AddToVFTable("OnMasterInCloseRange", "rene_OnMasterInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:210` (callback or registration): <code>function rene_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:216` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "rene_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:219` (callback or registration): <code>function rene_OnTargetDied()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:222` (callback or registration): <code>AddToVFTable("OnTargetDied", "rene_OnTargetDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:225` (callback or registration): <code>function rene_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:228` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "rene_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:231` (callback or registration): <code>function rene_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:234` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "rene_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:237` (callback or registration): <code>function rene_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:238` (condition/gate): <code>if(GetState() == CST_MOVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:244` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "rene_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:247` (callback or registration): <code>function rene_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:251` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "rene_OnTargetInCloseRange");</code>

### resurrectinggod.luac — .local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac

Relevant calls: 8; source lines: 143; engine readiness: 11/31.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 54 | `StopTimerCB` | `1=m_timerID` | imported_script_function | PARTIAL |
| 58 | `DoSkill` | `1=0` | global | PARTIAL |
| 77 | `PlayMusic` | `1="HellKingOneLoopMusic"; 2=500` | global | PARTIAL |
| 90 | `StopTimer` | `1=m_timerID` | global | PORTED |
| 96 | `StartTimerCB` | `1=g_timer; 2=true; 3=ResurrectingGodTimer` | imported_script_function | PARTIAL |
| 111 | `defender:GetID` | `` | object_method | PORTED |
| 128 | `StopTimerCB` | `1=m_timerID` | imported_script_function | PARTIAL |
| 139 | `StopTimerCB` | `1=m_timerID` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:13` (authored assignment): <code>local g_timer               = 1000;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:15` (authored assignment): <code>local hasTimerStarted       = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:20` (authored assignment, condition/gate): <code>local musicStarted_flag     = false; -- checks if the boss music has started or not.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:21` (authored assignment): <code>local hasKilledSelf         = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:28` (authored assignment): <code>g_timer             = 1000;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:30` (authored assignment): <code>hasTimerStarted     = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:31` (authored assignment): <code>m_timerID           = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:33` (authored assignment, condition/gate): <code>musicStarted_flag   = false; -- checks if the boss music has started or not.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:34` (authored assignment): <code>hasKilledSelf       = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:45` (skill/timer/script action): <code>CallPyScript("start_phase_01_event_01");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:47` (skill/timer/script action): <code>CallPyScript("start_phase_01_event_02");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:49` (skill/timer/script action): <code>CallPyScript("start_phase_01_event_03");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:54` (skill/timer/script action): <code>StopTimerCB(m_timerID);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:55` (authored assignment): <code>m_timerID = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:56` (authored assignment): <code>hasKilledSelf = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:57` (authored assignment): <code>hasTimerStarted = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:58` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:65` (callback or registration): <code>function ResurrectingGod_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:69` (callback or registration): <code>AddToVFTable("OnInitFinal", "ResurrectingGod_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:74` (callback or registration): <code>function ResurrectingGod_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:75` (condition/gate): <code>if (not musicStarted_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:76` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:80` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "ResurrectingGod_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:85` (callback or registration): <code>function ResurrectingGod_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:88` (condition/gate): <code>if (not hasTimerStarted) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:89` (condition/gate): <code>if (m_timerID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:90` (skill/timer/script action): <code>StopTimer(m_timerID);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:92` (authored assignment): <code>m_timerID = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:96` (authored assignment, skill/timer/script action): <code>m_timerID = StartTimerCB(g_timer, true, ResurrectingGodTimer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:97` (authored assignment): <code>hasTimerStarted = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:103` (condition/gate): <code>if(HPPercentCurr &lt; 100 ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:107` (callback or registration): <code>AddToVFTable("OnUpdate", "ResurrectingGod_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:109` (callback or registration): <code>function ResurrectingGod_OnTargetHit(attacker, defender)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:113` (condition/gate): <code>if(HPPercentCurr &lt; 100 ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:118` (callback or registration): <code>AddToVFTable("OnTargetHit", "ResurrectingGod_OnTargetHit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:122` (callback or registration): <code>function ResurrectingGod_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:123` (condition/gate): <code>if (not hasKilledSelf) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:124` (skill/timer/script action): <code>CallPyScript("end_MadrukPhase1");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:127` (condition/gate): <code>if (m_timerID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:128` (skill/timer/script action): <code>StopTimerCB(m_timerID);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:129` (authored assignment): <code>m_timerID = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:130` (authored assignment): <code>hasTimerStarted    = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:133` (callback or registration): <code>AddToVFTable("OnDied", "ResurrectingGod_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:137` (callback or registration): <code>function ResurrectingGod_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:138` (condition/gate): <code>if (m_timerID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:139` (skill/timer/script action): <code>StopTimerCB(m_timerID);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:140` (authored assignment): <code>m_timerID = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/resurrectinggod.luac:143` (callback or registration): <code>AddToVFTable("OnTerminate", "ResurrectingGod_OnTerminate");</code>

### roottroll.luac — .local-inputs/assets-extra/android/data/scripts/ai/roottroll.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### roottroll_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac

Relevant calls: 48; source lines: 386; engine readiness: 37/93.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 52 | `DoSkill` | `1=2` | global | PARTIAL |
| 57 | `DoSkill` | `1=3` | global | PARTIAL |
| 73 | `PlayAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate_02"); 2=nil; 3=true` | global | PARTIAL |
| 76 | `PlayAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate_02"); 2=nil; 3=true` | global | PARTIAL |
| 81 | `PlayAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate_02"); 2=nil; 3=true` | global | PARTIAL |
| 97 | `PlayAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate_02"); 2=nil; 3=true` | global | PARTIAL |
| 100 | `DoSkill` | `1=0` | global | PARTIAL |
| 105 | `DoSkill` | `1=0` | global | PARTIAL |
| 109 | `DoSkill` | `1=0` | global | PARTIAL |
| 111 | `PlayAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate_02"); 2=nil; 3=true` | global | PARTIAL |
| 131 | `HasTarget` | `` | global | PORTED |
| 132 | `GetTarget` | `` | global | PORTED |
| 133 | `ClearTarget` | `` | global | PORTED |
| 136 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 137 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 138 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 140 | `TargetListSearch` | `1=RANGED_RANGE` | global | PARTIAL |
| 145 | `IsTargetListEmpty` | `` | global | MISSING |
| 147 | `GetTargetListTop` | `` | global | PARTIAL |
| 149 | `target:IsPlayer` | `` | object_method | PARTIAL |
| 154 | `SetTarget` | `1=target` | global | PORTED |
| 159 | `PopTargetList` | `` | global | MISSING |
| 161 | `IsTargetListEmpty` | `` | global | MISSING |
| 167 | `RegisterAnim` | `1=GetPyOID("AnimDict", "root_troll_intimidate_02")` | global | PORTED |
| 216 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=isAttackReady` | imported_script_function | PARTIAL |
| 219 | `StartTimerCB` | `1=SEARCH_DELAY; 2=true; 3=searchForEfficientTarget` | imported_script_function | PARTIAL |
| 224 | `StartTimerCB` | `1=SP_ATTACK_DELAY; 2=true; 3=Do_SpecialAttack` | imported_script_function | PARTIAL |
| 227 | `RegisterAnims` | `` | script_function | PORTED |
| 235 | `PlayMusic` | `1=LEVEL_MUSIC; 2=1000` | global | PARTIAL |
| 243 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 247 | `StopTimerCB` | `1=timer_spot_targeting` | imported_script_function | PARTIAL |
| 251 | `StopTimerCB` | `1=timer_sp_attack_flag` | imported_script_function | PARTIAL |
| 274 | `PlayMusic` | `1=BOSS_MUSIC; 2=500` | global | PARTIAL |
| 280 | `HasTarget` | `` | global | PORTED |
| 281 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 290 | `GetTarget` | `` | global | PORTED |
| 291 | `ClearTarget` | `` | global | PORTED |
| 307 | `HasTarget` | `` | global | PORTED |
| 308 | `GetTarget` | `` | global | PORTED |
| 310 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 311 | `GetTarget` | `` | global | PORTED |
| 326 | `GetTarget` | `` | global | PORTED |
| 328 | `GetTarget` | `` | global | PORTED |
| 340 | `GetTarget` | `` | global | PORTED |
| 356 | `GetTarget` | `` | global | PORTED |
| 368 | `StartTimerCB` | `1=500; 2=false; 3=performingSpecialMeleeAttack` | imported_script_function | PARTIAL |
| 373 | `StartTimerCB` | `1=500; 2=false; 3=performingSpecialAttack` | imported_script_function | PARTIAL |
| 378 | `StartTimerCB` | `1=500; 2=false; 3=performingSpecialAttack` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:2` (authored assignment, condition/gate): <code>local attack_flag             = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:3` (authored assignment, condition/gate): <code>local musicStarted_flag       = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:4` (authored assignment): <code>local sp_attack_flag          = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:6` (authored assignment): <code>local intimidateRange_flag        = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:10` (authored assignment): <code>local STATE_SKILL                = GetPyCst("AIStates", "Skill");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:11` (authored assignment): <code>local STATE_IDLE             = GetPyCst("AIStates", "Idle");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:27` (authored assignment, condition/gate): <code>attack_flag              = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:28` (authored assignment, condition/gate): <code>musicStarted_flag        = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:29` (authored assignment): <code>sp_attack_flag           = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:31` (authored assignment): <code>intimidateRange_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:34` (condition/gate): <code>if (timer_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:35` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:37` (condition/gate): <code>if (timer_spot_targeting ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:38` (authored assignment): <code>timer_spot_targeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:40` (condition/gate): <code>if (timer_sp_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:41` (authored assignment): <code>timer_sp_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:47` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:52` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:57` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:62` (authored assignment): <code>sp_attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:67` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:69` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:70` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:71` (condition/gate): <code>if (sp_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:72` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:77` (authored assignment): <code>intimidateRange_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:79` (authored assignment): <code>sp_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:82` (authored assignment): <code>intimidateRange_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:84` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:91` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:93` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:94` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:95` (condition/gate): <code>if ( sp_attack_flag ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:96` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:100` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:102` (authored assignment): <code>sp_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:104` (condition/gate): <code>if (firstMeleeSkill_prob == -1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:105` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:108` (condition/gate): <code>if (rnd_roll &lt; firstMeleeSkill_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:109` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:116` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:131` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:142` (authored assignment): <code>local targetChosen = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:143` (authored assignment): <code>local target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:145` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:147` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:149` (condition/gate): <code>if (target:IsPlayer()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:150` (authored assignment): <code>targetChosen = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:153` (condition/gate): <code>if (targetChosen) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:166` (callback or registration): <code>function RegisterAnims()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:167` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "root_troll_intimidate_02"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:174` (callback or registration): <code>function myBoss_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:216` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:218` (condition/gate): <code>if (timed_spotTargeting) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:219` (authored assignment, skill/timer/script action): <code>timer_spot_targeting = StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:222` (condition/gate): <code>if (timed_specialAttack) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:224` (authored assignment, skill/timer/script action): <code>timer_sp_attack_flag = StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:229` (callback or registration): <code>AddToVFTable("OnInitFinal", "myBoss_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:232` (callback or registration): <code>local function myBoss_OnEndGameCleanUp()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:233` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:234` (condition/gate): <code>if (LEVEL_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:237` (skill/timer/script action): <code>CallPyScript("ResumeLevelMusic");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:239` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:242` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:243` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:244` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:246` (condition/gate): <code>if (timer_spot_targeting) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:247` (skill/timer/script action): <code>StopTimerCB(timer_spot_targeting);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:248` (authored assignment): <code>timer_spot_targeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:250` (condition/gate): <code>if (timer_sp_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:251` (skill/timer/script action): <code>StopTimerCB(timer_sp_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:252` (authored assignment): <code>timer_sp_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:257` (callback or registration): <code>function myBoss_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:260` (callback or registration): <code>AddToVFTable("OnDied", "myBoss_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:263` (callback or registration): <code>function myBoss_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:266` (callback or registration): <code>AddToVFTable("OnTerminate", "myBoss_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:269` (callback or registration): <code>function myBoss_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:270` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:271` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:272` (condition/gate): <code>if (not musicStarted_flag and state == STATE_IDLE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:273` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:280` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:284` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "myBoss_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:288` (callback or registration): <code>function myBoss_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:292` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:294` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "myBoss_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:299` (callback or registration): <code>function myBoss_OnTargetInSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:301` (callback or registration): <code>AddToVFTable("OnTargetInSight", "myBoss_OnTargetInSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:306` (callback or registration): <code>function myBoss_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:307` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:314` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "myBoss_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:321` (callback or registration): <code>function myBoss_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:322` (condition/gate): <code>if (isUsingSkills and not isMeleeOnly) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:331` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "myBoss_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:335` (callback or registration): <code>function myBoss_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:336` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:343` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "myBoss_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:351` (callback or registration): <code>function myBoss_OnTargetInMeleeRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:352` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:359` (callback or registration): <code>AddToVFTable("OnTargetInMeleeRange", "myBoss_OnTargetInMeleeRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:366` (callback or registration): <code>function myBoss_OnEndOfAnim()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:368` (skill/timer/script action): <code>StartTimerCB( 500, false, performingSpecialMeleeAttack );</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:373` (skill/timer/script action): <code>StartTimerCB( 500, false, performingSpecialAttack );</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:377` (condition/gate): <code>if ( intimidateRange_flag ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:378` (skill/timer/script action): <code>StartTimerCB( 500, false, performingSpecialAttack );</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:379` (authored assignment): <code>intimidateRange_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_core.luac:382` (callback or registration): <code>AddToVFTable("OnEndOfAnim", "myBoss_OnEndOfAnim");</code>

### roottroll_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly                   = false; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting           = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills             = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack          = true; -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:18` (authored assignment): <code>MELEE_RANGE                  = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:19` (authored assignment): <code>RANGED_RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob     = 70; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob        = -1; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:27` (authored assignment): <code>ATTACK_DELAY             = 4500; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:28` (authored assignment): <code>SEARCH_DELAY             = 4000; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY              = 12000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:32` (authored assignment): <code>BOSS_MUSIC                   = "EarthTrollMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC                  = ""; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac:34` (authored assignment): <code>MUSIC_RESUME             = true;</code>

### sandworm01.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac

Relevant calls: 0; source lines: 75; engine readiness: 4/8.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:13` (authored assignment, condition/gate): <code>attack_flag              = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:14` (authored assignment, condition/gate): <code>first_melee_flag     = true;     -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:15` (authored assignment, condition/gate): <code>fa_phase1_flag           = true;     -- checks if a first action can be performed when switching to phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:16` (authored assignment, condition/gate): <code>fa_phase2_flag           = true;     -- checks if a first action can be performed when switching to phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:17` (authored assignment, condition/gate): <code>SE_event1_flag           = true;     -- checks if special event 1 can be performed (Enters weakness state at HP &lt; 75%).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:18` (authored assignment, condition/gate): <code>SE_event2_flag           = true;     -- checks if special event 2 can be performed (Enters weakness state at HP &lt; 50%).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:20` (authored assignment, condition/gate): <code>diveAnim_flag            = false; -- checks if the OnEndOfAnim event can be used to check the dive anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:22` (authored assignment, condition/gate): <code>intimidateAnim_flag  = 0; -- checks if the OnEndOfAnim event can be used to check the intimidate anim status and which action to take after.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:26` (authored assignment): <code>timer_attack_flag            = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:27` (authored assignment): <code>timer_outOfRange_flag    = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:28` (authored assignment): <code>timer_dive_flag          = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:29` (authored assignment): <code>timer_intimidate_flag    = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:36` (authored assignment): <code>ST_WEAK          = 3;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:38` (authored assignment): <code>SK_STATE         = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:40` (authored assignment): <code>SK_PHASE_ID      = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:44` (authored assignment): <code>MELEE_RANGE      = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:45` (authored assignment): <code>RANGED_RANGE = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:48` (authored assignment): <code>normalRangedAttack_prob      = 35;                   -- chances in PCT to perform a regular ranged attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:49` (authored assignment): <code>normalMeleeAttack1_prob  = 50;               -- chances in PCT to perform a regular melee attack in phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:50` (authored assignment): <code>normalMeleeAttack2_prob      = {20, 20, 60}; -- chances in PCT to perform one of the 3 regular melee/ranged attacks in phase 2 - !Cumulative values - MUST MATCH 100%.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:53` (authored assignment): <code>firstMeleeAttack2_prob       = 50;               -- chances in PCT to perform a first regular melee attack in phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:55` (authored assignment): <code>phase2_initPct                   = 75;                   -- HP PCT of the character at when to init phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:56` (authored assignment): <code>sk_spawn_points              = {                 -- Spawn point names table for the Dive attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:66` (authored assignment): <code>ATTACK_DELAY                 = 1000;     -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:67` (authored assignment): <code>WEAK_DELAY                   = 6000; -- Timer for weak state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:68` (authored assignment): <code>DIVE_DELAY                   = 2000; -- Timer for dive state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:69` (authored assignment): <code>INTIMIDATE_DELAY         = 3000; -- Timer for intimidate state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:70` (authored assignment): <code>OUTOFRANGE_MIN_DELAY     = 500;  -- Timer for out of range minimum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac:71` (authored assignment): <code>OUTOFRANGE_MAX_DELAY     = 2000; -- Timer for out of range maximum reaction delay.</code>

### sandworm02.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac

Relevant calls: 0; source lines: 76; engine readiness: 4/8.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:13` (authored assignment, condition/gate): <code>attack_flag              = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:14` (authored assignment, condition/gate): <code>first_melee_flag     = true;     -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:15` (authored assignment, condition/gate): <code>fa_phase1_flag           = true;     -- checks if a first action can be performed when switching to phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:16` (authored assignment, condition/gate): <code>fa_phase2_flag           = true;     -- checks if a first action can be performed when switching to phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:17` (authored assignment, condition/gate): <code>SE_event1_flag           = true;     -- checks if special event 1 can be performed (Enters weakness state at HP &lt; 75%).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:18` (authored assignment, condition/gate): <code>SE_event2_flag           = true;     -- checks if special event 2 can be performed (Enters weakness state at HP &lt; 50%).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:20` (authored assignment, condition/gate): <code>diveAnim_flag            = false; -- checks if the OnEndOfAnim event can be used to check the dive anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:22` (authored assignment, condition/gate): <code>intimidateAnim_flag      = 0; -- checks if the OnEndOfAnim event can be used to check the intimidate anim status and which action to take after.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:26` (authored assignment): <code>timer_attack_flag        = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:27` (authored assignment): <code>timer_outOfRange_flag    = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:28` (authored assignment): <code>timer_dive_flag          = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:29` (authored assignment): <code>timer_intimidate_flag    = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:36` (authored assignment): <code>ST_WEAK          = 3;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:38` (authored assignment): <code>SK_STATE         = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:40` (authored assignment): <code>SK_PHASE_ID      = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:44` (authored assignment): <code>MELEE_RANGE      = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:45` (authored assignment): <code>RANGED_RANGE = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:48` (authored assignment): <code>normalRangedAttack_prob      = 35;                   -- chances in PCT to perform a regular ranged attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:49` (authored assignment): <code>normalMeleeAttack1_prob  = 50;               -- chances in PCT to perform a regular melee attack in phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:50` (authored assignment): <code>normalMeleeAttack2_prob      = {20, 20, 60}; -- chances in PCT to perform one of the 3 regular melee/ranged attacks in phase 2 - !Cumulative values - MUST MATCH 100%.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:53` (authored assignment): <code>firstMeleeAttack2_prob       = 50;               -- chances in PCT to perform a first regular melee attack in phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:55` (authored assignment): <code>phase2_initPct                   = 75;                   -- HP PCT of the character at when to init phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:56` (authored assignment): <code>sk_spawn_points              = {                 -- Spawn point names table for the Dive attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:66` (authored assignment): <code>ATTACK_DELAY                 = 1000;     -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:67` (authored assignment): <code>WEAK_DELAY                   = 6000; -- Timer for weak state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:68` (authored assignment): <code>DIVE_DELAY                   = 2000; -- Timer for dive state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:69` (authored assignment): <code>INTIMIDATE_DELAY         = 3000; -- Timer for intimidate state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:70` (authored assignment): <code>OUTOFRANGE_MIN_DELAY     = 500;  -- Timer for out of range minimum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac:71` (authored assignment): <code>OUTOFRANGE_MAX_DELAY     = 2000; -- Timer for out of range maximum reaction delay.</code>

### sandworm_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac

Relevant calls: 69; source lines: 484; engine readiness: 42/124.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 14 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 18 | `StopTimerCB` | `1=timer_outOfRange_flag` | imported_script_function | PARTIAL |
| 22 | `StopTimerCB` | `1=timer_dive_flag` | imported_script_function | PARTIAL |
| 26 | `StopTimerCB` | `1=timer_intimidate_flag` | imported_script_function | PARTIAL |
| 37 | `HasTarget` | `` | global | PORTED |
| 38 | `GetTarget` | `` | global | PORTED |
| 39 | `ClearTarget` | `` | global | PORTED |
| 42 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 43 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 44 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 46 | `TargetListSearch` | `1=RANGED_RANGE` | global | PARTIAL |
| 51 | `IsTargetListEmpty` | `` | global | MISSING |
| 53 | `GetTargetListTop` | `` | global | PARTIAL |
| 55 | `target:IsPlayer` | `` | object_method | PARTIAL |
| 60 | `SetTarget` | `1=target` | global | PORTED |
| 65 | `PopTargetList` | `` | global | MISSING |
| 67 | `IsTargetListEmpty` | `` | global | MISSING |
| 78 | `HasTarget` | `` | global | PORTED |
| 79 | `LookAtTarget` | `` | global | PARTIAL |
| 96 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_idle")` | global | PORTED |
| 97 | `RegisterAnim` | `1=GetPyOID("AnimDict", "sandworm_rewind_hole")` | global | PORTED |
| 98 | `RegisterAnim` | `1=GetPyOID("AnimDict", "sandworm_rewind_hole_idle")` | global | PORTED |
| 99 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_emerge")` | global | PORTED |
| 127 | `DoSkill` | `1=2` | global | PARTIAL |
| 137 | `DoSkill` | `1=1` | global | PARTIAL |
| 143 | `DoSkill` | `1=1` | global | PARTIAL |
| 146 | `DoSkill` | `1=0` | global | PARTIAL |
| 160 | `DoSkill` | `1=0` | global | PARTIAL |
| 162 | `DoSkill` | `1=1` | global | PARTIAL |
| 168 | `DoSkill` | `1=0` | global | PARTIAL |
| 170 | `DoSkill` | `1=1` | global | PARTIAL |
| 189 | `target:GetName` | `` | object_method | PARTIAL |
| 192 | `target:GetName` | `` | object_method | PARTIAL |
| 225 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 228 | `GetTarget` | `` | global | PORTED |
| 232 | `CheckForClosestSpawnPoint` | `1=target` | script_function | PORTED |
| 234 | `CheckForClosestSpawnPoint` | `1=target` | script_function | PORTED |
| 237 | `SetActorPosition` | `1=targetSpot` | global | PARTIAL |
| 241 | `DoSkill` | `1=3` | global | PARTIAL |
| 245 | `SetIsTargetable` | `1=true` | global | PARTIAL |
| 256 | `SetIsActorTargetable` | `1=false` | global | MISSING |
| 258 | `HasTarget` | `` | global | PORTED |
| 259 | `GetTarget` | `` | global | PORTED |
| 265 | `PlayAnim` | `1=GetPyOID("AnimDict", "sandworm_rewind_hole")` | global | PARTIAL |
| 334 | `RegisterAnims` | `` | script_function | PORTED |
| 337 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=isAttackReady` | imported_script_function | PARTIAL |
| 343 | `HasTarget` | `` | global | PORTED |
| 345 | `SetTarget` | `1=enemy` | global | PORTED |
| 354 | `StartTimerCB` | `1=OUTOFRANGE_DELAY; 2=false; 3=SetSwampkingInDiveState` | imported_script_function | PARTIAL |
| 365 | `Do_RangedAttackPhase1` | `` | script_function | PORTED |
| 369 | `Do_RangedAttackPhase2` | `` | script_function | PORTED |
| 379 | `DoSkill` | `1=1` | global | PARTIAL |
| 383 | `Do_MeleeAttackPhase1` | `` | script_function | PORTED |
| 387 | `Do_MeleeAttackPhase2` | `` | script_function | PORTED |
| 413 | `HasTarget` | `` | global | PORTED |
| 414 | `GetTarget` | `` | global | PORTED |
| 415 | `ClearTarget` | `` | global | PORTED |
| 423 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 427 | `StopTimerCB` | `1=timer_outOfRange_flag` | imported_script_function | PARTIAL |
| 431 | `StopTimerCB` | `1=timer_dive_flag` | imported_script_function | PARTIAL |
| 435 | `StopTimerCB` | `1=timer_intimidate_flag` | imported_script_function | PARTIAL |
| 440 | `GetTarget` | `` | global | PORTED |
| 441 | `GetTarget` | `` | global | PORTED |
| 442 | `ClearTarget` | `` | global | PORTED |
| 463 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 465 | `StartTimerCB` | `1=DIVE_DELAY; 2=false; 3=SetSwampkingInIdleState` | imported_script_function | PARTIAL |
| 475 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 477 | `StartTimerCB` | `1=500; 2=false; 3=function() DoSkill(2); end` | imported_script_function | PARTIAL |
| 477 | `DoSkill` | `1=2` | global | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:5` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:7` (authored assignment, condition/gate): <code>attack_flag            = false;    -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:8` (authored assignment, condition/gate): <code>first_melee_flag   = true;     -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:9` (authored assignment, condition/gate): <code>diveAnim_flag      = false;    -- checks if the OnEndOfAnim event can be used to check the dive anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:10` (authored assignment): <code>intimidateAnim_flag   = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:13` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:14` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:15` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:17` (condition/gate): <code>if (timer_outOfRange_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:18` (skill/timer/script action): <code>StopTimerCB(timer_outOfRange_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:19` (authored assignment): <code>timer_outOfRange_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:21` (condition/gate): <code>if (timer_dive_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:22` (skill/timer/script action): <code>StopTimerCB(timer_dive_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:23` (authored assignment): <code>timer_Dive_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:25` (condition/gate): <code>if (timer_intimidate_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:26` (skill/timer/script action): <code>StopTimerCB(timer_intimidate_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:27` (authored assignment): <code>timer_intimidate_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:37` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:48` (authored assignment): <code>local target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:49` (authored assignment): <code>local targetChosen = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:51` (condition/gate): <code>if (not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:53` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:55` (condition/gate): <code>if (target:IsPlayer()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:56` (authored assignment): <code>targetChosen = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:59` (condition/gate): <code>if (targetChosen) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:76` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:78` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:82` (condition/gate): <code>if (SK_STATE == ST_IDLE and state ~= GetPyCst("AIStates", "Skill")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:83` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Attack is ready!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:84` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:86` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Preventing attack to be ready!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:87` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:95` (callback or registration): <code>function RegisterAnims()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:96` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:97` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "sandworm_rewind_hole"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:98` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "sandworm_rewind_hole_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:99` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_emerge"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:109` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Ranged Attack phase 1, random roll on the attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:111` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:113` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:115` (condition/gate): <code>if (prob_pct &lt; normalRangedAttack_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:118` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(3) - Dive)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:124` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Ranged Attack phase 2 - DoSkill(2) - Spit"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:125` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:127` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:132` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Melee Attack phase 1, checking for initial melee attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:133` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:136` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is - first melee attack - DoSkill(1) - Bite 2)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:137` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:139` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:141` (condition/gate): <code>if (prob_pct &lt; normalMeleeAttack1_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:142` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(1) - Bite 2)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:143` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:145` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(0) - Bite 1)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:146` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:152` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Melee Attack phase 2, checking for initial melee attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:153` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:157` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:159` (condition/gate): <code>if (prob_pct &lt; firstMeleeAttack2_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:160` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:162` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:165` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:167` (condition/gate): <code>if (prob_pct &lt; normalMeleeAttack2_prob[1]) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:168` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:170` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:187` (authored assignment): <code>objectName = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:188` (condition/gate): <code>if (target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:189` (authored assignment): <code>local short_distance = FromFixed(GetDistanceBetween(sk_spawn_points[1], target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:191` (condition/gate): <code>if (obj) then -- Strangely OBJ sometimes becomes "nil"</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:192` (authored assignment): <code>local distance = FromFixed(GetDistanceBetween(obj, target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:194` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Player: "..target:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:206` (authored assignment): <code>objectName = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:207` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] No Target. Fallback: "..sk_spawn_points[1]); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:209` (condition/gate): <code>if (objectName==nil) then Trace("WARNING!!! NOT WORKING!"); objectName = sk_spawn_points[3]; end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:220` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going back in IDLE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:222` (condition/gate): <code>if (SK_STATE == ST_DIVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:224` (condition/gate): <code>if (not HasTarget) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:228` (authored assignment): <code>local target = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:231` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:232` (authored assignment): <code>targetSpot = CheckForClosestSpawnPoint(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:234` (authored assignment): <code>targetSpot = CheckForClosestSpawnPoint(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:239` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:241` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:244` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:251` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:253` (condition/gate): <code>if (SK_STATE == ST_IDLE and (state ~= GetPyCst("AIStates", "Skill"))) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:254` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into DIVE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:258` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:262` (authored assignment): <code>SK_STATE = ST_DIVE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:263` (authored assignment): <code>diveAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:270` (condition/gate): <code>if (SK_STATE ~= ST_INTIMIDATE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:271` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into INTIMIDATE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:273` (authored assignment): <code>SK_STATE = ST_INTIMIDATE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:274` (authored assignment): <code>intimidateAnim_flag = nextAction;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:287` (callback or registration): <code>function sandworm_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:324` (condition/gate): <code>if (timer_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:325` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:332` (authored assignment): <code>SK_PHASE_ID = 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:336` (condition/gate): <code>if (debugTrace_flag) then Trace("Starting attack timer..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:337` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:339` (callback or registration): <code>AddToVFTable("OnInitFinal", "sandworm_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:342` (callback or registration): <code>function sandworm_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:343` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:348` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "sandworm_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:351` (callback or registration): <code>function sandworm_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:352` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:353` (authored assignment): <code>OUTOFRANGE_DELAY = GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:354` (authored assignment, skill/timer/script action): <code>timer_outOfRange_flag = StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:357` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "sandworm_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:360` (callback or registration): <code>function sandworm_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:363` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:364` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:368` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:373` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "sandworm_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:376` (callback or registration): <code>function sandworm_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:377` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:378` (condition/gate): <code>if (first_melee_flag and attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:379` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:381` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:386` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:391` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "sandworm_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:394` (callback or registration): <code>function sandworm_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:399` (condition/gate): <code>if(SK_PHASE_ID == 1 and HPPercentCurr &lt;= phase2_initPct ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:400` (authored assignment): <code>SK_PHASE_ID = 2;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:402` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:404` (condition/gate): <code>if (SK_STATE ~= ST_DIVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:409` (callback or registration): <code>AddToVFTable("OnUpdate", "sandworm_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:411` (callback or registration): <code>function sandworm_OnTargetDied()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:413` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:418` (callback or registration): <code>AddToVFTable("OnTargetDied", "sandworm_OnTargetDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:421` (callback or registration): <code>local function sandworm_OnEndGameCleanUp()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:422` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:423` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:424` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:426` (condition/gate): <code>if (timer_outOfRange_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:427` (skill/timer/script action): <code>StopTimerCB(timer_outOfRange_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:428` (authored assignment): <code>timer_outOfRange_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:430` (condition/gate): <code>if (timer_dive_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:431` (skill/timer/script action): <code>StopTimerCB(timer_dive_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:432` (authored assignment): <code>timer_Dive_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:434` (condition/gate): <code>if (timer_intimidate_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:435` (skill/timer/script action): <code>StopTimerCB(timer_intimidate_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:436` (authored assignment): <code>timer_intimidate_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:440` (condition/gate): <code>if (GetTarget()~=nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:447` (callback or registration): <code>function sandworm_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:450` (callback or registration): <code>AddToVFTable("OnDied", "sandworm_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:453` (callback or registration): <code>function sandworm_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:456` (callback or registration): <code>AddToVFTable("OnTerminate", "sandworm_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:459` (callback or registration): <code>function sandworm_OnEndOfAnim()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:460` (condition/gate): <code>if (SK_STATE == ST_DIVE and diveAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:461` (authored assignment): <code>diveAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:465` (authored assignment, skill/timer/script action): <code>timer_dive_flag = StartTimerCB(DIVE_DELAY, false, SetSwampkingInIdleState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:468` (condition/gate): <code>if (SK_STATE == ST_INTIMIDATE and intimidateAnim_flag &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:470` (condition/gate): <code>if (intimidateAnim_flag == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:473` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(2) - Spit)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:477` (authored assignment, skill/timer/script action): <code>timer_intimidate_flag = StartTimerCB(500, false, function() DoSkill(2); end);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:478` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:481` (authored assignment): <code>intimidateAnim_flag = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:484` (callback or registration): <code>AddToVFTable("OnEndOfAnim", "sandworm_OnEndOfAnim");</code>

### sandworm_queen.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac

Relevant calls: 0; source lines: 74; engine readiness: 4/8.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:13` (authored assignment, condition/gate): <code>attack_flag              = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:14` (authored assignment, condition/gate): <code>first_melee_flag     = true;     -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:15` (authored assignment, condition/gate): <code>fa_phase1_flag           = true;     -- checks if a first action can be performed when switching to phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:16` (authored assignment, condition/gate): <code>fa_phase2_flag           = true;     -- checks if a first action can be performed when switching to phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:17` (authored assignment, condition/gate): <code>SE_event1_flag           = true;     -- checks if special event 1 can be performed (Enters weakness state at HP &lt; 75%).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:18` (authored assignment, condition/gate): <code>SE_event2_flag           = true;     -- checks if special event 2 can be performed (Enters weakness state at HP &lt; 50%).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:20` (authored assignment, condition/gate): <code>diveAnim_flag            = false; -- checks if the OnEndOfAnim event can be used to check the dive anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:22` (authored assignment, condition/gate): <code>intimidateAnim_flag  = 0; -- checks if the OnEndOfAnim event can be used to check the intimidate anim status and which action to take after.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:26` (authored assignment): <code>timer_attack_flag            = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:27` (authored assignment): <code>timer_outOfRange_flag    = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:28` (authored assignment): <code>timer_dive_flag          = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:29` (authored assignment): <code>timer_intimidate_flag    = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:36` (authored assignment): <code>ST_WEAK          = 3;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:38` (authored assignment): <code>SK_STATE         = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:40` (authored assignment): <code>SK_PHASE_ID      = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:44` (authored assignment): <code>MELEE_RANGE      = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:45` (authored assignment): <code>RANGED_RANGE = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:48` (authored assignment): <code>normalRangedAttack_prob      = 35;                   -- chances in PCT to perform a regular ranged attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:49` (authored assignment): <code>normalMeleeAttack1_prob  = 50;               -- chances in PCT to perform a regular melee attack in phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:50` (authored assignment): <code>normalMeleeAttack2_prob      = {20, 20, 60}; -- chances in PCT to perform one of the 3 regular melee/ranged attacks in phase 2 - !Cumulative values - MUST MATCH 100%.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:53` (authored assignment): <code>firstMeleeAttack2_prob       = 50;               -- chances in PCT to perform a first regular melee attack in phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:55` (authored assignment): <code>phase2_initPct                   = 75;                   -- HP PCT of the character at when to init phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:56` (authored assignment): <code>sk_spawn_points              = {                 -- Spawn point names table for the Dive attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:66` (authored assignment): <code>ATTACK_DELAY                 = 1000;     -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:67` (authored assignment): <code>WEAK_DELAY                   = 6000; -- Timer for weak state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:68` (authored assignment): <code>DIVE_DELAY                   = 2000; -- Timer for dive state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:69` (authored assignment): <code>INTIMIDATE_DELAY         = 3000; -- Timer for intimidate state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:70` (authored assignment): <code>OUTOFRANGE_MIN_DELAY     = 500;  -- Timer for out of range minimum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac:71` (authored assignment): <code>OUTOFRANGE_MAX_DELAY     = 2000; -- Timer for out of range maximum reaction delay.</code>

### sandworm_small.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac

Relevant calls: 58; source lines: 493; engine readiness: 38/129.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 81 | `ClearTarget` | `` | global | PORTED |
| 83 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 84 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 85 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 87 | `TargetListSearch` | `1=MELEE_RANGE` | global | PARTIAL |
| 89 | `IsTargetListEmpty` | `` | global | MISSING |
| 90 | `GetTargetListTop` | `` | global | PARTIAL |
| 92 | `SetTarget` | `1=target` | global | PORTED |
| 103 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Friend")` | global | MISSING |
| 104 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 105 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 107 | `target:TargetListSearch` | `1=MELEE_RANGE` | object_method | PARTIAL |
| 108 | `GetTargetListSize` | `` | global | MISSING |
| 121 | `HasTarget` | `` | global | PORTED |
| 122 | `LookAtTarget` | `` | global | PARTIAL |
| 139 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_idle")` | global | PORTED |
| 140 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_intimidate")` | global | PORTED |
| 141 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_dive")` | global | PORTED |
| 142 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_emerge")` | global | PORTED |
| 143 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall")` | global | PORTED |
| 144 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall_idle")` | global | PORTED |
| 145 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall_idle_to_idle")` | global | PORTED |
| 161 | `DoSkill` | `1=1` | global | PARTIAL |
| 167 | `DoSkill` | `1=1` | global | PARTIAL |
| 170 | `DoSkill` | `1=0` | global | PARTIAL |
| 185 | `DoSkill` | `1=0` | global | PARTIAL |
| 187 | `DoSkill` | `1=1` | global | PARTIAL |
| 193 | `DoSkill` | `1=0` | global | PARTIAL |
| 195 | `DoSkill` | `1=1` | global | PARTIAL |
| 218 | `target:GetName` | `` | object_method | PARTIAL |
| 223 | `target:GetName` | `` | object_method | PARTIAL |
| 239 | `GetTarget` | `` | global | PORTED |
| 243 | `CheckForClosestSpawnPoint` | `1=target` | script_function | PORTED |
| 248 | `SetActorPosition` | `1=targetSpot` | global | PARTIAL |
| 252 | `DoSkill` | `1=3` | global | PARTIAL |
| 254 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_fall_idle_to_idle"); 2=nil; 3=true` | global | PARTIAL |
| 259 | `SetIsActorTargetable` | `1=true` | global | MISSING |
| 268 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_fall"); 2=GetPyOID("AnimDict", "swampking_fall_idle")` | global | PARTIAL |
| 274 | `HasTarget` | `` | global | PORTED |
| 275 | `GetTarget` | `` | global | PORTED |
| 283 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_dive")` | global | PARTIAL |
| 284 | `SetIsActorTargetable` | `1=false` | global | MISSING |
| 295 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_intimidate"); 2=nil; 3=true` | global | PARTIAL |
| 312 | `RegisterAnims` | `` | script_function | PORTED |
| 356 | `HasTarget` | `` | global | PORTED |
| 358 | `SetTarget` | `1=enemy` | global | PORTED |
| 367 | `StartTimerCB` | `1=OUTOFRANGE_DELAY; 2=false; 3=SetSwampkingInDiveState` | imported_script_function | PARTIAL |
| 377 | `StartTimerCB` | `1=DIVE_DELAY; 2=false; 3=SetSwampkingInDiveState` | imported_script_function | PARTIAL |
| 394 | `DoSkill` | `1=1` | global | PARTIAL |
| 398 | `Do_MeleeAttackPhase1` | `` | script_function | PORTED |
| 402 | `Do_MeleeAttackPhase2` | `` | script_function | PORTED |
| 455 | `searchForClosestTarget` | `` | script_function | PORTED |
| 457 | `StartTimerCB` | `1=DIVE_DELAY; 2=false; 3=SetSwampkingInIdleState` | imported_script_function | PARTIAL |
| 467 | `searchForClosestTarget` | `` | script_function | PORTED |
| 469 | `StartTimerCB` | `1=500; 2=false; 3=function() DoSkill(2); end` | imported_script_function | PARTIAL |
| 469 | `DoSkill` | `1=2` | global | PARTIAL |
| 479 | `searchForClosestTarget` | `` | script_function | PORTED |
| 484 | `StartTimerCB` | `1=WEAK_DELAY; 2=false; 3=SetSwampkingInIdleState` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:15` (authored assignment, condition/gate): <code>local attack_flag                = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:16` (authored assignment, condition/gate): <code>local first_melee_flag       = true;     -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:17` (authored assignment, condition/gate): <code>local fa_phase1_flag         = true;     -- checks if a first action can be performed when switching to phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:18` (authored assignment, condition/gate): <code>local fa_phase2_flag         = true;     -- checks if a first action can be performed when switching to phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:19` (authored assignment, condition/gate): <code>local SE_event1_flag         = true;     -- checks if special event 1 can be performed (Enters weakness state at HP &lt; 75%).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:20` (authored assignment, condition/gate): <code>local SE_event2_flag         = true;     -- checks if special event 2 can be performed (Enters weakness state at HP &lt; 50%).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:22` (authored assignment, condition/gate): <code>local diveAnim_flag          = false; -- checks if the OnEndOfAnim event can be used to check the dive anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:23` (authored assignment, condition/gate): <code>local weakAnim_flag          = false; -- checks if the OnEndOfAnim event can be used to check the weak anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:24` (authored assignment, condition/gate): <code>local weakidleAnim_flag      = false; -- checks if the OnEndOfAnim event can be used to check the weak return to idle anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:26` (authored assignment, condition/gate): <code>local intimidateAnim_flag    = 0; -- checks if the OnEndOfAnim event can be used to check the intimidate anim status and which action to take after.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:30` (authored assignment): <code>local timer_attack_flag      = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:31` (authored assignment): <code>local timer_outOfRange_flag  =0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:32` (authored assignment): <code>local OutRangeCombat         = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:39` (authored assignment): <code>local ST_WEAK            = 3;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:41` (authored assignment): <code>local SK_STATE           = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:43` (authored assignment): <code>local SK_PHASE_ID        = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:47` (authored assignment): <code>local MELEE_RANGE        = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:48` (authored assignment): <code>local RANGED_RANGE   = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:51` (authored assignment): <code>local normalRangedAttack_prob        = 1;                                        -- chances in PCT to perform a regular ranged attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:52` (authored assignment): <code>local normalMeleeAttack1_prob    = 50;                                   -- chances in PCT to perform a regular melee attack in phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:53` (authored assignment): <code>local normalMeleeAttack2_prob        = {39, 1, 60};                          -- chances in PCT to perform one of the 3 regular melee/ranged attacks in phase 2 - !Cumulative values - MUST MATCH 100%.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:56` (authored assignment): <code>local firstMeleeAttack2_prob         = 50;                                   -- chances in PCT to perform a first regular melee attack in phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:58` (authored assignment): <code>local phase2_initPct                 = -1;                                       -- HP PCT of the character at when to init phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:59` (authored assignment): <code>local sk_spawn_points                = {"_prim_SandWorm_Leech_01"};  -- Spawn point names table for the Dive attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:65` (authored assignment): <code>local ATTACK_DELAY               = 100;  -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:66` (authored assignment): <code>local WEAK_DELAY                 = 1500; -- Timer for weak state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:67` (authored assignment): <code>local DIVE_DELAY                 = 1000; -- Timer for dive state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:68` (authored assignment): <code>local INTIMIDATE_DELAY           = 2000; -- Timer for intimidate state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:69` (authored assignment): <code>local OUTOFRANGE_MIN_DELAY       = 500;  -- Timer for out of range minimum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:70` (authored assignment): <code>local OUTOFRANGE_MAX_DELAY       = 2000; -- Timer for out of range maximum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:89` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:90` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:96` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:108` (authored assignment): <code>local size = GetTargetListSize();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:119` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:121` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:125` (condition/gate): <code>if (SK_STATE == ST_IDLE and state ~= GetPyCst("AIStates", "Skill")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:126` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Attack is ready!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:127` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:129` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Preventing attack to be ready!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:130` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:138` (callback or registration): <code>function RegisterAnims()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:139` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:140` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_intimidate"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:141` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_dive"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:142` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_emerge"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:143` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:144` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:145` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle_to_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:156` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Melee Attack phase 1, checking for initial melee attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:157` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:160` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is - first melee attack - DoSkill(1) - Bite 2)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:161` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:163` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:165` (condition/gate): <code>if (prob_pct &lt; normalMeleeAttack1_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:166` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(1) - Bite 2)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:167` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:169` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(0) - Bite 1)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:170` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:177` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Melee Attack phase 2, checking for initial melee attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:178` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:182` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:184` (condition/gate): <code>if (prob_pct &lt; firstMeleeAttack2_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:185` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:187` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:190` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:192` (condition/gate): <code>if (prob_pct &lt; normalMeleeAttack2_prob[1]) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:193` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:195` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:217` (authored assignment): <code>local short_distance = FromFixed(GetDistanceBetween(sk_spawn_points[1], GetSelf():GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:218` (authored assignment): <code>local short_distance2 = FromFixed(GetDistanceBetween(target:GetName(), GetSelf():GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:220` (authored assignment): <code>objectName = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:221` (condition/gate): <code>if (target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:223` (authored assignment): <code>objectName = target:GetName();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:236` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going back in IDLE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:237` (authored assignment): <code>OutRangeCombat          = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:238` (condition/gate): <code>if (SK_STATE == ST_DIVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:239` (authored assignment): <code>local target = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:242` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:243` (authored assignment): <code>targetSpot = CheckForClosestSpawnPoint(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:245` (authored assignment): <code>targetSpot = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:250` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:252` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:255` (authored assignment): <code>weakidleAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:258` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:263` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] In WEAK state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:265` (authored assignment): <code>SK_STATE = ST_WEAK;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:266` (authored assignment): <code>weakAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:272` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into DIVE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:274` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:275` (authored assignment): <code>local myTarget = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:280` (authored assignment): <code>SK_STATE = ST_DIVE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:281` (authored assignment): <code>diveAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:288` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into INTIMIDATE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:290` (authored assignment): <code>SK_STATE = ST_INTIMIDATE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:291` (authored assignment): <code>intimidateAnim_flag = nextAction;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:293` (condition/gate): <code>if (debugTrace_flag) then Trace("intimidateAnim_flag: "..intimidateAnim_flag); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:307` (callback or registration): <code>function sandworm_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:310` (authored assignment): <code>SK_PHASE_ID = 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:352` (callback or registration): <code>AddToVFTable("OnInitFinal", "sandworm_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:355` (callback or registration): <code>function sandworm_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:356` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:361` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "sandworm_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:364` (callback or registration): <code>function sandworm_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:365` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:366` (authored assignment): <code>OUTOFRANGE_DELAY = GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:367` (authored assignment, skill/timer/script action): <code>timer_outOfRange_flag = StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:370` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "sandworm_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:373` (callback or registration): <code>function sandworm_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:374` (condition/gate): <code>if (SK_STATE ~= ST_DIVE and OutRangeCombat) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:375` (authored assignment): <code>OutRangeCombat          = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:376` (condition/gate): <code>if (DIVE_DELAY==nil) then DIVE_DELAY=1000; end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:377` (authored assignment, skill/timer/script action): <code>local hide = StartTimerCB(DIVE_DELAY, false, SetSwampkingInDiveState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:378` (condition/gate): <code>if (debugTrace_flag2) then Trace("Tring to dive because the player is not in range"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:387` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "sandworm_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:390` (callback or registration): <code>function sandworm_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:391` (authored assignment): <code>attack_flag=true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:392` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:393` (condition/gate): <code>if (first_melee_flag and attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:394` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:396` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:401` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:406` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "sandworm_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:409` (callback or registration): <code>function sandworm_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:414` (condition/gate): <code>if(SK_PHASE_ID == 1 and HPPercentCurr &lt;= 75 and SE_event1_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:419` (condition/gate): <code>if(SK_PHASE_ID == 1 and HPPercentCurr &lt;= 50 and SE_event2_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:420` (condition/gate): <code>if (SK_STATE == ST_WEAK) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:428` (condition/gate): <code>if(SK_PHASE_ID == 1 and HPPercentCurr &lt;= phase2_initPct ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:429` (authored assignment): <code>SK_PHASE_ID = 2;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:431` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:433` (condition/gate): <code>if (SK_STATE ~= ST_DIVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:440` (callback or registration): <code>AddToVFTable("OnUpdate", "sandworm_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:443` (callback or registration): <code>function sandworm_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:444` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:448` (callback or registration): <code>AddToVFTable("OnDied", "sandworm_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:451` (callback or registration): <code>function sandworm_OnEndOfAnim()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:452` (condition/gate): <code>if (SK_STATE == ST_DIVE and diveAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:453` (authored assignment): <code>diveAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:457` (skill/timer/script action): <code>StartTimerCB(DIVE_DELAY, false, SetSwampkingInIdleState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:460` (condition/gate): <code>if (SK_STATE == ST_INTIMIDATE and intimidateAnim_flag &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:462` (condition/gate): <code>if (intimidateAnim_flag == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:465` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(2) - Spit)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:469` (skill/timer/script action): <code>StartTimerCB(500, false, function() DoSkill(2); end);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:470` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:473` (authored assignment): <code>intimidateAnim_flag = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:476` (condition/gate): <code>if (SK_STATE == ST_WEAK and weakAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:477` (authored assignment): <code>weakAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:481` (authored assignment): <code>buff = CreateBuff(DEBUFF_ID, WEAK_DELAY, false);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:484` (skill/timer/script action): <code>StartTimerCB(WEAK_DELAY, false, SetSwampkingInIdleState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:487` (condition/gate): <code>if (weakidleAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:488` (authored assignment): <code>weakidleAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:493` (callback or registration): <code>AddToVFTable("OnEndOfAnim", "sandworm_OnEndOfAnim");</code>

### sandworm_small_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac

Relevant calls: 69; source lines: 456; engine readiness: 42/126.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 6 | `ClearTarget` | `` | global | PORTED |
| 8 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 9 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 10 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 12 | `TargetListSearch` | `1=MELEE_RANGE` | global | PARTIAL |
| 14 | `IsTargetListEmpty` | `` | global | MISSING |
| 15 | `GetTargetListTop` | `` | global | PARTIAL |
| 17 | `SetTarget` | `1=target` | global | PORTED |
| 31 | `HasTarget` | `` | global | PORTED |
| 32 | `LookAtTarget` | `` | global | PARTIAL |
| 49 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_idle")` | global | PORTED |
| 50 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_intimidate")` | global | PORTED |
| 51 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_dive")` | global | PORTED |
| 52 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_emerge")` | global | PORTED |
| 53 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall")` | global | PORTED |
| 54 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall_idle")` | global | PORTED |
| 55 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall_idle_to_idle")` | global | PORTED |
| 71 | `DoSkill` | `1=1` | global | PARTIAL |
| 77 | `DoSkill` | `1=1` | global | PARTIAL |
| 80 | `DoSkill` | `1=0` | global | PARTIAL |
| 95 | `DoSkill` | `1=0` | global | PARTIAL |
| 97 | `DoSkill` | `1=1` | global | PARTIAL |
| 103 | `DoSkill` | `1=0` | global | PARTIAL |
| 105 | `DoSkill` | `1=1` | global | PARTIAL |
| 128 | `target:GetName` | `` | object_method | PARTIAL |
| 133 | `target:GetName` | `` | object_method | PARTIAL |
| 149 | `GetTarget` | `` | global | PORTED |
| 153 | `CheckForClosestSpawnPoint` | `1=target` | script_function | PORTED |
| 158 | `SetActorPosition` | `1=targetSpot` | global | PARTIAL |
| 159 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_emerge"); attack_flag = false; DoSkill(3); elseif (SK_STATE == ST_WEAK) then PlayAnim(GetPyOID("AnimDict", "swampking_fall_idle_to_idle"), nil, true); weakidleAnim_flag = true; end SK_STATE = ST_IDLE; SetIsTargetable(true); end function SetSwampkingInWeakState() if (debugTrace_flag) then Trace("[SANDWORM/STATES] In WEAK state..."); end SK_STATE = ST_WEAK; weakAnim_flag = true; PlayAnim(GetPyOID("AnimDict", "swampking_fall"), GetPyOID("AnimDict", "swampking_fall_idle")); end function SetSwampkingInDiveState() if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into DIVE state..."); end if (HasTarget()) then local myTarget = GetTarget(); ClearAggro(myTarget); end SK_STATE = ST_DIVE; diveAnim_flag = true; PlayAnim(GetPyOID("AnimDict", "swampking_dive")); SetIsTargetable(false); end function SetSwampkingInIntimidateState(nextAction) if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into INTIMIDATE state..."); end SK_STATE = ST_INTIMIDATE; intimidateAnim_flag = nextAction; if (debugTrace_flag) then Trace("intimidateAnim_flag: "..intimidateAnim_flag); end PlayAnim(GetPyOID("AnimDict", "swampking_intimidate"), nil, true); end -- ------------------------------------------------------------------------------- -- ------------------------------------------------------------------------------- -- CALLBACKS -- ------------------------------------------------------------------------------- -- ------------------------------------------------------------------------------- function sandworm_OnInit() MarkAsFlying(true); SK_PHASE_ID = 1; RegisterAnims(); ---------------------------------------------------------------------------------------- -- Dynamic level for monster. Monster must have there LevelMax param set to use dynamic level ---------------------------------------------------------------------------------------- local m_lvl_max = FromFixed(GetProp(GetPyStruct("CharacterProperties", "LevelMax"))); local m_lvl_min = FromFixed(GetProp(GetPyStruct("CharacterProperties", "LevelMin"))); local m_lvl_offset = FromFixed(GetProp(GetPyStruct("CharacterProperties", "LevelOffset"))); local pl_lvl = GetHostPlayerLevel(); local pl_diff = GetHostPlayerDifficulty(); local m_lvl = pl_lvl; local m_level_min; 2=m_level_max = GetCurrentLevelRange(pl_diff); if(m_lvl_max > -1 ) then if(m_level_max and m_level_max > -1 ) then if( m_lvl <= m_level_min ) then m_lvl = m_level_min; else if( m_lvl >= m_level_max ) then m_lvl = m_level_max; end end else if( m_lvl <= m_lvl_min ) then m_lvl = m_lvl_min; else if( m_lvl >= m_lvl_max ) then m_lvl = m_lvl_max; end end end m_lvl = m_lvl + m_lvl_offset; SetLevel(ToFixed(m_lvl)); end -- Reset all timers if they are still active if (timer_attack_flag) then StopTimerCB(timer_attack_flag); timer_attack_flag = nil; end if (timer_outOfRange_flag) then StopTimerCB(timer_outOfRange_flag); timer_outOfRange_flag = nil; end if (timer_idle_flag) then StopTimerCB(timer_idle_flag); timer_idle_flag = nil; end if (timer_intimidate_flag) then StopTimerCB(timer_intimidate_flag); timer_intimidate_flag = nil; end if (timer_weak_flag) then StopTimerCB(timer_weak_flag); timer_weak_flag = nil; end -- if (debugTrace_flag) then Trace("Starting attack timer..."); end -- timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady); end AddToVFTable("OnInitFinal", "sandworm_OnInit"); -- ------------------------------------------------------------------------------- function sandworm_OnEnemySpotted(enemy) if(not HasTarget()) then AddAggro(enemy, 0); SetTarget(enemy); end end AddToVFTable("OnEnemySpotted", "sandworm_OnEnemySpotted"); -- ------------------------------------------------------------------------------- function sandworm_OnTargetOutOfSight() if (SK_PHASE_ID == 1) then OUTOFRANGE_DELAY = GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY); timer_outOfRange_flag = StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState); end end AddToVFTable("OnTargetOutOfSight", "sandworm_OnTargetOutOfSight"); -- ------------------------------------------------------------------------------- function sandworm_OnTargetInRangedRange() --if (SK_STATE ~= ST_DIVE) then -- first_melee_flag = true; -- If the target get out of range, the worm dive in to get him. -- isAttackReady(); -- StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState); -- StartTimerCB(OUTOFRANGE_DELAY+1000, false, SetSwampkingInIdleState); --end end AddToVFTable("OnTargetInRangedRange", "sandworm_OnTargetInRangedRange"); -- ------------------------------------------------------------------------------- function sandworm_OnTargetInCloseRange() attack_flag=true; if (SK_PHASE_ID == 1) then if (first_melee_flag and attack_flag) then DoSkill(1); first_melee_flag = false; attack_flag = false; elseif (attack_flag) then Do_MeleeAttackPhase1(); end elseif (SK_PHASE_ID == 2) then if (attack_flag) then Do_MeleeAttackPhase2(); end end end AddToVFTable("OnTargetInCloseRange", "sandworm_OnTargetInCloseRange"); -- ------------------------------------------------------------------------------- function sandworm_OnUpdate(timestamp) OnUpdate(timestamp); local HPCurr; 3=HPMax; 4=HPPercentCurr = GetPropHP(); if(SK_PHASE_ID == 1 and HPPercentCurr <= 75 and SE_event1_flag) then --SE_event1_flag = false; --SetSwampkingInWeakState(); end if(SK_PHASE_ID == 1 and HPPercentCurr <= 50 and SE_event2_flag) then if (SK_STATE == ST_WEAK) then SE_event2_flag = false; else --SE_event2_flag = false; --SetSwampkingInWeakState(); end end if(SK_PHASE_ID == 1 and HPPercentCurr <= phase2_initPct ) then SK_PHASE_ID = 2; attack_flag = false; if (SK_STATE ~= ST_DIVE) then --SetSwampkingInDiveState(); end end end AddToVFTable("OnUpdate", "sandworm_OnUpdate"); function sandworm_OnTargetDied() Stop(); if (HasTarget()) then ClearAggro(GetTarget()); ClearTarget(); end end AddToVFTable("OnTargetDied", "sandworm_OnTargetDied"); -- ------------------------------------------------------------------------------- local function sandworm_OnEndGameCleanUp() if (HasTarget()) then ClearAggro(GetTarget()); ClearTarget(); end if (timer_attack_flag) then StopTimerCB(timer_attack_flag); timer_attack_flag = nil; end if (timer_outOfRange_flag) then StopTimerCB(timer_outOfRange_flag); timer_outOfRange_flag = nil; end if (timer_idle_flag) then StopTimerCB(timer_idle_flag); timer_idle_flag = nil; end if (timer_intimidate_flag) then StopTimerCB(timer_intimidate_flag); timer_intimidate_flag = nil; end if (timer_weak_flag) then StopTimerCB(timer_weak_flag); timer_weak_flag = nil; end end -- ------------------------------------------------------------------------------- function sandworm_OnDied(killer) sandworm_OnEndGameCleanUp(); end AddToVFTable("OnDied", "sandworm_OnDied"); -- ------------------------------------------------------------------------------- function sandworm_OnTerminate() sandworm_OnEndGameCleanUp(); end AddToVFTable("OnTerminate", "sandworm_OnTerminate"); -- ------------------------------------------------------------------------------- function sandworm_OnEndOfAnim() if (SK_STATE == ST_DIVE and diveAnim_flag) then diveAnim_flag = false; searchForClosestTarget(); timer_idle_flag = StartTimerCB(DIVE_DELAY, false, SetSwampkingInIdleState); end if (SK_STATE == ST_INTIMIDATE and intimidateAnim_flag > 0) then if (intimidateAnim_flag == 1) then SetSwampkingInIdleState(); elseif (intimidateAnim_flag == 2) then if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(2) - Spit)"); end searchForClosestTarget(); timer_intimidate_flag = StartTimerCB(500, false, function() DoSkill(2); end); SK_STATE = ST_IDLE; end intimidateAnim_flag = 0; end if (SK_STATE == ST_WEAK and weakAnim_flag) then weakAnim_flag = false; searchForClosestTarget(); buff = CreateBuff(DEBUFF_ID, WEAK_DELAY, false); ApplyBuff(DEBUFF_ID, buff); timer_weak_flag = StartTimerCB(WEAK_DELAY, false, SetSwampkingInIdleState); end if (weakidleAnim_flag) then weakidleAnim_flag = false; SetSwampkingInIntimidateState(1); end end AddToVFTable("OnEndOfAnim", "sandworm_OnEndOfAnim");` | global | PARTIAL |
| 162 | `DoSkill` | `1=3` | global | PARTIAL |
| 164 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_fall_idle_to_idle"); 2=nil; 3=true` | global | PARTIAL |
| 169 | `SetIsTargetable` | `1=true` | global | PARTIAL |
| 178 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_fall"); 2=GetPyOID("AnimDict", "swampking_fall_idle")` | global | PARTIAL |
| 184 | `HasTarget` | `` | global | PORTED |
| 185 | `GetTarget` | `` | global | PORTED |
| 193 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_dive")` | global | PARTIAL |
| 194 | `SetIsTargetable` | `1=false` | global | PARTIAL |
| 205 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_intimidate"); 2=nil; 3=true` | global | PARTIAL |
| 222 | `RegisterAnims` | `` | script_function | PORTED |
| 257 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 261 | `StopTimerCB` | `1=timer_outOfRange_flag` | imported_script_function | PARTIAL |
| 265 | `StopTimerCB` | `1=timer_idle_flag` | imported_script_function | PARTIAL |
| 269 | `StopTimerCB` | `1=timer_intimidate_flag` | imported_script_function | PARTIAL |
| 273 | `StopTimerCB` | `1=timer_weak_flag` | imported_script_function | PARTIAL |
| 283 | `HasTarget` | `` | global | PORTED |
| 285 | `SetTarget` | `1=enemy` | global | PORTED |
| 294 | `StartTimerCB` | `1=OUTOFRANGE_DELAY; 2=false; 3=SetSwampkingInDiveState` | imported_script_function | PARTIAL |
| 316 | `DoSkill` | `1=1` | global | PARTIAL |
| 320 | `Do_MeleeAttackPhase1` | `` | script_function | PORTED |
| 324 | `Do_MeleeAttackPhase2` | `` | script_function | PORTED |
| 366 | `HasTarget` | `` | global | PORTED |
| 367 | `GetTarget` | `` | global | PORTED |
| 368 | `ClearTarget` | `` | global | PORTED |
| 375 | `HasTarget` | `` | global | PORTED |
| 376 | `GetTarget` | `` | global | PORTED |
| 377 | `ClearTarget` | `` | global | PORTED |
| 380 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 384 | `StopTimerCB` | `1=timer_outOfRange_flag` | imported_script_function | PARTIAL |
| 388 | `StopTimerCB` | `1=timer_idle_flag` | imported_script_function | PARTIAL |
| 392 | `StopTimerCB` | `1=timer_intimidate_flag` | imported_script_function | PARTIAL |
| 396 | `StopTimerCB` | `1=timer_weak_flag` | imported_script_function | PARTIAL |
| 418 | `searchForClosestTarget` | `` | script_function | PORTED |
| 420 | `StartTimerCB` | `1=DIVE_DELAY; 2=false; 3=SetSwampkingInIdleState` | imported_script_function | PARTIAL |
| 430 | `searchForClosestTarget` | `` | script_function | PORTED |
| 432 | `StartTimerCB` | `1=500; 2=false; 3=function() DoSkill(2); end` | imported_script_function | PARTIAL |
| 432 | `DoSkill` | `1=2` | global | PARTIAL |
| 442 | `searchForClosestTarget` | `` | script_function | PORTED |
| 447 | `StartTimerCB` | `1=WEAK_DELAY; 2=false; 3=SetSwampkingInIdleState` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:14` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:15` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:21` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:29` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:31` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:35` (condition/gate): <code>if (SK_STATE == ST_IDLE and state ~= GetPyCst("AIStates", "Skill")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:36` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Attack is ready!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:37` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:39` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Preventing attack to be ready!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:40` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:48` (callback or registration): <code>function RegisterAnims()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:49` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:50` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_intimidate"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:51` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_dive"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:52` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_emerge"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:53` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:54` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:55` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle_to_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:66` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Melee Attack phase 1, checking for initial melee attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:67` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:70` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is - first melee attack - DoSkill(1) - Bite 2)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:71` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:73` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:75` (condition/gate): <code>if (prob_pct &lt; normalMeleeAttack1_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:76` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(1) - Bite 2)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:77` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:79` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(0) - Bite 1)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:80` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:87` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Melee Attack phase 2, checking for initial melee attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:88` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:92` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:94` (condition/gate): <code>if (prob_pct &lt; firstMeleeAttack2_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:95` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:97` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:100` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:102` (condition/gate): <code>if (prob_pct &lt; normalMeleeAttack2_prob[1]) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:103` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:105` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:127` (authored assignment): <code>local short_distance = FromFixed(GetDistanceBetween(sk_spawn_points[1], GetSelf():GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:128` (authored assignment): <code>local short_distance2 = FromFixed(GetDistanceBetween(target:GetName(), GetSelf():GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:130` (authored assignment): <code>objectName = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:131` (condition/gate): <code>if (target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:133` (authored assignment): <code>objectName = target:GetName();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:146` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going back in IDLE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:148` (condition/gate): <code>if (SK_STATE == ST_DIVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:149` (authored assignment): <code>local target = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:152` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:153` (authored assignment): <code>targetSpot = CheckForClosestSpawnPoint(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:155` (authored assignment): <code>targetSpot = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:160` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:162` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:165` (authored assignment): <code>weakidleAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:168` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:173` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] In WEAK state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:175` (authored assignment): <code>SK_STATE = ST_WEAK;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:176` (authored assignment): <code>weakAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:182` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into DIVE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:184` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:185` (authored assignment): <code>local myTarget = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:190` (authored assignment): <code>SK_STATE = ST_DIVE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:191` (authored assignment): <code>diveAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:198` (condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into INTIMIDATE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:200` (authored assignment): <code>SK_STATE = ST_INTIMIDATE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:201` (authored assignment): <code>intimidateAnim_flag = nextAction;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:203` (condition/gate): <code>if (debugTrace_flag) then Trace("intimidateAnim_flag: "..intimidateAnim_flag); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:217` (callback or registration): <code>function sandworm_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:220` (authored assignment): <code>SK_PHASE_ID = 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:256` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:257` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:258` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:260` (condition/gate): <code>if (timer_outOfRange_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:261` (skill/timer/script action): <code>StopTimerCB(timer_outOfRange_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:262` (authored assignment): <code>timer_outOfRange_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:264` (condition/gate): <code>if (timer_idle_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:265` (skill/timer/script action): <code>StopTimerCB(timer_idle_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:266` (authored assignment): <code>timer_idle_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:268` (condition/gate): <code>if (timer_intimidate_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:269` (skill/timer/script action): <code>StopTimerCB(timer_intimidate_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:270` (authored assignment): <code>timer_intimidate_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:272` (condition/gate): <code>if (timer_weak_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:273` (skill/timer/script action): <code>StopTimerCB(timer_weak_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:274` (authored assignment): <code>timer_weak_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:279` (callback or registration): <code>AddToVFTable("OnInitFinal", "sandworm_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:282` (callback or registration): <code>function sandworm_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:283` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:288` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "sandworm_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:291` (callback or registration): <code>function sandworm_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:292` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:293` (authored assignment): <code>OUTOFRANGE_DELAY = GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:294` (authored assignment, skill/timer/script action): <code>timer_outOfRange_flag = StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:297` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "sandworm_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:300` (callback or registration): <code>function sandworm_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:309` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "sandworm_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:312` (callback or registration): <code>function sandworm_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:313` (authored assignment): <code>attack_flag=true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:314` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:315` (condition/gate): <code>if (first_melee_flag and attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:316` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:318` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:323` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:328` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "sandworm_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:331` (callback or registration): <code>function sandworm_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:336` (condition/gate): <code>if(SK_PHASE_ID == 1 and HPPercentCurr &lt;= 75 and SE_event1_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:341` (condition/gate): <code>if(SK_PHASE_ID == 1 and HPPercentCurr &lt;= 50 and SE_event2_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:342` (condition/gate): <code>if (SK_STATE == ST_WEAK) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:350` (condition/gate): <code>if(SK_PHASE_ID == 1 and HPPercentCurr &lt;= phase2_initPct ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:351` (authored assignment): <code>SK_PHASE_ID = 2;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:353` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:355` (condition/gate): <code>if (SK_STATE ~= ST_DIVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:362` (callback or registration): <code>AddToVFTable("OnUpdate", "sandworm_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:364` (callback or registration): <code>function sandworm_OnTargetDied()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:366` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:371` (callback or registration): <code>AddToVFTable("OnTargetDied", "sandworm_OnTargetDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:374` (callback or registration): <code>local function sandworm_OnEndGameCleanUp()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:375` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:379` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:380` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:381` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:383` (condition/gate): <code>if (timer_outOfRange_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:384` (skill/timer/script action): <code>StopTimerCB(timer_outOfRange_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:385` (authored assignment): <code>timer_outOfRange_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:387` (condition/gate): <code>if (timer_idle_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:388` (skill/timer/script action): <code>StopTimerCB(timer_idle_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:389` (authored assignment): <code>timer_idle_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:391` (condition/gate): <code>if (timer_intimidate_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:392` (skill/timer/script action): <code>StopTimerCB(timer_intimidate_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:393` (authored assignment): <code>timer_intimidate_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:395` (condition/gate): <code>if (timer_weak_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:396` (skill/timer/script action): <code>StopTimerCB(timer_weak_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:397` (authored assignment): <code>timer_weak_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:402` (callback or registration): <code>function sandworm_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:405` (callback or registration): <code>AddToVFTable("OnDied", "sandworm_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:408` (callback or registration): <code>function sandworm_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:411` (callback or registration): <code>AddToVFTable("OnTerminate", "sandworm_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:414` (callback or registration): <code>function sandworm_OnEndOfAnim()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:415` (condition/gate): <code>if (SK_STATE == ST_DIVE and diveAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:416` (authored assignment): <code>diveAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:420` (authored assignment, skill/timer/script action): <code>timer_idle_flag = StartTimerCB(DIVE_DELAY, false, SetSwampkingInIdleState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:423` (condition/gate): <code>if (SK_STATE == ST_INTIMIDATE and intimidateAnim_flag &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:425` (condition/gate): <code>if (intimidateAnim_flag == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:428` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(2) - Spit)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:432` (authored assignment, skill/timer/script action): <code>timer_intimidate_flag = StartTimerCB(500, false, function() DoSkill(2); end);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:433` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:436` (authored assignment): <code>intimidateAnim_flag = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:439` (condition/gate): <code>if (SK_STATE == ST_WEAK and weakAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:440` (authored assignment): <code>weakAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:444` (authored assignment): <code>buff = CreateBuff(DEBUFF_ID, WEAK_DELAY, false);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:447` (authored assignment, skill/timer/script action): <code>timer_weak_flag = StartTimerCB(WEAK_DELAY, false, SetSwampkingInIdleState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:450` (condition/gate): <code>if (weakidleAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:451` (authored assignment): <code>weakidleAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:456` (callback or registration): <code>AddToVFTable("OnEndOfAnim", "sandworm_OnEndOfAnim");</code>

### swampking_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac

Relevant calls: 106; source lines: 768; engine readiness: 184/185.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 88 | `StopSound` | `1="SwampKingMusic"; 2=2000` | global | PORTED |
| 141 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 145 | `StopTimerCB` | `1=timer_diving_flag` | imported_script_function | PARTIAL |
| 149 | `StopTimerCB` | `1=timer_divepattern_flag` | imported_script_function | PARTIAL |
| 153 | `StopTimerCB` | `1=emerge_timer` | imported_script_function | PARTIAL |
| 157 | `StopTimerCB` | `1=dive_timer` | imported_script_function | PARTIAL |
| 161 | `StopTimerCB` | `1=skill2_timer` | imported_script_function | PARTIAL |
| 173 | `HasTarget` | `` | global | PORTED |
| 174 | `GetTarget` | `` | global | PORTED |
| 175 | `ClearTarget` | `` | global | PORTED |
| 178 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | PORTED |
| 179 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | PORTED |
| 180 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | PORTED |
| 182 | `TargetListSearch` | `1=RANGED_RANGE` | global | PORTED |
| 186 | `IsTargetListEmpty` | `` | global | PORTED |
| 188 | `GetTargetListTop` | `` | global | PORTED |
| 190 | `myTarget:IsPlayer` | `` | object_method | PORTED |
| 195 | `SetTarget` | `1=myTarget` | global | PORTED |
| 200 | `PopTargetList` | `` | global | PORTED |
| 202 | `IsTargetListEmpty` | `` | global | PORTED |
| 213 | `HasTarget` | `` | global | PORTED |
| 214 | `LookAtTarget` | `` | global | PORTED |
| 231 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_idle")` | global | PORTED |
| 232 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_intimidate")` | global | PORTED |
| 233 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_dive")` | global | PORTED |
| 234 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_emerge")` | global | PORTED |
| 235 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall")` | global | PORTED |
| 236 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall_idle")` | global | PORTED |
| 237 | `RegisterAnim` | `1=GetPyOID("AnimDict", "swampking_fall_idle_to_idle")` | global | PORTED |
| 260 | `DoSkill` | `1=2` | global | PORTED |
| 270 | `DoSkill` | `1=1` | global | PORTED |
| 276 | `DoSkill` | `1=1` | global | PORTED |
| 279 | `DoSkill` | `1=0` | global | PORTED |
| 293 | `DoSkill` | `1=0` | global | PORTED |
| 295 | `DoSkill` | `1=1` | global | PORTED |
| 301 | `DoSkill` | `1=0` | global | PORTED |
| 303 | `DoSkill` | `1=1` | global | PORTED |
| 320 | `target:GetName` | `` | object_method | PORTED |
| 323 | `target:GetName` | `` | object_method | PORTED |
| 373 | `StartTimerCB` | `1=500; 2=false; 3=GoToDivePattern` | imported_script_function | PARTIAL |
| 386 | `StopTimerCB` | `1=timer_diving_flag` | imported_script_function | PARTIAL |
| 388 | `StartTimerCB` | `1=INDIVEPATTERN_DELAY; 2=true; 3=SetSwampkingInDiveState` | imported_script_function | PARTIAL |
| 390 | `Do_RangedAttackPhase2` | `` | script_function | PORTED |
| 395 | `StopTimerCB` | `1=timer_divepattern_flag` | imported_script_function | PARTIAL |
| 397 | `StartTimerCB` | `1=GOTODIVE_DELAY; 2=true; 3=GoToDivePattern` | imported_script_function | PARTIAL |
| 406 | `CheckForRandomSpawnPoint` | `` | script_function | PORTED |
| 407 | `SetActorPosition` | `1=targetSpot` | global | PORTED |
| 417 | `GetTarget` | `` | global | PORTED |
| 421 | `CheckForClosestSpawnPoint` | `1=target` | script_function | PORTED |
| 426 | `SetActorPosition` | `1=targetSpot` | global | PORTED |
| 430 | `DoSkill` | `1=2` | global | PORTED |
| 435 | `SetIsTargetable` | `1=true` | global | PORTED |
| 445 | `HasTarget` | `` | global | PORTED |
| 446 | `GetTarget` | `` | global | PORTED |
| 452 | `SetIsTargetable` | `1=false` | global | PORTED |
| 454 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_dive")` | global | PORTED |
| 455 | `PlaySound` | `1="sfx_serpent_submerge"; 2=false; 3=0; 4=false` | global | PORTED |
| 460 | `SetIsTargetable` | `1=true` | global | PORTED |
| 470 | `DoSkill` | `1=3` | global | PORTED |
| 488 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_intimidate"); 2=nil; 3=true` | global | PORTED |
| 490 | `PlaySound` | `1="sfx_serpent_intimidate_1"; 2=false; 3=0; 4=false` | global | PORTED |
| 491 | `PlaySound` | `1="sfx_serpent_intimidate_2"; 2=false; 3=0; 4=false` | global | PORTED |
| 492 | `PlaySound` | `1="sfx_serpent_intimidate_3"; 2=false; 3=0; 4=false` | global | PORTED |
| 550 | `RegisterAnims` | `` | script_function | PORTED |
| 553 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 556 | `StopTimerCB` | `1=timer_diving_flag` | imported_script_function | PARTIAL |
| 559 | `StopTimerCB` | `1=timer_divepattern_flag` | imported_script_function | PARTIAL |
| 570 | `PlayMusic` | `1="SwampKingMusic"; 2=500` | global | PORTED |
| 573 | `HasTarget` | `` | global | PORTED |
| 575 | `SetTarget` | `1=enemy` | global | PORTED |
| 600 | `Do_RangedAttackPhase1` | `` | script_function | PORTED |
| 605 | `Do_RangedAttackPhase2` | `` | script_function | PORTED |
| 618 | `DoSkill` | `1=1` | global | PORTED |
| 622 | `Do_MeleeAttackPhase1` | `` | script_function | PORTED |
| 626 | `Do_MeleeAttackPhase2` | `` | script_function | PORTED |
| 650 | `HasTarget` | `` | global | PORTED |
| 650 | `GetTarget` | `` | global | PORTED |
| 652 | `StopSound` | `1="SwampKingMusic"; 2=2000` | global | PORTED |
| 653 | `PlayMusic` | `1="SwampHubAmbientMusic"; 2=1000` | global | PORTED |
| 656 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 657 | `StopTimerCB` | `1=timer_diving_flag` | imported_script_function | PARTIAL |
| 668 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=isAttackReady` | imported_script_function | PARTIAL |
| 669 | `StartTimerCB` | `1=GOTODIVE_DELAY; 2=true; 3=GoToDivePattern` | imported_script_function | PARTIAL |
| 677 | `HasTarget` | `` | global | PORTED |
| 678 | `GetTarget` | `` | global | PORTED |
| 679 | `ClearTarget` | `` | global | PORTED |
| 682 | `StopSound` | `1="SwampKingMusic"; 2=2000` | global | PORTED |
| 689 | `HasTarget` | `` | global | PORTED |
| 690 | `GetTarget` | `` | global | PORTED |
| 691 | `ClearTarget` | `` | global | PORTED |
| 694 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 698 | `StopTimerCB` | `1=timer_diving_flag` | imported_script_function | PARTIAL |
| 702 | `StopTimerCB` | `1=timer_divepattern_flag` | imported_script_function | PARTIAL |
| 706 | `StopTimerCB` | `1=emerge_timer` | imported_script_function | PARTIAL |
| 710 | `StopTimerCB` | `1=dive_timer` | imported_script_function | PARTIAL |
| 714 | `StopTimerCB` | `1=skill2_timer` | imported_script_function | PARTIAL |
| 721 | `StopSound` | `1="SwampKingMusic"; 2=2000` | global | PORTED |
| 722 | `PlaySound` | `1="SwampHubAmbientMusic"; 2=true; 3=1000; 4=true` | global | PORTED |
| 739 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 740 | `StartTimerCB` | `1=500; 2=false; 3=MoveToPosition` | imported_script_function | PARTIAL |
| 741 | `StartTimerCB` | `1=DIVE_DELAY; 2=false; 3=SetSwampkingInEmergeState` | imported_script_function | PARTIAL |
| 742 | `StartTimerCB` | `1=3000; 2=false; 3=TargetAgain` | imported_script_function | PARTIAL |
| 749 | `StartTimerCB` | `1=500; 2=false; 3=DiveAttackDecision` | imported_script_function | PARTIAL |
| 759 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 761 | `StartTimerCB` | `1=500; 2=false; 3=function() DoSkill(2); end` | imported_script_function | PARTIAL |
| 761 | `DoSkill` | `1=2` | global | PORTED |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:3` (authored assignment, condition/gate): <code>local attack_flag                 = false;    -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:4` (authored assignment, condition/gate): <code>local first_melee_flag                = true;     -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:6` (authored assignment, condition/gate): <code>local diveAnim_flag                   = false; -- checks if the OnEndOfAnim event can be used to check the dive anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:7` (authored assignment, condition/gate): <code>local emergeAnim_flag             = false; -- check if the OnEndOfAnim event can be used to check the emerge anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:8` (authored assignment, condition/gate): <code>local weakAnim_flag                   = false; -- checks if the OnEndOfAnim event can be used to check the weak anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:10` (authored assignment, condition/gate): <code>local musicStarted_flag              = false; -- checks if the boss music has started or not.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:12` (authored assignment, condition/gate): <code>local intimidateAnim_flag            = 0;    -- checks if the OnEndOfAnim event can be used to check the intimidate anim status and which action to take after.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:16` (authored assignment): <code>local diveCounter                    = 0; -- checks how many times Dive has been performed</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:20` (authored assignment): <code>local timer_attack_flag              = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:21` (authored assignment): <code>local timer_diving_flag              = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:22` (authored assignment): <code>local timer_divepattern_flag         = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:23` (authored assignment): <code>local emerge_timer                   = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:24` (authored assignment): <code>local dive_timer                 = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:25` (authored assignment): <code>local skill2_timer                   = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:34` (authored assignment): <code>local SK_STATE                       = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:36` (authored assignment): <code>local SK_PHASE_ID                    = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:40` (authored assignment): <code>local MELEE_RANGE                    = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:41` (authored assignment): <code>local RANGED_RANGE                   = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:44` (authored assignment): <code>local normalRangedAttack_prob        = 80;                   -- chances in PCT to perform a regular ranged attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:45` (authored assignment): <code>local normalMeleeAttack1_prob        = 80;               -- chances in PCT to perform a regular melee attack in phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:46` (authored assignment): <code>local normalMeleeAttack2_prob        = {40, 50, 10}; -- chances in PCT to perform one of the 3 regular melee/ranged attacks in phase 2 - !Cumulative values - MUST MATCH 100%.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:49` (authored assignment): <code>local firstMeleeAttack2_prob         = 50;               -- chances in PCT to perform a first regular melee attack in phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:51` (authored assignment): <code>local phase2_initPct             = 75;                   -- HP PCT of the character at when to init phase 2. 25%</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:52` (authored assignment): <code>local sk_spawn_points                =   {                   -- Spawn point names table for the Dive attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:62` (authored assignment): <code>local ATTACK_DELAY                   = 1500;     -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:63` (authored assignment): <code>local WEAK_DELAY                 = 7000; -- Timer for weak state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:64` (authored assignment): <code>local DIVE_DELAY                 = 1000; -- Timer for dive state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:65` (authored assignment): <code>local INTIMIDATE_DELAY               = 2000; -- Timer for intimidate state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:66` (authored assignment): <code>local OUTOFRANGE_MIN_DELAY           = 1000; -- Timer for out of range minimum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:67` (authored assignment): <code>local OUTOFRANGE_MAX_DELAY           = 3000; -- Timer for out of range maximum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:68` (authored assignment): <code>local GOTODIVE_DELAY             = 15000; -- Timer to enter Dive state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:69` (authored assignment): <code>local INDIVEPATTERN_DELAY            = 8500;  -- Timer to enter into Dive and Spit pattern.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:79` (authored assignment): <code>SK_STATE                     = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:80` (authored assignment, condition/gate): <code>attack_flag                      = false;    -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:81` (authored assignment, condition/gate): <code>first_melee_flag             = true;     -- checks if the first melee attack can be initialized.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:83` (authored assignment, condition/gate): <code>diveAnim_flag                    = false; -- checks if the OnEndOfAnim event can be used to check the dive anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:84` (authored assignment, condition/gate): <code>emergeAnim_flag                  = false; -- check if the OnEndOfAnim event can be used to check the emerge anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:85` (authored assignment, condition/gate): <code>weakAnim_flag                    = false; -- checks if the OnEndOfAnim event can be used to check the weak anim status.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:87` (authored assignment, condition/gate): <code>musicStarted_flag                = false; -- checks if the boss music has started or not.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:90` (authored assignment, condition/gate): <code>intimidateAnim_flag              = 0; -- checks if the OnEndOfAnim event can be used to check the intimidate anim status and which action to take after.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:94` (authored assignment): <code>diveCounter                      = 0; -- checks how many times Dive has been performed</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:106` (authored assignment): <code>SK_PHASE_ID                     = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:110` (authored assignment): <code>MELEE_RANGE                     = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:111` (authored assignment): <code>RANGED_RANGE                    = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:114` (authored assignment): <code>normalRangedAttack_prob         = 80;                   -- chances in PCT to perform a regular ranged attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:115` (authored assignment): <code>normalMeleeAttack1_prob         = 80;               -- chances in PCT to perform a regular melee attack in phase 1.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:116` (authored assignment): <code>normalMeleeAttack2_prob         = {40, 50, 10}; -- chances in PCT to perform one of the 3 regular melee/ranged attacks in phase 2 - !Cumulative values - MUST MATCH 100%.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:119` (authored assignment): <code>firstMeleeAttack2_prob          = 50;               -- chances in PCT to perform a first regular melee attack in phase 2.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:121` (authored assignment): <code>phase2_initPct                  = 75;                   -- HP PCT of the character at when to init phase 2. 25%</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:122` (authored assignment): <code>sk_spawn_points                 =   {                   -- Spawn point names table for the Dive attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:130` (authored assignment): <code>ATTACK_DELAY                    = 1500;     -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:131` (authored assignment): <code>WEAK_DELAY                      = 7000; -- Timer for weak state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:132` (authored assignment): <code>DIVE_DELAY                      = 1000; -- Timer for dive state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:133` (authored assignment): <code>INTIMIDATE_DELAY                = 2000; -- Timer for intimidate state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:134` (authored assignment): <code>OUTOFRANGE_MIN_DELAY            = 1000; -- Timer for out of range minimum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:135` (authored assignment): <code>OUTOFRANGE_MAX_DELAY            = 3000; -- Timer for out of range maximum reaction delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:136` (authored assignment): <code>GOTODIVE_DELAY                  = 15000; -- Timer to enter Dive state.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:137` (authored assignment): <code>INDIVEPATTERN_DELAY             = 8500;  -- Timer to enter into Dive and Spit pattern.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:140` (condition/gate): <code>if (timer_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:141` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:142` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:144` (condition/gate): <code>if (timer_diving_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:145` (skill/timer/script action): <code>StopTimerCB(timer_diving_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:146` (authored assignment): <code>timer_diving_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:148` (condition/gate): <code>if (timer_divepattern_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:149` (skill/timer/script action): <code>StopTimerCB(timer_divepattern_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:150` (authored assignment): <code>timer_divepattern_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:152` (condition/gate): <code>if (emerge_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:153` (skill/timer/script action): <code>StopTimerCB(emerge_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:154` (authored assignment): <code>emerge_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:156` (condition/gate): <code>if (dive_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:157` (skill/timer/script action): <code>StopTimerCB(dive_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:158` (authored assignment): <code>dive_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:160` (condition/gate): <code>if (skill2_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:161` (skill/timer/script action): <code>StopTimerCB(skill2_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:162` (authored assignment): <code>skill2_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:173` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:184` (authored assignment): <code>local targetChosen = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:186` (condition/gate): <code>if (not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:188` (authored assignment): <code>local myTarget = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:190` (condition/gate): <code>if (myTarget:IsPlayer()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:191` (authored assignment): <code>targetChosen = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:194` (condition/gate): <code>if (targetChosen) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:211` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:213` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:217` (condition/gate): <code>if (SK_STATE == ST_IDLE and (state ~= GetPyCst("AIStates", "Skill")) and (not canDiveNow)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:218` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Attack is ready!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:219` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:221` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Preventing attack to be ready!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:222` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:230` (callback or registration): <code>function RegisterAnims()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:231` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:232` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_intimidate"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:233` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_dive"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:234` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_emerge"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:235` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:236` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:237` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "swampking_fall_idle_to_idle"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:247` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Ranged Attack phase 1, random roll on the attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:249` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:251` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:257` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Ranged Attack phase 2 - DoSkill(2) - Spit"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:258` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:260` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:265` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Melee Attack phase 1, checking for initial melee attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:266` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:269` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Result is - first melee attack - DoSkill(1) - Bite 2)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:270` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:272` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:274` (condition/gate): <code>if (prob_pct &lt; normalMeleeAttack1_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:275` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Result is DoSkill(1) - Bite 2)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:276` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:278` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Result is DoSkill(0) - Bite 1)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:279` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:285` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Melee Attack phase 2, checking for initial melee attack..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:286` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:290` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:292` (condition/gate): <code>if (prob_pct &lt; firstMeleeAttack2_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:293` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:295` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:298` (authored assignment): <code>local prob_pct = GetRand(1,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:300` (condition/gate): <code>if (prob_pct &lt; normalMeleeAttack2_prob[1]) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:301` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:303` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:318` (authored assignment): <code>objectName = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:319` (condition/gate): <code>if (target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:320` (authored assignment): <code>local short_distance = FromFixed(GetDistanceBetween(sk_spawn_points[1], target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:322` (condition/gate): <code>if (obj) then -- Strangely OBJ sometimes becomes "nil"</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:323` (authored assignment): <code>local distance = FromFixed(GetDistanceBetween(obj, target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:325` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Player: Player: "..target:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:337` (authored assignment): <code>objectName = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:338` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] No Target. Fallback: "..sk_spawn_points[1]); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:340` (condition/gate): <code>if (objectName==nil) then Trace("WARNING!!! NOT WORKING!"); objectName = sk_spawn_points[3]; end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:349` (authored assignment): <code>local numSpawners = table.getn(sk_spawn_points);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:350` (authored assignment): <code>local randObjectPos = GetRand(1, numSpawners);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:352` (condition/gate): <code>if (not weakAnim_flag) then  -- weakAnim_flag is set to true when Actor is less than 25% health</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:354` (authored assignment): <code>objectName = sk_spawn_points[randObjectPos];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:357` (authored assignment): <code>objectName = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:366` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:367` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:370` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:371` (condition/gate): <code>if (state ~= GetPyCst("AIStates", "Idle")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:373` (skill/timer/script action): <code>StartTimerCB(500, false, GoToDivePattern);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:383` (condition/gate): <code>if (diveCounter &lt; 2) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:384` (authored assignment): <code>diveCounter = diveCounter + 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:386` (skill/timer/script action): <code>StopTimerCB(timer_diving_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:388` (authored assignment, skill/timer/script action): <code>timer_divepattern_flag = StartTimerCB(INDIVEPATTERN_DELAY, true, SetSwampkingInDiveState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:389` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING/STATES] Going to SPIT from DECISION..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:392` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING/STATES] Going back in IDLE state from DECISION..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:393` (authored assignment): <code>diveCounter = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:395` (skill/timer/script action): <code>StopTimerCB(timer_divepattern_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:396` (authored assignment): <code>timer_divepattern_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:397` (authored assignment, skill/timer/script action): <code>timer_diving_flag = StartTimerCB(GOTODIVE_DELAY, true, GoToDivePattern);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:406` (authored assignment): <code>local targetSpot = CheckForRandomSpawnPoint();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:414` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING/STATES] Going back in IDLE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:416` (condition/gate): <code>if (SK_STATE == ST_DIVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:417` (authored assignment): <code>local target = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:420` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:421` (authored assignment): <code>targetSpot = CheckForClosestSpawnPoint(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:423` (authored assignment): <code>targetSpot = sk_spawn_points[1];</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:428` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:430` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:433` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:439` (condition/gate): <code>if (SK_STATE ~= ST_DIVE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:441` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING/STATES] Going into DIVE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:443` (authored assignment): <code>SK_STATE = ST_DIVE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:445` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:446` (authored assignment): <code>local myTarget = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:451` (authored assignment): <code>diveAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:464` (condition/gate): <code>if (SK_STATE ~= ST_EMERGE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:466` (authored assignment): <code>SK_STATE = ST_EMERGE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:469` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING/STATES] Playing EMERGE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:470` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:474` (authored assignment): <code>emergeAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:479` (condition/gate): <code>if (SK_STATE ~= ST_INTIMIDATE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:481` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING/STATES] Going into INTIMIDATE state..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:483` (authored assignment): <code>SK_STATE = ST_INTIMIDATE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:484` (authored assignment): <code>intimidateAnim_flag = nextAction;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:486` (condition/gate): <code>if (debugTrace_flag) then Trace("intimidateAnim_flag: "..intimidateAnim_flag); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:489` (authored assignment): <code>local soundselect = GetRand(1,3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:490` (condition/gate): <code>if (soundselect==1) then PlaySound("sfx_serpent_intimidate_1", false, 0, false); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:491` (condition/gate): <code>if (soundselect==2) then PlaySound("sfx_serpent_intimidate_2", false, 0, false); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:492` (condition/gate): <code>if (soundselect==3) then PlaySound("sfx_serpent_intimidate_3", false, 0, false); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:505` (callback or registration): <code>function swampking_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:548` (authored assignment): <code>SK_PHASE_ID = 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:551` (condition/gate): <code>if (timer_attack_flag==-1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:552` (condition/gate): <code>if (timer_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:553` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:555` (condition/gate): <code>if (timer_diving_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:556` (skill/timer/script action): <code>StopTimerCB(timer_diving_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:558` (condition/gate): <code>if (timer_divepattern_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:559` (skill/timer/script action): <code>StopTimerCB(timer_divepattern_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:563` (callback or registration): <code>AddToVFTable("OnInitFinal", "swampking_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:566` (callback or registration): <code>function swampking_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:568` (condition/gate): <code>if (not musicStarted_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:569` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:573` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:580` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "swampking_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:583` (callback or registration): <code>function swampking_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:585` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:586` (authored assignment): <code>OUTOFRANGE_DELAY = GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:587` (condition/gate): <code>if (debugTrace_flag) then Trace("The target lost..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:591` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "swampking_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:594` (callback or registration): <code>function swampking_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:598` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:599` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:601` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Enemy is in range, attack 1:"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:604` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:606` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Enemy is in range, attack 2:"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:611` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "swampking_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:614` (callback or registration): <code>function swampking_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:616` (condition/gate): <code>if (SK_PHASE_ID == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:617` (condition/gate): <code>if (first_melee_flag and attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:618` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:620` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:625` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:631` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "swampking_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:634` (callback or registration): <code>function swampking_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:639` (condition/gate): <code>if(HPPercentCurr &lt;= 25 and (not weakAnim_flag)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:640` (authored assignment): <code>weakAnim_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:643` (condition/gate): <code>if(SK_PHASE_ID == 1 and HPPercentCurr &lt;= phase2_initPct ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:644` (authored assignment): <code>SK_PHASE_ID = 2;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:646` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:650` (condition/gate): <code>if (HasTarget() and GetTarget():IsDead()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:651` (condition/gate): <code>if (debugTrace_flag) then Trace("The target is dead now..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:655` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:656` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:657` (skill/timer/script action): <code>StopTimerCB(timer_diving_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:658` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:659` (authored assignment): <code>timer_diving_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:664` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:665` (condition/gate): <code>if (state == GetPyCst("AIStates", "Idle")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:667` (condition/gate): <code>if (debugTrace_flag) then Trace("Starting attack timer..."); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:668` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:669` (authored assignment, skill/timer/script action): <code>timer_diving_flag = StartTimerCB(GOTODIVE_DELAY, true, GoToDivePattern);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:673` (callback or registration): <code>AddToVFTable("OnUpdate", "swampking_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:675` (callback or registration): <code>function swampking_OnTargetDied()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:677` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:681` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:685` (callback or registration): <code>AddToVFTable("OnTargetDied", "swampking_OnTargetDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:688` (callback or registration): <code>local function swampking_OnEndGameCleanUp()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:689` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:693` (condition/gate): <code>if (timer_attack_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:694` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:695` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:697` (condition/gate): <code>if (timer_diving_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:698` (skill/timer/script action): <code>StopTimerCB(timer_diving_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:699` (authored assignment): <code>timer_diving_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:701` (condition/gate): <code>if (timer_divepattern_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:702` (skill/timer/script action): <code>StopTimerCB(timer_divepattern_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:703` (authored assignment): <code>timer_divepattern_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:705` (condition/gate): <code>if (emerge_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:706` (skill/timer/script action): <code>StopTimerCB(emerge_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:707` (authored assignment): <code>emerge_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:709` (condition/gate): <code>if (dive_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:710` (skill/timer/script action): <code>StopTimerCB(dive_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:711` (authored assignment): <code>dive_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:713` (condition/gate): <code>if (skill2_timer ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:714` (skill/timer/script action): <code>StopTimerCB(skill2_timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:715` (authored assignment): <code>skill2_timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:720` (callback or registration): <code>function swampking_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:726` (callback or registration): <code>AddToVFTable("OnDied", "swampking_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:729` (callback or registration): <code>function swampking_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:732` (callback or registration): <code>AddToVFTable("OnTerminate", "swampking_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:735` (callback or registration): <code>function swampking_OnEndOfAnim()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:736` (condition/gate): <code>if (SK_STATE == ST_DIVE and diveAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:737` (authored assignment): <code>diveAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:740` (authored assignment, skill/timer/script action): <code>dive_timer1 = StartTimerCB(500, false, MoveToPosition);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:741` (authored assignment, skill/timer/script action): <code>emerge_timer = StartTimerCB(DIVE_DELAY, false, SetSwampkingInEmergeState);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:742` (skill/timer/script action): <code>StartTimerCB(3000, false, TargetAgain);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:743` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Started Emerge Delay timer"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:746` (condition/gate): <code>if (SK_STATE == ST_EMERGE and emergeAnim_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:747` (condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Ended EMERGE anim"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:748` (authored assignment): <code>emergeAnim_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:749` (authored assignment, skill/timer/script action): <code>dive_timer = StartTimerCB(500, false, DiveAttackDecision);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:752` (condition/gate): <code>if (SK_STATE == ST_INTIMIDATE and intimidateAnim_flag &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:754` (condition/gate): <code>if (intimidateAnim_flag == 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:757` (skill/timer/script action, condition/gate): <code>if (debugTrace_flag) then Trace("[SWAMPKING] Result is DoSkill(2) - Spit)"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:761` (authored assignment, skill/timer/script action): <code>skill2_timer = StartTimerCB(500, false, function() DoSkill(2); end);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:762` (authored assignment): <code>SK_STATE = ST_IDLE;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:765` (authored assignment): <code>intimidateAnim_flag = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:768` (callback or registration): <code>AddToVFTable("OnEndOfAnim", "swampking_OnEndOfAnim");</code>

### winddragon.luac — .local-inputs/assets-extra/android/data/scripts/ai/winddragon.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### winddragon_core.luac — .local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac

Relevant calls: 47; source lines: 464; engine readiness: 37/132.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 30 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 34 | `StopTimerCB` | `1=timer_spot_targeting` | imported_script_function | PARTIAL |
| 38 | `StopTimerCB` | `1=timer_sp_attack_flag` | imported_script_function | PARTIAL |
| 46 | `object:GetState` | `` | object_method | PORTED |
| 142 | `DoSkill` | `1=2` | global | PARTIAL |
| 145 | `DoSkill` | `1=0` | global | PARTIAL |
| 151 | `DoSkill` | `1=1` | global | PARTIAL |
| 156 | `DoSkill` | `1=1` | global | PARTIAL |
| 160 | `DoSkill` | `1=4` | global | PARTIAL |
| 163 | `DoSkill` | `1=3` | global | PARTIAL |
| 181 | `DoSkill` | `1=2` | global | PARTIAL |
| 183 | `DoSkill` | `1=0` | global | PARTIAL |
| 188 | `DoSkill` | `1=0` | global | PARTIAL |
| 192 | `DoSkill` | `1=0` | global | PARTIAL |
| 195 | `DoSkill` | `1=2` | global | PARTIAL |
| 197 | `DoSkill` | `1=3` | global | PARTIAL |
| 217 | `HasTarget` | `` | global | PORTED |
| 218 | `GetTarget` | `` | global | PORTED |
| 219 | `ClearTarget` | `` | global | PORTED |
| 222 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 223 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 224 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 226 | `TargetListSearch` | `1=RANGED_RANGE` | global | PARTIAL |
| 230 | `IsTargetListEmpty` | `` | global | MISSING |
| 233 | `GetTargetListTop` | `` | global | PARTIAL |
| 235 | `myTarget:IsPlayer` | `` | object_method | PARTIAL |
| 240 | `SetTarget` | `1=myTarget` | global | PORTED |
| 245 | `PopTargetList` | `` | global | MISSING |
| 247 | `IsTargetListEmpty` | `` | global | MISSING |
| 295 | `StartTimerCB` | `1=ATTACK_DELAY; 2=true; 3=isAttackReady` | imported_script_function | PARTIAL |
| 318 | `StartTimerCB` | `1=SEARCH_DELAY; 2=true; 3=searchForEfficientTarget` | imported_script_function | PARTIAL |
| 322 | `StartTimerCB` | `1=SP_ATTACK_DELAY; 2=true; 3=Do_SpecialAttack` | imported_script_function | PARTIAL |
| 334 | `PlayMusic` | `1=LEVEL_MUSIC; 2=1000` | global | PARTIAL |
| 342 | `StopTimerCB` | `1=timer_attack_flag` | imported_script_function | PARTIAL |
| 368 | `PlayMusic` | `1=BOSS_MUSIC; 2=500` | global | PARTIAL |
| 374 | `HasTarget` | `` | global | PORTED |
| 375 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 387 | `GetTarget` | `` | global | PORTED |
| 388 | `ClearTarget` | `` | global | PORTED |
| 406 | `HasTarget` | `` | global | PORTED |
| 407 | `GetTarget` | `` | global | PORTED |
| 409 | `searchForEfficientTarget` | `` | script_function | PORTED |
| 410 | `GetTarget` | `` | global | PORTED |
| 427 | `GetTarget` | `` | global | PORTED |
| 429 | `GetTarget` | `` | global | PORTED |
| 441 | `GetTarget` | `` | global | PORTED |
| 459 | `GetTarget` | `` | global | PORTED |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:2` (authored assignment, condition/gate): <code>local attack_flag                = false; -- checks if the character is ready to attack.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:3` (authored assignment, condition/gate): <code>local musicStarted_flag      = false; -- checks if the boss music has started.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:4` (authored assignment): <code>local sp_attack_flag         = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:7` (authored assignment): <code>local STATE_SKILL                = GetPyCst("AIStates", "Skill");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:8` (authored assignment): <code>local STATE_IDLE             = GetPyCst("AIStates", "Idle");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:11` (authored assignment): <code>local timer_attack_flag         = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:12` (authored assignment): <code>local timer_spot_targeting      = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:13` (authored assignment): <code>local timer_sp_attack_flag      = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:14` (authored assignment, condition/gate): <code>local allowed                   = false;    -- check if the NPc is really spawned before allowing it to attack and move</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:25` (authored assignment): <code>attack_flag             = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:26` (authored assignment): <code>musicStarted_flag       = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:27` (authored assignment): <code>sp_attack_flag          = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:28` (authored assignment, condition/gate): <code>allowed                 = false;    -- check if the NPc is really spawned before allowing it to attack and move</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:29` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:30` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:31` (authored assignment): <code>timer_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:33` (condition/gate): <code>if (timer_spot_targeting) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:34` (skill/timer/script action): <code>StopTimerCB(timer_spot_targeting);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:35` (authored assignment): <code>timer_spot_targeting = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:37` (condition/gate): <code>if (timer_sp_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:38` (skill/timer/script action): <code>StopTimerCB(timer_sp_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:39` (authored assignment): <code>timer_sp_attack_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:46` (authored assignment): <code>local state = object:GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:47` (condition/gate): <code>if( state == GetPyCst("AIStates", "Dead")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:48` (condition/gate): <code>if (traces) then Trace("--&gt; State: Dead"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:51` (condition/gate): <code>if( state == GetPyCst("AIStates", "Idle")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:52` (condition/gate): <code>if (traces) then Trace("--&gt; State: Idle"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:55` (condition/gate): <code>if( state == GetPyCst("AIStates", "Limbus")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:56` (condition/gate): <code>if (traces) then Trace("--&gt; State: Limbus"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:59` (condition/gate): <code>if( state == GetPyCst("AIStates", "Spawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:60` (condition/gate): <code>if (traces) then Trace("--&gt; State: Spawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:63` (condition/gate): <code>if( state == GetPyCst("AIStates", "Despawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:64` (condition/gate): <code>if (traces) then Trace("--&gt; State: Despawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:67` (condition/gate): <code>if( state == GetPyCst("AIStates", "Move")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:68` (condition/gate): <code>if (traces) then Trace("--&gt; State: Move"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:71` (condition/gate): <code>if( state == GetPyCst("AIStates", "Attack")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:72` (condition/gate): <code>if (traces) then Trace("--&gt; State: Attack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:75` (condition/gate): <code>if( state == GetPyCst("AIStates", "Skill")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:76` (condition/gate): <code>if (traces) then Trace("--&gt; State: Skill"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:79` (condition/gate): <code>if( state == GetPyCst("AIStates", "Cast")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:80` (condition/gate): <code>if (traces) then Trace("--&gt; State: Cast"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:83` (condition/gate): <code>if( state == GetPyCst("AIStates", "Scared")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:84` (condition/gate): <code>if (traces) then Trace("--&gt; State: Scared"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:87` (condition/gate): <code>if( state == GetPyCst("AIStates", "Stunned")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:88` (condition/gate): <code>if (traces) then Trace("--&gt; State: Stunned"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:91` (condition/gate): <code>if( state == GetPyCst("AIStates", "KnockedBack")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:92` (condition/gate): <code>if (traces) then Trace("--&gt; State: KnockedBack"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:96` (condition/gate): <code>if( state == GetPyCst("AIStates", "Injured")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:97` (condition/gate): <code>if (traces) then Trace("--&gt; State: Injured"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:100` (condition/gate): <code>if( state == GetPyCst("AIStates", "Interact")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:101` (condition/gate): <code>if (traces) then Trace("--&gt; State: Interact"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:104` (condition/gate): <code>if( state == GetPyCst("AIStates", "Anim")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:105` (condition/gate): <code>if (traces) then Trace("--&gt; State: Anim"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:108` (condition/gate): <code>if( state == GetPyCst("AIStates", "Reviving")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:109` (condition/gate): <code>if (traces) then Trace("--&gt; State: Reviving"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:112` (condition/gate): <code>if( state == GetPyCst("AIStates", "Revived")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:113` (condition/gate): <code>if (traces) then Trace("--&gt; State: Revived"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:116` (condition/gate): <code>if( state == GetPyCst("AIStates", "PreSpawn")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:117` (condition/gate): <code>if (traces) then Trace("--&gt; State: PreSpawn"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:125` (authored assignment): <code>attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:130` (authored assignment): <code>sp_attack_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:135` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:137` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:138` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:139` (condition/gate): <code>if (sp_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:140` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:142` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:145` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:147` (authored assignment): <code>sp_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:150` (condition/gate): <code>if (firstRangedSkill_prob == -1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:151` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:154` (condition/gate): <code>if (rnd_roll &lt; firstRangedSkill_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:156` (skill/timer/script action): <code>DoSkill(1);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:158` (condition/gate): <code>if (timed_specialAttack) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:160` (skill/timer/script action): <code>DoSkill(4);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:163` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:168` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:175` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:177` (condition/gate): <code>if (state ~= STATE_SKILL) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:178` (condition/gate): <code>if (attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:179` (condition/gate): <code>if ( sp_attack_flag ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:180` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:181` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:183` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:185` (authored assignment): <code>sp_attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:187` (condition/gate): <code>if (firstMeleeSkill_prob == -1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:188` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:191` (condition/gate): <code>if (rnd_roll &lt; firstMeleeSkill_prob) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:192` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:194` (condition/gate): <code>if (not timed_specialAttack) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:195` (skill/timer/script action): <code>DoSkill(2);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:197` (skill/timer/script action): <code>DoSkill(3);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:202` (authored assignment): <code>attack_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:217` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:228` (authored assignment): <code>local targetChosen = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:230` (condition/gate): <code>if (not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:233` (authored assignment): <code>local myTarget = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:235` (condition/gate): <code>if (myTarget:IsPlayer()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:236` (authored assignment): <code>targetChosen = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:239` (condition/gate): <code>if (targetChosen) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:255` (callback or registration): <code>function myBoss_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:295` (authored assignment, skill/timer/script action): <code>timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:297` (authored assignment): <code>MELEE_RANGE        = 400;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:298` (authored assignment): <code>RANGED_RANGE   = 2500;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:305` (callback or registration): <code>AddToVFTable("OnInitFinal", "myBoss_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:310` (callback or registration): <code>function myBoss_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:314` (authored assignment): <code>local check = getAIState(GetSelf(), false);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:317` (condition/gate): <code>if (timed_spotTargeting) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:318` (authored assignment, skill/timer/script action): <code>timer_spot_targeting = StartTimerCB(SEARCH_DELAY, true, searchForEfficientTarget);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:321` (condition/gate): <code>if (timed_specialAttack) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:322` (authored assignment, skill/timer/script action): <code>timer_sp_attack_flag = StartTimerCB(SP_ATTACK_DELAY, true, Do_SpecialAttack);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:327` (callback or registration): <code>AddToVFTable("OnUpdate", "myBoss_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:332` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:333` (condition/gate): <code>if (LEVEL_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:336` (skill/timer/script action): <code>CallPyScript("ResumeLevelMusic");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:338` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:341` (condition/gate): <code>if (timer_attack_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:342` (skill/timer/script action): <code>StopTimerCB(timer_attack_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:348` (callback or registration): <code>function myBoss_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:351` (callback or registration): <code>AddToVFTable("OnDied", "myBoss_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:355` (callback or registration): <code>function myBoss_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:358` (callback or registration): <code>AddToVFTable("OnTerminate", "myBoss_OnTerminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:362` (callback or registration): <code>function myBoss_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:364` (condition/gate): <code>if (BOSS_MUSIC ~= "") then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:365` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:366` (condition/gate): <code>if (not musicStarted_flag and state == STATE_IDLE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:367` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:374` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:379` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "myBoss_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:384` (callback or registration): <code>function myBoss_OnTargetOutOfSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:389` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:392` (callback or registration): <code>AddToVFTable("OnTargetOutOfSight", "myBoss_OnTargetOutOfSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:397` (callback or registration): <code>function myBoss_OnTargetInSight()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:399` (callback or registration): <code>AddToVFTable("OnTargetInSight", "myBoss_OnTargetInSight");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:404` (callback or registration): <code>function myBoss_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:406` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:414` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "myBoss_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:421` (callback or registration): <code>function myBoss_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:423` (condition/gate): <code>if (isUsingSkills and not isMeleeOnly) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:433` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "myBoss_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:435` (callback or registration): <code>function myBoss_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:437` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:445` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "myBoss_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:453` (callback or registration): <code>function myBoss_OnTargetInMeleeRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:455` (condition/gate): <code>if (isUsingSkills) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:463` (callback or registration): <code>AddToVFTable("OnTargetInMeleeRange", "myBoss_OnTargetInMeleeRange");</code>

### winddragon_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:2` (authored assignment, condition/gate): <code>isMeleeOnly              = false; -- if enabled, the boss will perform only melee attacks/skills</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:3` (authored assignment, condition/gate): <code>timed_spotTargeting      = false;    -- if enabled, it will search for the more efficient target at regular interval (Timer: SEARCH_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:4` (authored assignment, condition/gate): <code>isUsingSkills            = true; -- if enabled, the boss will use skills only for all attacks, otherwise it will ignore its own skill table.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:13` (authored assignment, condition/gate): <code>timed_specialAttack     = true; -- if enabled, the boss will perform a special attack at regular interval (Timer: SP_ATTACK_DELAY).</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:18` (authored assignment): <code>MELEE_RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:19` (authored assignment): <code>RANGED_RANGE            = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:21` (authored assignment): <code>firstMeleeSkill_prob    = 90; -- chances in PCT to perform the first melee attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:23` (authored assignment): <code>firstRangedSkill_prob   = 50; -- chances in PCT to perform the first ranged attack of the skilltable.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:27` (authored assignment): <code>ATTACK_DELAY            = 750; -- Timer attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:28` (authored assignment): <code>SEARCH_DELAY            = 1500; -- Timer search delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:29` (authored assignment): <code>SP_ATTACK_DELAY         = 15000; -- Timer special attack delay.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:32` (authored assignment): <code>BOSS_MUSIC              = "TwoHeadedDragonMusic"; -- Must put the label of the boss music here unless the boss won't have any background music while fighting.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:33` (authored assignment): <code>LEVEL_MUSIC             = "WindTempleAmbientMusic"; -- Must put the label of the level ambient music here unless it won't resume to the level music after defeating the boss.</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac:34` (authored assignment): <code>MUSIC_RESUME            = false;</code>

### witch.luac — .local-inputs/assets-extra/android/data/scripts/ai/witch.luac

Relevant calls: 36; source lines: 233; engine readiness: 82/83.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 22 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | PORTED |
| 23 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | PORTED |
| 24 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | PORTED |
| 26 | `TargetListSearch` | `1=RANGE` | global | PORTED |
| 30 | `IsTargetListEmpty` | `` | global | PORTED |
| 32 | `GetTargetListTop` | `` | global | PORTED |
| 34 | `target:IsPlayer` | `` | object_method | PORTED |
| 39 | `SetTarget` | `1=target` | global | PORTED |
| 44 | `PopTargetList` | `` | global | PORTED |
| 46 | `IsTargetListEmpty` | `` | global | PORTED |
| 52 | `GetPosition` | `` | global | PORTED |
| 56 | `PlayFX` | `1=TELEPORT_FX; 2=x; 3=y; 4=z` | global | PORTED |
| 57 | `PlaySound` | `1="sfx_teleport"; 2=false; 3=0; 4=false` | global | PORTED |
| 60 | `WarpTo` | `1=0.0; 2=MOVE_RANGE; 3=0.0; 4=true` | global | PORTED |
| 65 | `WarpTo` | `1=0.0; 2=-MOVE_RANGE; 3=0.0; 4=true` | global | PORTED |
| 70 | `WarpTo` | `1=MOVE_RANGE; 2=0.0; 3=0.0; 4=true` | global | PORTED |
| 75 | `WarpTo` | `1=-MOVE_RANGE; 2=0.0; 3=0.0; 4=true` | global | PORTED |
| 83 | `GetPosition` | `` | global | PORTED |
| 84 | `PlayFX` | `1=TELEPORT_FX; 2=x1; 3=y1; 4=z1` | global | PORTED |
| 129 | `StopTimerCB` | `1=timer_teleport_flag` | imported_script_function | PARTIAL |
| 143 | `PlayMusic` | `1="SwampKingMusic"; 2=500` | global | PORTED |
| 146 | `HasTarget` | `` | global | PORTED |
| 151 | `StartTimerCB` | `1=MOVE_DELAY; 2=true; 3=DoTeleport` | imported_script_function | PARTIAL |
| 159 | `ClearTarget` | `` | global | PORTED |
| 162 | `StopTimerCB` | `1=timer_teleport_flag` | imported_script_function | PARTIAL |
| 171 | `GetTarget` | `` | global | PORTED |
| 172 | `ClearTarget` | `` | global | PORTED |
| 183 | `GetTarget` | `` | global | PORTED |
| 191 | `LookAtTarget` | `` | global | PORTED |
| 192 | `DoSkill` | `1=0` | global | PORTED |
| 206 | `defender:GetID` | `` | object_method | PORTED |
| 209 | `HasTarget` | `` | global | PORTED |
| 210 | `SetTarget` | `1=attacker` | global | PORTED |
| 218 | `PlayMusic` | `1="SwampHubAmbientMusic"; 2=1000` | global | PORTED |
| 220 | `StopTimerCB` | `1=timer_teleport_flag` | imported_script_function | PARTIAL |
| 229 | `StopTimerCB` | `1=timer_teleport_flag` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:2` (authored assignment): <code>local TELEPORT_FX  = GetPyOID("AnimatedEffectTable", "spell_dh2_prince_mage_stone_escape_magefx");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:3` (authored assignment): <code>local MOVE_RANGE   = 900.0;        -- Teleportation range</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:4` (authored assignment): <code>local MOVE_DELAY   = 10000.0;      -- Delay between teleports</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:6` (authored assignment): <code>local musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:20` (authored assignment): <code>local RANGE = GetProp(GetPyStruct("CharacterProperties", "RangeMinDistance"));</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:28` (authored assignment): <code>local targetChosen = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:30` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:32` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:34` (condition/gate): <code>if (target:IsPlayer()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:35` (authored assignment): <code>targetChosen = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:38` (condition/gate): <code>if (targetChosen) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:91` (callback or registration): <code>function Witch_OnInit()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:128` (condition/gate): <code>if (timer_teleport_flag ~= nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:129` (skill/timer/script action): <code>StopTimerCB(timer_teleport_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:130` (authored assignment): <code>timer_teleport_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:133` (authored assignment): <code>musicStarted_flag = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:137` (callback or registration): <code>AddToVFTable("OnInitFinal", "Witch_OnInit");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:140` (callback or registration): <code>function Witch_OnEnemySpotted(enemy)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:141` (condition/gate): <code>if (not musicStarted_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:142` (authored assignment): <code>musicStarted_flag = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:146` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:150` (condition/gate): <code>if(not timer_teleport_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:151` (authored assignment, skill/timer/script action): <code>timer_teleport_flag = StartTimerCB(MOVE_DELAY, true, DoTeleport);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:154` (callback or registration): <code>AddToVFTable("OnEnemySpotted", "Witch_OnEnemySpotted");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:157` (callback or registration): <code>function Witch_OnTargetDied()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:161` (condition/gate): <code>if (timer_teleport_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:162` (skill/timer/script action): <code>StopTimerCB(timer_teleport_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:163` (authored assignment): <code>timer_teleport_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:166` (callback or registration): <code>AddToVFTable("OnTargetDied", "Witch_OnTargetDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:169` (callback or registration): <code>function Witch_OnTargetOutOfRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:175` (callback or registration): <code>AddToVFTable("OnTargetOutOfRange", "Witch_OnTargetOutOfRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:180` (callback or registration): <code>function Witch_OnTargetInRangedRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:181` (authored assignment): <code>local state = GetState();</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:182` (condition/gate): <code>if( state == GetPyCst("AIStates", "Idle") and not HasPath() ) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:186` (callback or registration): <code>AddToVFTable("OnTargetInRangedRange", "Witch_OnTargetInRangedRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:189` (callback or registration): <code>function Witch_OnTargetInCloseRange()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:192` (skill/timer/script action): <code>DoSkill(0);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:194` (callback or registration): <code>AddToVFTable("OnTargetInCloseRange", "Witch_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:195` (callback or registration): <code>AddToVFTable("OnTargetInMeleeRange", "Witch_OnTargetInCloseRange");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:199` (callback or registration): <code>function Witch_OnUpdate(timestamp)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:201` (callback or registration): <code>AddToVFTable("OnUpdate", "Witch_OnUpdate");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:204` (callback or registration): <code>function monster_OnTargetHit(attacker, defender)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:209` (condition/gate): <code>if(not HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:217` (callback or registration): <code>function Witch_OnDied(killer)</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:219` (condition/gate): <code>if (timer_teleport_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:220` (skill/timer/script action): <code>StopTimerCB(timer_teleport_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:221` (authored assignment): <code>timer_teleport_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:224` (callback or registration): <code>AddToVFTable("OnDied", "Witch_OnDied");</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:227` (callback or registration): <code>function Witch_OnTerminate()</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:228` (condition/gate): <code>if (timer_teleport_flag) then</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:229` (skill/timer/script action): <code>StopTimerCB(timer_teleport_flag);</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:230` (authored assignment): <code>timer_teleport_flag = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:233` (callback or registration): <code>AddToVFTable("OnTerminate", "Witch_OnTerminate");</code>

### balrog_flames_skill.luac — .local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac

Relevant calls: 15; source lines: 105; engine readiness: 2/27.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 33 | `TargetListSearch` | `1=RANGE; 2=ANGLE` | global | PARTIAL |
| 34 | `IsTargetListEmpty` | `` | global | MISSING |
| 35 | `GetTargetListTop` | `` | global | PARTIAL |
| 40 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 51 | `IsTargetListEmpty` | `` | global | MISSING |
| 53 | `StopTimerCB` | `1=timer` | imported_script_function | PARTIAL |
| 74 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 75 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "AttackableOnly")` | global | MISSING |
| 76 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 78 | `TargetListSearch` | `1=RANGE; 2=360` | global | PARTIAL |
| 79 | `IsTargetListEmpty` | `` | global | MISSING |
| 80 | `GetTargetListTop` | `` | global | PARTIAL |
| 82 | `SetTarget` | `1=target` | global | PORTED |
| 93 | `StartSkillTimerCB` | `1=DAMAGE_TICK; 2=true; 3=doDamage` | imported_script_function | PARTIAL |
| 105 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:16` (authored assignment): <code>local RANGE             = 800;      -- Harcoded the distance to match the Flame FX</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:19` (authored assignment): <code>local DAMAGE_TICK       = 500.0;    -- The duration before doing damage to the target</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:22` (authored assignment): <code>local CLASS_ID          = GetPyOID("ClassTable", "Balrog_FlamesAttack");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:24` (authored assignment): <code>local target            = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:27` (authored assignment): <code>local skill_level       = 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:34` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:35` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:40` (authored assignment): <code>local damage = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:51` (condition/gate): <code>if (count&gt;=DAMAGE_REPEAT or IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:53` (skill/timer/script action): <code>StopTimerCB(timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:62` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:67` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:72` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:79` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:80` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:84` (condition/gate): <code>if (traces) then Trace ("The Balrog pre-skill is invoked"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:89` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:92` (condition/gate): <code>if (traces) then Trace ("The Balrog skill is invoked"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:93` (skill/timer/script action): <code>StartSkillTimerCB(DAMAGE_TICK, true, doDamage);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:97` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/balrog_flames_skill.luac:101` (callback or registration): <code>local function OnDelayedSkill_()</code>

### darkqueen2_melee_00.luac — .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac

Relevant calls: 8; source lines: 66; engine readiness: 0/13.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 38 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 39 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 40 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 42 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 44 | `IsTargetListEmpty` | `` | global | MISSING |
| 45 | `GetTargetListTop` | `` | global | PARTIAL |
| 57 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 66 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:12` (authored assignment): <code>local RANGE             = 150.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:14` (authored assignment): <code>local CLASS_ID          = GetPyOID("ClassTable", "DarkQueen2_MeleeAttack");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:24` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:28` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:32` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:37` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:44` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:45` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:48` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:53` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:55` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:57` (authored assignment): <code>local dmg = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_melee_00.luac:62` (callback or registration): <code>local function OnPostSkill_()</code>

### darkqueen2_ranged_00.luac — .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac

Relevant calls: 9; source lines: 68; engine readiness: 4/22.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 38 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 39 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 41 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 42 | `IsTargetListEmpty` | `` | global | MISSING |
| 43 | `GetTargetListTop` | `` | global | PARTIAL |
| 51 | `SpawnSkillProjectile` | `1=PROJECTILE_ID` | imported_script_function | PARTIAL |
| 63 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 68 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:12` (authored assignment): <code>local RANGE            = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:14` (authored assignment): <code>local PROJECTILE_ID        = GetProp(GetPyStruct("CharacterProperties", "RangeProjectileID"));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:15` (authored assignment): <code>local CLASS_ID         = GetPyOID("ClassTable", "DarkQueen2_RangedAttack");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:16` (authored assignment): <code>local target           = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:17` (authored assignment): <code>local skill_level      = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:24` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:27` (authored assignment): <code>skill_level    = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:31` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:36` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:42` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:43` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:49` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:55` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:59` (callback or registration): <code>local function OnDelayedSkill_(target)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_ranged_00.luac:60` (condition/gate): <code>if(target) then</code>

### darkqueen2_slave_ranged_00.luac — .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac

Relevant calls: 9; source lines: 68; engine readiness: 4/22.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 38 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 39 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 41 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 42 | `IsTargetListEmpty` | `` | global | MISSING |
| 43 | `GetTargetListTop` | `` | global | PARTIAL |
| 51 | `SpawnSkillProjectile` | `1=PROJECTILE_ID` | imported_script_function | PARTIAL |
| 63 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 68 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:12` (authored assignment): <code>local RANGE          = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:14` (authored assignment): <code>local PROJECTILE_ID      = GetProp(GetPyStruct("CharacterProperties", "RangeProjectileID"));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:15` (authored assignment): <code>local CLASS_ID           = GetPyOID("ClassTable", "DarkQueen2_Slave_RangedAttack");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:16` (authored assignment): <code>local target             = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:17` (authored assignment): <code>local skill_level        = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:24` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:27` (authored assignment): <code>skill_level  = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:31` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:36` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:42` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:43` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:49` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:55` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:59` (callback or registration): <code>local function OnDelayedSkill_(target)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_slave_ranged_00.luac:60` (condition/gate): <code>if(target) then</code>

### darkqueen2_summons_00.luac — .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac

Relevant calls: 1; source lines: 162; engine readiness: 0/28.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 156 | `RegisterSkillEx` | `1={ update=OnSkillUpdate_, cleanup=OnSkillCleanUp_, check=OnSkillCheck_, pre=OnPreSkill_, skill=OnSkill_, info=OnSkillInfo_, }` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:21` (authored assignment): <code>local MOVE_RANGE  = 500.0;        -- Teleportation range</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:22` (authored assignment): <code>local RANGE           = 500.0;        -- Explosion radius</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:26` (authored assignment): <code>local bufftime    = 10000     -- duration of the effect on the dead</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:28` (authored assignment): <code>local RISE_FX = GetPyOID("AnimatedEffectTable", "MadrukFaeries_Spawn");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:29` (authored assignment): <code>local CLASS_ID            = GetPyOID("ClassTable", "Skill_Necromancer_Rise");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:32` (authored assignment): <code>local skill_level = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:113` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:118` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:122` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:127` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:131` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:132` (condition/gate): <code>if (traces) then Trace("Queen is summoning!!!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:141` (callback or registration): <code>local function OnSkillCleanUp_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:156` (callback or registration): <code>RegisterSkillEx({   update=OnSkillUpdate_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:157` (authored assignment): <code>cleanup=OnSkillCleanUp_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:158` (authored assignment): <code>check=OnSkillCheck_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:159` (authored assignment): <code>pre=OnPreSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:160` (authored assignment): <code>skill=OnSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:161` (authored assignment): <code>info=OnSkillInfo_,</code>

### flaminite_slam.luac — .local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac

Relevant calls: 10; source lines: 71; engine readiness: 0/17.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 38 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 39 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 40 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 44 | `TargetListSearch` | `1=500; 2=120; 3=0; 4=-40; 5=0; 6=true` | global | PARTIAL |
| 45 | `IsTargetListEmpty` | `` | global | MISSING |
| 46 | `GetTargetListTop` | `` | global | PARTIAL |
| 48 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Root_Slam")` | global | PARTIAL |
| 59 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 61 | `target:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "elem_dmg_01_fire")` | object_method | PARTIAL |
| 71 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:12` (authored assignment): <code>local RANGE              = 300.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:24` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:28` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:32` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:37` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:45` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:46` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:50` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:56` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:57` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:59` (authored assignment): <code>local dmg = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_slam.luac:67` (callback or registration): <code>local function OnPostSkill_()</code>

### flaminite_sp_attack.luac — .local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac

Relevant calls: 19; source lines: 97; engine readiness: 0/26.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 24 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 26 | `target:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "elem_dmg_01_fire")` | object_method | PARTIAL |
| 32 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 33 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 34 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 36 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 38 | `IsTargetListEmpty` | `` | global | MISSING |
| 39 | `GetTargetListTop` | `` | global | PARTIAL |
| 40 | `PopTargetList` | `` | global | MISSING |
| 43 | `IsTargetListEmpty` | `` | global | MISSING |
| 45 | `StopTimerCB` | `1=timer` | imported_script_function | PARTIAL |
| 68 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 69 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 70 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 72 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 74 | `IsTargetListEmpty` | `` | global | MISSING |
| 75 | `GetTargetListTop` | `` | global | PARTIAL |
| 88 | `StartSkillTimerCB` | `1=1000; 2=true; 3=checktarget` | imported_script_function | PARTIAL |
| 97 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:12` (authored assignment): <code>local RANGE             = 500.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:14` (authored assignment): <code>local CLASS_ID          = GetPyOID("ClassTable", "Flaminite_Attack");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:19` (authored assignment): <code>local timer             = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:24` (authored assignment): <code>local dmg = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:38` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:39` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:45` (skill/timer/script action): <code>StopTimerCB(timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:47` (authored assignment): <code>timer = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:54` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:58` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:62` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:67` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:74` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:75` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:79` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:80` (condition/gate): <code>if (traces) then Trace("there is nobody to attack here!!!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:85` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:86` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:88` (authored assignment, skill/timer/script action): <code>timer = StartSkillTimerCB(1000, true, checktarget);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/flaminite_sp_attack.luac:93` (callback or registration): <code>local function OnPostSkill_()</code>

### lighthouseemissary_melee_00.luac — .local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac

Relevant calls: 13; source lines: 72; engine readiness: 0/18.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 39 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 40 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 41 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 43 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 45 | `IsTargetListEmpty` | `` | global | MISSING |
| 46 | `GetTargetListTop` | `` | global | PARTIAL |
| 55 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 57 | `IsTargetListEmpty` | `` | global | MISSING |
| 59 | `GetTargetListTop` | `` | global | PARTIAL |
| 61 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 62 | `PopTargetList` | `` | global | MISSING |
| 63 | `IsTargetListEmpty` | `` | global | MISSING |
| 72 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:12` (authored assignment): <code>local RANGE             = 150.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:14` (authored assignment): <code>local CLASS_ID          = GetPyOID("ClassTable", "Emissary_MeleeAttack");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:15` (authored assignment): <code>local FX                = GetPyOID("AnimatedEffectTable", "skill_dh2_monster_lighthouse_melee");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:25` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:29` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:33` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:38` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:45` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:46` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:49` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:54` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:57` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:59` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_melee_00.luac:68` (callback or registration): <code>local function OnPostSkill_()</code>

### lighthouseemissary_ranged_00.luac — .local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac

Relevant calls: 14; source lines: 100; engine readiness: 0/23.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 32 | `TargetListSearch` | `1=RANGE; 2=ANGLE` | global | PARTIAL |
| 33 | `IsTargetListEmpty` | `` | global | MISSING |
| 34 | `GetTargetListTop` | `` | global | PARTIAL |
| 37 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 48 | `IsTargetListEmpty` | `` | global | MISSING |
| 50 | `StopTimerCB` | `1=timer` | imported_script_function | PARTIAL |
| 71 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 72 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 73 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 75 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 76 | `IsTargetListEmpty` | `` | global | MISSING |
| 77 | `GetTargetListTop` | `` | global | PARTIAL |
| 88 | `StartSkillTimerCB` | `1=DAMAGE_TICK; 2=true; 3=doDamage` | imported_script_function | PARTIAL |
| 100 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:16` (authored assignment): <code>local RANGE            = 800;      -- Harcoded the distance to match the Flame FX</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:19` (authored assignment): <code>local DAMAGE_TICK      = 500.0;    -- The duration before doing damage to the target</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:22` (authored assignment): <code>local CLASS_ID         = GetPyOID("ClassTable", "Emissary_RangedAttack");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:24` (authored assignment): <code>local target           = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:33` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:34` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:37` (authored assignment): <code>local damage = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:48` (condition/gate): <code>if (count&gt;=DAMAGE_REPEAT or IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:50` (skill/timer/script action): <code>StopTimerCB(timer);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:59` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:64` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:69` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:76` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:77` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:80` (condition/gate): <code>if (traces) then Trace ("The Emissary flame pre-skill is invoked"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:84` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:87` (condition/gate): <code>if (traces) then Trace ("The Emissary flame skill is invoked"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:88` (skill/timer/script action): <code>StartSkillTimerCB(DAMAGE_TICK, true, doDamage);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:92` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/lighthouseemissary_ranged_00.luac:96` (callback or registration): <code>local function OnDelayedSkill_()</code>

### madruk_aoeknockback.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac

Relevant calls: 10; source lines: 62; engine readiness: 0/13.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 36 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 37 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 38 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 40 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 45 | `IsTargetListEmpty` | `` | global | MISSING |
| 49 | `GetTargetListTop` | `` | global | PARTIAL |
| 50 | `PopTargetList` | `` | global | MISSING |
| 52 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 53 | `IsTargetListEmpty` | `` | global | MISSING |
| 62 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:13` (authored assignment): <code>local RANGE         = 300.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:22` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:26` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:30` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:35` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:44` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:45` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:49` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_aoeknockback.luac:58` (callback or registration): <code>local function OnPostSkill_()</code>

### madruk_dyingattack.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac

Relevant calls: 2; source lines: 47; engine readiness: 0/4.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Madruk_Shockwave")` | global | PARTIAL |
| 47 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_; 8=nil` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac:21` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac:26` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac:31` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac:36` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac:41` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac:42` (skill/timer/script action): <code>CallPyScript("end_MadrukPhase2");</code>

### madruk_fastenergybolt.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac

Relevant calls: 9; source lines: 68; engine readiness: 3/21.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 38 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 39 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 41 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 42 | `IsTargetListEmpty` | `` | global | MISSING |
| 43 | `GetTargetListTop` | `` | global | PARTIAL |
| 51 | `SpawnSkillProjectile` | `1=PROJECTILE_ID` | imported_script_function | PARTIAL |
| 63 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 68 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:12` (authored assignment): <code>local RANGE           = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:16` (authored assignment): <code>local target          = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:17` (authored assignment): <code>local skill_level         = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:24` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:27` (authored assignment): <code>skill_level   = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:31` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:36` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:42` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:43` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:49` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:55` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:59` (callback or registration): <code>local function OnDelayedSkill_(target)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_fastenergybolt.luac:60` (condition/gate): <code>if(target) then</code>

### madruk_slam.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac

Relevant calls: 10; source lines: 74; engine readiness: 0/16.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 39 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 40 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 41 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 43 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 45 | `IsTargetListEmpty` | `` | global | MISSING |
| 46 | `GetTargetListTop` | `` | global | PARTIAL |
| 58 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 61 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Madruk_Slam")` | global | PARTIAL |
| 64 | `target:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "spell_earth_hurt")` | object_method | PARTIAL |
| 74 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:13` (authored assignment): <code>local RANGE             = 400.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:25` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:29` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:33` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:38` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:45` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:46` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:49` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:54` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:55` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:58` (authored assignment): <code>local dmg = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slam.luac:70` (callback or registration): <code>local function OnPostSkill_()</code>

### madruk_slowenergybolt.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac

Relevant calls: 12; source lines: 72; engine readiness: 3/24.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 38 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 39 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 41 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 42 | `IsTargetListEmpty` | `` | global | MISSING |
| 43 | `GetTargetListTop` | `` | global | PARTIAL |
| 50 | `IsTargetListEmpty` | `` | global | MISSING |
| 51 | `GetTargetListTop` | `` | global | PARTIAL |
| 53 | `PopTargetList` | `` | global | MISSING |
| 55 | `SpawnSkillProjectile` | `1=PROJECTILE_ID` | imported_script_function | PARTIAL |
| 67 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 72 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:12` (authored assignment): <code>local RANGE           = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:16` (authored assignment): <code>local target          = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:17` (authored assignment): <code>local skill_level         = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:24` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:27` (authored assignment): <code>skill_level   = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:31` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:36` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:42` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:43` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:49` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:50` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:51` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:59` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:63` (callback or registration): <code>local function OnDelayedSkill_(target)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruk_slowenergybolt.luac:64` (condition/gate): <code>if(target) then</code>

### madrukminion_bufferspell.luac — .local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac

Relevant calls: 11; source lines: 67; engine readiness: 0/17.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 33 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Madruk_orange_faery_cast")` | global | PARTIAL |
| 36 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Friend")` | global | MISSING |
| 37 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 38 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 40 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 41 | `IsTargetListEmpty` | `` | global | MISSING |
| 42 | `GetTargetListTop` | `` | global | PARTIAL |
| 51 | `target:CreateBuff` | `1=CLASS_ID; 2=BUFF_DURATION` | object_method | PARTIAL |
| 52 | `target:ApplyBuff` | `1=CLASS_ID; 2=buff` | object_method | PARTIAL |
| 54 | `target:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Madruk_shield")` | object_method | PARTIAL |
| 67 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:12` (authored assignment): <code>local RANGE            = 1000;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:23` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:27` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:32` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:35` (condition/gate): <code>if(not target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:41` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:42` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:49` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:50` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:51` (authored assignment): <code>buff = target:CreateBuff(CLASS_ID, BUFF_DURATION);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:59` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_bufferspell.luac:63` (callback or registration): <code>local function OnDelayedSkill_(target)</code>

### madrukminion_healerspell.luac — .local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac

Relevant calls: 11; source lines: 84; engine readiness: 1/18.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 34 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Friend")` | global | MISSING |
| 35 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 36 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 38 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 40 | `IsTargetListEmpty` | `` | global | MISSING |
| 41 | `GetTargetListTop` | `` | global | PARTIAL |
| 47 | `target:GetPropHP` | `` | object_method | PARTIAL |
| 66 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Madruk_green_faery_cast")` | global | PARTIAL |
| 74 | `target:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Madruk_heal")` | object_method | PARTIAL |
| 75 | `target:SetProp` | `1=GetPyStruct("CharacterProperties", "HP"); 2=ToFixed(targethealingAmount)` | object_method | PARTIAL |
| 84 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:12` (authored assignment): <code>local RANGE            = 1000;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:13` (authored assignment): <code>local healingAmount        = 10; -- PCT of HP healed each time the skill is used</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:14` (authored assignment): <code>local targetHPPercentMax   = 100; -- PCT HP threshold (the character will start healing below that value).</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:27` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:31` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:33` (condition/gate): <code>if(not target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:40` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:41` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:49` (condition/gate): <code>if(targetHPPercentCurr &lt; targetHPPercentMax) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:60` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:64` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:68` (authored assignment): <code>targethealingAmount = (((targetHPPercentCurr + healingAmount) * targetHPMax) / 100);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:70` (condition/gate): <code>if (targethealingAmount &gt; targetHPMax) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:71` (authored assignment): <code>targethealingAmount = targetHPMax;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madrukminion_healerspell.luac:80` (callback or registration): <code>local function OnPostSkill_()</code>

### madruktorso_beamray.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac

Relevant calls: 14; source lines: 76; engine readiness: 2/22.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 36 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 37 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 38 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 40 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 42 | `IsTargetListEmpty` | `` | global | MISSING |
| 43 | `GetTargetListTop` | `` | global | PARTIAL |
| 52 | `TargetListSearch` | `1=RANGE; 2=ANGLE; 3=0; 4=0; 5=0; 6=true` | global | PARTIAL |
| 53 | `IsTargetListEmpty` | `` | global | MISSING |
| 55 | `GetTargetListTop` | `` | global | PARTIAL |
| 58 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 63 | `target:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Madruk_Torso_laser_hurt")` | object_method | PARTIAL |
| 66 | `PopTargetList` | `` | global | MISSING |
| 67 | `IsTargetListEmpty` | `` | global | MISSING |
| 76 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:12` (authored assignment): <code>local RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:22` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:26` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:30` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:35` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:42` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:43` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:46` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:51` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:53` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:58` (authored assignment): <code>local dmg = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_beamray.luac:72` (callback or registration): <code>local function OnPostSkill_()</code>

### madruktorso_bite.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac

Relevant calls: 8; source lines: 65; engine readiness: 0/12.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 38 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 39 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 40 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 42 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 44 | `IsTargetListEmpty` | `` | global | MISSING |
| 45 | `GetTargetListTop` | `` | global | PARTIAL |
| 56 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 65 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:12` (authored assignment): <code>local RANGE                = 50.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:24` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:28` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:32` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:37` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:44` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:45` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:48` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:53` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:54` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:56` (authored assignment): <code>local dmg = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_bite.luac:61` (callback or registration): <code>local function OnPostSkill_()</code>

### madruktorso_drainlife.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac

Relevant calls: 40; source lines: 178; engine readiness: 4/70.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 46 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 47 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 48 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 51 | `invisible_man:WarpTo` | `1=x; 2=y; 3=z; 4=false` | object_method | PARTIAL |
| 52 | `TargetListSearch` | `1=150; 2=360; 3=invisible_man` | global | PARTIAL |
| 53 | `invisible_man:WarpTo` | `1=0; 2=0; 3=0; 4=false` | object_method | PARTIAL |
| 54 | `IsTargetListEmpty` | `` | global | MISSING |
| 56 | `GetTargetListTop` | `` | global | PARTIAL |
| 57 | `target:IsCharacter` | `` | object_method | PARTIAL |
| 62 | `target:SetStunState` | `1=3000; 2=false` | object_method | MISSING |
| 63 | `target:PlayActorAnim` | `1=GetPyOID("AnimDict", "spell_dh2_prince_drained_life_complete")` | object_method | PARTIAL |
| 66 | `target:GetPropHP` | `` | object_method | PARTIAL |
| 67 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 78 | `target:CreateBuff` | `1=BUFF_ID; 2=3000; 3=false; 4=1; 5=TargetBuffFX` | object_method | PARTIAL |
| 79 | `target:ApplyBuff` | `1=BUFF_ID; 2=target_buff` | object_method | PARTIAL |
| 84 | `PlaySound` | `1="sfx_king_of_hell_magic_attack_hit"; 2=false; 3=0; 4=false` | global | PARTIAL |
| 92 | `target:EnableCollisions` | `1=false` | object_method | MISSING |
| 93 | `target:WarpTo` | `1=player` | object_method | PARTIAL |
| 95 | `invisible_man:PlayFX` | `1=TargetFX` | object_method | PARTIAL |
| 96 | `PlayFX` | `1=SkillFX` | global | PARTIAL |
| 97 | `invisible_man:GetPosition` | `` | object_method | PORTED |
| 98 | `StartSkillTimerCB` | `1=2000; 2=0; 3=drainlife` | imported_script_function | PARTIAL |
| 105 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Neutral")` | global | MISSING |
| 106 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 107 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 108 | `TargetListSearch` | `1=TARGET_RANGE; 2=ANGLE` | global | PARTIAL |
| 112 | `IsTargetListEmpty` | `` | global | MISSING |
| 113 | `GetTargetListTop` | `` | global | PARTIAL |
| 115 | `target:GetName` | `` | object_method | PARTIAL |
| 120 | `PopTargetList` | `` | global | MISSING |
| 122 | `IsTargetListEmpty` | `` | global | MISSING |
| 133 | `RegisterAnim` | `1=GetPyOID("AnimDict", "spell_dh2_prince_drained_life_complete")` | global | PORTED |
| 143 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 144 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 145 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 147 | `TargetListSearch` | `1=RANGE; 2=ANGLE` | global | PARTIAL |
| 149 | `IsTargetListEmpty` | `` | global | MISSING |
| 150 | `GetTargetListTop` | `` | global | PARTIAL |
| 158 | `LookAtTarget` | `` | global | PARTIAL |
| 171 | `RegisterSkillEx` | `1={ update=OnSkillUpdate_, check=OnSkillCheck_, pre=OnPreSkill_, skill=OnSkill_, post=OnPostSkill_, delayed=OnDelayedSkill_, info=OnSkillInfo_, }` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:14` (authored assignment): <code>local RANGE               = 1000;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:15` (authored assignment): <code>local TARGET_RANGE        = 5000;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:21` (authored assignment): <code>local BUFF_ID         = GetPyOID("ClassTable", "Skill_Illusionist_Figments_buff");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:24` (authored assignment): <code>local SkillFX             =   GetPyOID("AnimatedEffectTable", "Skill_Madruk_Torso_lifedrain");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:25` (authored assignment): <code>local BuffFX              =   GetPyOID("AnimatedEffectTable", "Skill_Madruk_Torso_lifedrain_persist");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:26` (authored assignment): <code>local TargetFX            =   GetPyOID("AnimatedEffectTable", "Spell_Madruk_Torso_lifedrain");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:27` (authored assignment): <code>local TargetBuffFX    =   GetPyOID("AnimatedEffectTable", "Spell_Madruk_Torso_lifedrain_persist");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:29` (authored assignment): <code>local target_buff     =   nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:49` (authored assignment): <code>local target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:54` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:56` (authored assignment): <code>target = GetTargetListTop()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:57` (condition/gate): <code>if(target:IsCharacter()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:67` (authored assignment): <code>local damage = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:68` (authored assignment): <code>local final_hp = GetPropHP() + FromFixed(damage); -- Final HP will be the damage caused on the MC</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:75` (condition/gate): <code>if (traces) then Trace ("Here is the final HP of the monster:"..final_hp); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:78` (authored assignment): <code>target_buff = target:CreateBuff(BUFF_ID, 3000 , false, 1, TargetBuffFX);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:94` (authored assignment): <code>invisible_man=target;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:98` (authored assignment, skill/timer/script action): <code>buff_timer = StartSkillTimerCB(2000, 0, drainlife);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:112` (condition/gate): <code>if (not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:113` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:114` (condition/gate): <code>if (traces) then Trace("Name of the found character"..target:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:115` (condition/gate): <code>if (target:GetName()=="_prim_NPC_Madruk_Target") then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:116` (condition/gate): <code>if (traces) then Trace ("Hey Cool! found the target"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:128` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:132` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:133` (callback or registration): <code>RegisterAnim(GetPyOID("AnimDict", "spell_dh2_prince_drained_life_complete"));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:137` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:142` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:149` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:150` (authored assignment): <code>player = GetTargetListTop()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:156` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:163` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:167` (callback or registration): <code>local function OnDelayedSkill_(target)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:171` (callback or registration): <code>RegisterSkillEx({   update=OnSkillUpdate_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:172` (authored assignment): <code>check=OnSkillCheck_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:173` (authored assignment): <code>pre=OnPreSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:174` (authored assignment): <code>skill=OnSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:175` (authored assignment): <code>post=OnPostSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:176` (authored assignment): <code>delayed=OnDelayedSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:177` (authored assignment): <code>info=OnSkillInfo_,</code>

### madruktorso_dyingattack.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac

Relevant calls: 2; source lines: 47; engine readiness: 0/4.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "fx_swampking_blue_shockwave")` | global | PARTIAL |
| 47 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_; 8=nil` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac:21` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac:26` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac:31` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac:36` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac:41` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac:43` (skill/timer/script action): <code>CallPyScript("end_MadrukPhase3ToPhase2");</code>

### prince_assassin_assassinate.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac

Relevant calls: 2; source lines: 51; engine readiness: 1/7.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 19 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 51 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:7` (authored assignment): <code>local CLASS_ID       = GetPyOID("ClassTable", "Skill_Assassin_Assassinate");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:10` (authored assignment): <code>local skill_level   = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:13` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:18` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:19` (authored assignment): <code>local slvl = GetCurrentSkillInfo();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:21` (condition/gate): <code>if(slvl ~= skill_level) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:22` (authored assignment): <code>skill_level = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:34` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:39` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:43` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac:47` (callback or registration): <code>local function OnPostSkill_()</code>

### prince_assassin_assassination.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac

Relevant calls: 20; source lines: 124; engine readiness: 9/44.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 39 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 52 | `HasSkillCooldown` | `` | imported_script_function | PARTIAL |
| 58 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 60 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "AttackableOnly")` | global | MISSING |
| 62 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "ClosestFirst")` | global | MISSING |
| 64 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 65 | `TargetListBackup` | `1="MaxAngleSearch"` | global | MISSING |
| 67 | `IsTargetListEmpty` | `` | global | MISSING |
| 68 | `GetTargetListTop` | `` | global | PARTIAL |
| 74 | `SetSkillCooldown` | `1=cooldown` | imported_script_function | PARTIAL |
| 81 | `TargetListSearch` | `1=RANGE; 2=ANGLE; 3="MaxAngleSearch"` | global | PARTIAL |
| 82 | `GetTargetListTop` | `` | global | PARTIAL |
| 86 | `target:GetProp` | `1=GetPyStruct("CharacterProperties", "Level")` | object_method | PORTED |
| 93 | `target:Kill` | `1=GetSelf()` | object_method | PARTIAL |
| 94 | `PlaySound` | `1="sfx_passive_skill_activate_4"; 2=false; 3=0; 4=false` | global | PARTIAL |
| 99 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 106 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 111 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 120 | `ClearTarget` | `` | global | PORTED |
| 124 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:12` (authored assignment): <code>local RANGE               = 100.00;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:15` (authored assignment): <code>local CLASS_ID            = GetPyOID("ClassTable", "Skill_Assassin_Assassination");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:16` (authored assignment): <code>local BOSS_CLASS_ID   = GetPyOID("ClassTable", "Skill_Assassin_Assassination_boss");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:17` (authored assignment): <code>local USE_COOLDOWN    = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:21` (authored assignment): <code>local skill_level     = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:32` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:38` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:39` (authored assignment): <code>local slvl = GetCurrentSkillInfo();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:41` (condition/gate): <code>if(slvl ~= skill_level) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:42` (authored assignment): <code>skill_level   = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:45` (authored assignment): <code>cooldown = FromFixed(GetProp(GetPyStruct("CharacterProperties", "SnS_Cooldown"), true));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:46` (condition/gate): <code>if(cooldown &lt; 0) then cooldown = 0; end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:51` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:56` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:67` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:68` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:71` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:78` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:79` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:86` (authored assignment): <code>local en_level =  target:GetProp(GetPyStruct("CharacterProperties", "Level"));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:89` (condition/gate): <code>if (traces) then Trace("[Skills/Assassinate] Player level is Level "..FromFixed(level).." and ennemy level is Level "..FromFixed(en_level)); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:91` (condition/gate): <code>if (not IsBoss(target)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:102` (condition/gate): <code>if (not IsBoss(target)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:119` (callback or registration): <code>local function OnPostSkill_()</code>

### prince_assassin_blade_deflect.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac

Relevant calls: 2; source lines: 53; engine readiness: 1/7.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 21 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 53 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:8` (authored assignment): <code>local CLASS_ID     = GetPyOID("ClassTable", "Skill_Assassin_BladeDeflect");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:11` (authored assignment): <code>local skill_level     = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:15` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:20` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:21` (authored assignment): <code>local slvl = GetCurrentSkillInfo();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:23` (condition/gate): <code>if(slvl ~= skill_level) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:24` (authored assignment): <code>skill_level   = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:36` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:41` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:45` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac:49` (callback or registration): <code>local function OnPostSkill_()</code>

### prince_assassin_blindness.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac

Relevant calls: 8; source lines: 111; engine readiness: 11/32.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 34 | `attacker:GetID` | `` | object_method | PORTED |
| 36 | `SetIsTargetable` | `1=true` | global | PARTIAL |
| 49 | `SetIsTargetable` | `1=true` | global | PARTIAL |
| 62 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 83 | `SetIsTargetable` | `1=true` | global | PARTIAL |
| 94 | `SetIsTargetable` | `1=false` | global | PARTIAL |
| 99 | `StartSkillTimerCB` | `1=BUFF_TIME; 2=false; 3=BuffExpired` | imported_script_function | PARTIAL |
| 111 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:15` (authored assignment): <code>local MANA_SLVL       = ToFixed(3);   -- Mana cost per skill level</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:17` (authored assignment): <code>local RANGE               = 1200.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:19` (authored assignment): <code>local BUFF_ID     = GetPyOID("ClassTable", "Skill_Assassin_Blindness");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:20` (authored assignment): <code>local BUFF_FX     = GetPyOID("AnimatedEffectTable", "spell_dh2_assassin_blindness_persist");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:24` (authored assignment): <code>local skill_level = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:32` (callback or registration): <code>local function OnTargetHit_Skill(attacker, defender)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:33` (condition/gate): <code>if (skill_level &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:34` (condition/gate): <code>if (attacker:GetID() == GetID()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:47` (condition/gate): <code>if(buff and buff_timer == timerId) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:55` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:60` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:62` (authored assignment): <code>local slvl = GetCurrentSkillInfo();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:63` (condition/gate): <code>if(slvl ~= skill_level) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:64` (authored assignment): <code>skill_level   = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:68` (authored assignment): <code>mana_cost = CalcManaCost(BUFF_ID, skill_level);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:69` (authored assignment): <code>RANGE = FromFixed(GetProp(GetPyStruct("CharacterProperties", "TempProp1"),true));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:71` (condition/gate): <code>if (RANGE==0) then RANGE=500; end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:75` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:80` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:92` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:95` (authored assignment): <code>buff = CreateBuff(BUFF_ID, BUFF_TIME, false, skill_level, BUFF_FX);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:99` (authored assignment, skill/timer/script action): <code>buff_timer = StartSkillTimerCB(BUFF_TIME, false, BuffExpired);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:103` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:106` (callback or registration): <code>local function OnDelayedSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blindness.luac:108` (condition/gate): <code>if (traces) then Trace("Blindness OnDelayedSkill called"); end</code>

### prince_assassin_contaminate.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac

Relevant calls: 7; source lines: 77; engine readiness: 9/26.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 22 | `defender:GetID` | `` | object_method | PORTED |
| 22 | `attacker:TargetInMeleeRange` | `` | object_method | MISSING |
| 24 | `attacker:GetProp` | `1=GetPyStruct("CharacterProperties", "Slow_Resist_Chance")` | object_method | PORTED |
| 34 | `attacker:CreateBuff` | `1=BUFF_ID; 2=duration; 3=false; 4=skill_level; 5=BUFF_PERSIST` | object_method | PARTIAL |
| 35 | `attacker:ApplyBuff` | `1=BUFF_ID; 2=buff` | object_method | PARTIAL |
| 49 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 77 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:7` (authored assignment): <code>local CLASS_ID       = GetPyOID("ClassTable", "Skill_Assassin_Contaminate");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:9` (authored assignment): <code>local BUFF_PERSIST   = GetPyOID("AnimatedEffectTable", "AUTO_DEBUFF_SLOW");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:12` (authored assignment): <code>local skill_level       = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:20` (callback or registration): <code>local function OnTargetHit_Skill(attacker, defender)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:21` (condition/gate): <code>if (skill_level &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:22` (condition/gate): <code>if (defender:GetID() == GetID() and attacker:TargetInMeleeRange()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:23` (authored assignment): <code>local proc_chance = GetRand(0,100);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:24` (authored assignment): <code>local enemy_proc = GetRand(0,FromFixed(attacker:GetProp(GetPyStruct("CharacterProperties", "Slow_Resist_Chance"))));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:32` (condition/gate): <code>if (traces) then Trace("Now Slow chances are "..slow_final.."%%"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:33` (condition/gate): <code>if (proc_chance &lt; slow_final) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:34` (authored assignment): <code>local buff = attacker:CreateBuff(BUFF_ID, duration, false, skill_level, BUFF_PERSIST);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:36` (condition/gate): <code>if (traces) then Trace("Slow DEBUFF will be applied to "..attacker:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:43` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:48` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:49` (authored assignment): <code>local slvl = GetCurrentSkillInfo();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:51` (condition/gate): <code>if(slvl ~= skill_level) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:52` (authored assignment): <code>skill_level = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:60` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:65` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:69` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_contaminate.luac:73` (callback or registration): <code>local function OnPostSkill_()</code>

### prince_assassin_grapling_hook.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac

Relevant calls: 18; source lines: 119; engine readiness: 7/33.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 38 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 55 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 56 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 57 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 59 | `TargetListSearch` | `1=RANGE; 2=ANGLE` | global | PARTIAL |
| 61 | `IsTargetListEmpty` | `` | global | MISSING |
| 62 | `GetTargetListTop` | `` | global | PARTIAL |
| 70 | `PlaySound` | `1="sfx_skill_rogue_ass_throwing_knife"; 2=false; 3=0; 4=false` | global | PARTIAL |
| 71 | `SpawnSkillProjectile` | `1=PROJECTILE_ID` | imported_script_function | PARTIAL |
| 76 | `ClearTarget` | `` | global | PORTED |
| 82 | `target:IsCharacter` | `` | object_method | PARTIAL |
| 86 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 89 | `target:SetStunState` | `1=duration; 2=true` | object_method | MISSING |
| 93 | `target:GetState` | `` | object_method | PORTED |
| 100 | `target:HeadTo` | `1=GetPosition()` | object_method | MISSING |
| 100 | `GetPosition` | `` | global | PORTED |
| 105 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 112 | `RegisterSkillEx` | `1={ update=OnSkillUpdate_, check=OnSkillCheck_, pre=OnPreSkill_, skill=OnSkill_, post=OnPostSkill_, delayed=OnDelayedSkill_, info=OnSkillInfo_, }` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:10` (authored assignment): <code>local RANGE               = 1000;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:13` (authored assignment): <code>local CLASS_ID            = GetPyOID("ClassTable", "Skill_Assassin_GrapplingHook");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:18` (authored assignment): <code>local skill_level = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:20` (authored assignment): <code>local target          = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:32` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:37` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:38` (authored assignment): <code>local slvl = GetCurrentSkillInfo();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:40` (condition/gate): <code>if(slvl ~= skill_level) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:41` (authored assignment): <code>skill_level   = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:49` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:54` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:61` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:69` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:75` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:80` (callback or registration): <code>local function OnDelayedSkill_(target)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:81` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:82` (condition/gate): <code>if(target:IsCharacter()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:87` (condition/gate): <code>if (not IsBoss(target)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:88` (condition/gate): <code>if (not IsMiniBoss(target)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:93` (condition/gate): <code>if(target:GetState() == GetPyCst("AIStates", "Stunned")) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:94` (condition/gate): <code>if (traces) then Trace(""..target:GetName().." is stunned... bringing him to papa"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:98` (condition/gate): <code>if (not IsBoss(target)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:99` (condition/gate): <code>if (not IsMiniBoss(target)) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:112` (callback or registration): <code>RegisterSkillEx({   update=OnSkillUpdate_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:113` (authored assignment): <code>check=OnSkillCheck_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:114` (authored assignment): <code>pre=OnPreSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:115` (authored assignment): <code>skill=OnSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:116` (authored assignment): <code>post=OnPostSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:117` (authored assignment): <code>delayed=OnDelayedSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:118` (authored assignment): <code>info=OnSkillInfo_,</code>

### prince_assassin_poison_sting.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac

Relevant calls: 5; source lines: 109; engine readiness: 7/22.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 57 | `StartSkillTimerCB` | `1=BUFF_TIME; 2=0; 3=BuffExpired` | imported_script_function | PARTIAL |
| 62 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 78 | `OnSkill_` | `` | local_function | PORTED |
| 101 | `ClearTarget` | `` | global | PORTED |
| 109 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:15` (authored assignment): <code>local MANA_SLVL        = ToFixed(3);   -- Mana cost per skill level</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:17` (authored assignment): <code>local RANGE                = 300.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:19` (authored assignment): <code>local BUFF_FX      = GetPyOID("AnimatedEffectTable", "skill_dh2_assassin_poison_sting_persist")</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:20` (authored assignment): <code>local BUFF_ID      = GetPyOID("ClassTable", "Skill_Assassin_PoisonSting");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:24` (authored assignment): <code>local skill_level  = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:37` (condition/gate): <code>if(buff and buff_timer == timerId) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:38` (condition/gate): <code>if (traces) then Trace("[Skills | Poison Sting] Buff expired"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:44` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:50` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:51` (condition/gate): <code>if (traces) then Trace("[Skills/Poison Sting] Activating the poison sting buff"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:53` (authored assignment): <code>buff = CreateBuff(BUFF_ID, BUFF_TIME, false, skill_level, BUFF_FX);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:57` (authored assignment, skill/timer/script action): <code>buff_timer = StartSkillTimerCB(BUFF_TIME, 0, BuffExpired);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:61` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:62` (authored assignment): <code>local slvl = GetCurrentSkillInfo();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:64` (condition/gate): <code>if(slvl ~= skill_level) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:65` (authored assignment): <code>skill_level    = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:68` (authored assignment): <code>mana_cost  = CalcManaCost(BUFF_ID, skill_level);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:71` (condition/gate): <code>if (traces) then Trace("[Skills/Poison Sting] Failed to determinate Buff time!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:83` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:88` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:90` (condition/gate): <code>if (traces) then Trace("[Skills/Poison Sting] Deactivating the poison sting buff"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:100` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:104` (callback or registration): <code>local function OnDelayedSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac:106` (condition/gate): <code>if (traces) then Trace("[Skills/Poison Sting] OnDelayedSkill called"); end</code>

### prince_assassin_treasure_hunter.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac

Relevant calls: 2; source lines: 51; engine readiness: 1/7.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 19 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 51 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:7` (authored assignment): <code>local CLASS_ID       = GetPyOID("ClassTable", "Skill_Assassin_TreasureHunter");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:10` (authored assignment): <code>local skill_level   = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:13` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:18` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:19` (authored assignment): <code>local slvl = GetCurrentSkillInfo();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:21` (condition/gate): <code>if(slvl ~= skill_level) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:22` (authored assignment): <code>skill_level = slvl;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:34` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:39` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:43` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac:47` (callback or registration): <code>local function OnPostSkill_()</code>

### sandworm_emerge.luac — .local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac

Relevant calls: 14; source lines: 73; engine readiness: 0/19.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 34 | `PlaySound` | `1="sfx_serpent_submerge"; 2=false; 3=0; 4=false` | global | PARTIAL |
| 36 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 37 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 38 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 40 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 42 | `IsTargetListEmpty` | `` | global | MISSING |
| 43 | `GetTargetListTop` | `` | global | PARTIAL |
| 57 | `GetTargetListTop` | `` | global | PARTIAL |
| 58 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 59 | `PopTargetList` | `` | global | MISSING |
| 60 | `IsTargetListEmpty` | `` | global | MISSING |
| 63 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "sandworm_shockwave")` | global | PARTIAL |
| 64 | `PlaySound` | `1="sfx_serpent_shockwave.wav"; 2=false; 3=0; 4=false` | global | PARTIAL |
| 73 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_; 8=nil` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:12` (authored assignment): <code>local RANGE             = 400.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:14` (authored assignment): <code>local CLASS_ID          = GetPyOID("ClassTable", "WormSkillAOEPushback");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:24` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:28` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:33` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:42` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:43` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:46` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:51` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:52` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:57` (authored assignment): <code>local target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_emerge.luac:69` (callback or registration): <code>local function OnPostSkill_()</code>

### sandworm_spit.luac — .local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac

Relevant calls: 27; source lines: 121; engine readiness: 10/48.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 45 | `HasTarget` | `` | global | PORTED |
| 46 | `GetTarget` | `` | global | PORTED |
| 50 | `g_target:GetName` | `` | object_method | PARTIAL |
| 53 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 54 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 55 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 57 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 59 | `IsTargetListEmpty` | `` | global | MISSING |
| 60 | `GetTargetListTop` | `` | global | PARTIAL |
| 70 | `SpawnSkillProjectile` | `1=PROJECTILE_ID` | imported_script_function | PARTIAL |
| 78 | `target:GetPosition` | `` | object_method | PORTED |
| 82 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 84 | `SummonTimerTrap` | `1=TIMERTRAP_ID; 2=TIMERTRAP_DAM_ID; 3=x; 4=y; 5=z` | global | PARTIAL |
| 88 | `hack:GetPosition` | `` | object_method | PORTED |
| 90 | `SummonTimerTrap` | `1=TIMERTRAP_ID; 2=TIMERTRAP_DAM_ID; 3=x; 4=y; 5=z` | global | PARTIAL |
| 94 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 95 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 96 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 98 | `TargetListSearch` | `1=AOE_RANGE; 2=360; 3=x; 4=y; 5=z` | global | PARTIAL |
| 100 | `IsTargetListEmpty` | `` | global | MISSING |
| 104 | `GetTargetListTop` | `` | global | PARTIAL |
| 105 | `PopTargetList` | `` | global | MISSING |
| 107 | `aoe_target:GetID` | `` | object_method | PORTED |
| 107 | `target:GetID` | `` | object_method | PORTED |
| 108 | `SkillCombatRoll` | `1=aoe_target` | imported_script_function | PARTIAL |
| 110 | `IsTargetListEmpty` | `` | global | MISSING |
| 115 | `RegisterSkillEx` | `1={ update=OnSkillUpdate_, check=OnSkillCheck_, pre=OnPreSkill_, skill=OnSkill_, delayed=OnDelayedSkill_, info=OnSkillInfo_, }` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:10` (authored assignment): <code>local RANGE               = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:11` (authored assignment): <code>local AOE_RANGE           = 250.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:15` (authored assignment): <code>local CLASS_ID                = GetPyOID("ClassTable", "WormSkillPoisonAttack");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:16` (authored assignment): <code>local AOE_CLASS_ID        = GetPyOID("ClassTable", "WormSkillPoisonAttackAOE");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:17` (authored assignment): <code>local PROJECTILE_ID       = GetProp(GetPyStruct("CharacterProperties", "RangeProjectileID"));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:19` (authored assignment): <code>local TIMERTRAP_ID        = GetPyOID("TimerTraps", "SandwormSteamCloud");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:20` (authored assignment): <code>local TIMERTRAP_DAM_ID    = GetPyOID("GameObjectDamager", "Level01Damage");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:31` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:35` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:39` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:44` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:45` (condition/gate): <code>if (HasTarget()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:46` (authored assignment): <code>g_target = GetTarget();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:50` (authored assignment): <code>local dist = FromFixed(GetDistanceBetween(GetName(), g_target:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:52` (condition/gate): <code>if (dist &gt; RANGE) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:59` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:60` (authored assignment): <code>g_target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:68` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:74` (callback or registration): <code>local function OnDelayedSkill_(target, projectile)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:77` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:100` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:104` (authored assignment): <code>local aoe_target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:107` (condition/gate): <code>if(not target or aoe_target:GetID() ~= target:GetID()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:115` (callback or registration): <code>RegisterSkillEx({   update=OnSkillUpdate_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:116` (authored assignment): <code>check=OnSkillCheck_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:117` (authored assignment): <code>pre=OnPreSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:118` (authored assignment): <code>skill=OnSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:119` (authored assignment): <code>delayed=OnDelayedSkill_,</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:120` (authored assignment): <code>info=OnSkillInfo_,</code>

### winddragon_aoebombs.luac — .local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac

Relevant calls: 38; source lines: 246; engine readiness: 5/61.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 57 | `obj:GetName` | `` | object_method | PARTIAL |
| 67 | `objspawns:GetName` | `` | object_method | PARTIAL |
| 126 | `StopTimerCB` | `1=timer_damage_dealer` | imported_script_function | PARTIAL |
| 128 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 129 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 130 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 132 | `TargetListSearch` | `1=SPLASH_RANGE; 2=360; 3=g_bomb_spawns[g_target_num]` | global | PARTIAL |
| 134 | `IsTargetListEmpty` | `` | global | MISSING |
| 136 | `GetTargetListTop` | `` | global | PARTIAL |
| 137 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 138 | `PopTargetList` | `` | global | MISSING |
| 139 | `IsTargetListEmpty` | `` | global | MISSING |
| 143 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 144 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 145 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 147 | `TargetListSearch` | `1=SPLASH_RANGE; 2=360; 3=g_targetBombs_list[damage_dealer_flag]` | global | PARTIAL |
| 149 | `IsTargetListEmpty` | `` | global | MISSING |
| 151 | `GetTargetListTop` | `` | global | PARTIAL |
| 152 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 153 | `PopTargetList` | `` | global | MISSING |
| 154 | `IsTargetListEmpty` | `` | global | MISSING |
| 167 | `StopTimerCB` | `1=timer_fx_launcher` | imported_script_function | PARTIAL |
| 169 | `getTargetMum` | `` | local_function | PORTED |
| 171 | `PlaySound` | `1="sfx_dragon_reaperstorm"; 2=false; 3=0; 4=false` | global | PARTIAL |
| 175 | `PlaySound` | `1="sfx_dragon_reaperstorm"; 2=false; 3=0; 4=false` | global | PARTIAL |
| 203 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 204 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 205 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 207 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 209 | `IsTargetListEmpty` | `` | global | MISSING |
| 211 | `GetTargetListTop` | `` | global | PARTIAL |
| 212 | `target:GetID` | `` | object_method | PORTED |
| 216 | `PopTargetList` | `` | global | MISSING |
| 217 | `IsTargetListEmpty` | `` | global | MISSING |
| 229 | `getTargetBombs` | `` | local_function | PORTED |
| 234 | `StartSkillTimerCB` | `1=LAUNCHER_DELAY; 2=true; 3=Do_PlayFX` | imported_script_function | PARTIAL |
| 235 | `StartSkillTimerCB` | `1=DAMAGE_DELAY; 2=true; 3=Do_Damages` | imported_script_function | PARTIAL |
| 246 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:14` (authored assignment): <code>local RANGE             = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:15` (authored assignment): <code>local SPLASH_RANGE  = 150; -- Damage range of the bomb once it lands.</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:18` (authored assignment): <code>local MAX_BOMBS     = 3;        -- Max number of bomb that can be launched when performing the skill.</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:21` (authored assignment): <code>local BOMB_FX_ID        = GetPyOID("AnimatedEffectTable", "WindDragonAoEBomb");</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:23` (authored assignment): <code>local LAUNCHER_DELAY    = 500;  -- Delay before each FX launch (since it's a delayed FX the damage delay needs to be tweaked accordingly)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:24` (authored assignment): <code>local DAMAGE_DELAY  = 750;  -- Delay before applying damage to the target (both timers are started at the same time).</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:28` (authored assignment): <code>local g_bomb_spawns = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:29` (authored assignment): <code>local g_target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:30` (authored assignment): <code>local g_targetBombs_list    = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:34` (authored assignment): <code>local g_target_num          = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:45` (condition/gate): <code>if (traces) then Trace("Get the bomb spawns!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:46` (authored assignment): <code>local spawns    = g_bomb_spawns;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:61` (authored assignment): <code>local num_spawns = 1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:63` (condition/gate): <code>if (#spawns &gt; 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:67` (authored assignment): <code>local obj_spawnName = objspawns:GetName();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:68` (condition/gate): <code>if (traces) then Trace("This target name is named:"..objspawns:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:69` (condition/gate): <code>if (obj_name == obj_spawnName) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:73` (authored assignment): <code>num_spawns = num_spawns + 1</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:88` (condition/gate): <code>if (traces) then Trace("Get the targets of the bombs:"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:96` (authored assignment): <code>g_targetBombs_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:97` (authored assignment): <code>dist = FromFixed(GetDistanceBetween(g_target_list[currIndex]:GetName(), g_bomb_spawns[k]:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:98` (condition/gate): <code>if (traces) then Trace("Target name is "..g_bomb_spawns[k]:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:99` (authored assignment): <code>dist_obj = g_bomb_spawns[k];</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:101` (authored assignment): <code>local temp_dist = FromFixed(GetDistanceBetween(g_target_list[currIndex]:GetName(), g_bomb_spawns[k]:GetName()));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:102` (condition/gate): <code>if (traces) then Trace("Target name is "..g_bomb_spawns[k]:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:105` (authored assignment): <code>dist_obj = g_bomb_spawns[k];</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:117` (condition/gate): <code>if (traces) then Trace("This is get Target Mum!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:118` (authored assignment): <code>g_target_num = GetRand(1,#g_bomb_spawns);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:124` (condition/gate): <code>if (damage_dealer_flag &gt; #g_targetBombs_list and damage_dealer_flag &gt; MAX_BOMBS and timer_damage_dealer) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:125` (authored assignment): <code>g_targetBombs_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:126` (skill/timer/script action): <code>StopTimerCB(timer_damage_dealer);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:134` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:136` (authored assignment): <code>local target = GetTargetListTop()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:149` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:151` (authored assignment): <code>local target = GetTargetListTop()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:164` (condition/gate): <code>if (traces) then Trace("Play the FX on"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:165` (condition/gate): <code>if (fx_launcher_flag &gt; #g_targetBombs_list and fx_launcher_flag &gt; MAX_BOMBS and timer_fx_launcher) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:166` (authored assignment): <code>g_targetBombs_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:167` (skill/timer/script action): <code>StopTimerCB(timer_fx_launcher);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:183` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:187` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:188` (condition/gate): <code>if (#g_bomb_spawns == 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:194` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:199` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:201` (authored assignment): <code>g_target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:209` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:211` (authored assignment): <code>local target = GetTargetListTop()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:212` (condition/gate): <code>if (target:GetID()~=nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:214` (condition/gate): <code>if (traces) then Trace("Target name is "..target:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:219` (authored assignment): <code>g_target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:220` (condition/gate): <code>if (traces) then Trace("Really no target to add this time!"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:222` (condition/gate): <code>if (traces) then Trace("Dragon AOE bomb skill"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:227` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:234` (authored assignment, skill/timer/script action): <code>timer_fx_launcher = StartSkillTimerCB(LAUNCHER_DELAY, true, Do_PlayFX);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:235` (authored assignment, skill/timer/script action): <code>timer_damage_dealer = StartSkillTimerCB(DAMAGE_DELAY, true, Do_Damages);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:242` (callback or registration): <code>local function OnPostSkill_()</code>

### winddragon_bite.luac — .local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac

Relevant calls: 8; source lines: 66; engine readiness: 0/13.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 38 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Enemy")` | global | MISSING |
| 39 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 40 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 42 | `TargetListSearch` | `1=RANGE` | global | PARTIAL |
| 44 | `IsTargetListEmpty` | `` | global | MISSING |
| 45 | `GetTargetListTop` | `` | global | PARTIAL |
| 57 | `SkillCombatRoll` | `1=target` | imported_script_function | PARTIAL |
| 66 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:12` (authored assignment): <code>local RANGE             = 150.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:24` (callback or registration): <code>local function OnSkillInfo_(slvl)</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:28` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:32` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:37` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:44` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:45` (authored assignment): <code>target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:48` (authored assignment): <code>target = nil;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:53` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:55` (condition/gate): <code>if(target) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:57` (authored assignment): <code>local dmg = SkillCombatRoll(target);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_bite.luac:62` (callback or registration): <code>local function OnPostSkill_()</code>

### winddragon_breathsweep.luac — .local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac

Relevant calls: 45; source lines: 266; engine readiness: 6/68.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 40 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 41 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 42 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 44 | `TargetListSearch` | `1=RANGE; 2=ANGLE_MAX` | global | PARTIAL |
| 45 | `TargetListBackup` | `1="breath_sweep"` | global | MISSING |
| 47 | `IsTargetListEmpty` | `` | global | MISSING |
| 51 | `GetTargetListTop` | `` | global | PARTIAL |
| 53 | `PopTargetList` | `` | global | MISSING |
| 54 | `IsTargetListEmpty` | `` | global | MISSING |
| 65 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 66 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 67 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 69 | `TargetListSearch` | `1=ATTACK_RANGE; 2=ANGLE; 3="breath_sweep"` | global | PARTIAL |
| 71 | `IsTargetListEmpty` | `` | global | MISSING |
| 74 | `GetTargetListTop` | `` | global | PARTIAL |
| 75 | `temp_target:GetID` | `` | object_method | PORTED |
| 79 | `object:GetID` | `` | object_method | PORTED |
| 91 | `PopTargetList` | `` | global | MISSING |
| 92 | `IsTargetListEmpty` | `` | global | MISSING |
| 102 | `searchForNewTarget` | `` | local_function | PORTED |
| 107 | `searchForTargetAtAngle` | `` | local_function | PORTED |
| 118 | `object:GetID` | `` | object_method | PORTED |
| 126 | `object:GetName` | `` | object_method | PARTIAL |
| 126 | `attackedObject:GetName` | `` | object_method | PARTIAL |
| 133 | `SkillCombatRoll` | `1=object` | imported_script_function | PARTIAL |
| 134 | `object:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "spell_wind_hurt")` | object_method | PARTIAL |
| 140 | `SkillCombatRoll` | `1=object` | imported_script_function | PARTIAL |
| 141 | `object:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "spell_wind_hurt")` | object_method | PARTIAL |
| 153 | `object:GetID` | `` | object_method | PORTED |
| 162 | `object:GetName` | `` | object_method | PARTIAL |
| 162 | `attackedObject:GetName` | `` | object_method | PARTIAL |
| 169 | `SkillCombatRoll` | `1=object` | imported_script_function | PARTIAL |
| 170 | `object:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "spell_wind_hurt")` | object_method | PARTIAL |
| 177 | `SkillCombatRoll` | `1=object` | imported_script_function | PARTIAL |
| 178 | `object:PlayFX` | `1=GetPyOID("AnimatedEffectTable", "spell_wind_hurt")` | object_method | PARTIAL |
| 209 | `StopTimerCB` | `1=timer_id` | imported_script_function | PARTIAL |
| 228 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 229 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 230 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "FrontalFirst")` | global | MISSING |
| 238 | `TargetListSearch` | `1=RANGE; 2=ANGLE_MAX` | global | PARTIAL |
| 239 | `TargetListBackup` | `1="breath_sweep"` | global | MISSING |
| 241 | `searchForNewTarget` | `` | local_function | PORTED |
| 250 | `StartSkillTimerCB` | `1=timer_time; 2=true; 3=DoDamage` | imported_script_function | PARTIAL |
| 257 | `StopTimerCB` | `1=timer_id` | imported_script_function | PARTIAL |
| 266 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:12` (authored assignment): <code>local RANGE              = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:13` (authored assignment): <code>local ATTACK_RANGE           = 1000.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:23` (authored assignment): <code>local timer_time             = 140;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:24` (authored assignment): <code>local timer_id               = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:26` (authored assignment): <code>local target_list            = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:27` (authored assignment): <code>local target_list_size       = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:29` (authored assignment): <code>local attacked_targets       = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:47` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:48` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:51` (authored assignment): <code>local target = GetTargetListTop()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:57` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:60` (authored assignment): <code>target_list_size = #target_list;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:71` (condition/gate): <code>if (not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:74` (authored assignment): <code>local temp_target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:75` (authored assignment): <code>local temp_target_ID = temp_target:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:77` (condition/gate): <code>if(#target_list &gt; 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:79` (authored assignment): <code>local target_ID = object:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:80` (condition/gate): <code>if (temp_target_ID == target_ID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:86` (authored assignment): <code>local target_ID = target_list[1]:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:87` (condition/gate): <code>if (temp_target_ID == target_ID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:88` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:95` (authored assignment): <code>target_list_size = #target_list;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:105` (condition/gate): <code>if (target_list_size &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:109` (condition/gate): <code>if (target_list_size &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:120` (condition/gate): <code>if (#attacked_targets &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:123` (authored assignment): <code>local gonnaAttackIt = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:126` (condition/gate): <code>if (object:GetName() == attackedObject:GetName()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:127` (authored assignment): <code>gonnaAttackIt = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:130` (condition/gate): <code>if(gonnaAttackIt) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:155` (condition/gate): <code>if (#attacked_targets &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:158` (authored assignment): <code>local gonnaAttackItReverse = true;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:162` (condition/gate): <code>if (object:GetName() == attackedObject:GetName()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:164` (authored assignment): <code>gonnaAttackItReverse = false;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:166` (condition/gate): <code>if (gonnaAttackItReverse) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:188` (authored assignment): <code>attacked_targets = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:209` (skill/timer/script action): <code>StopTimerCB(timer_id);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:213` (authored assignment): <code>target_list_size = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:214` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:218` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:222` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:227` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:234` (authored assignment): <code>attacked_targets = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:245` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:250` (authored assignment, skill/timer/script action): <code>timer_id = StartSkillTimerCB(timer_time, true, DoDamage);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:255` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:257` (skill/timer/script action): <code>StopTimerCB(timer_id);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:258` (authored assignment): <code>target_list_size = 0;</code>

### winddragon_insidesweep.luac — .local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac

Relevant calls: 31; source lines: 206; engine readiness: 4/50.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 40 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 41 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 42 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 49 | `TargetListSearch` | `1=ATTACK_RANGE; 2=ANGLE_MAX` | global | PARTIAL |
| 50 | `TargetListBackup` | `1="insideSweep"` | global | MISSING |
| 52 | `IsTargetListEmpty` | `` | global | MISSING |
| 56 | `GetTargetListTop` | `` | global | PARTIAL |
| 58 | `PopTargetList` | `` | global | MISSING |
| 59 | `IsTargetListEmpty` | `` | global | MISSING |
| 69 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 70 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 71 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 73 | `TargetListSearch` | `1=ATTACK_RANGE; 2=DEAD_ANGLE; 3="insideSweep"` | global | PARTIAL |
| 75 | `IsTargetListEmpty` | `` | global | MISSING |
| 77 | `GetTargetListTop` | `` | global | PARTIAL |
| 78 | `temp_target:GetID` | `` | object_method | PORTED |
| 82 | `v:GetID` | `` | object_method | PORTED |
| 95 | `PopTargetList` | `` | global | MISSING |
| 96 | `IsTargetListEmpty` | `` | global | MISSING |
| 115 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 116 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 117 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 126 | `TargetListSearch` | `1=RANGE; 2=ANGLE_MAX_SEARCH` | global | PARTIAL |
| 127 | `TargetListBackup` | `1="insideSweep"` | global | MISSING |
| 140 | `getDeadAngleTargetSearchList` | `` | local_function | PORTED |
| 146 | `SkillCombatRoll` | `1=target_list[curr_index]` | imported_script_function | PARTIAL |
| 155 | `SkillCombatRoll` | `1=target_list[1]` | imported_script_function | PARTIAL |
| 179 | `StopTimerCB` | `1=timer_id` | imported_script_function | PARTIAL |
| 191 | `StartSkillTimerCB` | `1=timer_time; 2=true; 3=DoDamage` | imported_script_function | PARTIAL |
| 199 | `StopTimerCB` | `1=timer_id` | imported_script_function | PARTIAL |
| 206 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:12` (authored assignment): <code>local RANGE              = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:13` (authored assignment): <code>local ATTACK_RANGE       = 1700.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:29` (authored assignment): <code>local timer_time             = 100;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:30` (authored assignment): <code>local timer_id           = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:32` (authored assignment): <code>local target_list        = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:33` (authored assignment): <code>local target_list_size   = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:52` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:53` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:56` (authored assignment): <code>local target = GetTargetListTop()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:62` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:65` (authored assignment): <code>target_list_size = #target_list;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:75` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:77` (authored assignment): <code>local temp_target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:78` (authored assignment): <code>local temp_target_ID = temp_target:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:80` (condition/gate): <code>if (#target_list &gt; 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:82` (authored assignment): <code>local target_ID = v:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:83` (condition/gate): <code>if (temp_target_ID == target_ID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:89` (authored assignment): <code>local target_ID = target_list[1]:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:90` (condition/gate): <code>if (temp_target_ID == target_ID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:91` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:92` (condition/gate): <code>if (traces) then Trace("#target_list: "..#target_list); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:99` (authored assignment): <code>target_list_size = #target_list;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:105` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:109` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:114` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:139` (condition/gate): <code>if(target_list_size &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:142` (condition/gate): <code>if(target_list_size &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:143` (condition/gate): <code>if (target_list_size &gt; 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:145` (condition/gate): <code>if (target_list[curr_index]:GetID()~=nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:149` (condition/gate): <code>if (traces) then Trace("The target list item "..curr_index.." is nil, Why?"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:154` (condition/gate): <code>if (target_list[1]:GetID()~=nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:157` (condition/gate): <code>if (traces) then Trace("The target is:"..target_list[1]:GetName()); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:159` (condition/gate): <code>if (traces) then Trace("The target list item 1 is nil, Why?:"); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:179` (skill/timer/script action): <code>StopTimerCB(timer_id);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:180` (authored assignment): <code>target_list_size = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:186` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:191` (authored assignment, skill/timer/script action): <code>timer_id = StartSkillTimerCB(timer_time, true, DoDamage);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:197` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:199` (skill/timer/script action): <code>StopTimerCB(timer_id);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:200` (authored assignment): <code>target_list_size = 0;</code>

### winddragon_outsidesweep.luac — .local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac

Relevant calls: 31; source lines: 196; engine readiness: 4/46.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 39 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 40 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 41 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 46 | `TargetListSearch` | `1=ATTACK_RANGE; 2=ANGLE_MAX` | global | PARTIAL |
| 47 | `TargetListBackup` | `1="insideSweep"` | global | MISSING |
| 49 | `IsTargetListEmpty` | `` | global | MISSING |
| 53 | `GetTargetListTop` | `` | global | PARTIAL |
| 55 | `PopTargetList` | `` | global | MISSING |
| 56 | `IsTargetListEmpty` | `` | global | MISSING |
| 66 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 67 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 68 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 70 | `TargetListSearch` | `1=ATTACK_RANGE; 2=DEAD_ANGLE; 3="insideSweep"` | global | PARTIAL |
| 72 | `IsTargetListEmpty` | `` | global | MISSING |
| 74 | `GetTargetListTop` | `` | global | PARTIAL |
| 75 | `temp_target:GetID` | `` | object_method | PORTED |
| 79 | `v:GetID` | `` | object_method | PORTED |
| 92 | `PopTargetList` | `` | global | MISSING |
| 93 | `IsTargetListEmpty` | `` | global | MISSING |
| 112 | `SetTargetListCharacterFilter` | `1=GetPyCst("AITargetList_CharacterFilter", "Player")` | global | MISSING |
| 113 | `SetTargetListObjectFilter` | `1=GetPyCst("AITargetList_ObjectFilter", "NONE")` | global | MISSING |
| 114 | `SetTargetListSorting` | `1=GetPyCst("AITargetList_Sorting", "NoSort")` | global | MISSING |
| 123 | `TargetListSearch` | `1=RANGE; 2=ANGLE_MAX_SEARCH` | global | PARTIAL |
| 124 | `TargetListBackup` | `1="insideSweep"` | global | MISSING |
| 137 | `getDeadAngleTargetSearchList` | `` | local_function | PORTED |
| 143 | `SkillCombatRoll` | `1=target_list[curr_index]` | imported_script_function | PARTIAL |
| 150 | `SkillCombatRoll` | `1=target_list[1]` | imported_script_function | PARTIAL |
| 169 | `StopTimerCB` | `1=timer_id` | imported_script_function | PARTIAL |
| 181 | `StartSkillTimerCB` | `1=timer_time; 2=true; 3=DoDamage` | imported_script_function | PARTIAL |
| 189 | `StopTimerCB` | `1=timer_id` | imported_script_function | PARTIAL |
| 196 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_` | imported_script_function | PARTIAL |

Authored source excerpts (direct lines; meanings across callbacks remain unresolved unless explained in the named flow notes):

- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:12` (authored assignment): <code>local RANGE                 = FromFixed(GetProp(GetPyStruct("CharacterProperties", "RangeMaxDistance")));</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:13` (authored assignment): <code>local ATTACK_RANGE      = 1000.0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:29` (authored assignment): <code>local timer_time            = 150;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:30` (authored assignment): <code>local timer_id          = -1;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:32` (authored assignment): <code>local target_list       = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:33` (authored assignment): <code>local target_list_size  = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:49` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:50` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:53` (authored assignment): <code>local target = GetTargetListTop()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:59` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:62` (authored assignment): <code>target_list_size = #target_list;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:72` (condition/gate): <code>if(not IsTargetListEmpty()) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:74` (authored assignment): <code>local temp_target = GetTargetListTop();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:75` (authored assignment): <code>local temp_target_ID = temp_target:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:77` (condition/gate): <code>if (#target_list &gt; 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:79` (authored assignment): <code>local target_ID = v:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:80` (condition/gate): <code>if (temp_target_ID == target_ID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:86` (authored assignment): <code>local target_ID = target_list[1]:GetID();</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:87` (condition/gate): <code>if (temp_target_ID == target_ID) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:88` (authored assignment): <code>target_list = {};</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:89` (condition/gate): <code>if (traces) then Trace("#target_list: "..#target_list); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:96` (authored assignment): <code>target_list_size = #target_list;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:102` (callback or registration): <code>local function OnSkillUpdate_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:106` (callback or registration): <code>local function OnSkillCheck_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:111` (callback or registration): <code>local function OnPreSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:121` (condition/gate): <code>if (traces) then Trace("SKILL OUTSIDESWEEP: DEAD_ANGLE: "..DEAD_ANGLE); end</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:136` (condition/gate): <code>if(target_list_size &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:139` (condition/gate): <code>if(target_list_size &gt; 0) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:140` (condition/gate): <code>if (target_list_size &gt; 1) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:142` (condition/gate): <code>if (not target_list[curr_index]==nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:149` (condition/gate): <code>if (not target_list[1]==nil) then</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:169` (skill/timer/script action): <code>StopTimerCB(timer_id);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:170` (authored assignment): <code>target_list_size = 0;</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:176` (callback or registration): <code>local function OnSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:181` (authored assignment, skill/timer/script action): <code>timer_id = StartSkillTimerCB(timer_time, true, DoDamage);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:187` (callback or registration): <code>local function OnPostSkill_()</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:189` (skill/timer/script action): <code>StopTimerCB(timer_id);</code>
- `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:190` (authored assignment): <code>target_list_size = 0;</code>

## Investigation evidence

**Visual evidence source:** the B155/B156 handoff records the original gameplay video sequence: Mud Lord emerge 18:46–18:52, bite attacks 18:53–18:56, dive/re-emerge 19:08–19:12.5, steam-cloud activations 19:17–19:24 and quest completion 20:48. Witch footage is summarized for 966–1000 s. These timestamps are inherited from `coordination/v21.7/B155-B156-HANDOFF.md`; this atlas pass did not independently replay the footage.

**Logic evidence:** Lua source call sites and arguments are extracted from `.local-inputs/assets-extra/android/data/scripts/`. Native addresses and body excerpts come from `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`. `Character::_DoSkill` (0x003b8bd8) requires a numeric slot, bounds-checks it against the skill list, then dispatches through `CharAI::AI_UseSkill`; script-side phase/range/timer gates still control admission. `Character::_SetIsTargetable` (0x003b7a64) stores a boolean targetability byte; the B155/B158 handoff says it gates player targeting, not damage.

**Expected behavior:** preserve authored callbacks, phases, skill slots, timers, search rules, animation/audio/effect hooks and data-table references. This atlas maps those contracts and current owners; it does not add runtime behavior.

## Mud Lord / Swamp King flow

`swampking_core.luac` registers init, spotted, range, update, death and animation-end callbacks. Update gates phase two at `HPPercentCurr <= phase2_initPct` (75), then requires AI Idle for attack/dive timers. Animation-end timers schedule movement, emerge, retarget and decision; `DiveAttackDecision` caps the dive sequence at two. `DoSkill` sites select bite/spit/emerge/intimidate slots. B158 confirms target reacquisition, relocation and targetability toggles in runtime. B159 supersedes early gaps: level scaling, damage calibration and AOE pushback were implemented. Remaining gaps include `MarkAsFlying`, camera-frustum awake latch, resetonhubchange, timer/culling differences, TimerTrap point-cloud bounds and projectile expiry.

## Witch flow

Static source asserts one `DoSkill(0)` and a 10,000 ms teleport cadence. B156 reports WitchImpactFX on the hero and teleport flashes; orange/red and blue lightning effects land on the Witch and are inferred to be player/faery effects from simultaneous hotbar cooldowns. The handoff found no additional Witch-side skill. This pass did not independently replay footage.

## Mapping and implementation evidence

IDA registration tables matched 271 function names and retained 498 binding/address records. 31 native contracts have reviewed meanings; 257 retain native evidence but need caller/admission/effect review. Port counts: 47 PORTED, 174 PARTIAL, 67 MISSING.

PORTED requires a source-compatible callback and production VM/session binding, or a clearly scoped source-backed owner for the named boss/Witch contexts. PARTIAL marks provider or owner limits. A name mention alone is not implementation evidence.

## Static verification

`coverage_check(ctx)` independently re-extracted all 219 scripts and 7982 call sites with no inventory differences. `known_checks(ctx)` passed exact call-list fixtures (Mud Lord: 46 names; Witch: 44 names) plus production source anchors for skill slots, timer/teleport, targetability and FX. These are static checks, not runtime tests.

## Integrated runtime / visual verification

No runtime or visual tests were run by this atlas task. B155/B158/B159/B156 reports document prior build-specific evidence but do not show integration of this atlas map into a new gameplay path.

## Remaining gaps

Call mapping: 4170 DONE and 3812 UNRESOLVED. Context engine-call readiness: 1670 PORTED, 4083 PARTIAL, 919 MISSING and 0 UNKNOWN. 4783 reference sites feed the combined resolution ledger. Dynamic calls, unresolved native contracts, enum/table rules and per-call production coverage remain follow-up.

The Android `.luac` files are plaintext Lua source, detected by file headers; seven `.nomedia` markers are excluded. No iOS scripts were found. Android IDA is the only native map; no cross-platform binding equivalence is assumed.
