#ifndef DH2_ENGINE_AUDIO_NATIVE_FORMAT_CHUNK_VIEW_HPP
#define DH2_ENGINE_AUDIO_NATIVE_FORMAT_CHUNK_VIEW_HPP

#include <cstddef>
#include <cstdint>
#include <limits>

// Adapted from codex/dh2-engine-reverse-engineering, e6da25b83086ea01fb1d4cd46ae12da3bbac645f.
// Bounded port views of observed VoxN framing, not the original parser ABI/body.
namespace dh2::engine_audio::native_format {

inline constexpr std::uint32_t kMagicVoxN = 0x4e786f56u;
inline constexpr std::uint32_t kTagAfmt = 0x746d6641u;
inline constexpr std::uint32_t kTagSegm = 0x6d676553u;
inline constexpr std::uint32_t kTagCuse = 0x65737543u;
inline constexpr std::uint32_t kTagGrps = 0x73707247u;
inline constexpr std::uint32_t kTagGrpe = 0x65707247u;
inline constexpr std::uint32_t kTagRule = 0x656c7552u;
inline constexpr std::uint32_t kTagPlst = 0x74736c50u;
inline constexpr std::uint32_t kTagStat = 0x74617453u;
inline constexpr std::uint32_t kTagTrsn = 0x6e737254u;
// Candidate API spellings retained with corrected values.
inline constexpr std::uint32_t kTagGprs = kTagGrps;
inline constexpr std::uint32_t kTagGpre = kTagGrpe;
inline constexpr std::uint32_t kTagPLst = kTagPlst;
inline constexpr std::uint32_t kTagTrns = kTagTrsn;

inline bool is_known_tag(std::uint32_t tag) noexcept {
    switch (tag) {
    case kTagAfmt: case kTagSegm: case kTagCuse: case kTagGrps:
    case kTagGrpe: case kTagRule: case kTagPlst: case kTagStat:
    case kTagTrsn: return true;
    default: return false;
    }
}

struct HeaderView { std::uint32_t first_section_read_bytes; };
struct ChunkView {
    std::uint32_t tag;
    std::size_t header_offset;
    const std::uint8_t* payload;
    std::size_t payload_size;
};
enum class ChunkReadStatus { kChunk, kEnd, kMalformed };

namespace detail {
inline bool span(const void* p, std::size_t n) noexcept {
    const auto a = reinterpret_cast<std::uintptr_t>(p);
    return (p != nullptr || n == 0) && n <= std::numeric_limits<std::uintptr_t>::max() - a;
}
template<class T> inline bool output(T* p) noexcept {
    return p != nullptr && reinterpret_cast<std::uintptr_t>(p) % alignof(T) == 0 && span(p, sizeof(T));
}
inline bool overlaps(const void* a, std::size_t an, const void* b, std::size_t bn) noexcept {
    if (an == 0 || bn == 0) return false;
    const auto ap = reinterpret_cast<std::uintptr_t>(a);
    const auto bp = reinterpret_cast<std::uintptr_t>(b);
    return ap < bp + bn && bp < ap + an; // Call only after span validation.
}
}

// p must have four readable bytes. Other entry points validate their spans first.
inline std::uint32_t read_u32_le(const std::uint8_t* p) noexcept {
    return static_cast<std::uint32_t>(p[0]) |
           (static_cast<std::uint32_t>(p[1]) << 8) |
           (static_cast<std::uint32_t>(p[2]) << 16) |
           (static_cast<std::uint32_t>(p[3]) << 24);
}

// Declared input spans must be real readable storage. Outputs must be live,
// writable, aligned and disjoint from input. Rejection leaves output unchanged.
inline bool read_header(const std::uint8_t* bytes, std::size_t size, HeaderView* out) noexcept {
    if (!detail::span(bytes, size) || !detail::output(out) || size < 8 ||
        detail::overlaps(bytes, size, out, sizeof(*out)) || read_u32_le(bytes) != kMagicVoxN)
        return false;
    const HeaderView value{read_u32_le(bytes + 4)};
    *out = value;
    return true;
}

// Borrowed, immutable chunk-buffer storage must outlive each returned payload.
// Unknown tags are preserved as raw records; is_known_tag is classification only.
// No payload or codec is decoded. End/error leave both output and offset unchanged.
inline ChunkReadStatus next_chunk(const std::uint8_t* bytes, std::size_t size,
                                  std::size_t* offset, ChunkView* out) noexcept {
    if (!detail::span(bytes, size) || !detail::output(offset) || !detail::output(out) ||
        detail::overlaps(bytes, size, offset, sizeof(*offset)) ||
        detail::overlaps(bytes, size, out, sizeof(*out)) ||
        detail::overlaps(offset, sizeof(*offset), out, sizeof(*out)))
        return ChunkReadStatus::kMalformed;
    const std::size_t header = *offset;
    if (header == size) return ChunkReadStatus::kEnd;
    if (header > size || size - header < 8) return ChunkReadStatus::kMalformed;
    const std::uint32_t n = read_u32_le(bytes + header + 4);
    if (n > size - header - 8) return ChunkReadStatus::kMalformed;
    const ChunkView value{read_u32_le(bytes + header), header, bytes + header + 8, n};
    *out = value;
    *offset = header + 8 + static_cast<std::size_t>(n); // Bounded by size above.
    return ChunkReadStatus::kChunk;
}

struct ContainerLayout {
    std::uint32_t first_section_read_bytes;
    std::uint32_t declared_file_bytes;
    std::uint32_t audio_base;
    std::size_t chunk_file_offset;
    std::size_t chunk_bytes;
    std::size_t audio_bytes;
};
enum class LayoutReadStatus { kLayout, kMalformed, kUnsupportedHeader };

// Accepts a prefix (at least 24 readable bytes) and independent physical file
// length. It need not load audio bytes. Supports the observed +4 == 16 layout.
// Other header sizes remain explicit unsupported layouts. The original parser
// computes chunk bytes = audio_base - 16 - first_section_read_bytes; its stream
// starts at 8 + first_section_read_bytes. Eight bytes before audio_base remain
// outside the chunk window. This port adds declared-size/bounds checks.
inline LayoutReadStatus read_container_layout(const std::uint8_t* prefix,
        std::size_t prefix_size, std::size_t file_size, ContainerLayout* out) noexcept {
    if (!detail::span(prefix, prefix_size) || !detail::output(out) || prefix_size < 24 ||
        file_size < 24 || prefix_size > file_size ||
        detail::overlaps(prefix, prefix_size, out, sizeof(*out)) ||
        read_u32_le(prefix) != kMagicVoxN)
        return LayoutReadStatus::kMalformed;
    const std::uint32_t count = read_u32_le(prefix + 4);
    if (count != 16) return LayoutReadStatus::kUnsupportedHeader;
    const std::uint32_t declared = read_u32_le(prefix + 16);
    const std::uint32_t audio = read_u32_le(prefix + 20);
    if (declared != file_size || audio < 32 || audio > file_size)
        return LayoutReadStatus::kMalformed;
    const ContainerLayout value{count, declared, audio, 24,
        static_cast<std::size_t>(audio) - 32, file_size - audio};
    *out = value;
    return LayoutReadStatus::kLayout;
}

} // namespace dh2::engine_audio::native_format
#endif
