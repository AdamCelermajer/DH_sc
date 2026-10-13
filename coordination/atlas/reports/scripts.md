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

### assassin2.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin2.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassin2_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin2_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassin3.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin3.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassin3_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassin3_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassindemon.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassindemon.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### assassindemon_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/assassindemon_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### balrog.luac — .local-inputs/assets-extra/android/data/scripts/ai/balrog.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### balrog_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/balrog_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

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

### darkqueen1_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### darkqueen2.luac — .local-inputs/assets-extra/android/data/scripts/ai/darkqueen2.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### darkqueen2_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/darkqueen2_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

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

### edwardboss_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/edwardboss_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### emissary.luac — .local-inputs/assets-extra/android/data/scripts/ai/emissary.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### emissary_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/emissary_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

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

### flaminite_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/flaminite_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### flaminiteed.luac — .local-inputs/assets-extra/android/data/scripts/ai/flaminiteed.luac

Relevant calls: 0; source lines: 2; engine readiness: 0/2.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### flaminiteed_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/flaminiteed_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

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

### miniboss.luac — .local-inputs/assets-extra/android/data/scripts/ai/miniboss.luac

Relevant calls: 0; source lines: 22; engine readiness: 1/5.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

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

### roottroll_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/roottroll_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### sandworm01.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm01.luac

Relevant calls: 0; source lines: 75; engine readiness: 4/8.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

### sandworm02.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm02.luac

Relevant calls: 0; source lines: 76; engine readiness: 4/8.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

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

### sandworm_queen.luac — .local-inputs/assets-extra/android/data/scripts/ai/sandworm_queen.luac

