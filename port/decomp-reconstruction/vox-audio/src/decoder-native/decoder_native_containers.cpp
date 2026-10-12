// vox::DecoderNative container builders (the Create*Container methods).
#include <cstddef>

#include "vox-audio/vox/decoder_native.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {

// 0x87215c. The allocation is extra_bytes + 36 * (count - 1), the size of count 36-byte
// rule records in the 32-bit layout with the trailing extra bytes. The pointer is always
// stored; the count only when the allocation succeeds.
void* DecoderNative::CreateTransitionRulesContainer(int count, int extra_bytes) {
    (void)extra_bytes;  // the trailing bytes of the original allocation are not kept
    rules_.assign(static_cast<std::size_t>(count > 0 ? count : 0), TransitionRule{});
    return rules_.data();
}

// 0x872188. An array of count 32-bit state ids.
void* DecoderNative::CreateStatesContainer(int count) {
    states_.assign(static_cast<std::size_t>(count > 0 ? count : 0), 0);
    return states_.data();
}

// 0x8721ac. The allocation is extra_bytes + 24 * (count - 1), the 24-byte record layout of
// the 32-bit build. On success the count is stored and the cue table is replaced by one
// empty vector per segment (the original reserves, builds a temporary and assigns it).
void* DecoderNative::CreateSegmentsInfoContainers(int count, int extra_bytes) {
    (void)extra_bytes;  // the trailing bytes of the original allocation are not kept
    const std::size_t n = static_cast<std::size_t>(count > 0 ? count : 0);
    segments_.assign(n, NativeChunk{});
    segment_cues_ = std::vector<std::vector<int>>(n);
    return segments_.data();
}

// 0x872a84. Replaces the transition list with count empty lists.
void DecoderNative::CreateTransitionsContainer(unsigned int count) {
    transitions_ = std::vector<std::vector<TransitionParams>>(count);
}

// 0x872ac8 (partial). The original returns the result of NativePlaylistsManager::Init,
// which the port does not return; IsValid() stands in for it.
int DecoderNative::CreatePlaylistsContainer(int count) {
    playlists_.Init(count);
    return playlists_.IsValid() ? 1 : 0;
}

}  // namespace vox
