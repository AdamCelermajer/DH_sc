#pragma once

#include <cstddef>
#include <cstdint>

// Bounded reader for the ASCII UserProperties text attached to scene nodes.
// Returned spans borrow the input bytes and remain valid only while they do.
namespace dh2::floor_types {

struct Span {
    const char* data;
    std::size_t size;
};

struct Property {
    bool found;
    Span value;
};

enum class Error : std::uint32_t {
    ok,
    argument,
    too_large,
    unterminated
};

constexpr std::size_t kMaxUserPropertiesBytes = 64U * 1024U;

// `available_bytes` is the checked extent of the user-data string and must
// include its NUL terminator. Lines are LF-separated. The first '=' separates
// key/value; a matching key without '=' has an explicit empty value. Duplicate
// keys use the last occurrence, matching the native map assignment behavior.
// Encoded %22 wrappers are removed from a value (a trailing CR is preserved).
Error find_property(const char* bytes, std::size_t available_bytes,
                    Span key, Property* out);

// Original PFFloor::_LoadNavMesh masks. Void and wall are separate category
// bits; hole and water are the low path-eligibility bits used by CanPathOn.
constexpr std::uint32_t kFloorTypeVoid = 0x01000000U;
constexpr std::uint32_t kFloorTypeWall = 0x02000000U;
constexpr std::uint32_t kFloorPathHole = 0x00000001U;
constexpr std::uint32_t kFloorPathWater = 0x00000002U;

// Only the explicit `floortypes` property value contributes type bits. The
// original loader leaves its tag string empty when the property is absent;
// node names are never used as a fallback. Matching is case-sensitive and
// multiple recognized substrings OR their masks.
std::uint32_t floor_type_mask(bool property_present, Span property_value);

// PFObject::CanPathOn: a zero floor mask is always allowed; otherwise every
// floor requirement bit must be present in the object's path mask.
bool can_path_on(std::uint32_t floor_mask, std::uint32_t object_path_mask);

} // namespace dh2::floor_types
