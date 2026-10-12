// vox::DecoderNative container builders (the Create*Container methods).
#include <cstddef>

#include "vox-audio/vox/decoder_native.hpp"
#include "vox-audio/vox/memory.hpp"

namespace vox {

// 0x87215c (partial). The allocation is extra_bytes + 36 * (count - 1), the size of
// count 36-byte rule records in the 32-bit layout with the trailing extra bytes. The
// record fields are not ported, so only the pointer and count are set.
void* DecoderNative::CreateTransitionRulesContainer(int count, int extra_bytes) {
    void* result = VoxAlloc(static_cast<std::size_t>(extra_bytes + 36 * count - 36));
    rules_ = result;
    if (result != nullptr) {
        rules_count_ = count;
    }
    return result;
}

// 0x872188. An array of count 32-bit state ids.
void* DecoderNative::CreateStatesContainer(int count) {
    void* result = VoxAlloc(sizeof(std::int32_t) * static_cast<std::size_t>(count));
    states_ = static_cast<std::int32_t*>(result);
    if (result != nullptr) {
        states_count_ = count;
    }
    return result;
}

// 0x8721ac (partial). The allocation is extra_bytes + 24 * (count - 1), the 24-byte
// record layout of the 32-bit build. The per-segment cue vectors are reproduced: one
// empty vector per segment.
void* DecoderNative::CreateSegmentsInfoContainers(int count, int extra_bytes) {
    void* result = VoxAlloc(static_cast<std::size_t>(extra_bytes + 24 * count - 24));
    segments_info_ = result;
    if (result != nullptr) {
        segments_count_ = count;
        segment_cues_ = std::vector<std::vector<int>>(static_cast<std::size_t>(count));
    }
    return result;
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
