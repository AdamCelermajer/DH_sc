# Actual Main runtime boundaries for all 80 table slots

Source inspection only. The original ELF SHA256 is 36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80. Each retained named symbol was decoded in <=256-byte read chunks; no original code ran. Symbol sizes may include literal pools. Literal short leaves below are identified only by their exact instruction sequence.

All Execute invocation and IsBlocking dispatch belong to Main. A bridge callback returning true only reports successful actual dispatch; a literal BX LR Execute does not define a source boolean result or justify a gameplay effect. V59 has no competing scheduler or IsBlocking policy. Main may use checked_data_borrow for its actual class/query receiver and must fail closed for an absent required body.

| Kind | Actual class | Init | Execute address / symbol bytes | IsBlocking address / symbol bytes / proven literal |
|---:|---|---|---|---|
|0|Script_ExecScript|0x455628 /4; BX LR|0x4607f4 /348|0x455c08 /56|
|1|Script_EnterCutSceneMode|0x455628 /4; BX LR|0x45a384 /748|0x455638 /8; literal 0|
|2|Script_ExitCutSceneMode|0x455628 /4; BX LR|0x4599ac /344|0x455640 /8; literal 0|
|3|Script_CONSOLE|0x455628 /4; BX LR|0x460e68 /384|0x455648 /8; literal 0|
|4|Script_SetCamera|0x455628 /4; BX LR|0x455678 /4; BX LR only|0x45567c /8; literal 0|
|5|Script_PlayCamera|0x455628 /4; BX LR|0x460110 /244|0x459370 /84|
|6|Script_SetCameraClip|0x455628 /4; BX LR|0x45c670 /592|0x455684 /8; literal 0|
|7|Script_WaitCamera|0x455628 /4; BX LR|0x45c5e8 /136|0x45932c /68|
|8|Script_SetCameraTarget|0x455628 /4; BX LR|0x460004 /268|0x4592cc /96|
|9|Script_EmptyImpl|0x455628 /4; BX LR|0x45c560 /136|0x455630 /8; literal 0|
|10|Script_StartDialog|0x459034|0x460fe8 /300|0x45568c /8; literal 0|
|11|Script_StartDialogID|0x455628 /4; BX LR|0x461114 /312|0x455694 /8; literal 0|
|12|Script_WaitDialog|0x455628 /4; BX LR|0x45c4d8 /136|0x4598d8 /40|
|13|Script_PlaySound|0x455628 /4; BX LR|0x45fefc /264|0x455650 /8; literal 0|
|14|Script_StopSound|0x455628 /4; BX LR|0x45fe2c /208|0x455658 /8; literal 0|
|15|Script_PlayLevelMusic|0x455628 /4; BX LR|0x45fd20 /268|0x455660 /8; literal 0|
|16|Script_EnterSafeZone|0x455628 /4; BX LR|0x45fc80 /160|0x455668 /8; literal 0|
|17|Script_LeaveSafeZone|0x455628 /4; BX LR|0x45fbe0 /160|0x455670 /8; literal 0|
|18|Script_PlayAnimById|0x455628 /4; BX LR|0x45e24c /296|0x45569c /8; literal 0|
|19|Script_PlayAnimByName|0x455628 /4; BX LR|0x45e124 /296|0x4556a4 /8; literal 0|
|20|Script_PlayEffect|0x4598b4|0x45fa9c /324|0x4556ac /8; literal 0|
|21|Script_StopEffect|0x455628 /4; BX LR|0x459890 /36|0x4556b4 /8; literal 0|
|22|Script_ShowFlash|0x45983c|0x45f8c8 /468|0x4556c4 /68|
|23|Script_HideFlash|0x4597e8|0x45f78c /316|0x455708 /68|
|24|Script_LockCharacter|0x455628 /4; BX LR|0x45dda0 /304|0x4556bc /8; literal 0|
|25|Script_UnlockCharacter|0x455628 /4; BX LR|0x45dc80 /288|0x45574c /8; literal 0|
|26|Script_Wait|0x455628 /4; BX LR|0x45c438 /160|0x455754 /24|
|27|Script_SetFaeryState|0x455628 /4; BX LR|0x45f6c8 /196|0x45576c /8; literal 0|
|28|Script_IncFaeryLevel|0x455628 /4; BX LR|0x45f5cc /252|0x455774 /8; literal 0|
|29|Script_PutCharacterInLimbus|0x455628 /4; BX LR|0x45f4e8 /228|0x45577c /8; literal 0|
|30|Script_SpawnCharacter|0x455628 /4; BX LR|0x45f400 /232|0x455784 /8; literal 0|
|31|Script_PutCharacterInIdle|0x455628 /4; BX LR|0x45d590 /192|0x45578c /8; literal 0|
|32|Script_MarkCharacterAsScripted|0x455628 /4; BX LR|0x45d4c8 /200|0x455794 /8; literal 0|
|33|Script_AIEnableTimer|0x455628 /4; BX LR|0x45579c /4; BX LR only|0x4557a0 /8; literal 0|
|34|Script_AIResetTimer|0x455628 /4; BX LR|0x4557a8 /4; BX LR only|0x4557ac /8; literal 0|
|35|Script_AIDoSkill|0x455628 /4; BX LR|0x4557b4 /4; BX LR only|0x4557b8 /8; literal 0|
|36|Script_AISetHP|0x455628 /4; BX LR|0x4557c0 /4; BX LR only|0x4557c4 /8; literal 0|
|37|Script_AIChangeScript|0x455628 /4; BX LR|0x4557cc /4; BX LR only|0x4557d0 /8; literal 0|
|38|Script_AISetInt|0x455628 /4; BX LR|0x45f2fc /260|0x4557d8 /8; literal 0|
|39|Script_StopActor|0x455628 /4; BX LR|0x45eec0 /488|0x4557e0 /8; literal 0|
|40|Script_MoveActor|0x455628 /4; BX LR|0x45f0a8 /596|0x459698 /132|
|41|Script_LookActor|0x455628 /4; BX LR|0x45ec50 /624|0x455818 /8; literal 0|
|42|Script_ShowActor|0x455628 /4; BX LR|0x45eb14 /316|0x455820 /8; literal 0|
|43|Script_HideActor|0x455628 /4; BX LR|0x45e028 /252|0x455828 /8; literal 0|
|44|Script_KillActor|0x455628 /4; BX LR|0x45ea04 /272|0x455830 /8; literal 0|
|45|Script_PlayActorAnim|0x45a2ec|0x45e890 /372|0x459628 /112|
|46|Script_SetActorPosition|0x455628 /4; BX LR|0x45e5c8 /712|0x455838 /8; literal 0|
|47|Script_SetActorMaster|0x455628 /4; BX LR|0x45e4b8 /272|0x455840 /8; literal 0|
|48|Script_DropLoot|0x455628 /4; BX LR|0x45e374 /324|0x4557e8 /8; literal 0|
|49|Script_AutoEquip|0x455628 /4; BX LR|0x45db88 /248|0x4557f0 /8; literal 0|
|50|Script_UnEquipItemFromSlot|0x455628 /4; BX LR|0x45da8c /252|0x4557f8 /8; literal 0|
|51|Script_UnEquipHands|0x455628 /4; BX LR|0x45ded0 /344|0x455800 /8; literal 0|
|52|Script_ReEquipHands|0x455628 /4; BX LR|0x45d95c /304|0x455808 /8; literal 0|
|53|Script_SetStatic|0x455628 /4; BX LR|0x45d868 /244|0x455810 /8; literal 0|
|54|Script_OpenDoor|0x455628 /4; BX LR|0x45d75c /268|0x455848 /60|
|55|Script_CloseDoor|0x455628 /4; BX LR|0x45d650 /268|0x455884 /56|
|56|Script_ActivateProjectileTrap|0x455628 /4; BX LR|0x45d3cc /252|0x4558bc /8; literal 0|
|57|Script_DeactivateProjectileTrap|0x455628 /4; BX LR|0x45d2d4 /248|0x4558c4 /8; literal 0|
|58|Script_ActivateTriggerPlate|0x455628 /4; BX LR|0x45d1d8 /252|0x4558cc /8; literal 0|
|59|Script_DeactivateTriggerPlate|0x455628 /4; BX LR|0x45d0e0 /248|0x4558d4 /8; literal 0|
|60|Script_SpawnContainer|0x455628 /4; BX LR|0x45d008 /216|0x4558dc /8; literal 0|
|61|Script_SetLevelState|0x455628 /4; BX LR|0x45cf40 /200|0x4558e4 /8; literal 0|
|62|Script_SetWorldMapLocationState|0x455628 /4; BX LR|0x45ce78 /200|0x4558ec /8; literal 0|
|63|Script_RestartLevel|0x455628 /4; BX LR|0x45cdd0 /168|0x4558f4 /8; literal 0|
|64|Script_ChangeLevel|0x455628 /4; BX LR|0x45cba0 /560|0x4558fc /8; literal 0|
|65|Script_EndGame|0x455628 /4; BX LR|0x45ca80 /288|0x455910 /8; literal 0|
|66|Script_AwardTrophy|0x455628 /4; BX LR|0x4593c4 /48|0x455920 /8; literal 0|
|67|Script_AwardEndGameTrophies|0x455628 /4; BX LR|0x459b04 /1120|0x455918 /8; literal 0|
|68|Script_ShowTrophies|0x455628 /4; BX LR|0x455904 /4; BX LR only|0x455908 /8; literal 1|
|69|Script_SaveGame|0x455628 /4; BX LR|0x45c994 /236|0x455928 /8; literal 0|
|70|Script_BlockSaveGame|0x455628 /4; BX LR|0x45c8c0 /212|0x455930 /8; literal 0|
|71|Script_EnqueueTutorialMessage|0x455628 /4; BX LR|0x4613d4 /196|0x455938 /8; literal 0|
|72|Script_SkipTutorialMessage|0x455628 /4; BX LR|0x460338 /4|0x455940 /8; literal 0|
|73|Script_SkipAllTutorialMessages|0x455628 /4; BX LR|0x46033c /148|0x455948 /8; literal 0|
|74|Script_EnqueueCharMenuTutorialMessage|0x455628 /4; BX LR|0x460b1c /276|0x455950 /8; literal 0|
|75|Script_SkipCharMenuTutorialMessage|0x455628 /4; BX LR|0x4603d0 /88|0x455958 /8; literal 0|
|76|Script_SkipAllCharMenuTutorialMessages|0x455628 /4; BX LR|0x460428 /76|0x455960 /8; literal 0|
|77|Script_LockTutorial|0x455628 /4; BX LR|0x45925c /112|0x455968 /8; literal 0|
|78|Script_DoTutorial|0x455628 /4; BX LR|0x460950 /264|0x455970 /8; literal 0|
|79|Script_FlushMessages|0x455628 /4; BX LR|0x460a58 /196|0x455978 /8; literal 0|

