// vox::VoxSoundPackXML: the parsed sound-pack tables and their accessors. Four vectors hold
// the records (sounds, groups, banks, events) and a case-insensitive map at +0x30 maps a sound
// name to its uid. The XML loader (LoadXML, the constructors and the destructor) is not ported,
// so the port starts from an empty pack and the tests fill the tables directly.
//
// Layout (32-bit offsets, kept as comments): sounds_ +0x00 (68-byte records), groups_ +0x0c
// (56-byte records), banks_ +0x18 (40-byte records), events_ +0x24 (44-byte records) and
// sound_uids_ +0x30. Index accessors return 0 (or -1 for uids) when the index is out of range
// or the record's own uid does not match the index. Name accessors that the binary implements as
// tail calls return whatever the index accessor returns.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "vox-audio/vox/string_compare.hpp"
#include "vox-audio/vox/voxengine_types.hpp"

namespace vox {

enum Vox3DSoundType : int;
enum FormatTypes : int;

// Bank record, 0x28 bytes: +0x00 uid, +0x04 priority behaviour, +0x08, +0x0c, name string at
// +0x10 (16 bytes), +0x20 and +0x24.
struct BankRecordXML {
    std::int32_t uid = 0;          // +0x00
    std::int32_t behavior = 0;     // +0x04
    std::int32_t field_08 = 0;     // +0x08
    std::int32_t field_0c = 0;     // +0x0c
    std::string name;              // +0x10 (SAllocator string in the original)
    std::int32_t field_20 = 0;     // +0x20
    std::int32_t field_24 = 0;     // +0x24
};

// Group record, 0x38 bytes: +0x00 uid, +0x18 name pointer, +0x1c name string, +0x30 and +0x34
// (the sound type).
struct GroupRecordXML {
    std::int32_t uid = 0;                    // +0x00
    std::int32_t unknown_04_[5] = {};        // +0x04..+0x17
    const char* name_ptr = nullptr;          // +0x18
    std::string name;                        // +0x1c (SAllocator string in the original)
    std::int32_t field_30 = 0;               // +0x30
    std::int32_t type = 0;                   // +0x34 (Vox3DSoundType)
};

// Event record, 0x2c bytes: +0x00 uid, +0x04 name, +0x10 sound uid vector, +0x1c, +0x1e and
// +0x20 (16-bit fields), +0x24 custom parameter count, +0x28 custom parameter array.
struct EventRecordXML {
    std::int32_t uid = 0;                          // +0x00
    const char* name = nullptr;                    // +0x04
    std::vector<std::int32_t> sound_uids;          // +0x10..+0x18
    std::int16_t field_1c = 0;                     // +0x1c
    std::int16_t field_1e = 0;                     // +0x1e
    std::int16_t field_20 = 0;                     // +0x20
    std::int32_t custom_count = 0;                 // +0x24
    const char* const* custom_values = nullptr;    // +0x28
};

// Sound record, 0x44 bytes: +0x00 uid, +0x04 emitter uid, +0x08, +0x0c data name, +0x10, +0x14
// format (byte), +0x15 and +0x16 bytes, +0x17 emitter flag (byte), +0x18..+0x38 nine words,
// +0x3c custom parameter count, +0x40 custom parameter array. The +0x16 byte is the group index
// when the emitter is read and a flag when the data source is read.
struct SoundRecordXML {
    std::int32_t uid = 0;                          // +0x00
    std::int32_t emitter_uid = 0;                  // +0x04
    std::int32_t field_08 = 0;                     // +0x08
    const char* data_name = nullptr;               // +0x0c
    std::int32_t field_10 = 0;                     // +0x10
    std::int8_t format = 0;                        // +0x14
    std::int8_t field_15 = 0;                      // +0x15
    std::int8_t field_16 = 0;                      // +0x16
    std::int8_t emitter_flag = 0;                  // +0x17
    std::int32_t emitter_values_[9] = {};          // +0x18..+0x38
    std::int32_t custom_count = 0;                 // +0x3c
    const char* const* custom_values = nullptr;    // +0x40
};

// Output of GetBankInfo(int, BankInfoXML&).
struct BankInfoXML {
    std::int32_t uid = 0;
    std::int32_t behavior = 0;
    std::int32_t field_08 = 0;
    std::int32_t field_0c = 0;
    std::int32_t field_24 = 0;
};

// Output of GetGroupInfo(int, GroupInfoXML&).
struct GroupInfoXML {
    std::int32_t uid = 0;
    const char* name = nullptr;
    std::int32_t field_30 = 0;
    std::int32_t type = 0;  // Vox3DSoundType
};

// Output of GetDataSourceInfo(int, DataSourceInfoXML&).
struct DataSourceInfoXML {
    std::int32_t uid = 0;
    std::int32_t field_08 = 0;
    const char* data_name = nullptr;
    std::int32_t format = 0;     // FormatTypes
    std::int32_t field_16 = 0;
    std::int32_t field_15 = 0;
    std::int32_t field_10 = 0;   // VoxSourceLoadingFlags
    std::int32_t custom_count = 0;
    const char* const* custom_values = nullptr;
};

// Output of GetEmitterInfo(int, EmitterInfoXML&). The original writes eighteen words; the
// nine sound-record words at +0x18..+0x38 and the count and array of +0x3c and +0x40 are kept
// apart here, so the structure is not a word-for-word copy.
struct EmitterInfoXML {
    std::int32_t uid = 0;
    std::int32_t field_08 = 0;
    std::int32_t emitter_uid = 0;
    std::int32_t group = 0;
    bool flag = false;                       // the byte at +0x10
    std::int32_t type = 0;                   // Vox3DSoundType, from the group
    const char* name = nullptr;              // from the group
    std::int32_t values_[9] = {};            // the sound record's +0x18..+0x38
    std::int32_t custom_count = 0;           // the sound record's +0x3c
    const char* const* custom_values = nullptr;  // the sound record's +0x40
};

// Output of GetEventInfo(int, EventInfoXML&).
struct EventInfoXML {
    std::int32_t uid = 0;
    const char* name = nullptr;
    const std::vector<std::int32_t>* sound_uids = nullptr;
    std::int32_t field_1c = 0;
    std::int16_t field_1e = 0;
    std::int16_t field_20 = 0;
    std::int32_t custom_count = 0;
    const char* const* custom_values = nullptr;
};

// Group definition the sound-pack loader fills (vox::GroupXMLDef, 56 bytes): uid at +0x00,
// a string at +0x04 and a second string at +0x1c, the group type at +0x34. What the two
// strings hold is not established. Only the constructors and the copy assignment are
// reconstructed here; the loader is not ported.
class GroupXMLDef {
public:
    GroupXMLDef();                                       // 0x88ade0
    GroupXMLDef(const GroupXMLDef& other);               // 0x88ae2c
    GroupXMLDef& operator=(const GroupXMLDef& other);    // 0x88b5f0

