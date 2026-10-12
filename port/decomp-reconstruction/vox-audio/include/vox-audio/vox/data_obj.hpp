// vox::DataObj: a loaded data source (a Handlable). Layout from the asm:
// +0x00 vtable (Handlable), +0x08 id, +0x10 ref count, +0x18 Handlable mutex,
// +0x1c group index (bit selected by IsGroup), +0x40 emitter list sentinel,
// +0x48 user data word, +0x4c need-to-die flag, +0x50 state word,
// +0x58 state mutex.
#pragma once

#include <cstdint>
#include <list>
#include <utility>

#include "vox-audio/vox/handlable.hpp"
#include "vox-audio/vox/mutex.hpp"

namespace vox {

class StreamInterface;
class DecoderInterface;

// Opaque user-data record as the original passes it: only the first word is used.
struct DataHandleUserData {
    std::uintptr_t word = 0;
};

class DataObj : public Handlable {
public:
    DataObj() = default;
    ~DataObj() override;  // 0x863c24 (D1), 0x863cac (D0)

    std::uintptr_t GetUserData();                                 // 0x865878
    void SetUserData(const DataHandleUserData& user_data);        // 0x8658a0
    bool IsReady();                                               // 0x8658cc: state == 0
    std::uint32_t GetDuration();                                  // 0x8658fc (partial)
    void GetDebugInfo(void* chunk);                               // 0x865954 (partial)
    void PrintDebug();                                            // 0x8659ec (partial)
    bool IsGroup(std::uint32_t groups);                           // 0x865a0c
    int GetGroup();                                               // 0x865a48
    bool ShouldDie();                                             // 0x865a70 (partial)
    void NeedToDie();                                             // 0x865aec
    void UnregisterEmitter(std::int64_t emitter_id, void* emitter);  // 0x865b30 (partial)
    void RegisterEmitter(std::int64_t emitter_id, void* emitter);    // 0x865ba8 (partial)
    void Update();                                                // 0x866dec (partial)

    int group_ = 0;                  // +0x1c
    int priority_bank_id_ = 0;       // +0x20 (set by VoxEngineInternal::SetPriorityBankId)
    int uid_ = 0;                    // +0x24 (returned by VoxEngineInternal::GetUid)
    int slot_ = 0;                   // +0x14: stamp slot in VoxEngineInternal::data_stamps_
    std::int32_t params_[4] = {};    // +0x28..+0x34: channels, unknown, bits, length (set by LoadDataSource)
    StreamInterface* stream_ = nullptr;   // +0x38
    DecoderInterface* decoder_ = nullptr; // +0x3c
    bool queued_ = false;            // +0x4c: already on the engine data queue
    std::uint16_t source_flags_ = 0; // +0x54: VoxSourceLoadingFlags of an async load
    std::uintptr_t user_data_ = 0;   // +0x48
    bool need_to_die_ = false;       // +0x4d
    int state_ = 0;                  // +0x50
    Mutex state_mutex_;              // +0x58
    // +0x40 emitter list: entries of (emitter id, emitter). The list node
    // word at +0x10 of each entry is not established, so the port keeps the
    // pair only.
    std::list<std::pair<std::int64_t, void*>> emitters_;
};

}  // namespace vox
