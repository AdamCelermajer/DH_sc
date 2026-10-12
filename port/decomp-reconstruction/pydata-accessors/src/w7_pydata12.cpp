// PyDataArrays and PyDataConstants, slice pydata12 (wave 7). Declarations: include/pydata-accessors/w7_pydata12.hpp.
// Each function lists its original address and mangled symbol (queue.csv). Status per row is in ledger_pydata12.csv.

#include "pydata-accessors/w7_pydata12.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

// Load() cases 0..69, in order. Case k opens "pydata/<name>", calls reloadData(stream, "<name>") when the
// open succeeds, and releases the stream. Case 70 returns 1 without advancing.
const char* const kLoadFiles[] = {
    "ai_pyarray.bin",  // case 0
    "ai_factions_pyarray.bin",  // case 1
    "animations_pyarray.bin",  // case 2
    "character_classes_pyarray.bin",  // case 3
    "character_properties_pyarray.bin",  // case 4
    "character_templates_pyarray.bin",  // case 5
    "common_pyarray.bin",  // case 6
    "design_pyarray.bin",  // case 7
    "dialogs_pyarray.bin",  // case 8
    "effects_pyarray.bin",  // case 9
    "faeries_pyarray.bin",  // case 10
    "fonts_pyarray.bin",  // case 11
    "game_objects_pyarray.bin",  // case 12
    "help_pages_pyarray.bin",  // case 13
    "item_powers_pyarray.bin",  // case 14
    "item_powers_monopoly_pyarray.bin",  // case 15
    "levels_pyarray.bin",  // case 16
    "loot_audiovisual_pyarray.bin",  // case 17
    "loot_table_pyarray.bin",  // case 18
    "projectiles_pyarray.bin",  // case 19
    "skills_pyarray.bin",  // case 20
    "sounds_pyarray.bin",  // case 21
    "spawn_groups_pyarray.bin",  // case 22
    "trophies_pyarray.bin",  // case 23
    "v2conditions_pyarray.bin",  // case 24
    "v2eventmanager_pyarray.bin",  // case 25
    "v2quests_pyarray.bin",  // case 26
    "worldmap_pyarray.bin",  // case 27
    "character_models_dictionary_pyarray.bin",  // case 28
    "animations_dictionary_pyarray.bin",  // case 29
    "effects_dictionary_pyarray.bin",  // case 30
    "projectiles_dictionary_pyarray.bin",  // case 31
    "game_objects_dictionary_pyarray.bin",  // case 32
    "common_text_pyarray.bin",  // case 33
    "sdd_dungeon_hunter_2_iphone_pyarray.bin",  // case 34
    "ai_pyarraynames.bin",  // case 35
    "ai_factions_pyarraynames.bin",  // case 36
    "animations_pyarraynames.bin",  // case 37
    "character_classes_pyarraynames.bin",  // case 38
    "character_properties_pyarraynames.bin",  // case 39
    "character_templates_pyarraynames.bin",  // case 40
    "common_pyarraynames.bin",  // case 41
    "design_pyarraynames.bin",  // case 42
    "dialogs_pyarraynames.bin",  // case 43
    "effects_pyarraynames.bin",  // case 44
    "faeries_pyarraynames.bin",  // case 45
    "fonts_pyarraynames.bin",  // case 46
    "game_objects_pyarraynames.bin",  // case 47
    "help_pages_pyarraynames.bin",  // case 48
    "item_powers_pyarraynames.bin",  // case 49
    "item_powers_monopoly_pyarraynames.bin",  // case 50
    "levels_pyarraynames.bin",  // case 51
    "loot_audiovisual_pyarraynames.bin",  // case 52
    "loot_table_pyarraynames.bin",  // case 53
    "projectiles_pyarraynames.bin",  // case 54
    "skills_pyarraynames.bin",  // case 55
    "sounds_pyarraynames.bin",  // case 56
    "spawn_groups_pyarraynames.bin",  // case 57
    "trophies_pyarraynames.bin",  // case 58
    "v2conditions_pyarraynames.bin",  // case 59
    "v2eventmanager_pyarraynames.bin",  // case 60
    "v2quests_pyarraynames.bin",  // case 61
    "worldmap_pyarraynames.bin",  // case 62
    "character_models_dictionary_pyarraynames.bin",  // case 63
    "animations_dictionary_pyarraynames.bin",  // case 64
    "effects_dictionary_pyarraynames.bin",  // case 65
    "projectiles_dictionary_pyarraynames.bin",  // case 66
    "game_objects_dictionary_pyarraynames.bin",  // case 67
    "common_text_pyarraynames.bin",  // case 68
    "sdd_dungeon_hunter_2_iphone_pyarraynames.bin",  // case 69
};
constexpr int kLoadDoneState = 70;
static_assert(sizeof(kLoadFiles) / sizeof(kLoadFiles[0]) == kLoadDoneState, "Load has 70 file states");

// Stand-in for StreamReader::readString in PyDataConstants::reloadData. Assumed form: a uint32 length
// prefix, then that many bytes, failing when the length does not fit the 256-byte buffer or the stream is
// short. The listing's readString arguments are not decoded, so this is not verified (see the ledger).
constexpr std::uint32_t kConstantsStringCap = 256;

bool ReadPrefixedString(IStreamBase* stream, char* buf) {
    const std::uint32_t len = StreamReader::readAs<std::uint32_t>(stream);
    if (len >= kConstantsStringCap) return false;
    if (len != 0 && (stream == nullptr || !stream->ReadBytes(buf, len))) return false;
    buf[len] = '\0';
    return true;
}

}  // namespace

