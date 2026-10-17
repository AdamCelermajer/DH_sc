#pragma once

#include <cstddef>
#include <cstdint>

namespace dh2::character_level_member {

// Source projection of the int-valued NetStructMember at PlayerInfo+0x310.
// Field offsets match the ARM32 object consumed by NetStructMember::SetChanged
// and NetStructMemberType<int>::SetValue. This is a host-side source kernel;
// it does not claim to construct the complete PlayerInfo network object.
#pragma pack(push, 4)
struct IntMember {
    std::uint32_t vtable;        // +0x00 (source ARM32 word)
    std::uint32_t size_bits;     // +0x04
    std::uint64_t revision;      // +0x08
    std::uint32_t field_10;      // +0x10
    std::uint32_t field_14;      // +0x14
    std::uint32_t source_stamp;  // +0x18
    std::uint8_t changed;        // +0x1c
    std::uint8_t reserved[3];
    std::int32_t value;          // +0x20
};
#pragma pack(pop)

struct State {
    IntMember* character_level_member;
    std::uint64_t* global_change_serial;
};

enum class Status : std::int32_t {
    complete = 0,
    invalid_argument,
};

struct Result {
    std::uint32_t temporary_marked;
    std::uint32_t member_marked;
    std::uint32_t member_value_changed;
    std::int32_t member_value_after;
    std::uint64_t serial_after;
};

// Exact leaf semantics of NetStructMemberType<int>::SetValue: same-value is
// a no-op; otherwise store +0x20 before the SetChanged effects. The serial is
// the shared NetStructMember change counter. Valid borrowed objects must
// remain alive for this synchronous call and may not overlap.
Status set_value(IntMember*, std::uint64_t* global_change_serial,
                 std::int32_t value, Result*);

// Shared NetStructMember::SetChanged leaf for all concrete member kinds.
// Only the common prefix through +0x1c is changed; the value is untouched.
Status mark_changed(IntMember*, std::uint64_t* global_change_serial);

// Bounded PlayerInfo::SetCharacterLevel wrapper. The original ARM body reads
// a stack word at [SP+0x20] before writing its temporary's +0x20 value. That
// word is not initialized in the function body; callers/tests must supply the
// observed residue explicitly. If it differs, source first marks the temporary
// changed, then dispatches SetValue on PlayerInfo+0x310. This argument exposes
// that source boundary instead of assuming the temporary starts with a value.
Status set_character_level(State*, std::int32_t value,
                           std::int32_t temporary_stack_residue, Result*);

static_assert(sizeof(IntMember) == 0x24);
static_assert(offsetof(IntMember, revision) == 0x08);
static_assert(offsetof(IntMember, field_10) == 0x10);
static_assert(offsetof(IntMember, field_14) == 0x14);
static_assert(offsetof(IntMember, source_stamp) == 0x18);
static_assert(offsetof(IntMember, changed) == 0x1c);
static_assert(offsetof(IntMember, value) == 0x20);
static_assert(sizeof(State) == 16);
static_assert(sizeof(Result) == 24);

}  // namespace dh2::character_level_member
