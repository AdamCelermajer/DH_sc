#pragma once
#include "visual_timeline.hpp"
#include "visual_motion.hpp"
#include "../game-data/animation_scheduler.hpp"
#include <map>

namespace dh2::actor {
using ClipBank=std::map<int,animation::Player>;
// Original triggered-event word0 is lag; word4 is the authored string pointer.
// The native ABI widens that immutable payload pointer. Sequence closure has
// event_id0x22 and null payload; it is not an authored trigger record.
struct EventHandoff {std::uint32_t event_id;std::int32_t lag_ms;const char* payload;};
enum EventPhase : std::uint32_t {scene_event=1,replay_event=2,sequence_event=3};
struct PlaybackEvent {
 EventHandoff handoff;std::int32_t clip;std::uint32_t timestamp,phase,reserved;
};
class Playback;
struct EventObserver {void* context=nullptr;void(*invoke)(void*,Playback&,const PlaybackEvent&)=nullptr;};
static_assert(sizeof(EventHandoff)==16&&sizeof(PlaybackEvent)==32&&sizeof(EventObserver)==16);
// Source scheduler/timeline/root bridge. Caller supplies authored state selection
// and the global speed producer. Does not implement Character FSM or blending.
class Playback {
 std::uint64_t event_generation=0;
 bool processing_completion=false;
 std::int32_t pending_sequence=-1;
 float pending_speed=1;
 bool apply_selection(const ClipBank&,visual::SceneBinding&,scene::Scene&,
                      float global_speed,std::string& error);
 bool animate(std::uint32_t,bool,std::uint32_t,const ClipBank&,
              visual::SceneBinding&,scene::Scene&,std::string&);
public:
 data::AnimationScheduler scheduler;
 timeline::State timeline{};
 timeline::Completion completion{};
 std::int32_t clip_id=-1;
 std::uint32_t root_timestamp=0,completions=0,restarts=0;
 bool displacement=false;
 // Optional synchronous observer. Borrowed bank/scene/Playback storage must
 // remain alive and immutable track bytes must not be erased during dispatch.
 // start() may be called from a trigger. The retained old manager finishes its
 // batch, then the outer pose re-fetches the active clip. start() during finite
 // closure uses original ANIM_Set's one pending-selection slot until callback
 // return. No independent event clock or deferred trigger queue is introduced.
 EventObserver observer{};
 animation::EventCursor event_cursor{};
 std::int32_t last_event_lag=0;
 std::uint32_t sequence_closed=0;
 Playback(){timeline.scale=1;timeline.library_present=1;timeline.clip_index=-1;}
 bool start(const data::AnimationTables&,int sequence,data::AnimationRandom&,
            const ClipBank&,visual::SceneBinding&,scene::Scene&,float global_speed,
            std::string& error);
 // Scene phase occurs before this frame's genuine physics Step.
 bool scene_phase(std::uint32_t absolute_ms,const ClipBank&,
                  visual::SceneBinding&,scene::Scene&,std::string& error);
 // Character animator phase occurs after Step. extra_ms is the applicator's
 // captured completion.extra_ms, retained from its transitional end callback.
 // It is not recalculated from the finalized timeline's current_ms.
 bool animator_phase(const data::AnimationTables&,data::AnimationRandom&,
                     const ClipBank&,visual::SceneBinding&,scene::Scene&,
                     float global_speed,std::int32_t extra_ms,std::string& error);
 bool set_speed(float global_speed,std::string& error);
 bool swap(const data::AnimationTables&,int desired,int old,data::AnimationRandom&,
           const ClipBank&,visual::SceneBinding&,scene::Scene&,float global_speed,
           std::string& error);
 // NewAnim synchronous service; public only for the replay callback adapter.
 bool replay_phase(std::uint32_t,const ClipBank&,visual::SceneBinding&,
                   scene::Scene&,std::string&);
};
}
extern "C" {
int dh2_actor_event_handoff(dh2::actor::EventHandoff*,std::int32_t*,const dh2::animation::TriggeredEvent*);
//1 emits original base-sequence closure,0 already closed,-1 malformed.
int dh2_actor_sequence_close(dh2::actor::EventHandoff*,std::uint32_t*);
}