## Direct reached engine and runtime leaves

The exact named method source, bytes, symbol metadata, and reached direct branch/call addresses are retained in source/constructor-data-runtime.json and matching source/<address>.asm. Direct-call lists include runtime helpers and do not constitute an implemented engine contract; indirect vtable dispatch remains inside Main. The decoded .asm includes literal pool words, which must not be interpreted as instructions reachable from the method.

### 0 Script_ExecScript

Execute: 0x460840 ->0x337888 _ZN13DebugSwitches4loadEv; 0x460854 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x460860 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x460868 ->0x318254 _ZNSsD1Ev; 0x460898 ->0x4605c0 _ZN13ScriptManager11StartScriptEiib; 0x460908 ->0x30eb2c no retained dynamic symbol; 0x460930 ->0x30e310 no retained dynamic symbol.
IsBlocking: 0x455c34 ->0x455bec _ZNK13ScriptManager15IsScriptRunningEi.

### 1 Script_EnterCutSceneMode

Execute: 0x45a3a0 ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45a3b0 ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45a3c0 ->0x42ca8c _ZN11MenuManager11GetInstanceEv; 0x45a3ec ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45a408 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45a410 ->0x3a9db4 _ZN9Character12ReloadSkillsEv; 0x45a41c ->0x31f668 _ZN11Application13ShowStatubBarEb; 0x45a420 ->0x7fd794 _Z9GetOnlinev; 0x45a430 ->0x42ca8c _ZN11MenuManager11GetInstanceEv; 0x45a434 ->0x42cb8c _ZN11MenuManager10GetHUDRootEv; 0x45a43c ->0x42ca8c _ZN11MenuManager11GetInstanceEv; 0x45a440 ->0x42cb8c _ZN11MenuManager10GetHUDRootEv; 0x45a444 ->0x7a7cac _ZNK8RenderFX7GetRootEv; 0x45a448 ->0x774154 _ZN7gameswf4root14get_root_movieEv; 0x45a468 ->0x7abe0c _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi; 0x45a488 ->0x7fd794 _Z9GetOnlinev; 0x45a4ec ->0x36e744 _ZN13PlayerManager9GetPlayerEib; 0x45a510 ->0x459900 _ZN10PlayerInfo13SetInCutsceneEb; 0x45a548 ->0x7fd794 _Z9GetOnlinev; 0x45a568 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45a584 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45a5b0 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45a610 ->0x814f84 _ZN15NetStructMember10SetChangedEv.
IsBlocking: no direct external branch/call in retained named scope.

