#pragma once

// Arrays::X::read(IStreamBase*) for slice arrays_27 (27 tables, see NOTES.md "Arrays element ABI").
// Layout: one byte block per table, stride from the listing (ARM), count word at members - 4, block base at
// members - 8. Elements are pydata::ArrayElement records; the read slot is vtable +0xc.
//
// Two shapes, following the existing headers:
//  - 21 tables (not declared in w7_arrays_00.hpp): struct Arrays::X with the static storage `members` and
//    `size` (defined in w7_arrays27.cpp) and a static read. The finalize step calls the free function that
//    w7_arrays_01.hpp declares for the same table (FinalizeX).
//  - 6 tables declared in w7_arrays_00.hpp (ProjectileTable, MerchantTable, ItemTypeList, ItemAudioVisualTable,
//    LevelList, FastTravelList): that header has no read slot and is not edited here, so the read is a free
//    function ReadX(IStreamBase*) in namespace Arrays. Its member wiring is pending.
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"

namespace Arrays {

// Free functions for the tables declared in w7_arrays_00.hpp (see the header note above).
void ReadProjectileTable(IStreamBase* stream);
void ReadMerchantTable(IStreamBase* stream);
void ReadItemTypeList(IStreamBase* stream);
void ReadItemAudioVisualTable(IStreamBase* stream);
void ReadLevelList(IStreamBase* stream);
void ReadFastTravelList(IStreamBase* stream);

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

}  // namespace Arrays
