#pragma once

// Arrays::X finalize() and read(IStreamBase*) for slice arrays_02 (see NOTES.md, "Element tables").
// Each table has the static `members` (first element; the block header is the original 32-bit stride word at
// members - 8, which is also the block base, and the count word at members - 4) and `size`. The storage is defined
// in w7_arrays_02.cpp for the tables declared here that are not in w7_arrays_00.hpp (CharAnimTable, CamAnimSetTable,
// AnimTable, AIFactionTable, AITable, GameDifficulties, DesignSettingsTable, RectTable, StatListTable,
// StatAutoAssignSchemeTable); the other tables share the storage of w7_arrays_00.cpp.
// Elements are host Structs::X records (element_table.hpp); the original 32-bit stride of each table is
// Arrays::ListingStride::X (array_strides.hpp).
#include <cstddef>
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"

namespace Arrays {

struct CharAnimTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct CamAnimSetTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct AnimTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct AIFactionTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct AITable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct GameDifficulties {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct DesignSettingsTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct RectTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct StatListTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct StatAutoAssignSchemeTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct StrID_Languages {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct GameObjectDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct Sounds {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct ProjectileDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct EffectDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct AnimDict {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct WorldMapLockers {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct WorldMap {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct v2Events {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct v2Conditions {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct TrophyTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct SpawnGroups {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct Sounds_bak {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct SoundGroupsRouting {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct SoundBankPlayback {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct Listeners {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};
struct CharSoundsTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
    static void read(IStreamBase* stream);
};

}  // namespace Arrays
