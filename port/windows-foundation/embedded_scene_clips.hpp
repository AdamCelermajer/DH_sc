#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace dh::foundation {
struct EmbeddedSceneClip { std::string name;std::int32_t start_ms{},end_ms{}; };
// Own names/ranges from the BDAE animation_clip library. No guessed durations.
// Failure preserves output; a static scene legitimately has an empty library.
bool decode_embedded_scene_clips(const std::uint8_t*,std::size_t,
    std::vector<EmbeddedSceneClip>&,std::string& error);
}