### 2 Script_ExitCutSceneMode

Execute: 0x4599b4 ->0x42ca8c _ZN11MenuManager11GetInstanceEv; 0x4599b8 ->0x42cb8c _ZN11MenuManager10GetHUDRootEv; 0x4599c0 ->0x42ca8c _ZN11MenuManager11GetInstanceEv; 0x4599c4 ->0x42cb8c _ZN11MenuManager10GetHUDRootEv; 0x4599c8 ->0x7a7cac _ZNK8RenderFX7GetRootEv; 0x4599cc ->0x774154 _ZN7gameswf4root14get_root_movieEv; 0x4599f0 ->0x7abe0c _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi; 0x459a14 ->0x7fd794 _Z9GetOnlinev; 0x459a2c ->0x80b1bc _ZN10CMessaging3GetEv; 0x459a44 ->0x80a244 _ZN8CMessage13CreateMessageEPKcb; 0x459a64 ->0x80e2a4 _ZN10CMessaging7SendMsgEP8CMessage; 0x459ac0 ->0x36e744 _ZN13PlayerManager9GetPlayerEib; 0x459ae4 ->0x459900 _ZN10PlayerInfo13SetInCutsceneEb.
IsBlocking: no direct external branch/call in retained named scope.

### 3 Script_CONSOLE

Execute: 0x460ea0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x460eb4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x460ec0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x460ec8 ->0x318254 _ZNSsD1Ev; 0x460ed0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x460ee4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x460ef0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x460efc ->0x318254 _ZNSsD1Ev; 0x460f2c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x460f3c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x460f5c ->0x4c4bdc _ZNK15PyDataConstants11getConstantEPKcS1_; 0x460f7c ->0x433ea8 _ZN9DialogMsgC1ERKSsS1_ii; 0x460f84 ->0x318254 _ZNSsD1Ev; 0x460f8c ->0x318254 _ZNSsD1Ev; 0x460f98 ->0x460dec no retained dynamic symbol; 0x460fa0 ->0x31feb0 _ZN9DialogMsgD1Ev; 0x460fc0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 4 Script_SetCamera

Execute: no direct external branch/call in retained named scope.
IsBlocking: no direct external branch/call in retained named scope.

### 5 Script_PlayCamera

Execute: 0x460148 ->0x337888 _ZN13DebugSwitches4loadEv; 0x46015c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x460168 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x460170 ->0x318254 _ZNSsD1Ev; 0x46017c ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x4601a8 ->0x40f904 _ZN11CameraLevel8PlayAnimEiib; 0x4601e8 ->0x30e310 no retained dynamic symbol.
IsBlocking: 0x45939c ->0x31f594 _ZNK11Application15GetCurrentLevelEv.

### 6 Script_SetCameraClip

Execute: 0x45c6a4 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45c6b8 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45c6c4 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45c6cc ->0x318254 _ZNSsD1Ev; 0x45c6d8 ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45c6fc ->0x30e964 no retained dynamic symbol; 0x45c76c ->0x30e964 no retained dynamic symbol; 0x45c788 ->0x30e964 no retained dynamic symbol; 0x45c7c4 ->0x30e964 no retained dynamic symbol; 0x45c824 ->0x30e004 no retained dynamic symbol; 0x45c87c ->0x30e004 no retained dynamic symbol; 0x45c888 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 7 Script_WaitCamera

Execute: 0x45c618 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45c62c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45c638 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45c640 ->0x318254 _ZNSsD1Ev; 0x45c65c ->0x30e310 no retained dynamic symbol.
IsBlocking: 0x459340 ->0x31f594 _ZNK11Application15GetCurrentLevelEv.

### 8 Script_SetCameraTarget

Execute: 0x46003c ->0x337888 _ZN13DebugSwitches4loadEv; 0x460050 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x46005c ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x460064 ->0x318254 _ZNSsD1Ev; 0x460074 ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x4600ac ->0x411b38 _ZN12CameraTarget9SetTargetEPKci; 0x4600d8 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x4600f0 ->0x4119c4 _ZN12CameraTarget9SetTargetEP10GameObjecti; 0x4600f8 ->0x30e310 no retained dynamic symbol.
IsBlocking: 0x4592f8 ->0x31f594 _ZNK11Application15GetCurrentLevelEv.

### 9 Script_EmptyImpl

Execute: 0x45c590 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45c5a4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45c5b0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45c5b8 ->0x318254 _ZNSsD1Ev; 0x45c5d4 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 10 Script_StartDialog

Execute: 0x461040 ->0x337888 _ZN13DebugSwitches4loadEv; 0x461054 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x461060 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x461068 ->0x318254 _ZNSsD1Ev; 0x46108c ->0x4c4bdc _ZNK15PyDataConstants11getConstantEPKcS1_; 0x4610b4 ->0x434434 _ZN9DialogMsgC1Eiiii; 0x4610c0 ->0x460dec no retained dynamic symbol; 0x4610c8 ->0x31feb0 _ZN9DialogMsgD1Ev; 0x4610e8 ->0x460474 no retained dynamic symbol; 0x4610f0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 11 Script_StartDialogID

