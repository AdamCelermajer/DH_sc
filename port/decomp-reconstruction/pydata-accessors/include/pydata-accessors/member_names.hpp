#pragma once

// Name tables behind the Arrays/Structs accessors (PyData loader fills them).
// Arrays::X tables are m_memberNames/size. Structs::X tables are m_dataNames with a count.
#include <cstdint>
#include "pydata-accessors/deps/stream_reader.hpp"

namespace pydata {
struct NameTable {
    char** names = nullptr;  // owned array of owned C strings, ordered, duplicates allowed
    int size = 0;
};

// Index of the first of the first `count` names equal to `name`, or -1.
int FindMemberName(const NameTable& table, int count, const char* name);
// Original finalizeNames(): frees each name, then the array, and clears it.
void FinalizeNameTable(NameTable& table);
// Original readNames()/skipNames(): count, check against size, then length-prefixed strings.
void ReadNameTable(NameTable& table, IStreamBase* stream);
}  // namespace pydata

namespace Arrays {
struct AIFactionTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct AITable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct AnimDict {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct AnimTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct AnimatedEffectTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct CamAnimSetTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct CharAnimTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct CharEffectTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct CharSoundsTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct CharacterTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct Charater_Templates {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct ClassTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct DesignSettingsTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct DestructibleContainers {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct DialogActors {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct Dialogs {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct Doors {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct DropTilePriorityTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct EffectDict {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ExplosiveTraps {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct FaeryListTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct FaeryTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct FastTravelList {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct FontPalette {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct FootstepEffectTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct GameDifficulties {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct GameObjectDamager {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct GameObjectDict {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct GameOptionTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct HelpPages {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct HintPages {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct InventoryTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ItemAudioVisualTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ItemBonusAttrMonopoly {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ItemList {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ItemPowerList {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ItemPowerTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct ItemTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct ItemTypeList {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct LevelList {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct LiftableObjects {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct Listeners {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct LootTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct MerchantTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ModelDict {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct NumProbArray {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct OpenableContainers {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct ProjectileDict {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ProjectileTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct ProjectileTraps {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct RectTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct SkillListTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct SkillTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct SoundBankPlayback {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct SoundGroupsRouting {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct Sounds {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct Sounds_bak {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct SpawnGroups {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct StatAutoAssignSchemeTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct StatListTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct StrID_Languages {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct TimerTraps {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct TriggerObjects {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct TriggerPlates {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct TriggerTraps {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct TrophyTable {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct WorldMap {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct WorldMapLockers {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
    static void skipNames(IStreamBase* stream);
};
struct v2Conditions {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct v2Events {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};
struct v2Quests {
    static pydata::NameTable m_memberNames;
    static void finalizeNames();
    static void readNames(IStreamBase* stream);
};

template <class T>
int GetMemberIDByString(const char* name);

}  // namespace Arrays

namespace Structs {
struct SoundAutoGen { static constexpr int kCount = 2; static pydata::NameTable m_dataNames; };
struct LangSheetList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct ColladaFile { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct WorldMapLocker { static constexpr int kCount = 2; static pydata::NameTable m_dataNames; };
struct WldMapLocation { static constexpr int kCount = 3; static pydata::NameTable m_dataNames; };
struct v2Quest { static constexpr int kCount = 17; static pydata::NameTable m_dataNames; };
struct v2Event { static constexpr int kCount = 3; static pydata::NameTable m_dataNames; };
struct v2CondAnd { static constexpr int kCount = 2; static pydata::NameTable m_dataNames; };
struct Trophy { static constexpr int kCount = 7; static pydata::NameTable m_dataNames; };
struct SpawnGroup { static constexpr int kCount = 3; static pydata::NameTable m_dataNames; };
struct Sound { static constexpr int kCount = 9; static pydata::NameTable m_dataNames; };
struct SoundGroup { static constexpr int kCount = 2; static pydata::NameTable m_dataNames; };
struct SoundBank { static constexpr int kCount = 2; static pydata::NameTable m_dataNames; };
struct Listener { static constexpr int kCount = 6; static pydata::NameTable m_dataNames; };
struct CharSounds { static constexpr int kCount = 6; static pydata::NameTable m_dataNames; };
struct Skill { static constexpr int kCount = 15; static pydata::NameTable m_dataNames; };
struct SkillList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct Projectile { static constexpr int kCount = 20; static pydata::NameTable m_dataNames; };
struct NumProbList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct Merchant { static constexpr int kCount = 3; static pydata::NameTable m_dataNames; };
struct Loot { static constexpr int kCount = 5; static pydata::NameTable m_dataNames; };
struct ItemTypeListList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct Item { static constexpr int kCount = 38; static pydata::NameTable m_dataNames; };
struct ItemListEntryList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct Inventory { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct TileOffsetList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct ItemAudioVisual { static constexpr int kCount = 3; static pydata::NameTable m_dataNames; };
struct LevelDeclaration { static constexpr int kCount = 15; static pydata::NameTable m_dataNames; };
struct FastTravelDestination { static constexpr int kCount = 5; static pydata::NameTable m_dataNames; };
struct ItemBonusAttrList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct ItemPowerRef { static constexpr int kCount = 8; static pydata::NameTable m_dataNames; };
struct ItemPowerEntryList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct HintPage { static constexpr int kCount = 2; static pydata::NameTable m_dataNames; };
struct HelpPage { static constexpr int kCount = 2; static pydata::NameTable m_dataNames; };
struct TriggerTrap { static constexpr int kCount = 5; static pydata::NameTable m_dataNames; };
struct TriggerPlate { static constexpr int kCount = 7; static pydata::NameTable m_dataNames; };
struct TriggerObject { static constexpr int kCount = 4; static pydata::NameTable m_dataNames; };
struct TimerTrap { static constexpr int kCount = 6; static pydata::NameTable m_dataNames; };
struct ProjectileTrap { static constexpr int kCount = 8; static pydata::NameTable m_dataNames; };
struct OpenableContainer { static constexpr int kCount = 8; static pydata::NameTable m_dataNames; };
struct LiftableObject { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct GameObjectDamage { static constexpr int kCount = 5; static pydata::NameTable m_dataNames; };
struct ExplosiveTrap { static constexpr int kCount = 4; static pydata::NameTable m_dataNames; };
struct Door { static constexpr int kCount = 4; static pydata::NameTable m_dataNames; };
struct DestructibleContainer { static constexpr int kCount = 15; static pydata::NameTable m_dataNames; };
struct FontColorDef { static constexpr int kCount = 2; static pydata::NameTable m_dataNames; };
struct Faery { static constexpr int kCount = 7; static pydata::NameTable m_dataNames; };
struct FaeryList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct FootstepEffect { static constexpr int kCount = 4; static pydata::NameTable m_dataNames; };
struct CharEffect { static constexpr int kCount = 5; static pydata::NameTable m_dataNames; };
struct AnimFXTpl { static constexpr int kCount = 4; static pydata::NameTable m_dataNames; };
struct DialogStepList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct DialogActor { static constexpr int kCount = 3; static pydata::NameTable m_dataNames; };
struct GameOption { static constexpr int kCount = 7; static pydata::NameTable m_dataNames; };
struct GameDifficulty { static constexpr int kCount = 5; static pydata::NameTable m_dataNames; };
struct DesignSettings { static constexpr int kCount = 43; static pydata::NameTable m_dataNames; };
struct Rect { static constexpr int kCount = 4; static pydata::NameTable m_dataNames; };
struct CharTemplate { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct StatListList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct StatAutoAssignTargetList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct CharacterProperties { static constexpr int kCount = 224; static pydata::NameTable m_dataNames; };
struct ClassFuncList { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct CharAnim { static constexpr int kCount = 37; static pydata::NameTable m_dataNames; };
struct CamAnimSet { static constexpr int kCount = 5; static pydata::NameTable m_dataNames; };
struct AnimTpl { static constexpr int kCount = 3; static pydata::NameTable m_dataNames; };
struct AIFactions { static constexpr int kCount = 1; static pydata::NameTable m_dataNames; };
struct AIProps { static constexpr int kCount = 15; static pydata::NameTable m_dataNames; };

template <class T>
int GetMemberIDByString(const char* name);

}  // namespace Structs
