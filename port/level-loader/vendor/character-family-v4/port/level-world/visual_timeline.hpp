#pragma once
#include <cstdint>
namespace dh2::timeline {
// Logical CTimelineController fields, not a binary engine overlay. Millisecond
// state is signed like original update(int); engine timestamps may wrap.
struct State {
 std::int32_t current_ms,start_ms,end_ms;
 std::uint32_t loop;
 float frame_seconds,start_seconds,length_seconds,last_seconds,current_seconds,scale;
 std::uint32_t library_present;
 std::int32_t clip_index;
 std::uint32_t ended,initialized;
};
// Called before final current_ms is recomputed, after clamp/wrap and ended
// writes. Reentrant jumps/range/scale updates are supported as in the original.
using Callback=void (*)(void*,State*);
struct Services {void* context;Callback invoke;};
struct Completion {std::int32_t extra_ms;std::uint32_t pending;};
// Explicit facts AFTER the existing scheduler/AnimatorSet has selected and
// bound the requested animation. Selection/blend weights are not invented.
struct ReplayFacts {
 std::int32_t previous_clip,mapped_clip,applicator_extra_ms;
 std::uint32_t requested_loop,displacement,root_present,root_timestamp,reserved;
};
struct ReplayResult {std::int32_t current_ms;std::uint32_t timestamp,restart,displacement;};
enum ReplayEvent : std::uint32_t {new_animation=1,blend_post=2};
using ReplayCallback=void (*)(void*,std::uint32_t,State*,const ReplayResult*);
struct ReplayServices {void* context;ReplayCallback invoke;};
static_assert(sizeof(State)==56&&sizeof(Services)==16&&sizeof(Completion)==8);
static_assert(sizeof(ReplayFacts)==32&&sizeof(ReplayResult)==16&&sizeof(ReplayServices)==16);
}
extern "C" {
// 0 success; -1 malformed caller contract before mutation. IEEE values remain
// permitted. Flag storage represents bytes (0..255), library_present is 0/1.
int dh2_timeline_update(dh2::timeline::State*,std::int32_t absolute_ms,const dh2::timeline::Services*);
int dh2_timeline_jump(dh2::timeline::State*,std::int32_t milliseconds);
int dh2_timeline_init(dh2::timeline::State*,std::int32_t start,std::int32_t end);
int dh2_timeline_range(dh2::timeline::State*,std::int32_t start,std::int32_t end,std::uint32_t jump);
// Bounds are explicit resolved clip facts; original virtual clip lookup remains
// in the existing authored-clip loader/scheduler.
int dh2_timeline_clip(dh2::timeline::State*,std::int32_t index,std::int32_t start,std::int32_t end);
int dh2_timeline_loop(dh2::timeline::State*,std::uint32_t loop);
int dh2_timeline_scale(dh2::timeline::State*,float scale);
// Original Animator::_HandleAnimEnding and CharAnimator::CalculateExtraTime.
// Null timeline leaves extra unchanged; notification still sets pending=1.
int dh2_timeline_notify(dh2::timeline::Completion*,const dh2::timeline::State*);
int dh2_timeline_extra(std::int32_t* extra,const dh2::timeline::State*);
// Recovered post-selection tail of BlendedAnimSetController::PlayClip.
// Returns1 accepted,0 mapped clip=-1,-1 malformed. new_animation executes the
// owner's NewAnim backend; its callback may synchronously update the timeline
// at root_timestamp and call SceneBinding::sample(current_ms,timestamp,restart).
// The blended controller does not set scene bit0x200 here; the separate base
// controller's PlayClip does. Existing scene ownership policy retains that.
int dh2_timeline_replay(dh2::timeline::ReplayResult*,dh2::timeline::State*,const dh2::timeline::ReplayFacts*,const dh2::timeline::ReplayServices*);
}