Relevant calls: 0; source lines: 74; engine readiness: 4/8.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

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
| 159 | `PlayAnim` | `1=GetPyOID("AnimDict", "swampking_emerge");
        attack_flag = false;

        DoSkill(3);
    elseif (SK_STATE == ST_WEAK) then
        PlayAnim(GetPyOID("AnimDict", "swampking_fall_idle_to_idle"), nil, true);
        weakidleAnim_flag = true;
    end

    SK_STATE = ST_IDLE;
    SetIsTargetable(true);
end

function SetSwampkingInWeakState()
    if (debugTrace_flag) then Trace("[SANDWORM/STATES] In WEAK state..."); end

    SK_STATE = ST_WEAK;
    weakAnim_flag = true;

    PlayAnim(GetPyOID("AnimDict", "swampking_fall"), GetPyOID("AnimDict", "swampking_fall_idle"));
end

function SetSwampkingInDiveState()
    if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into DIVE state..."); end

    if (HasTarget()) then
        local myTarget = GetTarget();

        ClearAggro(myTarget);
    end

    SK_STATE = ST_DIVE;
    diveAnim_flag = true;

    PlayAnim(GetPyOID("AnimDict", "swampking_dive"));
    SetIsTargetable(false);
end

function SetSwampkingInIntimidateState(nextAction)
    if (debugTrace_flag) then Trace("[SANDWORM/STATES] Going into INTIMIDATE state..."); end

    SK_STATE = ST_INTIMIDATE;
    intimidateAnim_flag = nextAction;

    if (debugTrace_flag) then Trace("intimidateAnim_flag: "..intimidateAnim_flag); end

    PlayAnim(GetPyOID("AnimDict", "swampking_intimidate"), nil, true);
end
-- -------------------------------------------------------------------------------





-- -------------------------------------------------------------------------------
-- CALLBACKS
-- -------------------------------------------------------------------------------
-- -------------------------------------------------------------------------------
function sandworm_OnInit()
    MarkAsFlying(true);

    SK_PHASE_ID = 1;

    RegisterAnims();
    ----------------------------------------------------------------------------------------
    -- Dynamic level for monster. Monster must have there LevelMax param set to use dynamic level
    ----------------------------------------------------------------------------------------
    local m_lvl_max = FromFixed(GetProp(GetPyStruct("CharacterProperties", "LevelMax")));
    local m_lvl_min = FromFixed(GetProp(GetPyStruct("CharacterProperties", "LevelMin")));
    local m_lvl_offset = FromFixed(GetProp(GetPyStruct("CharacterProperties", "LevelOffset")));
    local pl_lvl = GetHostPlayerLevel();
    local pl_diff = GetHostPlayerDifficulty();
    local m_lvl = pl_lvl;
    local m_level_min; 2=m_level_max = GetCurrentLevelRange(pl_diff);

    if(m_lvl_max > -1 ) then
        if(m_level_max and m_level_max > -1 ) then
            if( m_lvl <= m_level_min ) then
                m_lvl = m_level_min;
            else
                if( m_lvl >= m_level_max ) then
                    m_lvl = m_level_max;
                end
            end
        else
            if( m_lvl <= m_lvl_min ) then
                m_lvl = m_lvl_min;
            else
                if( m_lvl >= m_lvl_max ) then
                    m_lvl = m_lvl_max;
                end
            end
        end
        m_lvl = m_lvl + m_lvl_offset;
        SetLevel(ToFixed(m_lvl));
    end
    -- Reset all timers if they are still active
    if (timer_attack_flag) then
        StopTimerCB(timer_attack_flag);
        timer_attack_flag = nil;
    end
    if (timer_outOfRange_flag) then
        StopTimerCB(timer_outOfRange_flag);
        timer_outOfRange_flag = nil;
    end
    if (timer_idle_flag) then
        StopTimerCB(timer_idle_flag);
        timer_idle_flag = nil;
    end
    if (timer_intimidate_flag) then
        StopTimerCB(timer_intimidate_flag);
        timer_intimidate_flag = nil;
    end
    if (timer_weak_flag) then
        StopTimerCB(timer_weak_flag);
        timer_weak_flag = nil;
    end
    --  if (debugTrace_flag) then Trace("Starting attack timer..."); end
    --  timer_attack_flag = StartTimerCB(ATTACK_DELAY, true, isAttackReady);
end
AddToVFTable("OnInitFinal", "sandworm_OnInit");

-- -------------------------------------------------------------------------------
function sandworm_OnEnemySpotted(enemy)
    if(not HasTarget()) then
        AddAggro(enemy, 0);
        SetTarget(enemy);
    end
end
AddToVFTable("OnEnemySpotted", "sandworm_OnEnemySpotted");

-- -------------------------------------------------------------------------------
function sandworm_OnTargetOutOfSight()
    if (SK_PHASE_ID == 1) then
        OUTOFRANGE_DELAY = GetRand(OUTOFRANGE_MIN_DELAY, OUTOFRANGE_MAX_DELAY);
        timer_outOfRange_flag = StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState);
    end
end
AddToVFTable("OnTargetOutOfSight", "sandworm_OnTargetOutOfSight");

-- -------------------------------------------------------------------------------
function sandworm_OnTargetInRangedRange()
    --if (SK_STATE ~= ST_DIVE) then
    --  first_melee_flag = true;
    --  If the target get out of range, the worm dive in to get him.
    --      isAttackReady();
    --      StartTimerCB(OUTOFRANGE_DELAY, false, SetSwampkingInDiveState);
    --      StartTimerCB(OUTOFRANGE_DELAY+1000, false, SetSwampkingInIdleState);
    --end
end
AddToVFTable("OnTargetInRangedRange", "sandworm_OnTargetInRangedRange");

-- -------------------------------------------------------------------------------
function sandworm_OnTargetInCloseRange()
    attack_flag=true;
    if (SK_PHASE_ID == 1) then
        if (first_melee_flag and attack_flag) then
            DoSkill(1);
            first_melee_flag = false;
            attack_flag = false;
        elseif (attack_flag) then
            Do_MeleeAttackPhase1();
        end
    elseif (SK_PHASE_ID == 2) then
        if (attack_flag) then
            Do_MeleeAttackPhase2();
        end
    end
end
AddToVFTable("OnTargetInCloseRange", "sandworm_OnTargetInCloseRange");

-- -------------------------------------------------------------------------------
function sandworm_OnUpdate(timestamp)
    OnUpdate(timestamp);

    local HPCurr; 3=HPMax; 4=HPPercentCurr = GetPropHP();

    if(SK_PHASE_ID == 1 and HPPercentCurr <= 75 and SE_event1_flag) then
        --SE_event1_flag = false;
        --SetSwampkingInWeakState();
    end

    if(SK_PHASE_ID == 1 and HPPercentCurr <= 50 and SE_event2_flag) then
        if (SK_STATE == ST_WEAK) then
            SE_event2_flag = false;
        else
            --SE_event2_flag = false;
            --SetSwampkingInWeakState();
        end
    end

    if(SK_PHASE_ID == 1 and HPPercentCurr <= phase2_initPct ) then
        SK_PHASE_ID = 2;

        attack_flag = false;

        if (SK_STATE ~= ST_DIVE) then
            --SetSwampkingInDiveState();
        end

    end

end
AddToVFTable("OnUpdate", "sandworm_OnUpdate");

function sandworm_OnTargetDied()
    Stop();
    if (HasTarget()) then
        ClearAggro(GetTarget());
        ClearTarget();
    end
end
AddToVFTable("OnTargetDied", "sandworm_OnTargetDied");

-- -------------------------------------------------------------------------------
local function sandworm_OnEndGameCleanUp()
    if (HasTarget()) then
        ClearAggro(GetTarget());
        ClearTarget();
    end
    if (timer_attack_flag) then
        StopTimerCB(timer_attack_flag);
        timer_attack_flag = nil;
    end
    if (timer_outOfRange_flag) then
        StopTimerCB(timer_outOfRange_flag);
        timer_outOfRange_flag = nil;
    end
    if (timer_idle_flag) then
        StopTimerCB(timer_idle_flag);
        timer_idle_flag = nil;
    end
    if (timer_intimidate_flag) then
        StopTimerCB(timer_intimidate_flag);
        timer_intimidate_flag = nil;
    end
    if (timer_weak_flag) then
        StopTimerCB(timer_weak_flag);
        timer_weak_flag = nil;
    end
end

-- -------------------------------------------------------------------------------
function sandworm_OnDied(killer)
    sandworm_OnEndGameCleanUp();
end
AddToVFTable("OnDied", "sandworm_OnDied");

-- -------------------------------------------------------------------------------
function sandworm_OnTerminate()
    sandworm_OnEndGameCleanUp();
end
AddToVFTable("OnTerminate", "sandworm_OnTerminate");

-- -------------------------------------------------------------------------------
function sandworm_OnEndOfAnim()
    if (SK_STATE == ST_DIVE and diveAnim_flag) then
        diveAnim_flag = false;

        searchForClosestTarget();

        timer_idle_flag = StartTimerCB(DIVE_DELAY, false, SetSwampkingInIdleState);
    end

    if (SK_STATE == ST_INTIMIDATE and intimidateAnim_flag > 0) then

        if (intimidateAnim_flag == 1) then
            SetSwampkingInIdleState();
        elseif (intimidateAnim_flag == 2) then
            if (debugTrace_flag) then Trace("[SANDWORM] Result is DoSkill(2) - Spit)"); end

            searchForClosestTarget();

            timer_intimidate_flag = StartTimerCB(500, false, function() DoSkill(2); end);
            SK_STATE = ST_IDLE;
        end

        intimidateAnim_flag = 0;
    end

    if (SK_STATE == ST_WEAK and weakAnim_flag) then
        weakAnim_flag = false;

        searchForClosestTarget();

        buff = CreateBuff(DEBUFF_ID, WEAK_DELAY, false);
        ApplyBuff(DEBUFF_ID, buff);

        timer_weak_flag = StartTimerCB(WEAK_DELAY, false, SetSwampkingInIdleState);
    end

    if (weakidleAnim_flag) then
        weakidleAnim_flag = false;

        SetSwampkingInIntimidateState(1);
    end
end
AddToVFTable("OnEndOfAnim", "sandworm_OnEndOfAnim");` | global | PARTIAL |
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

