// Arrays::X::finalize() and read(IStreamBase*) for slice arrays_02 (27 tables, 29 functions).
// Shared bodies:
//   FinalizeArrayTable: finalize slot +0x8 on each element (forward), then the element slot +0x0 (D1) from the
//     last element back to the first, then the block at members - 8 is freed; members is cleared.
//   ReadArrayTable: count (readAs<uint32>), Table::finalize(), size = count, one block of stride*count + 8 bytes
//     with the header words (stride, count), members = block + 8, each element gets its vptr and the listed
//     zeroed words, then each element's read slot +0xc runs in order.
// The element stride is the listing's stride for each table, as in w7_arrays_00.cpp.
// Element layout note: a vptr is stored as the host pointer of the Structs class, and the zeroed words use the
// listing's offsets. The port is not binary-compatible with the original (see NOTES.md).

#include <cstring>
#include <initializer_list>

#include "pydata-accessors/array_element.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_arrays_02.hpp"

namespace {

// The vptr of Elem: the first word of a polymorphic object (Itanium ABI).
template <class Elem>
const void* VtableOfTemporary() {
    Elem tmp;
    const void* vptr = nullptr;
    std::memcpy(&vptr, &tmp, sizeof vptr);
    return vptr;
}

inline pydata::ArrayElement* ElementAt(std::uint8_t* p) {
    return reinterpret_cast<pydata::ArrayElement*>(p);
}

template <class Table, std::size_t Stride>
void FinalizeArrayTable() {
    if (Table::members == nullptr) return;
    if (Table::size != 0) {
        for (std::uint32_t i = 0; i < Table::size; ++i) {
            ElementAt(Table::members + Stride * i)->finalize();
        }
    }
    if (Table::members != nullptr) {
        std::uint32_t count = 0;
        std::memcpy(&count, Table::members - 4, sizeof count);
        std::uint8_t* first = Table::members;
        std::uint8_t* last = first + Stride * count;
        while (last != first) {
            std::uint8_t* cur = last - Stride;
            ElementAt(cur)->~ArrayElement();
            last = cur;
        }
        CustomFree(first - 8);
    }
    Table::members = nullptr;
}

template <class Table, std::size_t Stride, class Elem>
void ReadArrayTable(IStreamBase* stream, std::initializer_list<std::size_t> zeroed) {
    const std::uint32_t count = StreamReader::readAs<std::uint32_t>(stream);
    Table::finalize();
    Table::size = count;
    std::uint8_t* block = static_cast<std::uint8_t*>(MemoryHintState(Stride * count + 8, 1));
    const std::uint32_t header_stride = Stride;
    std::memcpy(block, &header_stride, sizeof header_stride);
    std::memcpy(block + 4, &count, sizeof count);
    Table::members = block + 8;
    const void* vptr = pydata::ElementVtable<Elem>();
    for (std::uint32_t i = 0; i < count; ++i) {
        std::uint8_t* element = Table::members + Stride * i;
        std::memcpy(element, &vptr, sizeof vptr);
        for (std::size_t offset : zeroed) {
            const std::uint32_t zero = 0;
            std::memcpy(element + offset, &zero, sizeof zero);
        }
    }
    for (std::uint32_t i = 0; i < Table::size; ++i) {
        ElementAt(Table::members + Stride * i)->read(stream);
    }
}

}  // namespace

namespace pydata {

template <>
const void* ElementVtable<Structs::GameDifficulty>() { return VtableOfTemporary<Structs::GameDifficulty>(); }

template <>
const void* ElementVtable<Structs::AIProps>() { return VtableOfTemporary<Structs::AIProps>(); }

template <>
const void* ElementVtable<Structs::DesignSettings>() { return VtableOfTemporary<Structs::DesignSettings>(); }

template <>
const void* ElementVtable<Structs::Rect>() { return VtableOfTemporary<Structs::Rect>(); }

template <>
const void* ElementVtable<Structs::SoundAutoGen>() { return VtableOfTemporary<Structs::SoundAutoGen>(); }

template <>
const void* ElementVtable<Structs::WorldMapLocker>() { return VtableOfTemporary<Structs::WorldMapLocker>(); }

template <>
const void* ElementVtable<Structs::WldMapLocation>() { return VtableOfTemporary<Structs::WldMapLocation>(); }

template <>
const void* ElementVtable<Structs::Trophy>() { return VtableOfTemporary<Structs::Trophy>(); }

template <>
const void* ElementVtable<Structs::SoundBank>() { return VtableOfTemporary<Structs::SoundBank>(); }

template <>
const void* ElementVtable<Structs::Listener>() { return VtableOfTemporary<Structs::Listener>(); }

}  // namespace pydata

