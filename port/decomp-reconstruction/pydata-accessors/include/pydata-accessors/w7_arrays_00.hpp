#pragma once

// Arrays::X tables, slice arrays_00 (see NOTES.md, "Element tables"). Each table has the static `members`
// (first element; the block header is the original 32-bit stride word at members - 8, which is also the block
// base, and the count word at members - 4) and `size` (the element count of the forward loop, compared unsigned
// in the original). Elements
// are host Structs::X records (virtual finalize/read, see element_table.hpp) reached through a typed pointer; the
// original 32-bit stride of each table is Arrays::ListingStride::X (array_strides.hpp). `read` is defined in
// w7_arrays27.cpp (the Hex-Rays bodies of the read family are one shape, pydata::ReadArrayTable).
#include <cstddef>
#include <cstdint>

class IStreamBase;  // the stream type of the element read slots (deps/stream_reader.hpp)

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
    static void read(IStreamBase* stream);  // 0x4b86f0
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
    static void read(IStreamBase* stream);  // 0x4b8acc
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
    static void read(IStreamBase* stream);  // 0x4b9810
};
struct SkillListTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4b9964
};
struct ProjectileTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4b9ab0
};
struct NumProbArray {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4b9bf4
};
struct MerchantTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4b9d40
};
struct LootTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4b9e8c
};
struct ItemTypeList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4b9fe0
};
struct ItemList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4ba27c
};
struct InventoryTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4ba3c8
};
struct DropTilePriorityTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4ba4fc
};
struct ItemAudioVisualTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4ba648
};
struct LevelList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4ba794
};
struct FastTravelList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4ba8e4
};
struct ItemBonusAttrMonopoly {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);  // 0x4baa30
};

}  // namespace Arrays
