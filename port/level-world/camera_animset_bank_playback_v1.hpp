#pragma once

#include "camera_animset_bank_v1.hpp"
#include "camera_animset_playback_v1.hpp"
#include "player_camera_rig_v1.hpp"

#include <cstddef>
#include <cstdint>
#include <string>

namespace dh2::camera_animset_bank_playback_v1 {

enum class Status : std::uint8_t {
    invalid_request,
    no_request,
    bank_resource_missing,
    playback_rejected,
    started
};

// The active camera animator must consume/copy this BDAE synchronously. The
// bank retains the bytes; this adapter introduces no camera or animation clock.
using StartBdae = bool (*)(void*,const camera_animset_v1::PlayRequest&,
                           const std::uint8_t*,std::size_t);
struct Backend { void* context=nullptr; StartBdae start_bdae=nullptr; };

Status start(camera_level_runtime_v1::Owner&,const camera_animset_bank_v1::Owner&,
             const camera_animset_v1::PlayRequest&,Backend,std::string& error);

// Concrete binding for the app's one authored camera graph and one timeline.
Status start_rig(camera_level_runtime_v1::Owner&,const camera_animset_bank_v1::Owner&,
                 const camera_animset_v1::PlayRequest&,
                 player_camera_rig_v1::Rig&,player_camera_rig_v1::Playback&,
                 std::string& error);

// Advances the one source camera timeline and mirrors CameraLevel::__Callback:
// when its non-looping clip finishes, clear the active-animation zoom mode.
bool advance_rig(camera_level_runtime_v1::Owner&,player_camera_rig_v1::Rig&,
                 player_camera_rig_v1::Playback&,std::uint32_t dt_ms,
                 player_camera_rig_v1::Pose*,std::string& error);

} // namespace dh2::camera_animset_bank_playback_v1