Execute: 0x461170 ->0x337888 _ZN13DebugSwitches4loadEv; 0x461184 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x461190 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x461198 ->0x318254 _ZNSsD1Ev; 0x4611b0 ->0x433f04 _Z20GetDialogMsgListByIdiRSt6vectorI9DialogMsgSaIS0_EE; 0x4611c0 ->0x45c0e8 _ZNSt6vectorI9DialogMsgSaIS0_EED1Ev; 0x4611e4 ->0x460dec no retained dynamic symbol; 0x461210 ->0x456230 _ZNKSt4priv20_Deque_iterator_baseI9DialogMsgE11_M_subtractERKS2_; 0x461228 ->0x459f64 no retained dynamic symbol; 0x461230 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 12 Script_WaitDialog

Execute: 0x45c508 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45c51c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45c528 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45c530 ->0x318254 _ZNSsD1Ev; 0x45c54c ->0x30e310 no retained dynamic symbol.
IsBlocking: 0x4598ec ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x4598f4 ->0x3ef24c _ZNK5Level15hasActiveDialogEv.

### 13 Script_PlaySound

Execute: 0x45ff44 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45ff58 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45ff64 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45ff6c ->0x318254 _ZNSsD1Ev; 0x45ff9c ->0x36b80c _ZN15VoxSoundManager4PlayEibiib; 0x45ffe4 ->0x36bd78 _ZN15VoxSoundManager9PlayMusicEibbi; 0x45ffec ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 14 Script_StopSound

Execute: 0x45fe60 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45fe74 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45fe80 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45fe88 ->0x318254 _ZNSsD1Ev; 0x45feac ->0x369fec _ZN15VoxSoundManager4StopEii; 0x45fedc ->0x36a1a0 _ZN15VoxSoundManager9StopMusicEi; 0x45fee4 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 15 Script_PlayLevelMusic

Execute: 0x45fd54 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45fd68 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45fd74 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45fd7c ->0x318254 _ZNSsD1Ev; 0x45fd88 ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45fdb8 ->0x36bd78 _ZN15VoxSoundManager9PlayMusicEibbi; 0x45fdd4 ->0x369514 _ZN15VoxSoundManager13SetMusicStateEPKc; 0x45fe00 ->0x369514 _ZN15VoxSoundManager13SetMusicStateEPKc; 0x45fe08 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 16 Script_EnterSafeZone

Execute: 0x45fcb0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45fcc4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45fcd0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45fcd8 ->0x318254 _ZNSsD1Ev; 0x45fcec ->0x36c014 _ZN15VoxSoundManager18SetInSafeZoneMusicEb; 0x45fd08 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 17 Script_LeaveSafeZone

Execute: 0x45fc10 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45fc24 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45fc30 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45fc38 ->0x318254 _ZNSsD1Ev; 0x45fc4c ->0x36c014 _ZN15VoxSoundManager18SetInSafeZoneMusicEb; 0x45fc68 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 18 Script_PlayAnimById

Execute: 0x45e2ac ->0x337888 _ZN13DebugSwitches4loadEv; 0x45e2c0 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45e2cc ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45e2d4 ->0x318254 _ZNSsD1Ev; 0x45e2f8 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e304 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45e314 ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45e35c ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 19 Script_PlayAnimByName

Execute: 0x45e184 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45e198 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45e1a4 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45e1ac ->0x318254 _ZNSsD1Ev; 0x45e1d0 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e1dc ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45e1ec ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45e234 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 20 Script_PlayEffect

Execute: 0x45fafc ->0x337888 _ZN13DebugSwitches4loadEv; 0x45fb10 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45fb1c ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45fb24 ->0x318254 _ZNSsD1Ev; 0x45fb48 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45fb58 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45fb68 ->0x30e964 no retained dynamic symbol; 0x45fb74 ->0x30e964 no retained dynamic symbol; 0x45fb80 ->0x30e964 no retained dynamic symbol; 0x45fbac ->0x495d14 _ZN15VisualFXManager13PlayAnimFXSetEiRK7Point3DIfEPK10GameObjectPNS_13AnimFXSetDataE; 0x45fbb8 ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45fbc4 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 21 Script_StopEffect

Execute: 0x4598a8 ->0x494660 _ZN15VisualFXManager21DropAnimatedFXSetByIDEi.
IsBlocking: no direct external branch/call in retained named scope.

### 22 Script_ShowFlash

Execute: 0x45f8fc ->0x337888 _ZN13DebugSwitches4loadEv; 0x45f914 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45f920 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45f928 ->0x318254 _ZNSsD1Ev; 0x45f964 ->0x42ca8c _ZN11MenuManager11GetInstanceEv; 0x45f974 ->0x30ebd4 no retained dynamic symbol; 0x45f998 ->0x42cb8c _ZN11MenuManager10GetHUDRootEv; 0x45f9a0 ->0x7a7cac _ZNK8RenderFX7GetRootEv; 0x45f9a4 ->0x774154 _ZN7gameswf4root14get_root_movieEv; 0x45f9c0 ->0x7abe0c _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi; 0x45f9c8 ->0x7a7cac _ZNK8RenderFX7GetRootEv; 0x45f9cc ->0x774154 _ZN7gameswf4root14get_root_movieEv; 0x45f9d4 ->0x42ca8c _ZN11MenuManager11GetInstanceEv; 0x45f9e8 ->0x30ed30 no retained dynamic symbol; 0x45fa1c ->0x7abe0c _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi; 0x45fa24 ->0x797124 _ZN7gameswf8as_value9drop_refsEv; 0x45fa2c ->0x7a7cac _ZNK8RenderFX7GetRootEv; 0x45fa30 ->0x774154 _ZN7gameswf4root14get_root_movieEv; 0x45fa58 ->0x7abe0c _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi; 0x45fa60 ->0x797124 _ZN7gameswf8as_value9drop_refsEv; 0x45fa74 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 23 Script_HideFlash

Execute: 0x45f7c0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45f7d8 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45f7e4 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45f7ec ->0x318254 _ZNSsD1Ev; 0x45f828 ->0x42ca8c _ZN11MenuManager11GetInstanceEv; 0x45f83c ->0x30ebd4 no retained dynamic symbol; 0x45f84c ->0x42cb8c _ZN11MenuManager10GetHUDRootEv; 0x45f858 ->0x42cb8c _ZN11MenuManager10GetHUDRootEv; 0x45f85c ->0x7a7cac _ZNK8RenderFX7GetRootEv; 0x45f860 ->0x774154 _ZN7gameswf4root14get_root_movieEv; 0x45f890 ->0x7abe0c _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi; 0x45f898 ->0x797124 _ZN7gameswf8as_value9drop_refsEv; 0x45f8ac ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 24 Script_LockCharacter

