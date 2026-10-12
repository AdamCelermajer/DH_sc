#pragma once

// Arrays::X finalize() tables, slice arrays_00 (see NOTES.md, "Arrays element ABI"). Each table has the
// static `members` (first element; a count word at members - 4, and the block base at members - 8) and
// `size` (the element count of the forward loop, compared unsigned in the original). Elements are
// pydata::ArrayElement records. The stride of each table is per class and is set in the .cpp.
#include <cstddef>
#include <cstdint>

namespace Arrays {

struct Sounds {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct StrID_Languages {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct GameObjectDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct ProjectileDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct EffectDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct AnimDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct ModelDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct WorldMapLockers {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct WorldMap {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct v2Quests {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct v2Events {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct v2Conditions {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct TrophyTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct SpawnGroups {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct Sounds_bak {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct SoundGroupsRouting {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct SoundBankPlayback {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct Listeners {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct CharSoundsTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct SkillTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct SkillListTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct ProjectileTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct NumProbArray {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct MerchantTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct LootTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct ItemTypeList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct ItemList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct InventoryTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct DropTilePriorityTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct ItemAudioVisualTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct LevelList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct FastTravelList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct ItemBonusAttrMonopoly {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};

}  // namespace Arrays
