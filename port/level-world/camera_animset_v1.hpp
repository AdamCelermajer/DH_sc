#pragma once

#include "../game-data/animation_tables.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace dh2::camera_animset_v1 {

// CameraLevel::Load registers the selected CamAnimSet's resources in this
// order: Template, Idle, Shake, Crit, then CamAnims. Level::_LoadCamera
// starts only Idle at level entry. Keep every slot and its resolved source
// path so the renderer can load the selected set without guessing a filename.
struct Resource {
    enum class Slot : std::uint8_t { template_animation, idle, shake, crit, cam_animation };
    Slot slot=Slot::idle;
    // Ordinal of AddAnim in CameraLevel::Load; playback still uses clip_id.
    std::int32_t registration_order=-1;
    std::int32_t clip_id=-1;
    std::string path;
};

struct Selection {
    std::string name;
    std::size_t row=0;
    std::int32_t idle_clip_id=-1;
    std::string idle_path;
    std::vector<Resource> resources;
};

// Resolves the configured CamAnimSet row through the original animation clip
// dictionary. The idle clip is mandatory because _LoadCamera immediately
// calls PlayAnim(idle, 0, false); optional -1 slots are omitted.
bool select(const data::AnimationTables&,const data::Dictionary& clips,
            const std::string& name,Selection&,std::string& error);

enum class Trigger : std::uint8_t { level_idle, animation_step, object_event, combat_shake,
                                    combat_crit, script_crit, script_external };
struct PlayRequest {
    bool present=false;
    Trigger trigger=Trigger::level_idle;
    Resource resource{};
    bool preserve_zoom=false;
    bool loop=false;
    float speed=1.0f;
};

// Source-order playback request resolver. It does not own an animation
// controller; the renderer consumes a request and applies the source flags.
class PlaybackOwner {
public:
    bool bind(const Selection&);
    bool level_idle(PlayRequest&,std::string& error) const;
    bool animation_step(const data::AnimationStep&,std::int32_t selected_random_clip,
                        PlayRequest&,std::string& error) const;
    bool object_event(const std::string&,const data::Dictionary&,bool shake_allowed,
                      PlayRequest&,std::string& error) const;
    bool combat_shake(bool critical_result,bool requester_is_player,bool shake_allowed,
                      PlayRequest&,std::string& error) const;
    bool script_play_camera(bool use_crit,std::int32_t external_clip,
                            PlayRequest&,std::string& error) const;
private:
    bool request(std::int32_t,Resource::Slot,Trigger,bool,PlayRequest&,
                 std::string&) const;
    bool request_clip(std::int32_t,Trigger,bool,PlayRequest&,
                      std::string&) const;
    Selection selection_{};
    bool bound_=false;
};

} // namespace dh2::camera_animset_v1
