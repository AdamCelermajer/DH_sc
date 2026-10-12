# Storage schemas added in the complete batch

Offsets are original ARM byte offsets. R4 means a raw four-byte value; B1 means one byte; C@n means CString storage read after the actual length at offset n. These denote source representation only. All 32 added commands have Init 0x455628. No runtime behavior is implemented here.

| Kind | Actual class | C1 bytes | Data bytes / factory / reader | Ordered Data actions | C1 scalar defaults |
|---:|---|---:|---|---|---|
|9|Script_EmptyImpl|16|8 /0x4574a0 /0x4ff8d4|4:R4|4:4=0|
|11|Script_StartDialogID|16|12 /0x457418 /0x5033e4|4:R4, 8:R4|4:4=0, 8:4=0|
|18|Script_PlayAnimById|16|28 /0x457254 /0x502594|4:R4, 8:R4, 12:B1, 16:R4, 20:C@16, 24:B1|none produced|
|28|Script_IncFaeryLevel|16|12 /0x456fd0 /0x503130|4:R4, 8:R4|4:4=0, 8:4=0|
|33|Script_AIEnableTimer|16|12 /0x456ea0 /0x4ff9b4|4:R4, 8:B1|4:4=0, 8:1=0|
|34|Script_AIResetTimer|16|8 /0x456e64 /0x4ff8c4|4:R4|4:4=0|
|35|Script_AIDoSkill|16|20 /0x456e28 /0x501ad8|4:R4, 8:R4, 12:R4, 16:C@12|none produced|
|36|Script_AISetHP|16|12 /0x456de8 /0x5030b8|4:R4, 8:R4|4:4=0, 8:4=0|
|37|Script_AIChangeScript|16|24 /0x456da8 /0x501974|4:R4, 8:R4, 12:C@8, 16:R4, 20:C@16|none produced|
|38|Script_AISetInt|16|28 /0x456d68 /0x5017b4|4:R4, 8:R4, 12:C@8, 16:R4, 20:C@16, 24:R4|none produced|
|47|Script_SetActorMaster|16|32 /0x456b3c /0x500d14|4:R4, 8:B1, 12:R4, 16:C@12, 20:B1, 24:R4, 28:C@24|none produced|
|48|Script_DropLoot|16|24 /0x456afc /0x500bb0|4:R4, 8:R4, 12:C@8, 16:R4, 20:C@16|none produced|
|49|Script_AutoEquip|16|16 /0x456ac0 /0x500af0|4:R4, 8:R4, 12:C@8|none produced|
|50|Script_UnEquipItemFromSlot|16|20 /0x456a84 /0x5009d4|4:R4, 8:R4, 12:R4, 16:C@12|none produced|
|53|Script_SetStatic|16|20 /0x4569d0 /0x500788|4:R4, 8:B1, 12:R4, 16:C@12|none produced|
|56|Script_ActivateProjectileTrap|20|16 /0x45691c /0x500530|4:R4, 8:R4, 12:C@8|none produced|
|57|Script_DeactivateProjectileTrap|20|16 /0x4568e0 /0x500470|4:R4, 8:R4, 12:C@8|none produced|
|58|Script_ActivateTriggerPlate|20|16 /0x4568a4 /0x5003b0|4:R4, 8:R4, 12:C@8|none produced|
|59|Script_DeactivateTriggerPlate|20|16 /0x456868 /0x5002f0|4:R4, 8:R4, 12:C@8|none produced|
|60|Script_SpawnContainer|16|16 /0x45682c /0x500230|4:R4, 8:R4, 12:C@8|none produced|
|61|Script_SetLevelState|16|16 /0x4567e8 /0x502fe4|4:R4, 8:R4, 12:R4|4:4=0, 8:4=0, 12:4=0|
|62|Script_SetWorldMapLocationState|16|16 /0x4567a4 /0x502f10|4:R4, 8:R4, 12:R4|4:4=0, 8:4=0, 12:4=0|
|64|Script_ChangeLevel|16|20 /0x45672c /0x500114|4:R4, 8:R4, 12:R4, 16:C@12|none produced|
|65|Script_EndGame|16|8 /0x4566f0 /0x4ff8bc|4:R4|4:4=0|
|66|Script_AwardTrophy|16|12 /0x4566b0 /0x502e98|4:R4, 8:R4|4:4=0, 8:4=0|
|67|Script_AwardEndGameTrophies|16|8 /0x456674 /0x4ff8b8|4:R4|4:4=0|
|71|Script_EnqueueTutorialMessage|16|16 /0x45657c /0x502dc4|4:R4, 8:R4, 12:R4|4:4=0, 8:4=0, 12:4=0|
|72|Script_SkipTutorialMessage|16|8 /0x456540 /0x4ff8a8|4:R4|4:4=0|
|73|Script_SkipAllTutorialMessages|16|8 /0x456504 /0x4ff8a4|4:R4|4:4=0|
|74|Script_EnqueueCharMenuTutorialMessage|16|28 /0x4564c4 /0x4fff54|4:R4, 8:R4, 12:C@8, 16:R4, 20:C@16, 24:R4|none produced|
|75|Script_SkipCharMenuTutorialMessage|16|8 /0x456488 /0x4ff8a0|4:R4|4:4=0|
|76|Script_SkipAllCharMenuTutorialMessages|16|8 /0x45644c /0x4ff89c|4:R4|4:4=0|

Prior 48 schemas and constructors are retained unchanged except honest provenance comments and the generic checked native-pointer route. Kind45 nested ObjectHandle stores remain grounded in retained V58 MEM_WRITE evidence, not a guessed straight-line factory projection.
