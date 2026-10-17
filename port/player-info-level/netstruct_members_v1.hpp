#pragma once

#include "character_level_member.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>

namespace dh2::netstruct_members_v1 {

enum class Status { complete, invalid_argument, invalid_state, allocation_failed,
                    missing_provider, provider_failed };
enum class Kind { integer, unsigned_integer, boolean, string32, byte_array };

// These callbacks are the reached CustomAlloc(size,2)/CustomFree boundary.
// They must not reenter or mutate this member. Release completes synchronously;
// if it throws, the pointer is retained and no subsequent source store occurs.
struct Memory {
    void* context = nullptr;
    void* (*allocate)(void*, std::size_t, int) = nullptr;
    void (*release)(void*, void*) = nullptr;
};
struct BytesView { const std::uint8_t* data = nullptr; std::int32_t size = 0; };
struct Bytes { std::uint8_t* data = nullptr; std::int32_t size = 0; };

// Stable, nonmovable typed backing. Int/UInt use header.value at +0x20; Bool
// uses header.reserved[0] at source +0x1d. The Level member is this same IntMember,
// which is passed directly to the existing SetCharacterLevel kernel.
// String/ByteArray values are owned here, never in a parallel field map.
struct Member {
    character_level_member::IntMember header{};
    Kind kind = Kind::integer;
    std::uint64_t* serial = nullptr;
    std::string text;
    Bytes bytes{};
    Memory memory{};
    bool constructed = false;
    bool busy = false;
    Member() = default;
    Member(const Member&) = delete;
    Member& operator=(const Member&) = delete;
    Member(Member&&) = delete;
    Member& operator=(Member&&) = delete;
    ~Member();
};

// Source ARM vtable words are diagnostic projection metadata, never native
// dispatch pointers. 'base' selects NetStructMember's destructor transition.
std::uint32_t vtable(Kind, std::uint32_t bits, bool base = false);
Status transition_to_base(Member&);
Status retire_payload(Member&);

// Constructors compare the actual prior backing value before setting the
// initial value. The caller must initialize header.value/reserved[0] from a
// documented allocation policy or supply the captured source residue. They
// are not implicitly replaced with zero. Other untouched padding is preserved.
Status construct_scalar(Member&, Kind, std::uint32_t bits,
                        std::int32_t initial, std::uint64_t* serial);
Status construct_string(Member&, std::string_view initial, std::uint64_t* serial);
Status construct_bytes(Member&, std::uint32_t bytes_width, BytesView initial,
                       std::uint64_t* serial, Memory);
Status copy_construct(Member&, const Member&);
Status assign(Member&, const Member&);
Status set_int(Member&, std::int32_t);
Status set_uint(Member&, std::uint32_t);
Status set_bool(Member&, std::uint8_t);
Status set_string(Member&, std::string_view);
Status set_bytes(Member&, BytesView);
// Source SetBuffer makes an owned temporary, then dispatches SetValue. Equal
// contents preserve the revision; temporary allocation/free still occur.
Status set_buffer(Member&, BytesView);
Status destroy(Member&);

struct Packet { std::uint64_t revision = 0, changed_bitmap = 0; };
using PacketHistory = std::map<int, std::map<int, Packet>>;
struct NetStruct {
    std::uint32_t source_vtable = 0;
    std::array<Member*, 64> members{};
    std::uint32_t count = 0;
    std::uint8_t enabled_108 = 0, field_124 = 0, field_125 = 0;
    std::uint32_t field_128 = 0;
    PacketHistory history;
    bool constructed = false;
    bool busy = false;
};
Status construct(NetStruct&);
Status declare_member(NetStruct&, Member&);
// The original copy and assignment copy all64 pointer entries unchanged.
// These remain borrowed source aliases, even when the destination has copied
// owned values. Callers must preserve their pointees; no implicit retarget.
Status copy_construct(NetStruct&, const NetStruct&);
Status assign(NetStruct&, const NetStruct&);
Status destroy(NetStruct&);

} // namespace dh2::netstruct_members_v1