    std::int32_t uid_ = 0;     // +0x00
    std::string name_;         // +0x04
    std::string label_;        // +0x1c
    std::int32_t type_ = 0;    // +0x34
};

class VoxSoundPackXML {
public:
    // Port-only: the original constructors (from an XML file name) are not reconstructed.
    VoxSoundPackXML() = default;

    // Index accessors.
    int GetBankInfo(int index, int& field_0c, int& field_08, PriorityBankBehavior& behavior) const;  // 0x889498
    int GetBankInfo(int index, BankInfoXML& out) const;                                               // 0x88952c
    int GetGroupInfo(int index, const char*& name, Vox3DSoundType& type) const;                       // 0x8895d4
    int GetGroupInfo(int index, GroupInfoXML& out) const;                                             // 0x889658
    int GetDataSourceInfo(int index, const char*& name, FormatTypes& format, int& field_16,
                          int& field_15, VoxSourceLoadingFlags& flags) const;                         // 0x8896f4
    int GetDataSourceInfo(int index, DataSourceInfoXML& out) const;                                   // 0x8897ac
    int GetEmitterInfo(int index, int& emitter_uid, int& group, bool& flag, Vox3DSoundType& type,
                       const char*& name) const;                                                      // 0x889894
    int GetEmitterInfo(int index, EmitterInfoXML& out) const;                                         // 0x88993c
    bool GetSoundCustomParam(int index, int param, const char*& value);                         // 0x889ac8
    int GetEventInfo(int index, EventInfoXML& out) const;                                             // 0x889b34
    int GetEventSize(int index);                                                                // 0x889c04
    bool GetEventCustomParam(int index, int param, const char*& value);                         // 0x889c54

    // Name accessors. The lookups return -1 when the name is not found; the wrappers return
    // what the index accessor they tail-call returns.
    int GetEventUid(const char* name) const;                                                          // 0x88a3f4
    bool GetEventCustomParam(const char* name, int param, const char*& value);                  // 0x88a464
    int GetEventSize(const char* name);                                                         // 0x88a490
    int GetEventInfo(const char* name, EventInfoXML& out) const;                                      // 0x88a4ac
    int GetSoundUid(const char* name) const;                                                          // 0x88a55c
    bool GetSoundCustomParam(const char* name, int param, const char*& value);                  // 0x88a590
    int GetEmitterInfo(const char* name, EmitterInfoXML& out) const;                                  // 0x88a5bc
    int GetEmitterInfo(const char* name, int& sound_uid, int& emitter_uid, int& group, bool& flag,
                       Vox3DSoundType& type, const char*& out_name) const;                            // 0x88a5e0
    int GetDataSourceInfo(const char* name, DataSourceInfoXML& out) const;                            // 0x88a62c
    int GetDataSourceInfo(const char* name, int& sound_uid, const char*& data_name, FormatTypes& format,
                          int& field_16, int& field_15, VoxSourceLoadingFlags& flags) const;         // 0x88a650
    int GetGroupUid(const char* name) const;                                                          // 0x88b8b8
    int GetGroupInfo(const char* name, GroupInfoXML& out) const;                                      // 0x88b960
    int GetGroupInfo(const char* name, int& group_uid, const char*& group_name,
                     Vox3DSoundType& type) const;                                                     // 0x88b984
    int GetBankUid(const char* name) const;                                                           // 0x88b9b8
    int GetBankInfo(const char* name, BankInfoXML& out) const;                                        // 0x88ba5c
    int GetBankInfo(const char* name, int& bank_uid, int& field_0c, int& field_08,
                    PriorityBankBehavior& behavior) const;                                            // 0x88ba80

    // Test access: the tables are filled directly by the tests.
    friend struct VoxSoundPackXMLTestAccess;

private:
    std::vector<SoundRecordXML> sounds_;                            // +0x00
    std::vector<GroupRecordXML> groups_;                            // +0x0c
    std::vector<BankRecordXML> banks_;                              // +0x18
    std::vector<EventRecordXML> events_;                            // +0x24
    std::map<const char*, std::int32_t, c8stringcomp> sound_uids_;  // +0x30
};

}  // namespace vox
