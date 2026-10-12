#pragma once
#include "../scene-materials/scene.hpp"
#include "events.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace dh2::animation {
enum class MissingTargets {reject,ignore};
struct Track {
    std::uint32_t node,type,segment;
    assets::Animation accessor;
    assets::Vector values;
};
class Player {
    friend class TransformSet;
    std::vector<std::uint8_t> bytes;
    std::vector<scene::Node> rest;
    std::vector<Track> tracks;
    std::vector<std::array<std::int32_t,2>> ranges;
public:
    EventTrack events;
    Player()=default;
    Player(const Player&)=delete;
    Player& operator=(const Player&)=delete;
    Player(Player&&) noexcept=default;
    Player& operator=(Player&&) noexcept=default;
    std::int32_t start=0,end=0;
    unsigned skipped=0;
    unsigned unbound=0;
    bool load(const std::uint8_t*,std::size_t,const scene::Scene&,std::string& error,MissingTargets=MissingTargets::reject);
    bool sample(scene::Scene&,std::int32_t milliseconds,std::string& error)const;
    unsigned track_count()const{return ranges.empty()?0:tracks.size()/ranges.size();}
    unsigned segment_count()const{return ranges.size();}
};

// Bounded native compiled animation-set ABI. Static sets accept full node
// position/quaternion/scale; dynamic sets also accept position-axis2..4 and
// authored scalar-angle9 interpreters. Resources and event names are owned.
struct TransformClipInput {std::int32_t id;const Player* player;};
enum class TransformTemplatePolicy {authored,none};
enum class TransformMismatchBehavior {prune=0,retain=1};
struct TransformTarget {
    std::string uri;
    std::uint32_t type,node,components; // node==UINT32_MAX: no scene binding
};
struct TransformBinding {
    std::uint32_t mode=1; // 2: accessor, 1: default or retained caller bytes
    bool has_default=false;
    float default_value[4]{};
};
struct TransformClip {
    std::int32_t id=0,start=0,end=0;
    EventTrack events;
};
class TransformSet {
    struct Storage {
        std::vector<std::uint8_t> bytes;
        std::vector<Track> tracks;
        std::vector<std::array<std::int32_t,2>> ranges;
    };
    std::vector<Storage> storage_;
    std::vector<TransformClip> clips_;
    std::vector<TransformTarget> targets_;
    std::vector<TransformBinding> bindings_;
    std::vector<std::uint32_t> animations_; // clip-major original track index
    bool compile_internal(const std::vector<TransformClipInput>&,const scene::Scene&,std::string&,
                          TransformTemplatePolicy,bool dynamic_channels);
public:
    TransformSet()=default;
    TransformSet(const TransformSet&)=delete;
    TransformSet& operator=(const TransformSet&)=delete;
    TransformSet(TransformSet&&) noexcept=default;
    TransformSet& operator=(TransformSet&&) noexcept=default;
    // Failure preserves this set. Template must be the authored, uncompensated
    // transformation graph. Static component/material/compressed channels fail
    // explicitly, including channels unbound in the legacy Player.
    bool compile(const std::vector<TransformClipInput>&,const scene::Scene& authored_template,std::string& error,
                 TransformTemplatePolicy=TransformTemplatePolicy::authored);
    // Original CDynamicAnimationSet domain1/2/3/4/5/9/10. Supplied library order
    // is authoritative; scene_bindings only resolves node IDs. Default values
    // come from each clip DB, then the optional designated default DB for both
    // modes. Strict0 prunes targets missing track AND clip default in any clip
    // before fallback; the game CreateAnimSet producer selects retain1.
    // Source compatibility merges1..4 and5/9, preserving the first handler.
    // Sampling dispatches each raw track's interpreter independently. Angle9
    // requires its authored axis default; generic6..8/11..13 stay unsupported.
    bool compile_dynamic(const std::vector<TransformClipInput>&,const scene::Scene& scene_bindings,std::string& error,
                         const Player* default_library=nullptr,
                         TransformMismatchBehavior=TransformMismatchBehavior::retain);
    const std::vector<TransformTarget>& targets()const{return targets_;}
    std::size_t clip_count()const{return clips_.size();}
    const TransformClip* clip(std::size_t index)const{return index<clips_.size()?&clips_[index]:nullptr;}
    std::int32_t find_clip(std::int32_t id)const;
    const TransformBinding* clip_target(std::size_t clip_index,std::size_t target_index)const;
    // No clock, event dispatch, node setter or scene reset. The same operation
    // serves a pose slot and shared root scratch. mode1/null leaves out intact.
    // Optional cursor receives the selected key. Original neighbor search has
    // inclusive upper boundaries, so prior cursor can affect exact-key selection.
    // A null cursor starts at zero. Rejects preserve both output and cursor.
    bool sample(std::size_t clip_index,std::size_t target_index,std::int32_t milliseconds,
                float* out,std::size_t capacity,std::int32_t* key_cursor,std::string& error,
                bool interpolate=true)const;
};
}
// Preserves original float-3 linear interpolation evaluation order.
extern "C" void dh2_animation_lerp3(float* out,const float* a,const float* b,float fraction);
extern "C" void dh2_animation_quaternion(float* out,const float* a,const float* b,float fraction);
