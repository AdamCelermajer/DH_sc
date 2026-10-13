# Native handler and port map

| Function | Native handler / addresses | Port status | Call sites | Status |
| --- | --- | --- | --- | --- |
| Trace | LuaScript::_Trace 0x0037ee80 | PARTIAL | 725 | UNRESOLVED |
| GetPyCst | LuaScript::_GetPyCst 0x0037f354 | PARTIAL | 608 | DONE |
| GetPyStruct | LuaScript::_GetPyStruct 0x0037f4a8 | PARTIAL | 442 | DONE |
| GetPyOID | LuaScript::_GetPyOID 0x0037f5fc | PARTIAL | 428 | DONE |
| AddToVFTable | LuaScript::_AddToVFTable 0x0037ec70 | PORTED | 256 | DONE |
| IsTargetListEmpty | GameObject::_IsTargetListEmpty; GameObject::_IsTargetListEmpty 0x0038e970,0x0038e970 | MISSING | 233 | DONE |
| ToFixed | LuaScript::_ToFixed 0x0037ebc4 | PORTED | 221 | UNRESOLVED |
| FromFixed | LuaScript::_FromFixed 0x0037ee84 | PORTED | 215 | UNRESOLVED |
| GetProp | Character::_GetProp; Character::_GetProp 0x003b9d8c,0x003b9d8c | PORTED | 195 | DONE |
| GetTargetListTop | GameObject::_GetTargetListTop; GameObject::_GetTargetListTop 0x0038eb68,0x0038eb68 | PARTIAL | 157 | DONE |
| TargetListSearch | GameObject::_TargetListSearch; GameObject::_TargetListSearch 0x0038fcf0,0x0038fcf0 | PARTIAL | 140 | DONE |
| SetProp | Character::_SetProp; Character::_SetProp 0x003b7eac,0x003b7eac | PARTIAL | 137 | UNRESOLVED |
| SetTargetListSorting | GameObject::_SetTargetListSorting; GameObject::_SetTargetListSorting 0x00390548,0x00390548 | MISSING | 130 | UNRESOLVED |
| SetTargetListCharacterFilter | GameObject::_SetTargetListCharacterFilter; GameObject::_SetTargetListCharacterFilter 0x00390690,0x00390690 | MISSING | 128 | UNRESOLVED |
| SetTargetListObjectFilter | GameObject::_SetTargetListObjectFilter; GameObject::_SetTargetListObjectFilter 0x00390624,0x00390624 | MISSING | 127 | UNRESOLVED |
| DoSkill | Character::_DoSkill; Character::_DoSkill 0x003b8bd8,0x003b8bd8 | PARTIAL | 116 | DONE |
| LookAt | Character::_LookAt; Character::_LookAt 0x003b8ed8,0x003b8ed8 | PARTIAL | 106 | UNRESOLVED |
| GetTarget | Character::_GetTarget; Character::_GetTarget 0x003b6c7c,0x003b6c7c | PORTED | 99 | UNRESOLVED |
| ApplyPropClass | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 85 | UNRESOLVED |
| PopTargetList | GameObject::_PopTargetList; GameObject::_PopTargetList 0x0038fbb8,0x0038fbb8 | MISSING | 83 | UNRESOLVED |
| ClearTarget | Character::_ClearTarget; Character::_ClearTarget 0x003b5690,0x003b5690 | PORTED | 73 | UNRESOLVED |
| GetRand | GameObject::_Rand; GameObject::_Rand 0x00393310,0x00393310 | PARTIAL | 68 | UNRESOLVED |
| RemoveBuff | Character::_RemoveBuff; Character::_RemoveBuff 0x003b842c,0x003b842c | PARTIAL | 64 | UNRESOLVED |
| MoveTo | Character::_MoveTo; Character::_MoveTo 0x003bada8,0x003bada8 | PARTIAL | 61 | UNRESOLVED |
| Stop | Character::_Stop; Character::_Stop 0x003b56b4,0x003b56b4 | PARTIAL | 59 | UNRESOLVED |
| attacker:GetProp | Character::_GetProp; Character::_GetProp 0x003b9d8c,0x003b9d8c | PORTED | 58 | DONE |
| HasTarget | Character::_HasTarget; Character::_HasTarget 0x003b6f50,0x003b6f50 | PORTED | 57 | UNRESOLVED |
| ApplyBuff | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 54 | UNRESOLVED |
| CreateBuff | Character::_CreateBuff; Character::_CreateBuff 0x003b86a8,0x003b86a8 | PARTIAL | 54 | UNRESOLVED |
| Include | LuaScript::_Include 0x0037efe4 | PARTIAL | 49 | UNRESOLVED |
| GetState | Character::_GetState; Character::_GetState 0x003b6d78,0x003b6d78 | PORTED | 42 | UNRESOLVED |
| PlaySound | LuaScript::_PlaySound 0x0037e578 | PARTIAL | 41 | DONE |
| HasMana | Character::_HasMana; Character::_HasMana 0x003b78ec,0x003b78ec | PARTIAL | 40 | UNRESOLVED |
| UseMana | Character::_UseMana; Character::_UseMana 0x003b7864,0x003b7864 | PARTIAL | 40 | UNRESOLVED |
| SetTarget | Character::_SetTarget; Character::_SetTarget 0x003b8f38,0x003b8f38 | PORTED | 38 | UNRESOLVED |
| GetSelf | GameObject::_GetSelf 0x0038eaf4 | MISSING | 37 | UNRESOLVED |
| PlayFX | GameObject::_PlayFX; GameObject::_PlayFX 0x003923cc,0x003923cc | PARTIAL | 36 | DONE |
| defender:GetProp | Character::_GetProp; Character::_GetProp 0x003b9d8c,0x003b9d8c | PORTED | 33 | DONE |
| HasPath | GameObject::_HasPath; GameObject::_HasPath 0x0038e98c,0x0038e98c | PARTIAL | 32 | UNRESOLVED |
| target:PlayFX | GameObject::_PlayFX; GameObject::_PlayFX 0x003923cc,0x003923cc | PARTIAL | 31 | DONE |
| AddAggro | Character::_AddAggro; Character::_AddAggro 0x003b8098,0x003b8098 | PARTIAL | 30 | UNRESOLVED |
| Attack | Character::_Attack; Character::_Attack 0x003b9f44,0x003b9f44 | PARTIAL | 30 | UNRESOLVED |
| ClearAggro | Character::_ClearAggro; Character::_ClearAggro 0x003b8648,0x003b8648 | PARTIAL | 30 | UNRESOLVED |
| RegisterAnim | Character::_RegisterAnim; Character::_RegisterAnim 0x003b70d8,0x003b70d8 | PORTED | 29 | UNRESOLVED |
| Summon | GameObject::_Summon; GameObject::_Summon 0x0039193c,0x0039193c | PARTIAL | 29 | UNRESOLVED |
| target:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 28 | UNRESOLVED |
| GetDistanceBetween | GameObject::_IsInRange 0x00390f1c | PARTIAL | 25 | UNRESOLVED |
| PlayAnim | Character::_PlayAnim; Character::_PlayAnim 0x003ba388,0x003ba388 | PARTIAL | 21 | DONE |
| PlayMusic | LuaScript::_PlayMusic 0x0037e420 | PARTIAL | 20 | DONE |
| GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 19 | UNRESOLVED |
| MulFixed | LuaScript::_MulFixed 0x0037e1a4 | PORTED | 18 | UNRESOLVED |
| SetInt | LuaScript::_SetInt 0x0037de5c | PARTIAL | 18 | UNRESOLVED |
| CallPyScript | LuaScript::_CallPyScript 0x0037ef3c | PARTIAL | 17 | UNRESOLVED |
| GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 17 | UNRESOLVED |
| GetPosition | GameObject::_GetPosition; GameObject::_GetPosition 0x0038e700,0x0038e700 | PORTED | 16 | UNRESOLVED |
| GetPropHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 16 | UNRESOLVED |
| PlaySound3D |   | MISSING | 16 | UNRESOLVED |
| SetLevel | Character::_SetLevel; Character::_SetLevel 0x003b73a4,0x003b73a4 | PORTED | 16 | UNRESOLVED |
| GetCurrentSpellInfo | Character::_GetCurrentSpellInfo; Character::_GetCurrentSpellInfo 0x003b6e30,0x003b6e30 | MISSING | 15 | UNRESOLVED |
| GetHostPlayerDifficulty | LuaScript::_GetHostPlayerDifficulty 0x0037cb8c | PARTIAL | 15 | UNRESOLVED |
| GetHostPlayerLevel | LuaScript::_GetHostPlayerLevel 0x0037cc00 | PARTIAL | 15 | UNRESOLVED |
| SpellCombatRoll | Character::_SpellCombatRoll; Character::_SpellCombatRoll 0x003b92a4,0x003b92a4 | PARTIAL | 15 | UNRESOLVED |
| target:ApplyBuff | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 15 | UNRESOLVED |
| target:CreateBuff | Character::_CreateBuff; Character::_CreateBuff 0x003b86a8,0x003b86a8 | PARTIAL | 15 | UNRESOLVED |
| GetCurrentLevelRange | LuaScript::_GetCurrentLevelRange 0x0037f1f0 | PARTIAL | 14 | UNRESOLVED |
| TargetListBackup | GameObject::_TargetListBackup; GameObject::_TargetListBackup 0x00390bd4,0x00390bd4 | MISSING | 14 | UNRESOLVED |
| WarpTo | Character::_WarpTo; Character::_WarpTo 0x003bb268,0x003bb268 | PARTIAL | 14 | UNRESOLVED |
| SetIsTargetable | Character::_SetIsTargetable; Character::_SetIsTargetable 0x003b7a64,0x003b7a64 | PARTIAL | 10 | DONE |
| defender:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 10 | UNRESOLVED |
| hack:GetPosition | GameObject::_GetPosition; GameObject::_GetPosition 0x0038e700,0x0038e700 | PORTED | 10 | UNRESOLVED |
| GetTargetListSize | GameObject::_GetTargetListSize; GameObject::_GetTargetListSize 0x0038e930,0x0038e930 | MISSING | 9 | UNRESOLVED |
| HasMaster | Character::_HasMaster; Character::_HasMaster 0x003b6f3c,0x003b6f3c | PARTIAL | 9 | UNRESOLVED |
| StartTimer | Character::_StartTimer; Character::_StartTimer 0x003b7590,0x003b7590 | PORTED | 9 | UNRESOLVED |
| target:IsCharacter | GameObject::_IsCharacter; GameObject::_IsCharacter 0x0038ea1c,0x0038ea1c | PARTIAL | 9 | UNRESOLVED |
| target:IsPlayer | GameObject::_IsPlayer; GameObject::_IsPlayer 0x0038e9f0,0x0038e9f0 | PARTIAL | 9 | UNRESOLVED |
| HasBow | Character::_HasBow; Character::_HasBow 0x003b6f84,0x003b6f84 | MISSING | 8 | UNRESOLVED |
| SetMaster | Character::_SetMaster; Character::_SetMaster 0x003b9084,0x003b9084 | PARTIAL | 8 | UNRESOLVED |
| attacker:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 8 | UNRESOLVED |
| player:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 8 | UNRESOLVED |
| Flee | Character::_Flee; Character::_Flee 0x003b91a0,0x003b91a0 | MISSING | 7 | UNRESOLVED |
| Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 7 | UNRESOLVED |
| SetBuffProp | Character::_SetProp; Character::_SetProp 0x003b7eac,0x003b7eac | MISSING | 7 | UNRESOLVED |
| object:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 7 | UNRESOLVED |
| ClearProps | Character::_ClearProps; Character::_ClearProps 0x003b82e4,0x003b82e4 | PARTIAL | 6 | UNRESOLVED |
| GetEquippedFaeryElement | Character::_GetEquippedFaeryElement; Character::_GetEquippedFaeryElement 0x003b6dc4,0x003b6dc4 | MISSING | 6 | UNRESOLVED |
| LookAtTarget | Character::_LookAtTarget; Character::_LookAtTarget 0x003b5684,0x003b5684 | PARTIAL | 6 | UNRESOLVED |
| object:GetState | Character::_GetState; Character::_GetState 0x003b6d78,0x003b6d78 | PORTED | 6 | UNRESOLVED |
| EnableCollisions | GameObject::_EnableCollisions; GameObject::_EnableCollisions 0x0039040c,0x0039040c | MISSING | 5 | UNRESOLVED |
| GetCurrentEquippedFaeryLevel | Character::_GetCurrentEquippedFaeryLevel; Character::_GetCurrentEquippedFaeryLevel 0x003b6df8,0x003b6df8 | MISSING | 5 | UNRESOLVED |
| GetInt | LuaScript::_GetInt 0x0037ec14 | PARTIAL | 5 | UNRESOLVED |
| GetMaster | Character::_GetMaster; Character::_GetMaster 0x003b6c70,0x003b6c70 | MISSING | 5 | UNRESOLVED |
| HeadTo | Character::_HeadTo; Character::_HeadTo 0x003ba71c,0x003ba71c | MISSING | 5 | UNRESOLVED |
| MarkAsFlying | GameObject::_MarkAsFlying; GameObject::_MarkAsFlying 0x00390900,0x00390900 | MISSING | 5 | DONE |
| Rand | LuaScript::_Rand 0x0037df30 | PARTIAL | 5 | UNRESOLVED |
| SetActorPosition | GameObject::_SetPosition; GameObject::_SetPosition 0x003928f0,0x003928f0 | PARTIAL | 5 | DONE |
| actor:PlayFX | GameObject::_PlayFX; GameObject::_PlayFX 0x003923cc,0x003923cc | PARTIAL | 5 | DONE |
| attacker:TargetInMeleeRange | Character::_TargetInMeleeRange; Character::_TargetInMeleeRange 0x003b7020,0x003b7020 | MISSING | 5 | UNRESOLVED |
| target:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 5 | UNRESOLVED |
| target:SetStunState | Character::_SetStunState; Character::_SetStunState 0x003b9580,0x003b9580 | MISSING | 5 | UNRESOLVED |
| victim:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 5 | UNRESOLVED |
| CanAttackFromRange | Character::_CanAttackFromRange; Character::_CanAttackFromRange 0x003b6f10,0x003b6f10 | MISSING | 4 | UNRESOLVED |
| CanAttackInMelee | Character::_CanAttackInMelee; Character::_CanAttackInMelee 0x003b6f64,0x003b6f64 | MISSING | 4 | UNRESOLVED |
| GetHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 4 | UNRESOLVED |
| KillRemainingMonsters |   | MISSING | 4 | UNRESOLVED |
| SetIsActorTargetable | Character::_SetIsTargetable; Character::_SetIsTargetable 0x003b7a64,0x003b7a64 | MISSING | 4 | UNRESOLVED |
| StopSound | LuaScript::_StopSound 0x0037e730 | PARTIAL | 4 | UNRESOLVED |
| SummonTimerTrap | GameObject::_SummonTimerTrap; GameObject::_SummonTimerTrap 0x00390c50,0x00390c50 | PARTIAL | 4 | UNRESOLVED |
| WarpBehind | Character::_WarpBehind; Character::_WarpBehind 0x003b8d98,0x003b8d98 | PARTIAL | 4 | UNRESOLVED |
| actor:ApplyBuff | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 4 | UNRESOLVED |
| actor:CreateBuff | Character::_CreateBuff; Character::_CreateBuff 0x003b86a8,0x003b86a8 | PARTIAL | 4 | UNRESOLVED |
| actor:GetProp | Character::_GetProp; Character::_GetProp 0x003b9d8c,0x003b9d8c | PORTED | 4 | DONE |
| attacker:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 4 | UNRESOLVED |
| object:PlayFX | GameObject::_PlayFX; GameObject::_PlayFX 0x003923cc,0x003923cc | PARTIAL | 4 | DONE |
| target:GetProp | Character::_GetProp; Character::_GetProp 0x003b9d8c,0x003b9d8c | PORTED | 4 | DONE |
| target:SetKnockBackState | Character::_SetKnockBackState; Character::_SetKnockBackState 0x003b7680,0x003b7680 | MISSING | 4 | UNRESOLVED |
| v:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 4 | UNRESOLVED |
| GetGameObjectsByType | LuaScript::_GetGameObjectsByType 0x0037f03c | PARTIAL | 3 | UNRESOLVED |
| GetHostPlayer | LuaScript::_GetHostPlayer 0x0037ca60 | PARTIAL | 3 | UNRESOLVED |
| GetObjectByName | GameObject::_GetObjectByName 0x00392848 | PARTIAL | 3 | UNRESOLVED |
| RegisterSummon | GameObject::_SummonTriggerTrap 0x00391670 | PARTIAL | 3 | UNRESOLVED |
| SetProjectileTarget | Character::_EnableSpotTargeting 0x003b7144 | MISSING | 3 | UNRESOLVED |
| attacker:Stop | Character::_Stop; Character::_Stop 0x003b56b4,0x003b56b4 | PARTIAL | 3 | UNRESOLVED |
| defender:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 3 | UNRESOLVED |
| defender:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 3 | UNRESOLVED |
| item:ApplyBuff | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 3 | UNRESOLVED |
| item:CreateBuff | Character::_CreateBuff; Character::_CreateBuff 0x003b86a8,0x003b86a8 | PARTIAL | 3 | UNRESOLVED |
| item:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 3 | UNRESOLVED |
| monster:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 3 | UNRESOLVED |
| monster:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 3 | UNRESOLVED |
| myTarget:IsPlayer | GameObject::_IsPlayer; GameObject::_IsPlayer 0x0038e9f0,0x0038e9f0 | PARTIAL | 3 | UNRESOLVED |
| obj:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 3 | UNRESOLVED |
| object:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 3 | UNRESOLVED |
| player:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 3 | UNRESOLVED |
| target:GetCharAIFlags | Character::_GetCharAIFlags; Character::_GetCharAIFlags 0x003b6e80,0x003b6e80 | MISSING | 3 | UNRESOLVED |
| target:GetPropHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 3 | UNRESOLVED |
| target:IsDoor | GameObject::_IsDoor; GameObject::_IsDoor 0x0038e9d8,0x0038e9d8 | MISSING | 3 | UNRESOLVED |
| target:SetScareState | Character::_SetScareState; Character::_SetScareState 0x003b96b0,0x003b96b0 | MISSING | 3 | UNRESOLVED |
| temp_target:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 3 | UNRESOLVED |
| BitAnd | LuaScript::_BitAnd 0x0037e9ec | PORTED | 2 | UNRESOLVED |
| DealDamages | GameObject::_DealDamages; GameObject::_DealDamages 0x003906fc,0x003906fc | PARTIAL | 2 | UNRESOLVED |
| GetNumPlayers | LuaScript::_GetNumPlayers 0x0037cbd8 | PARTIAL | 2 | UNRESOLVED |
| GetStateTime | Character::_GetStateTime; Character::_GetStateTime 0x003b6d6c,0x003b6d6c | PORTED | 2 | UNRESOLVED |
| HasShield | Character::_HasShield; Character::_HasShield 0x003b6fa4,0x003b6fa4 | PARTIAL | 2 | UNRESOLVED |
| SetSkillCooldownTimerId__ | Character::_SetSkillCooldownTimerId; Character::_SetSkillCooldownTimerId 0x003b97e0,0x003b97e0 | MISSING | 2 | UNRESOLVED |
| SetSpellCooldownTimerId__ | Character::_SetSpellCooldownTimerId; Character::_SetSpellCooldownTimerId 0x003b90e4,0x003b90e4 | MISSING | 2 | UNRESOLVED |
| StopTimer | Character::_StopTimer; Character::_StopTimer 0x003b7064,0x003b7064 | PORTED | 2 | UNRESOLVED |
| aoe_target:GetID | GameObject::_GetID; GameObject::_GetID 0x0038ebe4,0x0038ebe4 | PORTED | 2 | UNRESOLVED |
| attackedObject:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 2 | UNRESOLVED |
| attacker:ApplyBuff | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 2 | UNRESOLVED |
| attacker:CreateBuff | Character::_CreateBuff; Character::_CreateBuff 0x003b86a8,0x003b86a8 | PARTIAL | 2 | UNRESOLVED |
| attacker:SetKnockBackState | Character::_SetKnockBackState; Character::_SetKnockBackState 0x003b7680,0x003b7680 | MISSING | 2 | UNRESOLVED |
| attacker:SetProp | Character::_SetProp; Character::_SetProp 0x003b7eac,0x003b7eac | PARTIAL | 2 | UNRESOLVED |
| attacker:SetStunState | Character::_SetStunState; Character::_SetStunState 0x003b9580,0x003b9580 | MISSING | 2 | UNRESOLVED |
| defender:ApplyBuff | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 2 | UNRESOLVED |
| defender:CreateBuff | Character::_CreateBuff; Character::_CreateBuff 0x003b86a8,0x003b86a8 | PARTIAL | 2 | UNRESOLVED |
| enemy:SetTarget | Character::_SetTarget; Character::_SetTarget 0x003b8f38,0x003b8f38 | PORTED | 2 | UNRESOLVED |
| friend:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 2 | UNRESOLVED |
| g_master:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 2 | UNRESOLVED |
| g_target:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 2 | UNRESOLVED |
| hasReachDestination |   | MISSING | 2 | UNRESOLVED |
| invisible_man:WarpTo | Character::_WarpTo; Character::_WarpTo 0x003bb268,0x003bb268 | PARTIAL | 2 | UNRESOLVED |
| monster:MoveTo | Character::_MoveTo; Character::_MoveTo 0x003bada8,0x003bada8 | PARTIAL | 2 | UNRESOLVED |
| monster:UNLOCK | GameObject::_Unlock 0x0038d7a8 | PARTIAL | 2 | UNRESOLVED |
| mothSlot1:UNLOCK | GameObject::_Unlock 0x0038d7a8 | PARTIAL | 2 | UNRESOLVED |
| mothSlot2:UNLOCK | GameObject::_Unlock 0x0038d7a8 | PARTIAL | 2 | UNRESOLVED |
| mothSlot3:UNLOCK | GameObject::_Unlock 0x0038d7a8 | PARTIAL | 2 | UNRESOLVED |
| mothSlot4:UNLOCK | GameObject::_Unlock 0x0038d7a8 | PARTIAL | 2 | UNRESOLVED |
| obj:GetPropHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 2 | UNRESOLVED |
| object:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 2 | UNRESOLVED |
| objspawns:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 2 | UNRESOLVED |
| oldTarget:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 2 | UNRESOLVED |
| player:WarpTo | Character::_WarpTo; Character::_WarpTo 0x003bb268,0x003bb268 | PARTIAL | 2 | UNRESOLVED |
| refData.callback |   | PARTIAL | 2 | UNRESOLVED |
| target:GetMaster | Character::_GetMaster; Character::_GetMaster 0x003b6c70,0x003b6c70 | MISSING | 2 | UNRESOLVED |
| target:GetPosition | GameObject::_GetPosition; GameObject::_GetPosition 0x0038e700,0x0038e700 | PORTED | 2 | UNRESOLVED |
| target:HasMaster | Character::_HasMaster; Character::_HasMaster 0x003b6f3c,0x003b6f3c | PARTIAL | 2 | UNRESOLVED |
| target:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 2 | UNRESOLVED |
| target:SetBuffProp | Character::_SetProp; Character::_SetProp 0x003b7eac,0x003b7eac | MISSING | 2 | UNRESOLVED |
| victim:LOCK | GameObject::_Lock 0x0038d5f8 | MISSING | 2 | UNRESOLVED |
| wave.precb |   | MISSING | 2 | UNRESOLVED |
| waypoint:GetPosition | GameObject::_GetPosition; GameObject::_GetPosition 0x0038e700,0x0038e700 | PORTED | 2 | UNRESOLVED |
| AddPlayer |   | PARTIAL | 1 | UNRESOLVED |
| CF__CalcDodge |   | MISSING | 1 | UNRESOLVED |
| ChangeAIState | CharAIScript::_ChangeState 0x003d9710 | PARTIAL | 1 | UNRESOLVED |
| GetCurrentSkillInfo__ | Character::_GetCurrentSkillInfo; Character::_GetCurrentSkillInfo 0x003b8f9c,0x003b8f9c | MISSING | 1 | UNRESOLVED |
| InitPathingLocations |   | MISSING | 1 | UNRESOLVED |
| IsMasterHostPlayer | Character::_IsMasterHostPlayer; Character::_IsMasterHostPlayer 0x003b6fc4,0x003b6fc4 | PARTIAL | 1 | UNRESOLVED |
| PlayActorAnim | Character::_PlayAnim; Character::_PlayAnim 0x003ba388,0x003ba388 | PARTIAL | 1 | UNRESOLVED |
| PopVFTable | LuaScript::_PopVFTable 0x0037dc44 | PORTED | 1 | UNRESOLVED |
| PushVFTable | LuaScript::_PushVFTable 0x0037be00 | PORTED | 1 | UNRESOLVED |
| RegenHP | Character::_RegenHP; Character::_RegenHP 0x003b77ec,0x003b77ec | PARTIAL | 1 | UNRESOLVED |
| RegisterAIState | CharAIScript::_RegisterState 0x003da144 | PARTIAL | 1 | UNRESOLVED |
| RegisterActorAnim | Character::_RegisterAnim; Character::_RegisterAnim 0x003b70d8,0x003b70d8 | PARTIAL | 1 | UNRESOLVED |
| Remove | TriggerTrap::_Remove; TriggerTrap::_Remove 0x0039dd18,0x0039dd18 | PARTIAL | 1 | UNRESOLVED |
| SLOT1_ITEM:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| SLOT2_ITEM:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| SLOT3_ITEM:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| SetGameType | LuaScript::_SetGameType 0x0037f878 | PARTIAL | 1 | UNRESOLVED |
| SetMaxPath | GameObject::_SetMaxPath; GameObject::_SetMaxPath 0x00390b3c,0x00390b3c | MISSING | 1 | UNRESOLVED |
| SkillCombatRoll__ | Character::_SkillCombatRoll; Character::_SkillCombatRoll 0x003b9fbc,0x003b9fbc | MISSING | 1 | UNRESOLVED |
| SpawnSkillProjectile__ | Character::_SpawnSkillProjectile; Character::_SpawnSkillProjectile 0x003b99d0,0x003b99d0 | MISSING | 1 | DONE |
| StopSkill__ | Character::_StopSkill; Character::_StopSkill 0x003b94ac,0x003b94ac | MISSING | 1 | UNRESOLVED |
| TargetInMeleeRange | Character::_TargetInMeleeRange; Character::_TargetInMeleeRange 0x003b7020,0x003b7020 | MISSING | 1 | UNRESOLVED |
| UpdatePlayerPosition |   | MISSING | 1 | UNRESOLVED |
| UpdatePlayers |   | MISSING | 1 | UNRESOLVED |
| actor:RegenHP | Character::_RegenHP; Character::_RegenHP 0x003b77ec,0x003b77ec | PARTIAL | 1 | UNRESOLVED |
| actor:RegenMP | Character::_RegenMP; Character::_RegenMP 0x003b7774,0x003b7774 | PARTIAL | 1 | UNRESOLVED |
| attackTarget:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| attackTarget:LOCK | GameObject::_Lock 0x0038d5f8 | MISSING | 1 | UNRESOLVED |
| attacker:ApplyPropClass | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 1 | UNRESOLVED |
| attacker:GetAttackRatingBonus | Character::_GetPropBonusAttackRating; Character::_GetPropBonusAttackRating 0x003b7b84,0x003b7b84 | MISSING | 1 | UNRESOLVED |
| attacker:GetDamageBonus | Character::_GetPropBonusDamage; Character::_GetPropBonusDamage 0x003b7acc,0x003b7acc | MISSING | 1 | UNRESOLVED |
| attacker:GetHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 1 | UNRESOLVED |
| attacker:GetHitCount | Character::_GetHitCount; Character::_GetHitCount 0x003b6cc8,0x003b6cc8 | MISSING | 1 | UNRESOLVED |
| attacker:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 1 | UNRESOLVED |
| cb_info.callback |   | PARTIAL | 1 | UNRESOLVED |
| defender:GetHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 1 | UNRESOLVED |
| defender:GetState | Character::_GetState; Character::_GetState 0x003b6d78,0x003b6d78 | PORTED | 1 | UNRESOLVED |
| defender:HasShield | Character::_HasShield; Character::_HasShield 0x003b6fa4,0x003b6fa4 | PARTIAL | 1 | UNRESOLVED |
| defender:IsCharacter | GameObject::_IsCharacter; GameObject::_IsCharacter 0x0038ea1c,0x0038ea1c | PARTIAL | 1 | UNRESOLVED |
| defender:RemoveBuff | Character::_RemoveBuff; Character::_RemoveBuff 0x003b842c,0x003b842c | PARTIAL | 1 | UNRESOLVED |
| defender:SetIsTargetable | Character::_SetIsTargetable; Character::_SetIsTargetable 0x003b7a64,0x003b7a64 | PARTIAL | 1 | DONE |
| defender:Stop | Character::_Stop; Character::_Stop 0x003b56b4,0x003b56b4 | PARTIAL | 1 | UNRESOLVED |
| fdata.func |   | PARTIAL | 1 | UNRESOLVED |
| firstTarget:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| firstTarget:PlayFX | GameObject::_PlayFX; GameObject::_PlayFX 0x003923cc,0x003923cc | PARTIAL | 1 | DONE |
| g_player:GetProp | Character::_GetProp; Character::_GetProp 0x003b9d8c,0x003b9d8c | PORTED | 1 | DONE |
| goToNextDestination |   | MISSING | 1 | UNRESOLVED |
| hack_self:GetPosition | GameObject::_GetPosition; GameObject::_GetPosition 0x0038e700,0x0038e700 | PORTED | 1 | UNRESOLVED |
| hellhole:GetHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 1 | UNRESOLVED |
| hellhole:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 1 | UNRESOLVED |
| invisible_man:GetPosition | GameObject::_GetPosition; GameObject::_GetPosition 0x0038e700,0x0038e700 | PORTED | 1 | UNRESOLVED |
| invisible_man:PlayFX | GameObject::_PlayFX; GameObject::_PlayFX 0x003923cc,0x003923cc | PARTIAL | 1 | DONE |
| monster:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| mothSlot1:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 1 | UNRESOLVED |
| mothSlot1:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 1 | UNRESOLVED |
| mothSlot1:LOCK | GameObject::_Lock 0x0038d5f8 | MISSING | 1 | UNRESOLVED |
| mothSlot2:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 1 | UNRESOLVED |
| mothSlot2:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 1 | UNRESOLVED |
| mothSlot2:LOCK | GameObject::_Lock 0x0038d5f8 | MISSING | 1 | UNRESOLVED |
| mothSlot3:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 1 | UNRESOLVED |
| mothSlot3:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 1 | UNRESOLVED |
| mothSlot3:LOCK | GameObject::_Lock 0x0038d5f8 | MISSING | 1 | UNRESOLVED |
| mothSlot4:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 1 | UNRESOLVED |
| mothSlot4:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 1 | UNRESOLVED |
| mothSlot4:LOCK | GameObject::_Lock 0x0038d5f8 | MISSING | 1 | UNRESOLVED |
| object:ApplyBuff | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 1 | UNRESOLVED |
| object:CreateBuff | Character::_CreateBuff; Character::_CreateBuff 0x003b86a8,0x003b86a8 | PARTIAL | 1 | UNRESOLVED |
| object:IsConnected | Character::_IsConnected; Character::_IsConnected 0x003b6ec4,0x003b6ec4 | MISSING | 1 | UNRESOLVED |
| objpath:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| objpath:GetPosition | GameObject::_GetPosition; GameObject::_GetPosition 0x0038e700,0x0038e700 | PORTED | 1 | UNRESOLVED |
| playAlarmAnimation |   | MISSING | 1 | UNRESOLVED |
| player:GetHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 1 | UNRESOLVED |
| player:IsConnected | Character::_IsConnected; Character::_IsConnected 0x003b6ec4,0x003b6ec4 | MISSING | 1 | UNRESOLVED |
| player:IsDead | GameObject::_IsDead; GameObject::_IsDead 0x0038ea48,0x0038ea48 | PARTIAL | 1 | UNRESOLVED |
| player:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 1 | UNRESOLVED |
| player:RemoveBuff | Character::_RemoveBuff; Character::_RemoveBuff 0x003b842c,0x003b842c | PARTIAL | 1 | UNRESOLVED |
| secondTarget:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| selfReferee:GetPosition | GameObject::_GetPosition; GameObject::_GetPosition 0x0038e700,0x0038e700 | PORTED | 1 | UNRESOLVED |
| startDebuffTimer |   | MISSING | 1 | UNRESOLVED |
| statue:SetMaster | Character::_SetMaster; Character::_SetMaster 0x003b9084,0x003b9084 | PARTIAL | 1 | UNRESOLVED |
| statue:SetProp | Character::_SetProp; Character::_SetProp 0x003b7eac,0x003b7eac | PARTIAL | 1 | UNRESOLVED |
| statueDummy:GetHP | Character::_GetPropHP; Character::_GetPropHP 0x003b6cd8,0x003b6cd8 | PARTIAL | 1 | UNRESOLVED |
| statueDummy:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 1 | UNRESOLVED |
| target:AddAggro | Character::_AddAggro; Character::_AddAggro 0x003b8098,0x003b8098 | PARTIAL | 1 | UNRESOLVED |
| target:ClearAggro | Character::_ClearAggro; Character::_ClearAggro 0x003b8648,0x003b8648 | PARTIAL | 1 | UNRESOLVED |
| target:ClearTarget | Character::_ClearTarget; Character::_ClearTarget 0x003b5690,0x003b5690 | PORTED | 1 | UNRESOLVED |
| target:EnableCollisions | GameObject::_EnableCollisions; GameObject::_EnableCollisions 0x0039040c,0x0039040c | MISSING | 1 | UNRESOLVED |
| target:GetState | Character::_GetState; Character::_GetState 0x003b6d78,0x003b6d78 | PORTED | 1 | UNRESOLVED |
| target:HeadTo | Character::_HeadTo; Character::_HeadTo 0x003ba71c,0x003ba71c | MISSING | 1 | UNRESOLVED |
| target:IsDecor | GameObject::_IsDecor; GameObject::_IsDecor 0x0038e9c0,0x0038e9c0 | MISSING | 1 | UNRESOLVED |
| target:Kill | Character::_Kill; Character::_Kill 0x003b7e40,0x003b7e40 | PARTIAL | 1 | UNRESOLVED |
| target:PlayActorAnim | Character::_PlayAnim; Character::_PlayAnim 0x003ba388,0x003ba388 | PARTIAL | 1 | UNRESOLVED |
| target:RegenHP | Character::_RegenHP; Character::_RegenHP 0x003b77ec,0x003b77ec | PARTIAL | 1 | UNRESOLVED |
| target:RegenMP | Character::_RegenMP; Character::_RegenMP 0x003b7774,0x003b7774 | PARTIAL | 1 | UNRESOLVED |
| target:SetIsTargetable | Character::_SetIsTargetable; Character::_SetIsTargetable 0x003b7a64,0x003b7a64 | PARTIAL | 1 | DONE |
| target:SetProp | Character::_SetProp; Character::_SetProp 0x003b7eac,0x003b7eac | PARTIAL | 1 | UNRESOLVED |
| target:SetTarget | Character::_SetTarget; Character::_SetTarget 0x003b8f38,0x003b8f38 | PORTED | 1 | UNRESOLVED |
| target:TargetListSearch | GameObject::_TargetListSearch; GameObject::_TargetListSearch 0x0038fcf0,0x0038fcf0 | PARTIAL | 1 | DONE |
| target:WarpTo | Character::_WarpTo; Character::_WarpTo 0x003bb268,0x003bb268 | PARTIAL | 1 | UNRESOLVED |
| tcallback.mycallback |   | MISSING | 1 | UNRESOLVED |
| thisTarget:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| v:ApplyBuff | Character::_ApplyPropClass; Character::_ApplyPropClass 0x003babdc,0x003babdc | PARTIAL | 1 | UNRESOLVED |
| v:CreateBuff | Character::_CreateBuff; Character::_CreateBuff 0x003b86a8,0x003b86a8 | PARTIAL | 1 | UNRESOLVED |
| victim:GetName | GameObject::_GetName; GameObject::_GetName 0x0038ebf0,0x0038ebf0 | PARTIAL | 1 | UNRESOLVED |
| victim:IsCharacter | GameObject::_IsCharacter; GameObject::_IsCharacter 0x0038ea1c,0x0038ea1c | PARTIAL | 1 | UNRESOLVED |
| victim:PlayFX | GameObject::_PlayFX; GameObject::_PlayFX 0x003923cc,0x003923cc | PARTIAL | 1 | DONE |
| wave.postcb |   | MISSING | 1 | UNRESOLVED |