namespace Arrays {

std::uint8_t* CharAnimTable::members = nullptr;
std::uint32_t CharAnimTable::size = 0;
std::uint8_t* CamAnimSetTable::members = nullptr;
std::uint32_t CamAnimSetTable::size = 0;
std::uint8_t* AnimTable::members = nullptr;
std::uint32_t AnimTable::size = 0;
std::uint8_t* AIFactionTable::members = nullptr;
std::uint32_t AIFactionTable::size = 0;
std::uint8_t* AITable::members = nullptr;
std::uint32_t AITable::size = 0;

// 0x4a9b38 _ZN6Arrays13CharAnimTable8finalizeEv
void CharAnimTable::finalize() { FinalizeArrayTable<CharAnimTable, 160>(); }

// 0x4a9cb8 _ZN6Arrays15CamAnimSetTable8finalizeEv
void CamAnimSetTable::finalize() { FinalizeArrayTable<CamAnimSetTable, 28>(); }

// 0x4a9e38 _ZN6Arrays9AnimTable8finalizeEv
void AnimTable::finalize() { FinalizeArrayTable<AnimTable, 20>(); }

// 0x4a9fb8 _ZN6Arrays14AIFactionTable8finalizeEv
void AIFactionTable::finalize() { FinalizeArrayTable<AIFactionTable, 12>(); }

// 0x4aa138 _ZN6Arrays7AITable8finalizeEv
void AITable::finalize() { FinalizeArrayTable<AITable, 68>(); }

// 0x4af5e8 _ZN6Arrays16GameDifficulties4readEP11IStreamBase
void GameDifficulties::read(IStreamBase* stream) { ReadArrayTable<GameDifficulties, 24, Structs::GameDifficulty>(stream, {}); }

// 0x4b3a38 _ZN6Arrays14AIFactionTable4readEP11IStreamBase
// partial: element class Structs::AIFactions is not in structs_records.hpp; ElementVtable<Structs::AIFactions> is declared only.
void AIFactionTable::read(IStreamBase* stream) { ReadArrayTable<AIFactionTable, 12, Structs::AIFactions>(stream, {8}); }

// 0x4b3b84 _ZN6Arrays7AITable4readEP11IStreamBase
void AITable::read(IStreamBase* stream) { ReadArrayTable<AITable, 68, Structs::AIProps>(stream, {44}); }

// 0x4b3cd0 _ZN6Arrays19DesignSettingsTable4readEP11IStreamBase
void DesignSettingsTable::read(IStreamBase* stream) { ReadArrayTable<DesignSettingsTable, 176, Structs::DesignSettings>(stream, {}); }

// 0x4b3e18 _ZN6Arrays9RectTable4readEP11IStreamBase
void RectTable::read(IStreamBase* stream) { ReadArrayTable<RectTable, 20, Structs::Rect>(stream, {}); }

// 0x4b40a8 _ZN6Arrays13StatListTable4readEP11IStreamBase
// partial: element class Structs::StatListList is not in structs_records.hpp; ElementVtable<Structs::StatListList> is declared only.
void StatListTable::read(IStreamBase* stream) { ReadArrayTable<StatListTable, 12, Structs::StatListList>(stream, {8}); }

// 0x4b41f4 _ZN6Arrays25StatAutoAssignSchemeTable4readEP11IStreamBase
// partial: element class Structs::StatAutoAssignTargetList is not in structs_records.hpp; ElementVtable<Structs::StatAutoAssignTargetList> is declared only.
void StatAutoAssignSchemeTable::read(IStreamBase* stream) { ReadArrayTable<StatAutoAssignSchemeTable, 12, Structs::StatAutoAssignTargetList>(stream, {8}); }

// 0x4b53f8 _ZN6Arrays15StrID_Languages4readEP11IStreamBase
// partial: element class Structs::LangSheetList is not in structs_records.hpp; ElementVtable<Structs::LangSheetList> is declared only.
void StrID_Languages::read(IStreamBase* stream) { ReadArrayTable<StrID_Languages, 12, Structs::LangSheetList>(stream, {8}); }

// 0x4b5544 _ZN6Arrays14GameObjectDict4readEP11IStreamBase
// partial: element class Structs::ColladaFile is not in structs_records.hpp; ElementVtable<Structs::ColladaFile> is declared only.
void GameObjectDict::read(IStreamBase* stream) { ReadArrayTable<GameObjectDict, 12, Structs::ColladaFile>(stream, {8}); }

// 0x4b5690 _ZN6Arrays6Sounds4readEP11IStreamBase
void Sounds::read(IStreamBase* stream) { ReadArrayTable<Sounds, 12, Structs::SoundAutoGen>(stream, {}); }

// 0x4b830c _ZN6Arrays14ProjectileDict4readEP11IStreamBase
// partial: element class Structs::ColladaFile is not in structs_records.hpp; ElementVtable<Structs::ColladaFile> is declared only.
void ProjectileDict::read(IStreamBase* stream) { ReadArrayTable<ProjectileDict, 12, Structs::ColladaFile>(stream, {8}); }

// 0x4b8458 _ZN6Arrays10EffectDict4readEP11IStreamBase
// partial: element class Structs::ColladaFile is not in structs_records.hpp; ElementVtable<Structs::ColladaFile> is declared only.
void EffectDict::read(IStreamBase* stream) { ReadArrayTable<EffectDict, 12, Structs::ColladaFile>(stream, {8}); }

// 0x4b85a4 _ZN6Arrays8AnimDict4readEP11IStreamBase
// partial: element class Structs::ColladaFile is not in structs_records.hpp; ElementVtable<Structs::ColladaFile> is declared only.
void AnimDict::read(IStreamBase* stream) { ReadArrayTable<AnimDict, 12, Structs::ColladaFile>(stream, {8}); }

// 0x4b883c _ZN6Arrays15WorldMapLockers4readEP11IStreamBase
void WorldMapLockers::read(IStreamBase* stream) { ReadArrayTable<WorldMapLockers, 12, Structs::WorldMapLocker>(stream, {}); }

// 0x4b8980 _ZN6Arrays8WorldMap4readEP11IStreamBase
void WorldMap::read(IStreamBase* stream) { ReadArrayTable<WorldMap, 20, Structs::WldMapLocation>(stream, {16}); }

// 0x4b8c9c _ZN6Arrays8v2Events4readEP11IStreamBase
// partial: element class Structs::v2Event is not in structs_records.hpp; ElementVtable<Structs::v2Event> is declared only.
void v2Events::read(IStreamBase* stream) { ReadArrayTable<v2Events, 24, Structs::v2Event>(stream, {12, 20}); }

// 0x4b8dec _ZN6Arrays12v2Conditions4readEP11IStreamBase
// partial: element class Structs::v2CondAnd is not in structs_records.hpp; ElementVtable<Structs::v2CondAnd> is declared only.
void v2Conditions::read(IStreamBase* stream) { ReadArrayTable<v2Conditions, 16, Structs::v2CondAnd>(stream, {8}); }

// 0x4b8f28 _ZN6Arrays11TrophyTable4readEP11IStreamBase
void TrophyTable::read(IStreamBase* stream) { ReadArrayTable<TrophyTable, 32, Structs::Trophy>(stream, {}); }

// 0x4b9060 _ZN6Arrays11SpawnGroups4readEP11IStreamBase
// partial: element class Structs::SpawnGroup is not in structs_records.hpp; ElementVtable<Structs::SpawnGroup> is declared only.
void SpawnGroups::read(IStreamBase* stream) { ReadArrayTable<SpawnGroups, 20, Structs::SpawnGroup>(stream, {16}); }

// 0x4b91ac _ZN6Arrays10Sounds_bak4readEP11IStreamBase
// partial: element class Structs::Sound is not in structs_records.hpp; ElementVtable<Structs::Sound> is declared only.
void Sounds_bak::read(IStreamBase* stream) { ReadArrayTable<Sounds_bak, 44, Structs::Sound>(stream, {16}); }

// 0x4b92f8 _ZN6Arrays18SoundGroupsRouting4readEP11IStreamBase
// partial: element class Structs::SoundGroup is not in structs_records.hpp; ElementVtable<Structs::SoundGroup> is declared only.
void SoundGroupsRouting::read(IStreamBase* stream) { ReadArrayTable<SoundGroupsRouting, 16, Structs::SoundGroup>(stream, {8}); }

// 0x4b9434 _ZN6Arrays17SoundBankPlayback4readEP11IStreamBase
void SoundBankPlayback::read(IStreamBase* stream) { ReadArrayTable<SoundBankPlayback, 12, Structs::SoundBank>(stream, {}); }

// 0x4b9578 _ZN6Arrays9Listeners4readEP11IStreamBase
void Listeners::read(IStreamBase* stream) { ReadArrayTable<Listeners, 28, Structs::Listener>(stream, {}); }

// 0x4b96bc _ZN6Arrays15CharSoundsTable4readEP11IStreamBase
// partial: element class Structs::CharSounds is not in structs_records.hpp; ElementVtable<Structs::CharSounds> is declared only.
void CharSoundsTable::read(IStreamBase* stream) { ReadArrayTable<CharSoundsTable, 40, Structs::CharSounds>(stream, {8, 16, 24, 32}); }

}  // namespace Arrays