Execute: 0x45ddfc ->0x337888 _ZN13DebugSwitches4loadEv; 0x45de10 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45de1c ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45de24 ->0x318254 _ZNSsD1Ev; 0x45de38 ->0x30e6e8 no retained dynamic symbol; 0x45de7c ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45de88 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45de98 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45deb0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 25 Script_UnlockCharacter

Execute: 0x45dcb8 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45dccc ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45dcd8 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45dce0 ->0x318254 _ZNSsD1Ev; 0x45dcf4 ->0x30e31c no retained dynamic symbol; 0x45dd50 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45dd5c ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45dd6c ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45dd80 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 26 Script_Wait

Execute: 0x45c46c ->0x337888 _ZN13DebugSwitches4loadEv; 0x45c484 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45c490 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45c498 ->0x318254 _ZNSsD1Ev; 0x45c4c4 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 27 Script_SetFaeryState

Execute: 0x45f6fc ->0x337888 _ZN13DebugSwitches4loadEv; 0x45f710 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45f71c ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45f724 ->0x318254 _ZNSsD1Ev; 0x45f73c ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45f754 ->0x3ae7d4 _ZN9Character13SetFaeryStateEji; 0x45f774 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 28 Script_IncFaeryLevel

Execute: 0x45f600 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45f614 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45f620 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45f628 ->0x318254 _ZNSsD1Ev; 0x45f62c ->0x7fd794 _Z9GetOnlinev; 0x45f650 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45f664 ->0x3ae61c _ZN9Character13IncFaeryLevelEj; 0x45f690 ->0x36f074 _ZN13PlayerManager20IsLocalPlayerHostingEv; 0x45f6b0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 29 Script_PutCharacterInLimbus

Execute: 0x45f520 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45f534 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45f540 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45f548 ->0x318254 _ZNSsD1Ev; 0x45f574 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45f57c ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45f594 ->0x3c1a74 _ZN16CharStateMachine17SM_SetLimbusStateEb; 0x45f5b4 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 30 Script_SpawnCharacter

Execute: 0x45f438 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45f44c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45f458 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45f460 ->0x318254 _ZNSsD1Ev; 0x45f48c ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45f494 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45f4b0 ->0x3c2734 _ZN16CharStateMachine16SM_SetSpawnStateEbb; 0x45f4d0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 31 Script_PutCharacterInIdle

Execute: 0x45d5d0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d5e4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d5f0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d5f8 ->0x318254 _ZNSsD1Ev; 0x45d618 ->0x4594a8 _Z30Script_CharacterState_CallbackPKciPFvP10ObjectBaseE; 0x45d634 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 32 Script_MarkCharacterAsScripted

Execute: 0x45d500 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d514 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d520 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d528 ->0x318254 _ZNSsD1Ev; 0x45d548 ->0x4594a8 _Z30Script_CharacterState_CallbackPKciPFvP10ObjectBaseE; 0x45d574 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 33 Script_AIEnableTimer

Execute: no direct external branch/call in retained named scope.
IsBlocking: no direct external branch/call in retained named scope.

### 34 Script_AIResetTimer

Execute: no direct external branch/call in retained named scope.
IsBlocking: no direct external branch/call in retained named scope.

### 35 Script_AIDoSkill

Execute: no direct external branch/call in retained named scope.
IsBlocking: no direct external branch/call in retained named scope.

### 36 Script_AISetHP

Execute: no direct external branch/call in retained named scope.
IsBlocking: no direct external branch/call in retained named scope.

### 37 Script_AIChangeScript

Execute: no direct external branch/call in retained named scope.
IsBlocking: no direct external branch/call in retained named scope.

### 38 Script_AISetInt

Execute: 0x45f334 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45f348 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45f354 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45f35c ->0x318254 _ZNSsD1Ev; 0x45f388 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45f394 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45f3c8 ->0x37d990 _ZN9LuaScript6SetIntEPKci; 0x45f3e8 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 39 Script_StopActor

Execute: 0x45eef8 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45ef0c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45ef18 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45ef20 ->0x318254 _ZNSsD1Ev; 0x45ef34 ->0x30e6e8 no retained dynamic symbol; 0x45ef74 ->0x33dd2c _ZN10ObjectBase9GetHandleEv; 0x45ef7c ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45ef94 ->0x40559c _ZN12v2Controller8Cmd_StopEv; 0x45f044 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45f050 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45f060 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45f07c ->0x40559c _ZN12v2Controller8Cmd_StopEv; 0x45f08c ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 40 Script_MoveActor

Execute: 0x45f0e4 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45f100 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45f10c ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45f114 ->0x318254 _ZNSsD1Ev; 0x45f13c ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45f148 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45f178 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45f184 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45f204 ->0x394bf8 _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb; 0x45f218 ->0x405540 _ZN12v2Controller10Cmd_MoveToEP10GameObject; 0x45f24c ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45f27c ->0x405318 _ZN12v2Controller10Cmd_WarpToERK7Point3DIfE; 0x45f294 ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45f2b4 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45f2c4 ->0x3949b0 _ZN10GameObject17DisableCollisionsEv; 0x45f2e4 ->0x30e310 no retained dynamic symbol.
IsBlocking: 0x4596e0 ->0x40559c _ZN12v2Controller8Cmd_StopEv; 0x459710 ->0x3a59ac _ZN9Character6ReviveEP10GameObjectb.

### 41 Script_LookActor

Execute: 0x45ec88 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45eca0 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45ecac ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45ecb4 ->0x318254 _ZNSsD1Ev; 0x45ecdc ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45ece8 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45ed00 ->0x33f524 _ZN12ObjectHandleC1EP10ObjectBase; 0x45ed14 ->0x30e31c no retained dynamic symbol; 0x45ed44 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45ed68 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45ed84 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45edbc ->0x4052bc _ZN12v2Controller10Cmd_LookAtEP10GameObject; 0x45edd4 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45ee20 ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45ee30 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45ee44 ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45ee60 ->0x36e744 _ZN13PlayerManager9GetPlayerEib; 0x45ee84 ->0x4052bc _ZN12v2Controller10Cmd_LookAtEP10GameObject; 0x45eea4 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 42 Script_ShowActor

