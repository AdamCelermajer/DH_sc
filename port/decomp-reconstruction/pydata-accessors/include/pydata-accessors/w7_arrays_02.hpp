#pragma once

// Arrays::X finalize() and read(IStreamBase*) for slice arrays_02 (see NOTES.md, "Arrays element ABI").
// Each table has the static `members` (first element; count word at members - 4, block base at members - 8)
// and `size`. Storage (members/size) is defined by the slice that owns the table's finalize().
// Element classes are Structs::X; their vtables are named in the .cpp by ElementVtable<Structs::X>.
#include <cstddef>
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"

namespace pydata {
// Vtable address point of the Structs element class Elem. Defined in the .cpp for the classes that
// structs_records.hpp defines; for the others it is declared only (see the partial rows of the ledger).
template <class Elem>
const void* ElementVtable();
}  // namespace pydata

namespace Structs {
class AIFactions;
class CharSounds;
class ColladaFile;
class LangSheetList;
class Sound;
class SoundGroup;
class SpawnGroup;
class StatAutoAssignTargetList;
class StatListList;
class v2CondAnd;
class v2Event;
}  // namespace Structs

namespace Arrays {

struct CharAnimTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct CamAnimSetTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
};
struct AnimTable {
    static std::uint8_t* members;
    static std::uint32_t size;
    static void finalize();
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
