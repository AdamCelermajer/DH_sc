#pragma once

#include "camera_animset_v1.hpp"
#include "camera_level_runtime_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::camera_animset_playback_v1 {

enum class Status : std::uint8_t { invalid_request, no_request, play_rejected, started };

// The renderer/AnimSetController owns the actual clip player. This mandatory
// callback is the provider seam; the adapter owns only CameraLevel's existing
// zoom/start-state transition and never creates another camera or timer.
using StartResource = bool (*)(void*,const camera_animset_v1::PlayRequest&);
struct Backend { void* context=nullptr; StartResource start_resource=nullptr; };

Status start(camera_level_runtime_v1::Owner&,const camera_animset_v1::PlayRequest&,
             Backend,std::string& error);

} // namespace dh2::camera_animset_playback_v1