Execute: 0x45eb54 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45eb64 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45eb70 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45eb78 ->0x318254 _ZNSsD1Ev; 0x45eb88 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45eb98 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45eba4 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45ebac ->0x318254 _ZNSsD1Ev; 0x45ebd8 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45ebe4 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45ec10 ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45ec1c ->0x38c600 _ZN10GameObject13DisableZoningEv; 0x45ec38 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 43 Script_HideActor

Execute: 0x45e060 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45e074 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45e080 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45e088 ->0x318254 _ZNSsD1Ev; 0x45e0b4 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e0c0 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45e0ec ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45e10c ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 44 Script_KillActor

Execute: 0x45ea3c ->0x337888 _ZN13DebugSwitches4loadEv; 0x45ea50 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45ea5c ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45ea64 ->0x318254 _ZNSsD1Ev; 0x45ea90 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45ea9c ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45eac8 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45eaec ->0x40570c _ZN12v2Controller8Cmd_KillEP10GameObjectb; 0x45eafc ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 45 Script_PlayActorAnim

Execute: 0x45e900 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45e914 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45e920 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45e928 ->0x318254 _ZNSsD1Ev; 0x45e94c ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e974 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45e984 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45e9e0 ->0x3c1aa8 _ZN16CharStateMachine15SM_SetAnimStateEibb; 0x45e9e8 ->0x30e310 no retained dynamic symbol.
IsBlocking: 0x459650 ->0x33ff8c _ZNK12ObjectHandle9GetObjectEb; 0x459664 ->0x33ff8c _ZNK12ObjectHandle9GetObjectEb.

### 46 Script_SetActorPosition

Execute: 0x45e600 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45e618 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45e624 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45e62c ->0x318254 _ZNSsD1Ev; 0x45e654 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e65c ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45e680 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e688 ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45e6c4 ->0x33dd2c _ZN10ObjectBase9GetHandleEv; 0x45e6cc ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45e6e4 ->0x3c1a00 _ZN16CharStateMachine15SM_SetIdleStateEb; 0x45e70c ->0x393db4 _ZN10GameObject11SetPositionERK7Point3DIfEb; 0x45e714 ->0x393e90 _ZN10GameObject19ForceUpdatePositionEv; 0x45e728 ->0x36e744 _ZN13PlayerManager9GetPlayerEib; 0x45e740 ->0x30eba4 no retained dynamic symbol; 0x45e750 ->0x30eba4 no retained dynamic symbol; 0x45e764 ->0x30eba4 no retained dynamic symbol; 0x45e780 ->0x393db4 _ZN10GameObject11SetPositionERK7Point3DIfEb; 0x45e788 ->0x393e90 _ZN10GameObject19ForceUpdatePositionEv; 0x45e79c ->0x36e744 _ZN13PlayerManager9GetPlayerEib; 0x45e7b4 ->0x30eba4 no retained dynamic symbol; 0x45e7c4 ->0x30eba4 no retained dynamic symbol; 0x45e7d8 ->0x30eba4 no retained dynamic symbol; 0x45e7f4 ->0x393db4 _ZN10GameObject11SetPositionERK7Point3DIfEb; 0x45e7fc ->0x393e90 _ZN10GameObject19ForceUpdatePositionEv; 0x45e810 ->0x36e744 _ZN13PlayerManager9GetPlayerEib; 0x45e82c ->0x30eba4 no retained dynamic symbol; 0x45e83c ->0x30eba4 no retained dynamic symbol; 0x45e84c ->0x30eba4 no retained dynamic symbol; 0x45e868 ->0x393db4 _ZN10GameObject11SetPositionERK7Point3DIfEb; 0x45e870 ->0x393e90 _ZN10GameObject19ForceUpdatePositionEv; 0x45e878 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 47 Script_SetActorMaster

Execute: 0x45e4f0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45e504 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45e510 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45e518 ->0x318254 _ZNSsD1Ev; 0x45e544 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e54c ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45e570 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e578 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45e590 ->0x3d4d80 _ZN6CharAI12AI_SetMasterEP9Character; 0x45e5b0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 48 Script_DropLoot

Execute: 0x45e3ac ->0x337888 _ZN13DebugSwitches4loadEv; 0x45e3c4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45e3d0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45e3d8 ->0x318254 _ZNSsD1Ev; 0x45e400 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e40c ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45e43c ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45e448 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45e460 ->0x3a5ae4 _ZNK9Character8DropLootEP10GameObject; 0x45e484 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45e494 ->0x33fee4 _ZN12ObjectHandlecvP10GameObjectEv; 0x45e4a0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 49 Script_AutoEquip

Execute: 0x45dbc0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45dbd4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45dbe0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45dbe8 ->0x318254 _ZNSsD1Ev; 0x45dc14 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45dc20 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45dc4c ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45dc68 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 50 Script_UnEquipItemFromSlot

Execute: 0x45dac4 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45dad8 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45dae4 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45daec ->0x318254 _ZNSsD1Ev; 0x45db18 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45db24 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45db50 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45db70 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 51 Script_UnEquipHands

Execute: 0x45df08 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45df1c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45df28 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45df30 ->0x318254 _ZNSsD1Ev; 0x45df5c ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45df68 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45df94 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45dfac ->0x3ffe3c _ZN13ItemInventory15GetEquippedItemEj; 0x45dfb8 ->0x3fc63c _ZNK13ItemInventory12GetItemIndexEPK12ItemInstance; 0x45dfcc ->0x3ffe3c _ZN13ItemInventory15GetEquippedItemEj; 0x45dfd8 ->0x3fc63c _ZNK13ItemInventory12GetItemIndexEPK12ItemInstance; 0x45e010 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 52 Script_ReEquipHands

Execute: 0x45d994 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d9a8 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d9b4 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d9bc ->0x318254 _ZNSsD1Ev; 0x45d9e8 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d9f4 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45da20 ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45da74 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 53 Script_SetStatic