### winddragon_settings.luac — .local-inputs/assets-extra/android/data/scripts/ai/winddragon_settings.luac

Relevant calls: 0; source lines: 34; engine readiness: 4/6.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |

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

### darkqueen2_summons_00.luac — .local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac

Relevant calls: 1; source lines: 162; engine readiness: 0/28.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 156 | `RegisterSkillEx` | `1={    update=OnSkillUpdate_; 2=cleanup=OnSkillCleanUp_; 3=check=OnSkillCheck_; 4=pre=OnPreSkill_; 5=skill=OnSkill_; 6=info=OnSkillInfo_; 7=}` | imported_script_function | PARTIAL |

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

### madruk_dyingattack.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruk_dyingattack.luac

Relevant calls: 2; source lines: 47; engine readiness: 0/4.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "Madruk_Shockwave")` | global | PARTIAL |
| 47 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_; 8=nil` | imported_script_function | PARTIAL |

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
| 171 | `RegisterSkillEx` | `1={    update=OnSkillUpdate_; 2=check=OnSkillCheck_; 3=pre=OnPreSkill_; 4=skill=OnSkill_; 5=post=OnPostSkill_; 6=delayed=OnDelayedSkill_; 7=info=OnSkillInfo_; 8=}` | imported_script_function | PARTIAL |

