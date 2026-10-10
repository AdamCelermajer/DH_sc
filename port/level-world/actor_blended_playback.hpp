#pragma once
#include "actor_playback.hpp"
#include "animation_blender.hpp"
#include "../engine-animation/animation_registration.hpp"
#include <array>

namespace dh2::actor {
class BlendedPlayback;
struct BlendedPlaybackEvent {
 PlaybackEvent event;
 std::uint32_t slot;
};
struct BlendedEventObserver {
 void* context=nullptr;
 void(*invoke)(void*,BlendedPlayback&,const BlendedPlaybackEvent&)=nullptr;
};
// Source _SetAnimStep's effects leaf: event26 and redirects precede it;
// selected clip blend/PlayClip and speed stores follow it. The immutable
// retained row is the scheduler's captured source step, not a guessed FX ID.
struct BlendedSelectionFxServicesV2 {
 void* context=nullptr;
 bool(*invoke)(void*,BlendedPlayback&,const data::AnimationStep&,std::string&)=nullptr;
};
enum BlendedEventPhase : std::uint32_t {animator_event=4,selection_event=5};
struct PlaybackSlot {
 timeline::State timeline{};
 animation::EventCursor event_cursor{};
 visual::Delta root_history{};
 std::int32_t clip_id=-1,compiled_clip=-1;
 std::uint64_t generation=0;
 std::vector<std::int32_t> key_cursors;
 PlaybackSlot(){timeline.scale=1;timeline.library_present=1;timeline.clip_index=-1;}
};
// Recovered two-slot node-transform domain. Compile once from the immutable
// authored scene template, then supply the same bank/graph identity at runtime.
// The bank must not be erased/recompiled from a synchronous observer.
class BlendedPlayback {
 struct TargetValues {std::vector<float> values;};
 animation::TransformSet compiled;
 const ClipBank* compiled_bank=nullptr;
 std::vector<animation::RegistrationEntry> game_registration;
 std::vector<std::int32_t> engine_dictionary_ids;
 std::vector<std::string> node_identities;
 std::vector<TargetValues> target_values;
 std::vector<std::uint32_t> target_dirty;
 std::int32_t root_target=-1;
 float global_speed=1;
 std::int32_t pending_sequence=-1;
 float pending_speed=1;
 bool ready(const ClipBank&,const visual::SceneBinding&,const scene::Scene&,std::string&)const;
 bool apply_selection(const ClipBank&,visual::SceneBinding&,scene::Scene&,std::string&);
 bool advance_slot(std::uint32_t,std::uint32_t,std::uint32_t,std::string&);
 bool animate(std::uint32_t,bool,std::uint32_t,const ClipBank&,visual::SceneBinding&,scene::Scene&,std::string&);
 bool reset_current(std::uint32_t,std::string&);
 void scheduler_event(std::uint32_t);
 bool bind_compiled(animation::TransformSet&&,const ClipBank&,const scene::Scene&,
                    const visual::SceneBinding&,bool initial_library_zero,std::string&,
                    bool occurrence_mapping=false);
public:
 data::AnimationScheduler scheduler;
 data::AnimationSelectionPolicyServicesV126 selection_policy_v126{};
 animation::BlenderState blend{0,0,0,0,0,0,{1,0}};
 std::array<PlaybackSlot,2> slots;
 // Aggregate applicator capture precedes pose; actor pending is raised only
 // by the current-slot CheckCallback after pose/root (or time-only) work.
 timeline::Completion applicator_completion{},completion{};
 visual::Delta aggregate{};
 std::uint32_t root_timestamp=0,completions=0,restarts=0,sequence_closed=1;
 bool displacement=false;
 bool stop_requested=false; // original animator+4a; consumed before pending gate
 // Authored28, finite22, Update27/25/23 and selection24/26 synchronously route
 // here. Scheduler events have null payload; their RaiseEvent returns are
 // ignored by the animator (AI forwarding remains an explicit caller service).
 BlendedEventObserver observer{};
 // Optional for historical metadata-only playback callers. Production actor
 // binding supplies the real manager; positive required failures stop before
 // blend/PlayClip while retaining source event26 mutations.
 BlendedSelectionFxServicesV2 selection_fx_v2{};
 std::int32_t last_event_lag=0;
 // Source enabled/bound target gates. Unbound compiled channels remain in the
 // ordered union, but receive neither sampling nor a scene setter.
 std::vector<std::uint8_t> target_enabled;
 bool compile(const ClipBank&,const scene::Scene& authored_template,
              const visual::SceneBinding&,std::string& error);
 bool source_rebind_render_v109(const ClipBank&,const animation::RegistrationSet&,const scene::Scene&,const visual::SceneBinding&,std::string&);
 // Explicit original resource registration order. Default overload uses the
 // native map's numeric-ID order, which is a caller producer, not an original
 // AnimationSet library-order claim.
 bool compile(const ClipBank&,const std::vector<std::int32_t>& clip_order,
              const scene::Scene& authored_template,const visual::SceneBinding&,
              std::string& error);
 // Actual dynamic library producer uses explicit registration order and a
 // registered default resource; scene graph supplies binding identities only.
 // Fresh AnimatorSets select library0 (including a first-registered template).
 bool compile_dynamic(const ClipBank&,const std::vector<std::int32_t>& clip_order,
                      const scene::Scene& scene_bindings,const visual::SceneBinding&,
                      std::string& error,const animation::Player* default_library=nullptr,
                      animation::TransformMismatchBehavior mismatch=animation::TransformMismatchBehavior::retain);
 // Source occurrence library and refreshed first-resource-identity game map.
 // Compiles every occurrence under its engine index; owns copied lookup
 // metadata/resources/events, so RegistrationSet may be destroyed afterward.
 // The explicit unique bank remains immutable/alive as the runtime binding.
 // Each occurrence must reference a canonical bank Player; a present bank
 // dictionary key must match that exact pointer. Alias dictionary keys may
 // share a canonical bank Player. Resource identities and pointers must map
 // one-to-one, and every bank Player must be registered. Default is a registered
 // bank Player with the matching canonical identity. Failure preserves playback.
 bool compile_dynamic(const ClipBank&,const animation::RegistrationSet&,
                      const scene::Scene& scene_bindings,const visual::SceneBinding&,
                      std::string& error,
                      animation::TransformMismatchBehavior mismatch=animation::TransformMismatchBehavior::retain);
 std::int32_t engine_index(std::int32_t dictionary_id)const;
 const animation::TransformClip* compiled_clip_v4(std::int32_t dictionary_id)const{
  const auto index=engine_index(dictionary_id);return index<0?nullptr:compiled.clip(std::size_t(index));
 }
 std::int32_t dictionary_id(std::int32_t engine_index)const;
 const animation::TransformSet& transform_set()const{return compiled;}
 const std::vector<float>& values(std::size_t target)const{return target_values.at(target).values;}
 timeline::State& current_timeline(){return slots.at(blend.current).timeline;}
 const timeline::State& current_timeline()const{return slots.at(blend.current).timeline;}
 std::int32_t current_clip()const{return slots.at(blend.current).clip_id;}
 std::int32_t current_engine_clip()const{return slots.at(blend.current).compiled_clip;}
 bool start(const data::AnimationTables&,int,data::AnimationRandom&,const ClipBank&,
            visual::SceneBinding&,scene::Scene&,float,std::string&);
 bool swap(const data::AnimationTables&,int desired,int old,data::AnimationRandom&,
           const ClipBank&,visual::SceneBinding&,scene::Scene&,float,std::string&);
 bool set_speed(float,std::string&);
 std::uint32_t animation_depth()const{return scheduler.frames().empty()?0u:std::uint32_t(scheduler.frames().size()-1);}
 std::uint32_t step_index()const{return sequence_closed?UINT32_MAX:scheduler.frames().empty()?0u:scheduler.frames().back().step;}
 std::uint32_t step_count(const data::AnimationTables&)const;
 void stop_loop(bool complete_next_update);
 // Whole source immediate ANIM_Stop/BlendedController.StopClip(true,0).
 // Null visual resets depth only. A present controller stops every retained
 // timeline, jumps the selected timeline to its end, then raises22 once when
 // the source sequence was open. Existing deferred StopLoop stays unchanged.
 bool stop_immediate_v1(bool visual_present,std::string&);
 void skip_next_step(){if(!sequence_closed)scheduler.skip_next_step();}
 // Public ANIM_SetStep mutates metadata only; it is distinct from private
 // _SetAnimStep, which prepares a clip and raises26 during selection.
 void set_step(std::uint32_t index){if(!sequence_closed)scheduler.set_step(index);}
 bool scene_phase(std::uint32_t,const ClipBank&,visual::SceneBinding&,scene::Scene&,std::string&);
 // Culled-node path: advances nonzero timelines/events, normalizes, checks
 // completion and commits time. It samples neither pose nor root history.
 bool time_phase(std::uint32_t,const ClipBank&,visual::SceneBinding&,scene::Scene&,std::string&);
 bool animator_phase(const data::AnimationTables&,data::AnimationRandom&,const ClipBank&,
                     visual::SceneBinding&,scene::Scene&,float,std::int32_t extra_ms,std::string&);
 bool replay_phase(std::uint32_t,const ClipBank&,visual::SceneBinding&,scene::Scene&,std::string&);
};
}