Execute: 0x45d8a0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d8b4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d8c0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d8c8 ->0x318254 _ZNSsD1Ev; 0x45d8f4 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d900 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45d93c ->0x33ff54 _ZN12ObjectHandlecvP9CharacterEv; 0x45d944 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 54 Script_OpenDoor

Execute: 0x45d798 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d7b0 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d7bc ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d7c4 ->0x318254 _ZNSsD1Ev; 0x45d7f0 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d7fc ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45d848 ->0x3e786c _ZN4Door4OpenEb; 0x45d850 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 55 Script_CloseDoor

Execute: 0x45d68c ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d6a4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d6b0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d6b8 ->0x318254 _ZNSsD1Ev; 0x45d6e4 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d6f0 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45d73c ->0x3e763c _ZN4Door5CloseEb; 0x45d744 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 56 Script_ActivateProjectileTrap

Execute: 0x45d404 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d41c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d428 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d430 ->0x318254 _ZNSsD1Ev; 0x45d45c ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d468 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45d4b0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 57 Script_DeactivateProjectileTrap

Execute: 0x45d30c ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d324 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d330 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d338 ->0x318254 _ZNSsD1Ev; 0x45d364 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d370 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45d3b4 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 58 Script_ActivateTriggerPlate

Execute: 0x45d210 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d228 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d234 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d23c ->0x318254 _ZNSsD1Ev; 0x45d268 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d274 ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45d2bc ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 59 Script_DeactivateTriggerPlate

Execute: 0x45d118 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d130 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d13c ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d144 ->0x318254 _ZNSsD1Ev; 0x45d170 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d17c ->0x33fdc0 _ZN12ObjectHandle9GetObjectEb; 0x45d1c0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 60 Script_SpawnContainer

Execute: 0x45d040 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45d054 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45d060 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45d068 ->0x318254 _ZNSsD1Ev; 0x45d094 ->0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_; 0x45d09c ->0x4593f4 _ZN12ObjectHandlecvPT_I9ContainerEEv; 0x45d0a8 ->0x39f3f0 _ZN9Container5SpawnEv; 0x45d0c8 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 61 Script_SetLevelState

Execute: 0x45cf74 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45cf88 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45cf94 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45cf9c ->0x318254 _ZNSsD1Ev; 0x45cfb4 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45cfd0 ->0x3bbca4 _ZN9Character16SG_SetLevelStateEiii; 0x45cff0 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 62 Script_SetWorldMapLocationState

Execute: 0x45ceac ->0x337888 _ZN13DebugSwitches4loadEv; 0x45cec0 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45cecc ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45ced4 ->0x318254 _ZNSsD1Ev; 0x45ceec ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45cf08 ->0x3bbda0 _ZN9Character17SG_SetMapLocStateEiii; 0x45cf28 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 63 Script_RestartLevel

Execute: 0x45ce00 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45ce14 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45ce20 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45ce28 ->0x318254 _ZNSsD1Ev; 0x45ce34 ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45ce40 ->0x3ef9a0 _ZN5Level7RestartEv; 0x45ce60 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 64 Script_ChangeLevel

Execute: 0x45cbd8 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45cbec ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45cbf8 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45cc00 ->0x318254 _ZNSsD1Ev; 0x45cc0c ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45cc20 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45cc28 ->0x3bb8e4 _ZN9Character20SG_GetGameDifficultyEv; 0x45cc40 ->0x3efc40 _ZN5Level16SG_SaveAllPlayerEb; 0x45cc8c ->0x30e31c no retained dynamic symbol; 0x45ccc8 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45ccd8 ->0x3bb854 _ZN9Character13SG_SetLevelIdEii; 0x45cce8 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45ccf0 ->0x3bc4a8 _ZN9Character7SG_SaveEv; 0x45ccf4 ->0x7fd794 _Z9GetOnlinev; 0x45cd20 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45cd28 ->0x3bb728 _ZN9Character10SG_GetSlotEv; 0x45cd60 ->0x32bdc8 _ZN11Application9LoadLevelEPKcijbbibjj; 0x45cd8c ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45cd98 ->0x36e09c _ZN13PlayerManager16GetHostingPlayerEv; 0x45cdac ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 65 Script_EndGame

Execute: 0x45cab0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45cac4 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45cad0 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45cad8 ->0x318254 _ZNSsD1Ev; 0x45caf0 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45cafc ->0x3bb8e4 _ZN9Character20SG_GetGameDifficultyEv; 0x45cb08 ->0x3bb8e4 _ZN9Character20SG_GetGameDifficultyEv; 0x45cb1c ->0x3bb854 _ZN9Character13SG_SetLevelIdEii; 0x45cb34 ->0x3bb89c _ZN9Character21SG_SetLevelEntryPointEii; 0x45cb40 ->0x3bb930 _ZN9Character28SG_SetGameDifficultyUnlockedEi; 0x45cb48 ->0x3bc4a8 _ZN9Character7SG_SaveEv; 0x45cb54 ->0x3bb950 _ZN9Character20SG_SetGameDifficultyEi; 0x45cb68 ->0x33a388 _ZN12StateMachine11SwitchStateEP9StateBaseb; 0x45cb84 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 66 Script_AwardTrophy

Execute: 0x4593e8 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi.
IsBlocking: no direct external branch/call in retained named scope.

### 67 Script_AwardEndGameTrophies

Execute: 0x459b24 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x459b3c ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459b50 ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459b60 ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459b74 ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459b88 ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459b9c ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459bb0 ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459bc4 ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459bd8 ->0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv; 0x459bec ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459bf8 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459c04 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459c10 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459c20 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459c2c ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459c38 ->0x3bc37c _ZN9Character15SG_GetNumQuestsEi; 0x459c80 ->0x3bc2f8 _ZN9Character15SG_GetQuestByIDEii; 0x459ca4 ->0x3bc2f8 _ZN9Character15SG_GetQuestByIDEii; 0x459cc0 ->0x3bc2f8 _ZN9Character15SG_GetQuestByIDEii; 0x459cc4 ->0x47f8a4 _ZNK5Quest11IsSecondaryEv; 0x459cec ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459cf8 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459d0c ->0x3df6e0 _ZNK14CharProperties12PROPS_GetIntEib; 0x459d24 ->0x3df6e0 _ZNK14CharProperties12PROPS_GetIntEib; 0x459d30 ->0x3bb828 _ZN9Character17SG_GetPlayerLevelEv; 0x459d4c ->0x3df6e0 _ZNK14CharProperties12PROPS_GetIntEib; 0x459d64 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459d74 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459d80 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459d8c ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459d9c ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459da8 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459db8 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459dc4 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459dd0 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459ddc ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459dec ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459df8 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459e04 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459e10 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459e20 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459e2c ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459e38 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459e44 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459e54 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459e60 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459e70 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459e7c ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459e8c ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459e98 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459ea4 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459eb0 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459ec0 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459ecc ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459edc ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459ee8 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi; 0x459ef4 ->0x3a3f70 _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc; 0x459f00 ->0x3813b8 _ZN13TrophyManager12UnlockTrophyEi.
IsBlocking: no direct external branch/call in retained named scope.