### madruktorso_dyingattack.luac — .local-inputs/assets-extra/android/data/scripts/skills/madruktorso_dyingattack.luac

Relevant calls: 2; source lines: 47; engine readiness: 0/4.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 37 | `PlayFX` | `1=GetPyOID("AnimatedEffectTable", "fx_swampking_blue_shockwave")` | global | PARTIAL |
| 47 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_; 8=nil` | imported_script_function | PARTIAL |

### prince_assassin_assassinate.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassinate.luac

Relevant calls: 2; source lines: 51; engine readiness: 1/7.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 19 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 51 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

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

### prince_assassin_blade_deflect.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_blade_deflect.luac

Relevant calls: 2; source lines: 53; engine readiness: 1/7.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 21 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 53 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

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
| 112 | `RegisterSkillEx` | `1={    update=OnSkillUpdate_; 2=check=OnSkillCheck_; 3=pre=OnPreSkill_; 4=skill=OnSkill_; 5=post=OnPostSkill_; 6=delayed=OnDelayedSkill_; 7=info=OnSkillInfo_; 8=}` | imported_script_function | PARTIAL |

### prince_assassin_poison_sting.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_poison_sting.luac

Relevant calls: 5; source lines: 109; engine readiness: 7/22.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 57 | `StartSkillTimerCB` | `1=BUFF_TIME; 2=0; 3=BuffExpired` | imported_script_function | PARTIAL |
| 62 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 78 | `OnSkill_` | `` | local_function | PORTED |
| 101 | `ClearTarget` | `` | global | PORTED |
| 109 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=OnDelayedSkill_; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

### prince_assassin_treasure_hunter.luac — .local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_treasure_hunter.luac

Relevant calls: 2; source lines: 51; engine readiness: 1/7.

| Line | Call | Arguments | Mode | Port status |
| ---: | --- | --- | --- | --- |
| 19 | `GetCurrentSkillInfo` | `` | imported_script_function | PARTIAL |
| 51 | `RegisterSkill` | `1=OnSkillUpdate_; 2=OnSkillCheck_; 3=OnPreSkill_; 4=OnSkill_; 5=OnPostSkill_; 6=nil; 7=OnSkillInfo_` | imported_script_function | PARTIAL |

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
| 115 | `RegisterSkillEx` | `1={    update=OnSkillUpdate_; 2=check=OnSkillCheck_; 3=pre=OnPreSkill_; 4=skill=OnSkill_; 5=delayed=OnDelayedSkill_; 6=info=OnSkillInfo_; 7=}` | imported_script_function | PARTIAL |

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

Call mapping: 4170 DONE and 3812 UNRESOLVED. Context engine-call readiness: 1670 PORTED, 4083 PARTIAL, 919 MISSING and 0 UNKNOWN. 4783 reference candidates await data-row resolution. Dynamic calls, unresolved native contracts, enum/table rules and per-call production coverage remain follow-up.

The Android `.luac` files are plaintext Lua source, detected by file headers; seven `.nomedia` markers are excluded. No iOS scripts were found. Android IDA is the only native map; no cross-platform binding equivalence is assumed.
