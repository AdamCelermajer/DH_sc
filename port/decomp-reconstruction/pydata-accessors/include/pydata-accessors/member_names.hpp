#pragma once

// Name tables behind the Arrays/Structs accessors (PyData loader fills them).
// Arrays::X tables are m_memberNames/size. Structs::X tables are m_dataNames with a count.
#include <cstdint>
#include "pydata-accessors/name_table.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_structs_1.hpp"
#include "pydata-accessors/w7_structs_2.hpp"
#include "pydata-accessors/w7_structs_3.hpp"
#include "pydata-accessors/w7_structs_5.hpp"



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
// The Structs name tables are members of the record classes themselves (m_dataNames and kCount); the record
// headers are included above.
template <class T>
int GetMemberIDByString(const char* name);

}  // namespace Structs