// ---------------------------------------------------------------------------------------------
// PyDataArrays
// ---------------------------------------------------------------------------------------------

// 0x4c0ddc _ZN12PyDataArraysC2EP19DataReloaderManager
// Partial. Stores the reloader and leaves the file and registry maps empty (member initializers).
// Not transcribed: the 142 addFuncsForFile(...) calls and the 142 registerClassByName(...) calls that
// follow in the listing. Each names Arrays::X::read / finalize / skipNames / readNames / finalizeNames
// or a Structs/Arrays GetMemberIDByString<T> instance; those are declared in other slices' headers with
// conflicting Arrays struct layouts, so they are not included in this translation unit.
PyDataArrays::PyDataArrays(DataReloaderManager* reloader) : m_reloader(reloader) {}

// 0x4af87c _ZN12PyDataArraysD1Ev (0x4af9bc _ZN12PyDataArraysD2Ev is the same body)
PyDataArrays::~PyDataArrays() {
    // Finalize each file's entries in tree order, then clear the registry, then the file map.
    for (auto& entry : m_files) {
        for (const _Funcs& funcs : entry.second) funcs.finalize();
    }
    m_classes.clear();
    m_files.clear();
}

// 0x4af9a0 _ZN12PyDataArraysD0Ev
void DeletingDestructor(PyDataArrays* self) {
    self->PyDataArrays::~PyDataArrays();
    CustomFree(self);
}

// 0x4bdccc _ZN12PyDataArrays19registerClassByNameEPKcPFiS1_E
PyDataArrays::ClassFactory& PyDataArrays::registerClassByName(const char* name, ClassFactory factory) {
    if (factory == nullptr) {
        if (gAssertLevel == 2) {
            // The original stores to address 0 here (dword_0 = 0), which faults.
            std::abort();
        } else if (gAssertLevel == 1) {
            std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "func",
                         "..\\..\\project_vs2005\\Game/..\\..\\sources\\Data\\PyData\\PyDataArrays.cpp", 70);
        }
    }
    // A null factory is stored as well, as in the listing.
    ClassFactory& slot = m_classes[std::string(name)];
    slot = factory;
    return slot;
}

// 0x4aa21c _ZN12PyDataArrays4LoadEv
// Partial, covering all 70 file states (cases 0..69) and the terminal state 70. The loader's open and
// release calls use the PyDataFileLoader stand-in because the loader class is not recovered.
// partial: g_pyDataLoader has no definition in the port, and PyDataArrays::reloadData (vtable +8) is declared only.
int PyDataArrays::Load() {
    const int state = m_state;
    PyDataFileLoader* const loader = g_pyDataLoader;
    if (state == kLoadDoneState) return 1;

    if (state >= 0 && state < kLoadDoneState) {
        const char* const name = kLoadFiles[state];
        const std::string path = std::string("pydata/") + name;
        IStreamBase* stream = loader->OpenFile(path.c_str());
        if (stream != nullptr) {
            reloadData(stream, name);
            loader->ReleaseFile(&stream);
        } else {
            std::printf("[PyData Arrays] : ERROR LOADING \"%s\"\n", path.c_str());
        }
    }
    // The default branch of the listing (states below 0 or above 70) also advances the state.
    m_state = m_state + 1;
    return 0;
}

// ---------------------------------------------------------------------------------------------
// PyDataConstants
// ---------------------------------------------------------------------------------------------

// 0x4c3668 _ZN15PyDataConstantsC2EP19DataReloaderManager
PyDataConstants::PyDataConstants(DataReloaderManager* reloader) : m_reloader(reloader) {}

// 0x4c4258 _ZN15PyDataConstantsD1Ev (0x4c42d4 _ZN15PyDataConstantsD2Ev is the same body)
PyDataConstants::~PyDataConstants() {
    m_classes.clear();
}

// 0x4c42b8 _ZN15PyDataConstantsD0Ev
void DeletingDestructor(PyDataConstants* self) {
    self->PyDataConstants::~PyDataConstants();
    CustomFree(self);
}

// 0x4c4b08 _ZNK15PyDataConstants15getConstantNameEPKci
const char* PyDataConstants::getConstantName(const char* className, int value) const {
    const auto cls = m_classes.find(std::string(className));
    if (cls == m_classes.end()) return "";
    for (const auto& entry : cls->second) {
        if (entry.second == value) return entry.first.c_str();
    }
    return "";
}

// 0x4c540c _ZN15PyDataConstants10reloadDataEP11IStreamBasePKc
// Partial. Parses the same layout as the listing: a uint32 class count; per class a name, a uint32 entry
// count, and per entry a name and an int value stored at m_classes[class][name].
// Not reproduced: the DebugSwitches load and the "isTracingPyDataConstants" lookup, whose result the
// listing discards (no observable effect in this body). The string reads use the assumed
// ReadPrefixedString form above, and the return value is a success flag, not the listing's last value.
bool PyDataConstants::reloadData(IStreamBase* stream, const char* fileName) {
    (void)fileName;
    const std::uint32_t classCount = StreamReader::readAs<std::uint32_t>(stream);
    char className[kConstantsStringCap];
    char key[kConstantsStringCap];
    for (std::uint32_t c = 0; c < classCount; ++c) {
        if (!ReadPrefixedString(stream, className)) return false;
        const std::uint32_t entryCount = StreamReader::readAs<std::uint32_t>(stream);
        for (std::uint32_t e = 0; e < entryCount; ++e) {
            if (!ReadPrefixedString(stream, key)) return false;
            const int value = StreamReader::readAs<int>(stream);
            m_classes[std::string(className)][std::string(key)] = value;
        }
    }
    return true;
}
