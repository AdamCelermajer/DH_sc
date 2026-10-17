#pragma once
#include "cnet_player_info_v1.hpp"
#include <mutex>

namespace dh2::player_info_record_v1 {
using Status = netstruct_members_v1::Status;
using Member = netstruct_members_v1::Member;
constexpr std::uint32_t character_level = 0x310, character_potions = 0x338;
constexpr std::uint32_t character_class = 0x360, character_death_timer = 0x388;
constexpr std::uint32_t character_xp = 0x428, character_gold = 0x478;
// Captured prior stack values read by Reset's temporary constructors. Native
// callers choose a defined backing policy; never read uninitialized C++ data.
struct ResetResidues {
    std::array<std::int32_t, 6> setters{}; // level, potions, class, death, XP, gold
    std::int32_t first_array = 0, controller_type = 0, field_4a0 = 0;
    std::array<std::uint8_t, 4> flags{};
};
// Borrow the sole process factory registration and +680 deleting provider.
// ensure_factory delivers the source static-guard/registration boundary; it
// must install working native Create/Delete functions in the existing owner.
struct Services {
    void* context = nullptr;
    Status (*ensure_factory)(void*) = nullptr;
    Status (*delete_loading_info)(void*, std::uintptr_t) = nullptr;
};
struct Record {
    cnet_player_info_v1::Record base;
    std::array<Member, 25> members;
    std::uintptr_t character_660 = 0, loading_info_680 = 0;
    std::int32_t save_slot_664 = 0, controller_668 = 0, internal_id_670 = 0;
    std::int32_t member_674 = 0, number_678 = 0, group_number_67c = 0;
    std::uint8_t local_66c = 0;
    std::uint32_t field_684 = 0;
    Services services{};
    bool constructed = false, busy = false;
    Record() = default;
    Record(const Record&) = delete;
    Record& operator=(const Record&) = delete;
    ~Record();
    Member* at(std::uint32_t source_offset);
    const Member* at(std::uint32_t source_offset) const;
};
// C1/C2 share reached semantics. Seed scalar/boolean member backing before
// construction. Offsets are source metadata, not native pointer arithmetic.
Status construct(Record&, std::uint64_t* serial, netstruct_members_v1::Memory,
                 Services, const ResetResidues&);
Status reset(Record&, const ResetResidues&);
Status copy_construct(Record&, const Record&);
Status assign(Record&, const Record&);
Status destroy(Record&);
// These six source setters construct a temporary using the same serial, then
// dispatch to the actual owned member. Level is the existing kernel's backing.
Status set_character_scalar(Record&, std::uint32_t source_offset,
                            std::int32_t value, std::int32_t stack_residue);
Status set_character_name(Record&, std::string_view);

// Native ABI adapter for the source process Create/Delete registration. The
// existing player-manager owner holds one Factory and lends it to all records.
// Allocation requests use sizeof(Record), preserving source allocation tag0;
// 0x688 is only the original ARM logical size. Member buffers still use tag2.
// The object allocator must provide Record alignment and nonthrowing release.
// Loading-info and record-copy aliases retain the source lifetime constraints.
class Factory {
public:
    using Create = Status (*)(Factory&, Record**);
    using Delete = Status (*)(Factory&, Record*);
private:
    std::uint64_t* serial_;
    netstruct_members_v1::Memory memory_;
    ResetResidues residues_;
    void* loading_context_;
    Status (*loading_delete_)(void*, std::uintptr_t);
    std::mutex guard_;
    Create create_ = nullptr;
    Delete delete_ = nullptr;
    bool active_ = false;
    static Status ensure(void*);
    static Status delete_loading(void*, std::uintptr_t);
    static Status create_native(Factory&, Record**);
    static Status delete_native(Factory&, Record*);
public:
    Factory(std::uint64_t* serial, netstruct_members_v1::Memory memory,
            ResetResidues residues = {}, void* loading_context = nullptr,
            Status (*loading_delete)(void*, std::uintptr_t) = nullptr)
        : serial_(serial), memory_(memory), residues_(residues),
          loading_context_(loading_context), loading_delete_(loading_delete) {}
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;
    Services services();
    Status construct_record(Record&);
    Status create_record(Record**);
    Status delete_record(Record*);
    bool registered();
};
} // namespace dh2::player_info_record_v1
