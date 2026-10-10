#pragma once
#include <cstdint>
//Original Structs global C1 4ddeb8 literal CString order; m_dataNames
//symbol sizes and registered getter identities come from the shipping ELF.
//Item fields23..37 are the actual C1 empty-string loop, not schema defaults.
namespace dh2::android_ui {
struct ProcessCompiledStructV121 {const char* type;const char*const* members;std::uint32_t count,source_names,getter;};
inline constexpr const char* process_members_AIProps_v121[]={"AttackDelay","CombatBeat","CombatMusic","DelayedLoad","Flags","InteractRadius","LeashDistance","MeleeRadius","OnAggroSFX","Script","SelfFX","Trophy","Type","ViewRadius","ViewRadiusNoAggro"};
//The compiled Structs member is the outer property. The AIFactionTable [ii]
//reader shape encodes the nested Id/Value records stored under this property.
inline constexpr const char* process_members_AIFactions_v121[]={"factions"};
inline constexpr const char* process_members_AnimTpl_v121[]={"Loop","Steps","Type"};
inline constexpr const char* process_members_CamAnimSet_v121[]={"CamAnims","Crit","Idle","Shake","Template"};
inline constexpr const char* process_members_CharAnim_v121[]={"Attack","AttackStatic","Blocking","DeadlyGreatKB","Despawn","DespawnGreatKB","Died","Dodging","GreatKnockedBack","Idle","IdleFromOOC","IdleOOC","IdleSneak","IdleToOOC","Injured","Interact","KnockedBack","LiftDrop","LiftIdle","LiftMove","Limbus","MenuIdle","MenuOnSelect","PreSpawn","Revived","Reviving","Run","Run180","RunSneak","Scared","Spawn","Spells","Stunned","Template","Walk","Walk180","WalkSneak"};
inline constexpr const char* process_members_ClassFuncList_v121[]={"list_entries"};
inline constexpr const char* process_members_CharacterProperties_v121[]={"AIFaction","AI","AnimTable","ModelFile","ModularVisual","ClassString","ClassDescString","Effects","Sounds","Loot","Dialogs","RespawnTime","Scale_X","Scale_Y","Scale_Z","Melee_Scale","Collision_Scale","FX_Scale","Name","Level","LevelMax","LevelMin","LevelOffset","KillCount","MPLevelKillCount","DeathCount","ClassID","StatAutoAssignScheme","SkillTree","FaeryList","RangeMinDistance","RangeMaxDistance","RangeProjectileID","XP","Max_XP","Value_XP","HP","HP_Bonus","Max_HP","Regen_HP","CombatRegen_HP","MP","MP_Bonus","Max_MP","Regen_MP","CombatRegen_MP","Speed_Modifier_Walk","Speed_Modifier_Rotation","Speed_Modifier_Attack","Opacity_Modifier","Rating_Attack","Rating_Attack_Bonus_Sword","Rating_Attack_Bonus_Axe","Rating_Attack_Bonus_Mace","Rating_Attack_Bonus_Dagger","Rating_Attack_Bonus_Range","Rating_Attack_Bonus_Staff","Rating_Attack_Bonus_Wand","Rating_Attack_Bonus_Dual","Rating_Defense","Rating_Dodge","Rating_Block","Rating_Block_Bonus_Shield","Rating_Critical","Rating_Critical_Bonus_Sword","Rating_Critical_Bonus_Axe","Rating_Critical_Bonus_Mace","Rating_Critical_Bonus_Dagger","Rating_Critical_Bonus_Range","Rating_Critical_Bonus_Staff","Rating_Critical_Bonus_Wand","Physical_Armor","Physical_Armor_Bonus_Shield","Magical_Armor","Resistance_Fire","Resistance_Water","Resistance_Lightning","Resistance_Earth","Resistance_Air","Damage_Min_Main_Hand","Damage_Max_Main_Hand","Damage_Min_Off_Hand","Damage_Max_Off_Hand","Damage_Bonus_Sword","Damage_Bonus_Axe","Damage_Bonus_Mace","Damage_Bonus_Dagger","Damage_Bonus_Range","Damage_Bonus_Staff","Damage_Bonus_Wand","Damage_Bonus_Dual","Damage_Bonus_TwoHand","Damage_Bonus_Stunned","Damage_Bonus_Sneaking","Damage_Bonus_Combo","Damage_Elemental_Min_Main_Hand","Damage_Elemental_Max_Main_Hand","Damage_Elemental_Type_Main_Hand","Damage_Elemental_Min_Off_Hand","Damage_Elemental_Max_Off_Hand","Damage_Elemental_Type_Off_Hand","Damage_Fire_Min_Main_Hand","Damage_Fire_Max_Main_Hand","Damage_Fire_Min_Off_Hand","Damage_Fire_Max_Off_Hand","Damage_Water_Min_Main_Hand","Damage_Water_Max_Main_Hand","Damage_Water_Min_Off_Hand","Damage_Water_Max_Off_Hand","Damage_Lightning_Min_Main_Hand","Damage_Lightning_Max_Main_Hand","Damage_Lightning_Min_Off_Hand","Damage_Lightning_Max_Off_Hand","Damage_Earth_Min_Main_Hand","Damage_Earth_Max_Main_Hand","Damage_Earth_Min_Off_Hand","Damage_Earth_Max_Off_Hand","Damage_Air_Min_Main_Hand","Damage_Air_Max_Main_Hand","Damage_Air_Min_Off_Hand","Damage_Air_Max_Off_Hand","Damage_Block_Mitigation","Damage_Crit_Intensification","Dot_Min_Damage","Dot_Max_Damage","Dot_Duration","Dot_Tick_Damage_Normal","Dot_Tick_Damage_Fire","Dot_Tick_Damage_Water","Dot_Tick_Damage_Lightning","Dot_Tick_Damage_Earth","Dot_Tick_Damage_Air","Leech_HP","Leech_MP","Hurt_Resist_Chance","Hurt_Target_Chance","Push_Resist_Chance","Push_Target_Chance","Stun_Resist_Chance","Stun_Target_Chance","Stun_Duration","Fear_Resist_Chance","Fear_Target_Chance","Fear_Duration","Slow_Resist_Chance","Slow_Target_Chance","Slow_Duration","Diminishing_Return","Stat_Points","Stat_Strength","Stat_Dexterity","Stat_Endurance","Stat_Energy","Prereq_Strength","Prereq_Dexterity","Prereq_Endurance","Prereq_Energy","Skill_Points","Spell_Rating_Attack","Spell_Rating_Attack_Bonus_Fire","Spell_Rating_Attack_Bonus_Water","Spell_Rating_Attack_Bonus_Lightning","Spell_Rating_Attack_Bonus_Earth","Spell_Rating_Attack_Bonus_Air","Spell_Rating_Dodge","Spell_Rating_Critical","Spell_Damage_Bonus_Fire","Spell_Damage_Bonus_Water","Spell_Damage_Bonus_Lightning","Spell_Damage_Bonus_Earth","Spell_Damage_Bonus_Air","Spell_Damage_Crit_Intensification","SnS_Level","SnS_ManaCost","SnS_MinDamage","SnS_MaxDamage","SnS_Duration","SnS_Cooldown","SnS_Dot_Elemental_Type","SnS_Dot_Min_Damage","SnS_Dot_Max_Damage","SnS_Dot_Duration","SnS_Hurt_Target_Chance","SnS_Push_Target_Chance","SnS_Stun_Target_Chance","SnS_Stun_Duration","SnS_Fear_Target_Chance","SnS_Fear_Duration","SnS_Slow_Target_Chance","SnS_Slow_Duration","TempProp1","TempProp2","Potion_HP_Modifier","Potion_MP_Modifier","Special_Loot_Max_Potions","Special_Loot_Gold_Multiplier","Special_Loot_Magical_Chance","Special_Transmute_Efficiency","Special_Sneak","Special_Sneak_Detection","Special_Bonus_To_XP","Special_Merchant_Price","Special_Equip_Main_In_Offhand","Special_Equip_2H_In_One","Threat_PerDamage","Threat_PerHeal","Threat_PerDamageToFriendly","Menu_Average_Melee_To_Hit","Menu_Average_Spell_To_Hit","Menu_Melee_Damage_Reduction","Visual_AlphaFadeInSpawn","Visual_AlphaFadeOutDespawn","Visual_BlinkWhenHit","Achievement_Transmute_Count","Achievement_Block_Count","Achievement_Evade_Count","Achievement_SkillUse_Count","Achievement_Break_Count","Achievement_Chests_Count","Achievement_Potions_Count","Achievement_Sell_Count","Achievement_Spent_Count","Achievement_KnockedDown_Count","Achievement_PickUp_Count"};
inline constexpr const char* process_members_StatAutoAssignTargetList_v121[]={"list_entries"};
inline constexpr const char* process_members_StatListList_v121[]={"list_entries"};
inline constexpr const char* process_members_CharTemplate_v121[]={"CharInfo"};
inline constexpr const char* process_members_Rect_v121[]={"Height","PosX","PosY","Width"};
inline constexpr const char* process_members_DesignSettings_v121[]={"AdditionalLeaderIconBonus","AggroTargetSwitchPct","AutoZoomRefPoint","AutoZoomStep","CombatMusicTrigger","CoopCamLimits_Bottom","CoopCamLimits_Sides","CoopCamLimits_Top","DefaultLevelWhenInvalid","DefaultOnlineMultiLevelHack","EnemyClickSensitivity","EnemySpottedAggro","ExtraMonsterDamageMultiplyerPerPlayer","ExtraMonsterHealthMultiplyerPerPlayer","LeaderIconBonus2Players","LeaderIconBonus3Players","LeaderIconBonus4Players","MiniMapZoomMaxLimit","MiniMapZoomMinLimit","NPCClickSensitivity","ObjectClickSensitivity","PlayerRotationSpeed","PlayerRunToWalkPercent","PlayerWalkToRunPercent","QuestMarker","QuestMarkerCompleted","QuestMarkerSecondary","QuestMarkerSecondaryCompleted","StateFeared","StatePoisonned","StateSlowed","StateStunned","TargetAttack","TargetMove","XP_LevelBonusScaleFactorPerLevelPct","XP_LevelMalusScaleFactorPerLevelPct","XP_LevelScaleFactorMaxPct","XP_LevelScaleFactorMinPct","XP_LevelScaleMaxLevelDiff","XP_MaxDistanceToReceiveXP","XP_PerPlayerReductionPct","ZoomMaxLimit","ZoomMinLimit"};
inline constexpr const char* process_members_GameDifficulty_v121[]={"DamageInputModifier","DamageOutputModifier","MaxPotionModifier","RespawnTimeModifier","XPRewardModifier"};
inline constexpr const char* process_members_GameOption_v121[]={"Default","Label","Max","Min","Step","Type","ValueStr"};
inline constexpr const char* process_members_DialogActor_v121[]={"FrameID","FrameLabel","Name"};
inline constexpr const char* process_members_DialogStepList_v121[]={"steps"};
inline constexpr const char* process_members_AnimFXTpl_v121[]={"ForceCache","LoopAFX","Steps","Type"};
inline constexpr const char* process_members_CharEffect_v121[]={"BloodDeathEffect","BloodEffect","FootprintEffect","SwooshEffect","TriggerFloorFX"};
inline constexpr const char* process_members_FootstepEffect_v121[]={"Effect","Floortype","RunSound","WalkSound"};
inline constexpr const char* process_members_FaeryList_v121[]={"List"};
inline constexpr const char* process_members_Faery_v121[]={"Description","Elemental","ModelFile","Name","SpellScript","SpellType","Type"};
inline constexpr const char* process_members_FontColorDef_v121[]={"glowcolor","textcolor"};
inline constexpr const char* process_members_DestructibleContainer_v121[]={"AirResist","DamageReduction","EarthResist","Effect","FireResist","HitPoints","InteractSound","KeepSolid","LightningResist","Loot","Script","TrapChance","TrapType","Visual","WaterResist"};
inline constexpr const char* process_members_Door_v121[]={"Script","SoundClose","SoundOpen","Visual"};
inline constexpr const char* process_members_ExplosiveTrap_v121[]={"Damage","Effect","Script","Visual"};
inline constexpr const char* process_members_GameObjectDamage_v121[]={"Damage","ImmunityDelay","Max","Min","Type"};
inline constexpr const char* process_members_LiftableObject_v121[]={"Visual"};
inline constexpr const char* process_members_OpenableContainer_v121[]={"Effect","InteractSound","KeepSolid","Loot","Script","TrapChance","TrapType","Visual"};
inline constexpr const char* process_members_ProjectileTrap_v121[]={"Damage","Delay","HurtChance","IsActive","Projectile","Script","Sound","Visual"};
inline constexpr const char* process_members_TimerTrap_v121[]={"Damage","Delay","HurtChance","Script","Sound","Visual"};
inline constexpr const char* process_members_TriggerObject_v121[]={"InteractionType","Script","Sound","Visual"};
inline constexpr const char* process_members_TriggerPlate_v121[]={"Behavior","IsActive","SoundOff","SoundOn","SpecificObject","TriggerType","Visual"};
inline constexpr const char* process_members_TriggerTrap_v121[]={"Damage","HurtChance","Script","Sound","Visual"};
inline constexpr const char* process_members_HelpPage_v121[]={"HelpStringCategory","HelpStringText"};
inline constexpr const char* process_members_HintPage_v121[]={"HintAvatarID","HintStringID"};
inline constexpr const char* process_members_ItemPowerEntryList_v121[]={"list_entries"};
inline constexpr const char* process_members_ItemPowerRef_v121[]={"Palette","SpecialEffect","AttrBonusList","Description","GoldBonusMultiplier","SortingOrder","GoldBonus","AttrMonopoly"};
inline constexpr const char* process_members_ItemBonusAttrList_v121[]={"list_entries"};
inline constexpr const char* process_members_FastTravelDestination_v121[]={"DescriptionId","EntryPointId","LevelName","LocationType","StringId"};
inline constexpr const char* process_members_LevelDeclaration_v121[]={"Dbg_IsStable","DynamicBusRouting","Hub","IsRandom","LevelDescription","LevelFile","LevelName","LevelState","MapName","MonsterLvlMax","MonsterLvlMaxHard","MonsterLvlMaxNightmare","MonsterLvlMin","MonsterLvlMinHard","MonsterLvlMinNightmare"};
inline constexpr const char* process_members_ItemAudioVisual_v121[]={"AudioDrop","AudioPickup","Visual"};
inline constexpr const char* process_members_TileOffsetList_v121[]={"list_entries"};
inline constexpr const char* process_members_Inventory_v121[]={"MaxItems"};
inline constexpr const char* process_members_ItemListEntryList_v121[]={"list_entries"};
inline constexpr const char* process_members_Item_v121[]={"Name","Material","ModularModule","AudioVisualID","Type","BaseStat","BaseProp","BaseElement","EquipmentSlottingType","Value","GoldValueMultiplier","LevelRequirement","StrengthRequirement","DexterityRequirement","EnduranceRequirement","EnergyRequirement","ClassRequirement","Param1","Param2","Param3","Param4","Param5","Param6","","","","","","","","","","","","","","",""};
inline constexpr const char* process_members_ItemTypeListList_v121[]={"list_entries"};
inline constexpr const char* process_members_Loot_v121[]={"RollType","NumRandomItemProbs","RandomLootEntries","FixedLootEntries","SubLoots"};
inline constexpr const char* process_members_Merchant_v121[]={"BuyMultiplier","SellMultiplier","MerchandiseList"};
inline constexpr const char* process_members_NumProbList_v121[]={"list_entries"};
inline constexpr const char* process_members_Projectile_v121[]={"Dedicated","DisappearOnHit","ExpireFX","ExpireSFX","FriendlyFire","ImpactFX","ImpactSFX","IsLaser","IsMagic","MaxDistance","MaxRotation","Model","ObjectImpactFX","ObjectImpactSFX","StayOnFloor","TargetLock","TimeBetweenHit","Timer","Velocity","VelocityDamping"};
inline constexpr const char* process_members_SkillList_v121[]={"List"};
inline constexpr const char* process_members_Skill_v121[]={"Anim","AnimIsMoving","DisplayProps","ElementalType","FairieDependantText","Flags","Level","Script","SkillAssignable","SkillCurrLevel","SkillDescription","SkillIcon","SkillName","SkillNextLevel","Type"};
inline constexpr const char* process_members_CharSounds_v121[]={"Die","Hurt","ImpactVsFlesh","ImpactVsMetal","IsFlesh","IsMetallic"};
inline constexpr const char* process_members_Listener_v121[]={"Anchor","MaxDistance","Orientation","RefDistance","RolloffFactor","UpVector"};
inline constexpr const char* process_members_SoundBank_v121[]={"MaxPlayback","MaxPlaybackBehavior"};
inline constexpr const char* process_members_SoundGroup_v121[]={"EnteringBus","Positioning3D"};
inline constexpr const char* process_members_Sound_v121[]={"Bank","Channel","FileName","Format","Group","LoadType","Priority","Repeat","Volume"};
inline constexpr const char* process_members_SpawnGroup_v121[]={"ActiveSpotOnly","Delay","Spawns"};
inline constexpr const char* process_members_Trophy_v121[]={"Desc","GLIndex","GLLive","Grade","Label","Name","Type"};
inline constexpr const char* process_members_v2CondAnd_v121[]={"Conds","Type"};
inline constexpr const char* process_members_v2Event_v121[]={"Level","Script","Triggers"};
inline constexpr const char* process_members_v2Quest_v121[]={"Name","Description","PostDescription","StaticObjective","PreReqs","Objectives","Rewards","RewardsHard","RewardsVeryHard","AcceptType","EndType","TargetLevel","Repeatable","State","Scripts","Priority","Act"};
inline constexpr const char* process_members_WldMapLocation_v121[]={"Name","State","LocationLevels"};
inline constexpr const char* process_members_WorldMapLocker_v121[]={"OnState","QuestID"};
inline constexpr const char* process_members_ColladaFile_v121[]={"Name"};
inline constexpr const char* process_members_LangSheetList_v121[]={"list"};
inline constexpr const char* process_members_SoundAutoGen_v121[]={"Id","Type"};
inline constexpr ProcessCompiledStructV121 process_compiled_structs_v121[]={
 {"AIProps",process_members_AIProps_v121,15,0x9a677c,0x4af590},
 {"AIFactions",process_members_AIFactions_v121,1,0x9a6914,0x4af4ec},
 {"AnimTpl",process_members_AnimTpl_v121,3,0x9a6a4c,0x4af420},
 {"CamAnimSet",process_members_CamAnimSet_v121,5,0x9a6a94,0x4af354},
 {"CharAnim",process_members_CharAnim_v121,37,0x9a6b0c,0x4af284},
 {"ClassFuncList",process_members_ClassFuncList_v121,1,0x9a6efc,0x4af1e0},
 {"CharacterProperties",process_members_CharacterProperties_v121,224,0x9a743c,0x4af110},
 {"StatAutoAssignTargetList",process_members_StatAutoAssignTargetList_v121,1,0x9eabb8,0x4af0e0},
 {"StatListList",process_members_StatListList_v121,1,0x9eabd0,0x4af03c},
 {"CharTemplate",process_members_CharTemplate_v121,1,0x9eac04,0x4aef98},
 {"Rect",process_members_Rect_v121,4,0x9eadb4,0x4aeecc},
 {"DesignSettings",process_members_DesignSettings_v121,43,0x9eae14,0x4aedfc},
 {"GameDifficulty",process_members_GameDifficulty_v121,5,0x9eb21c,0x4aed30},
 {"GameOption",process_members_GameOption_v121,7,0x9eb294,0x4aec64},
 {"DialogActor",process_members_DialogActor_v121,3,0x9eb5dc,0x4aeb98},
 {"DialogStepList",process_members_DialogStepList_v121,1,0x9eb6cc,0x4aeaf4},
 {"AnimFXTpl",process_members_AnimFXTpl_v121,4,0x9eb804,0x4aea28},
 {"CharEffect",process_members_CharEffect_v121,5,0x9eb864,0x4ae95c},
 {"FootstepEffect",process_members_FootstepEffect_v121,4,0x9eb8dc,0x4ae890},
 {"FaeryList",process_members_FaeryList_v121,1,0x9eb9e4,0x4ae7ec},
 {"Faery",process_members_Faery_v121,7,0x9eb93c,0x4ae720},
 {"FontColorDef",process_members_FontColorDef_v121,2,0x9eba14,0x4ae660},
 {"DestructibleContainer",process_members_DestructibleContainer_v121,15,0x9eba44,0x4ae594},
 {"Door",process_members_Door_v121,4,0x9ebbac,0x4ae53c},
 {"ExplosiveTrap",process_members_ExplosiveTrap_v121,4,0x9ebc0c,0x4ae470},
 {"GameObjectDamage",process_members_GameObjectDamage_v121,5,0x9ebc6c,0x4ae3a4},
 {"LiftableObject",process_members_LiftableObject_v121,1,0x9ebd90,0x4ae300},
 {"OpenableContainer",process_members_OpenableContainer_v121,8,0x9ebda8,0x4ae234},
 {"ProjectileTrap",process_members_ProjectileTrap_v121,8,0x9ebe68,0x4ae1dc},
 {"TimerTrap",process_members_TimerTrap_v121,6,0x9ebf28,0x4ae110},
 {"TriggerObject",process_members_TriggerObject_v121,4,0x9ebfb8,0x4ae044},
 {"TriggerPlate",process_members_TriggerPlate_v121,7,0x9ec018,0x4adf78},
 {"TriggerTrap",process_members_TriggerTrap_v121,5,0x9ec0c0,0x4adeac},
 {"HelpPage",process_members_HelpPage_v121,2,0x9ec138,0x4addec},
 {"HintPage",process_members_HintPage_v121,2,0x9ec168,0x4add2c},
 {"ItemPowerEntryList",process_members_ItemPowerEntryList_v121,1,0x9ec1c8,0x4adc88},
 {"ItemPowerRef",process_members_ItemPowerRef_v121,8,0x9ec228,0x4adbbc},
 {"ItemBonusAttrList",process_members_ItemBonusAttrList_v121,1,0x9eec40,0x4adb18},
 {"FastTravelDestination",process_members_FastTravelDestination_v121,5,0x9eec58,0x4ada4c},
 {"LevelDeclaration",process_members_LevelDeclaration_v121,15,0x9eecd0,0x4ad980},
 {"ItemAudioVisual",process_members_ItemAudioVisual_v121,3,0x9eee38,0x4ad928},
 {"TileOffsetList",process_members_TileOffsetList_v121,1,0x9f2318,0x4ad884},
 {"Inventory",process_members_Inventory_v121,1,0x9f23a8,0x4ad7e0},
 {"ItemListEntryList",process_members_ItemListEntryList_v121,1,0x9f2288,0x4ad73c},
 {"Item",process_members_Item_v121,38,0x9eefe8,0x4ad66c},
 {"ItemTypeListList",process_members_ItemTypeListList_v121,1,0x9f23c0,0x4ad5c8},
 {"Loot",process_members_Loot_v121,5,0x9f21c8,0x4ad4fc},
 {"Merchant",process_members_Merchant_v121,3,0x9f2360,0x4ad430},
 {"NumProbList",process_members_NumProbList_v121,1,0x9f22d0,0x4ad38c},
 {"Projectile",process_members_Projectile_v121,20,0x9f23d8,0x4ad2c0},
 {"SkillList",process_members_SkillList_v121,1,0x9f3ce4,0x4ad21c},
 {"Skill",process_members_Skill_v121,15,0x9f3cfc,0x4ad150},
 {"CharSounds",process_members_CharSounds_v121,6,0x9f4020,0x4ad084},
 {"Listener",process_members_Listener_v121,6,0x9f40f8,0x4acfb8},
 {"SoundBank",process_members_SoundBank_v121,2,0x9f3e94,0x4acef8},
 {"SoundGroup",process_members_SoundGroup_v121,2,0x9f3e64,0x4ace38},
 {"Sound",process_members_Sound_v121,9,0x9f3ec4,0x4acd6c},
 {"SpawnGroup",process_members_SpawnGroup_v121,3,0x9f41d0,0x4acca0},
 {"Trophy",process_members_Trophy_v121,7,0x9f4218,0x4acc48},
 {"v2CondAnd",process_members_v2CondAnd_v121,2,0x9f4d4c,0x4acbfc},
 {"v2Event",process_members_v2Event_v121,3,0x9f51d0,0x4acb30},
 {"v2Quest",process_members_v2Quest_v121,17,0x9f5038,0x4aca64},
 {"WldMapLocation",process_members_WldMapLocation_v121,3,0x9f60f4,0x4ac998},
 {"WorldMapLocker",process_members_WorldMapLocker_v121,2,0x9f613c,0x4ac8d8},
 {"ColladaFile",process_members_ColladaFile_v121,1,0x9eac1c,0x4ac834},
 {"LangSheetList",process_members_LangSheetList_v121,1,0x9f619c,0x4ac634},
 {"SoundAutoGen",process_members_SoundAutoGen_v121,2,0x9f3f9c,0x4ac574},
};
}
