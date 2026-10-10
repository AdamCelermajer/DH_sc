#include "retained_animation_owner.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation {
namespace {
float source_add(float a,float b) { volatile float value=a+b;return value; }
float source_multiply(float a,float b) { volatile float value=a*b;return value; }
std::uint32_t word(const std::uint8_t* p) { return p[0]|std::uint32_t(p[1])<<8|std::uint32_t(p[2])<<16|std::uint32_t(p[3])<<24; }
}

bool RetainedAnimationOwner::bind_events(const std::string& clip,const std::uint8_t* bytes,
                                         std::size_t size,std::string& error) {
    error.clear();
    if(events_.count(clip)) { error="Retained event clip already bound";return false; }
    std::int32_t start=0,end=0;
    if(!visual_.animation_range(clip,start,end,error)) return false;
    dh2::resources::BresView bres{};
    if(dh2_bres_open(&bres,bytes,size)!=dh2::resources::BresError::ok
       || bres.root_offset>size || size-bres.root_offset<36) { error="Event source BRES rejected";return false; }
    if(word(bytes+bres.root_offset+28)!=std::uint32_t(start)
       || word(bytes+bres.root_offset+32)!=std::uint32_t(end)) {
        std::vector<EmbeddedSceneClip> ranges;
        if(!decode_embedded_scene_clips(bytes,size,ranges,error))return false;
        const auto found=std::find_if(ranges.begin(),ranges.end(),[&](const auto& range){
            return range.name==clip&&range.start_ms==start&&range.end_ms==end;
        });
        if(found==ranges.end()){error="Event source range differs from bound visual clip";return false;}
    }
    auto track=std::make_shared<dh2::animation::EventTrack>();
    if(!track->load(bres,error)) return false;
    events_.emplace(clip,std::move(track));return true;
}

bool RetainedAnimationOwner::select(const std::string& clip,bool loop,float rate,
                                    std::int32_t blend_out_ms,bool move_go,
                                    RetainedAnimationFrame& output,std::string& error,
                                    std::int32_t replay_extra_ms) {
    error.clear();
    if(!root_ || !events_.count(clip)) { error="Retained root sampler or original events are unbound";return false; }
    const auto before=pose_.slots();
    if(!pose_.select(clip,loop,rate,blend_out_ms,error,replay_extra_ms)) return false;
    move_go_=move_go;
    return collect(before,true,output,error);
}

bool RetainedAnimationOwner::advance(double seconds,RetainedAnimationFrame& output,std::string& error) {
    const auto before=pose_.slots();
    if(!pose_.advance(seconds,error)) return false;
    return collect(before,false,output,error);
}

bool RetainedAnimationOwner::collect(const std::array<RetainedPoseSlot,2>& before,
                                     bool selection,RetainedAnimationFrame& output,std::string& error) {
    error.clear();
    auto cursors=cursors_;auto generations=generations_;auto histories=histories_;
    auto completion=completion_;RetainedAnimationFrame frame;frame.move_go=move_go_;
    const auto& slots=pose_.slots();const auto& samples=pose_.samples();
    if(samples.empty()) { error="Retained animation has no sampled wall timestamp";return false; }
    const auto timestamp=samples.front().wall_timestamp_ms;
    for(std::uint32_t index=0;index<2;++index) {
        if(slots[index].generation!=generations[index]) {
            generations[index]=slots[index].generation;cursors[index]={};
        }
    }
    // Source NewAnim replay reset is gated by displacement/root/timestamp, and
    // resets only CURRENT history to clip start at timestamp+1. Other histories
    // survive selection, including zero-weight slots and same resource replay.
    if(selection && move_go_ && timestamp!=0) {
        const auto current=pose_.current_slot();std::array<float,3> point{};
        if(!root_(slots[current].clip_id,slots[current].timeline.start_ms,point,error)
           || dh2_visual_reset_delta(&histories[current],timestamp+1,point.data())) {
            if(error.empty()) error="Source current-slot root reset rejected";
            return false;
        }
    }
    for(const auto& sample:samples) {
        const auto& slot=slots[sample.slot];
        const auto track=events_.find(slot.clip_id);
        if(track==events_.end()) { error="Sampled retained clip has no original event track";return false; }
        struct Dispatch {RetainedAnimationFrame* output;const RetainedPoseSample* sample;};
        Dispatch context{&frame,&sample};
        const auto emit=[](const dh2::animation::TriggeredEvent* event,void* raw) {
            auto& c=*static_cast<Dispatch*>(raw);const auto& s=*c.sample;
            c.output->events.push_back({event->name,s.clip_id,s.slot,s.wall_timestamp_ms,s.generation,event->lag_ms});
        };
        if(!dh2_events_update(&track->second->view(),&cursors[sample.slot],sample.previous_ms,
                              sample.current_ms,slot.timeline.start_ms,slot.timeline.end_ms,emit,&context)) {
            error="Source retained event dispatch rejected";return false;
        }
        if(sample.slot==pose_.current_slot()) {
            // Timeline callback precedes final current_ms conversion. Preserve
            // prior integer current_ms while using post-clamp float/frame fields.
            bool crossed=sample.ended_now;
            if(slot.timeline.loop) {
                const float prior=sample.initialized_now ? float(sample.previous_ms)/1000.f
                    : before[sample.slot].timeline.current_seconds;
                const float reached=source_add(prior,sample.source_frame_seconds);
                crossed=reached>float(slot.timeline.end_ms)/1000.f;
            }
            if(crossed) {
                auto callback_state=slot.timeline;callback_state.current_ms=sample.previous_ms;
                if(dh2_timeline_notify(&completion,&callback_state)) { error="Source completion notification rejected";return false; }
            }
        }
    }
    // Keep a SINGLE scratch across physical slot0 then1. A callback representing
    // a default-less binding may retain the preceding slot's bytes deliberately.
    std::array<float,3> scratch{},sum{};
    for(std::uint32_t index=0;index<2;++index) {
        if(slots[index].bound && !root_(slots[index].clip_id,slots[index].timeline.current_ms,scratch,error)) return false;
        for(float coordinate:scratch) if(!std::isfinite(coordinate)) { error="Nonfinite authored root sample";return false; }
        if(dh2_visual_calculate_delta(&histories[index],timestamp,scratch.data())) { error="Source retained root delta rejected";return false; }
        const float weight=pose_.blend_state().weights[index];
        for(unsigned axis:{1u,2u,0u}) sum[axis]=source_add(sum[axis],source_multiply(weight,histories[index].value[axis]));
    }
    for(float value:sum) if(!std::isfinite(value)) { error="Retained weighted root delta overflow";return false; }
    frame.authored_motion={sum[0],sum[1],sum[2]};frame.completion=completion;
    cursors_=cursors;generations_=generations;histories_=histories;completion_=completion;
    output=std::move(frame);return true;
}

dh2::timeline::Completion RetainedAnimationOwner::take_completion() noexcept {
    const auto result=completion_;completion_.pending=0;return result;
}
void RetainedAnimationOwner::clear() noexcept {
    pose_.clear();cursors_={};generations_={};histories_={};completion_={};move_go_=false;
}

} // namespace dh::foundation
