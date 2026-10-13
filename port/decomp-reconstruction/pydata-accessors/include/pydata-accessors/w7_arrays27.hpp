#pragma once

// Arrays::X::read(IStreamBase*) for slice arrays_27 and the other tables whose storage is declared elsewhere
// (see NOTES.md, "Element tables"). Layout: one byte block per table, count word at members - 4, the original
// 32-bit stride word at members - 8 (block base). The elements are host Structs::X records at host stride,
// constructed in place and reached through a typed pointer (element_table.hpp); the listing stride of each table
// is Arrays::ListingStride::X (array_strides.hpp). The read slot of an element is vtable +0xc.
//
// This header declares the 28 tables that no other header declares (struct Arrays::X with the static storage
// `members` and `size`, defined in w7_arrays27.cpp, and a static read). The finalize step of these tables calls
// the free function that w7_arrays_01.hpp declares for the same table (FinalizeX).
// The reads of the 11 tables that w7_arrays_00.hpp declares (ProjectileTable, MerchantTable, ItemTypeList,
// ItemAudioVisualTable, LevelList, FastTravelList, LootTable, ItemList, DropTilePriorityTable, ModelDict,
// v2Quests) are static members declared there and defined in w7_arrays27.cpp as well.
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"

namespace Arrays {

struct ItemPowerTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct HintPages {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct HelpPages {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct TriggerTraps {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct TriggerPlates {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct TriggerObjects {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct TimerTraps {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct ProjectileTraps {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct OpenableContainers {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct LiftableObjects {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct GameObjectDamager {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct ExplosiveTraps {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct Doors {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct DestructibleContainers {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct FontPalette {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct FootstepEffectTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct CharEffectTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct AnimatedEffectTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct Dialogs {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct DialogActors {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct GameOptionTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct ItemPowerList {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct FaeryTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct FaeryListTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct Charater_Templates {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct CharacterTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct ClassTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};
struct ItemTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void read(IStreamBase* stream);
};

}  // namespace Arrays