### 68 Script_ShowTrophies

Execute: no direct external branch/call in retained named scope.
IsBlocking: no direct external branch/call in retained named scope.

### 69 Script_SaveGame

Execute: 0x45c9c4 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45c9dc ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45c9e8 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45c9f0 ->0x318254 _ZNSsD1Ev; 0x45c9f8 ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45ca08 ->0x3ef238 _ZN5Level13SG_BlockLevelEb; 0x45ca14 ->0x3f059c _ZN5Level9QuickSaveEb; 0x45ca28 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45ca40 ->0x3bb770 _ZN9Character8SG_BlockEb; 0x45ca48 ->0x3bc4a8 _ZN9Character7SG_SaveEv; 0x45ca68 ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 70 Script_BlockSaveGame

Execute: 0x45c8f0 ->0x337888 _ZN13DebugSwitches4loadEv; 0x45c908 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x45c914 ->0x337a88 _ZN13DebugSwitches9GetSwitchERKSs; 0x45c91c ->0x318254 _ZNSsD1Ev; 0x45c930 ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x45c944 ->0x3bb770 _ZN9Character8SG_BlockEb; 0x45c94c ->0x31f594 _ZNK11Application15GetCurrentLevelEv; 0x45c95c ->0x3ef238 _ZN5Level13SG_BlockLevelEb; 0x45c97c ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 71 Script_EnqueueTutorialMessage

Execute: 0x461454 ->0x456298 _ZNKSt4priv20_Deque_iterator_baseI11TutorialMsgE11_M_subtractERKS2_; 0x461474 ->0x45a030 no retained dynamic symbol; 0x461484 ->0x46124c _ZNSt5dequeI11TutorialMsgSaIS0_EE18_M_push_back_aux_vERKS0_.
IsBlocking: no direct external branch/call in retained named scope.

### 72 Script_SkipTutorialMessage

Execute: 0x460338 ->0x46026c no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 73 Script_SkipAllTutorialMessages

Execute: 0x46038c ->0x708f00 no retained dynamic symbol; 0x4603c4 ->0x46026c no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 74 Script_EnqueueCharMenuTutorialMessage

Execute: 0x460b5c ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x460b70 ->0x3140ec _ZNSsC1EPKcRKSaIcE; 0x460b84 ->0x433b44 _ZN19CharMenuTutorialMsgC1EiRKSsS1_; 0x460b8c ->0x318254 _ZNSsD1Ev; 0x460b94 ->0x318254 _ZNSsD1Ev; 0x460bb0 ->0x30de54 no retained dynamic symbol; 0x460bc0 ->0x3109e0 _ZNSs9_M_assignEPKcS0_; 0x460bcc ->0x30de54 no retained dynamic symbol; 0x460bdc ->0x3109e0 _ZNSs9_M_assignEPKcS0_; 0x460bf0 ->0x45b7f4 _ZNSt5dequeI19CharMenuTutorialMsgSaIS0_EE9push_backERKS0_; 0x460bf8 ->0x318254 _ZNSsD1Ev; 0x460c00 ->0x318254 _ZNSsD1Ev; 0x460c1c ->0x30e310 no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 75 Script_SkipCharMenuTutorialMessage

Execute: 0x4603f8 ->0x441d58 _ZNSt5dequeI19CharMenuTutorialMsgSaIS0_EE9pop_frontEv; 0x460414 ->0x45a0fc no retained dynamic symbol.
IsBlocking: no direct external branch/call in retained named scope.

### 76 Script_SkipAllCharMenuTutorialMessages

Execute: 0x460454 ->0x441d58 _ZNSt5dequeI19CharMenuTutorialMsgSaIS0_EE9pop_frontEv.
IsBlocking: no direct external branch/call in retained named scope.

### 77 Script_LockTutorial

Execute: 0x4592b0 ->0x317e98 _ZN16updateJob_thread6Start2Ev; 0x4592b8 ->0x46cb34 _ZN15SavegameManager12saveSettingsEv.
IsBlocking: no direct external branch/call in retained named scope.

### 78 Script_DoTutorial

Execute: 0x46097c ->0x36e478 _ZN13PlayerManager14GetLocalPlayerEib; 0x460990 ->0x3bb8e4 _ZN9Character20SG_GetGameDifficultyEv; 0x4609b4 ->0x7fd794 _Z9GetOnlinev; 0x4609d8 ->0x4591f0 _ZNK13ScriptManager13GetIDFromNameEPKcb; 0x4609f4 ->0x4605c0 _ZN13ScriptManager11StartScriptEiib; 0x460a34 ->0x317e98 _ZN16updateJob_thread6Start2Ev; 0x460a40 ->0x46cb34 _ZN15SavegameManager12saveSettingsEv.
IsBlocking: no direct external branch/call in retained named scope.

### 79 Script_FlushMessages

Execute: 0x460a60 ->0x460474 no retained dynamic symbol; 0x460a88 ->0x383ef4 _ZNSt5dequeI14AchievementMsgSaIS0_EE9pop_frontEv; 0x460a9c ->0x460474 no retained dynamic symbol; 0x460ac0 ->0x383d10 _ZNSt5dequeI9StatusMsgSaIS0_EE9pop_frontEv; 0x460af4 ->0x383da8 _ZNSt5dequeI15OnlineStatusMsgSaIS0_EE9pop_frontEv.
IsBlocking: no direct external branch/call in retained named scope.

