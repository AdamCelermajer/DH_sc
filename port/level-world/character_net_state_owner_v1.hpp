#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::character_net_state_owner_v1 {

// Source anchors (image base 0): Character::Character 0x3a9340 calls
// NetStructCharacter::C1 at 0x3a975c and 0x3a9764, storing the two pointers
// at Character+0x100/+0x104. Character::PopulateOutgoingNetStruct
// (0x3a6f0c) updates the first instance; InterpretIncomingNetStruct
// (0x3a7f10) consumes the second. Character::~Character (0x3a7dfc) destroys
// secondary before primary. This owner preserves those boundaries and leaves
// concrete member/bitstream behavior to the canonical NetStruct provider.

using Identity = std::uintptr_t;

enum class Role : std::uint8_t { outgoing_primary, incoming_secondary };
enum class FieldKind : std::uint8_t {
    interpolated_float, float_value, unsigned_integer, integer, boolean,
};

struct FieldSpec {
    std::uint16_t byte_offset;
    FieldKind kind;
    std::uint8_t bit_width;
    std::int32_t range_min;
    std::int32_t range_max;
};

struct FieldState {
    float float_value{};
    std::uint32_t unsigned_value{};
    std::int32_t integer_value{};
    bool boolean_value{};
    std::uint64_t revision{};
    bool constructed{};
};

struct InterpolationSample {
    std::uint32_t source_timestamp{};
    float value{};
};

struct InterpolationState {
    std::array<InterpolationSample, 20> samples{};
    std::uint32_t start_index{};
    std::uint32_t auxiliary_index{};
    std::uint32_t current_index{};
    std::uint32_t end_index{};
    std::uint32_t capacity{40};
    std::uint32_t elapsed_total{};
    std::uint32_t sample_count{};
    std::uint32_t average_elapsed{};
};

struct BitStream {
    void* context{};
    int (*write_u32)(void*, std::uint32_t, std::uint8_t){};
    int (*read_u32)(void*, std::uint8_t, std::uint32_t*){};
};

// Character::NetStructCharacter declares these members in this exact order.
// Unknown source member names are intentionally omitted; widths/ranges and
// offsets are the properties established by its constructors and vtables.
inline constexpr std::array<FieldSpec, 11> source_fields{{
    {0x130, FieldKind::interpolated_float, 18, -200000, 200000},
    {0x228, FieldKind::interpolated_float, 18, -200000, 200000},
    {0x320, FieldKind::interpolated_float, 16, -50000, 50000},
    {0x418, FieldKind::float_value, 16, -1, 1},
    {0x440, FieldKind::float_value, 16, -1, 1},
    {0x468, FieldKind::unsigned_integer, 32, 0, 0},
    {0x490, FieldKind::unsigned_integer, 32, 0, 0},
    {0x4b8, FieldKind::integer, 16, 0, 0},
    {0x4e0, FieldKind::boolean, 1, 0, 1},
    {0x500, FieldKind::boolean, 1, 0, 1},
    {0x520, FieldKind::boolean, 1, 0, 1},
}};

struct SourceComponent {
    Identity identity{};
    Identity character_identity{};
    std::uint32_t component_offset{};
    std::uint32_t schema_id{};
    std::uint8_t declared_member_count{};
    bool constructor_complete{};
    std::array<FieldState, 11> fields{};
    std::array<InterpolationState, 3> interpolation{};
    std::uint64_t* change_counter{};
    bool members_ready{};
};

inline constexpr std::uint32_t character_netstruct_schema_v1 = 0x4e534331;
inline constexpr std::uint32_t primary_component_offset = 0x1508;
inline constexpr std::uint32_t secondary_component_offset = 0x1a48;

struct Services {
    void* context{};
    int (*construct)(void*, Identity character, Role, std::uint32_t component_offset,
                     SourceComponent*, std::string&){};
    int (*populate_outgoing)(void*, const SourceComponent&, std::string&){};
    int (*interpret_incoming)(void*, const SourceComponent&, std::string&){};
    int (*destroy)(void*, const SourceComponent&, std::string&){};
};

struct Owner {
    Identity character_identity{};
    SourceComponent primary{};
    SourceComponent secondary{};
    bool constructed{};
};

enum class Status : std::uint8_t {
    complete, invalid_argument, service_unavailable, service_failed,
    invalid_component, cleanup_incomplete, member_not_ready, invalid_field,
    invalid_value, bitstream_failed,
};

struct Result {
    std::uint32_t constructed_components{};
    std::uint32_t retired_components{};
    Role failed_role{Role::outgoing_primary};
};

Status construct(Owner*, Identity character, std::uint64_t* change_counter,
                 const Services*, Result*,
                 std::string& error);
Status populate_outgoing(Owner*, const Services*, std::string& error);
Status interpret_incoming(Owner*, const Services*, std::string& error);
Status destroy(Owner*, const Services*, Result*, std::string& error) noexcept;
Status initialize_members(SourceComponent*, std::uint64_t* change_counter) noexcept;
// Source-order factory hooks: Character::Character constructs primary at
// 0x3a975c, then secondary at 0x3a9764. These hooks publish each embedded
// member owner in that order without speculatively constructing its sibling.
Status construct_component(Owner*, Role, Identity character,
                           std::uint64_t* change_counter) noexcept;
Status destroy_component(Owner*, Role, Identity character) noexcept;
Status set_float(SourceComponent*, std::size_t field, float value) noexcept;
Status set_unsigned(SourceComponent*, std::size_t field, std::uint32_t value) noexcept;
Status set_integer(SourceComponent*, std::size_t field, std::int32_t value) noexcept;
Status set_boolean(SourceComponent*, std::size_t field, bool value) noexcept;
bool test_float(std::size_t field, float value) noexcept;
Status write_member(const SourceComponent*, std::size_t field,
                    const BitStream&) noexcept;
Status read_member(SourceComponent*, std::size_t field,
                   const BitStream&) noexcept;
Status post_load(SourceComponent*, std::size_t field,
                 std::uint32_t source_timestamp,
                 std::uint32_t current_frame_time) noexcept;

} // namespace dh2::character_net_state_owner_v1
