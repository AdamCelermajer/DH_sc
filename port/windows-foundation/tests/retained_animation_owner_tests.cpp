#include "../retained_animation_owner.hpp"
#include "../asset_catalog.hpp"
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool value,const std::string& error) { if(!value) throw std::runtime_error(error); }
bool near(float a,float b) { return std::abs(a-b)<.0001f; }
}
int main(int argc,char**argv) {
    try {
        check(argc==2,"Expected original asset root");AssetCatalog assets(argv[1]);
        CharacterVisual visual;CharacterVisualConfig config;
        config.model_path="models/prince_modular.bdae";config.template_clip_path="animations/prince_template_anim.bdae";
        config.clips={{"idle","animations/prince_idle_shield.bdae"},{"walk","animations/prince_walk_1hand.bdae"},
                      {"attack","animations/prince_1hand_combo_01.bdae"}};
        config.skin_id_contains="_default_warrior-mesh-skin";config.expected_controller_count=4;
        config.motion_node_id="auto";config.consume_root_motion=true;
        std::string error;check(visual.load(assets,config,error),error);
        bool retain_idle=false;std::array<int,2> calls{};
        auto sample=[&](const std::string& clip,std::int32_t time,std::array<float,3>& scratch,std::string& e) {
            ++calls[clip=="walk"?0:1];if(retain_idle&&clip=="idle") return true;
            Vec3 value;if(!visual.sample_root_translation(clip,time,value,e)) return false;
            scratch={value.x,value.y,value.z};return true;
        };
        RetainedAnimationOwner owner(visual,sample);
        std::map<std::string,dh2::animation::EventTrack> tracks;
        for(const auto& clip:config.clips) {
            const auto bytes=assets.read(clip.second);
            check(owner.bind_events(clip.first,bytes.data(),bytes.size(),error),error);
            dh2::resources::BresView view{};check(dh2_bres_open(&view,bytes.data(),bytes.size())==dh2::resources::BresError::ok,"Event BRES");
            check(tracks[clip.first].load(view,error),error);
        }
        RetainedAnimationFrame frame;
        check(owner.select("idle",true,1,100,true,frame,error),error);
        check(owner.advance(.02,frame,error),error);retain_idle=true;
        check(owner.select("walk",false,1,0,true,frame,error),error);
        check(owner.advance(.05,frame,error),error);
        check(owner.advance(.05,frame,error),error);
        const auto& state=owner.pose();const auto& current=state.slots()[state.current_slot()];
        Vec3 point,start;
        check(visual.sample_root_translation("walk",current.timeline.current_ms,point,error),error);
        check(visual.sample_root_translation("walk",current.timeline.start_ms,start,error),error);
        check(near(frame.authored_motion.x,point.x-start.x)&&near(frame.authored_motion.y,point.y-start.y),
              "Weighted root movement differs from all-slot source history");
        check(std::hypot(frame.authored_motion.x,frame.authored_motion.y)>.0001f,"Root test used a stationary real sample");
        const auto zero=1-state.current_slot();
        check(state.blend_state().weights[zero]==0 && owner.root_histories()[zero].timestamp==120,
              "Zero-weight root history was not updated");
        check(near(owner.root_histories()[zero].previous[0],point.x)
              &&near(owner.root_histories()[zero].previous[1],point.y),"Default-less slot did not retain preceding scratch");
        check(calls[0]>0&&calls[1]>0,"All-slot root sampler gates differ");
        check(owner.advance(0,frame,error)&&frame.authored_motion.x==0&&frame.authored_motion.y==0,
              "Same timestamp root motion was applied twice");
        check(owner.select("walk",true,1,0,false,frame,error),error);
        check(!frame.move_go,"MoveGO false was ignored");
        check(owner.advance(.05,frame,error)&&!frame.move_go,"MoveGO false changed during root tracking");
        // Actual serialized attack markers and completion: compare every frame
        // with original native kernels, including callback's pre-current_ms state.
        retain_idle=false;
        check(owner.select("attack",false,1,0,true,frame,error),error);
        std::array<dh2::animation::EventCursor,2> cursors{};
        bool got_hit=false;
        for(double delta:{.01,.03,.1,.3,.4,.2}) {
            const auto before=owner.pose().slots();
            check(owner.advance(delta,frame,error),error);
            std::vector<RetainedAnimationEvent> expected;
            dh2::timeline::Completion source_completion{};
            for(const auto& record:owner.pose().samples()) {
                const auto& slot=owner.pose().slots()[record.slot];
                struct EventContext {std::vector<RetainedAnimationEvent>* output;const RetainedPoseSample* record;};
                EventContext context{&expected,&record};
                auto emit=[](const dh2::animation::TriggeredEvent* event,void* raw) {
                    const auto& c=*static_cast<EventContext*>(raw);const auto& r=*c.record;
                    c.output->push_back({event->name,r.clip_id,r.slot,r.wall_timestamp_ms,r.generation,event->lag_ms});
                };
                check(dh2_events_update(&tracks[slot.clip_id].view(),&cursors[record.slot],record.previous_ms,
                                       record.current_ms,slot.timeline.start_ms,slot.timeline.end_ms,emit,&context),"Reference events");
                if(record.slot==owner.pose().current_slot()) {
                    auto timeline=before[record.slot].timeline;
                    dh2::timeline::Services services{&source_completion,[](void* raw,dh2::timeline::State* state) {
                        check(dh2_timeline_notify(static_cast<dh2::timeline::Completion*>(raw),state)==0,"Source notify");
                    }};
                    std::int32_t stamp;std::memcpy(&stamp,&record.wall_timestamp_ms,4);
                    check(dh2_timeline_update(&timeline,stamp,&services)==0,"Reference timeline");
                }
            }
            check(expected.size()==frame.events.size(),"Source event batch size differs");
            for(std::size_t index=0;index<expected.size();++index) {
                check(expected[index].name==frame.events[index].name&&expected[index].lag_ms==frame.events[index].lag_ms
                      &&expected[index].slot==frame.events[index].slot,"Source event order/lag differs");
                got_hit|=frame.events[index].name=="attack_mainhand";
            }
            const auto actual=owner.take_completion();
            check(actual.pending==source_completion.pending
                  &&(!actual.pending||actual.extra_ms==source_completion.extra_ms),"Source pre-final completion remainder differs");
        }
        check(got_hit,"Real attack_mainhand marker never dispatched");
        owner.clear();
        check(owner.select("attack",false,1,100,true,frame,error),error);
        check(owner.advance(.01,frame,error),error);
        const auto outgoing=owner.pose().current_slot();
        check(owner.select("idle",true,1,0,false,frame,error),error);
        check(owner.advance(.03,frame,error),error);
        bool outgoing_hit=false;
        for(const auto& event:frame.events) outgoing_hit|=event.name=="attack_mainhand"&&event.slot==outgoing;
        check(outgoing_hit && outgoing!=owner.pose().current_slot(),"Fading outgoing authored marker suppressed");
        owner.take_completion();
        const auto before_loop=owner.pose().slots();
        const auto loop_length=before_loop[owner.pose().current_slot()].timeline.length_seconds;
        check(owner.advance(double(loop_length)+.2,frame,error),error);
        dh2::timeline::Completion expected_loop{};
        for(const auto& record:owner.pose().samples()) if(record.slot==owner.pose().current_slot()) {
            auto source=before_loop[record.slot].timeline;
            dh2::timeline::Services services{&expected_loop,[](void* raw,dh2::timeline::State* state) {
                check(dh2_timeline_notify(static_cast<dh2::timeline::Completion*>(raw),state)==0,"Loop source notify");
            }};
            std::int32_t stamp;std::memcpy(&stamp,&record.wall_timestamp_ms,4);
            check(dh2_timeline_update(&source,stamp,&services)==0,"Loop source update");
        }
        const auto loop_completion=owner.take_completion();
        check(loop_completion.pending==1 && expected_loop.pending==1
              &&loop_completion.extra_ms==expected_loop.extra_ms,"Loop completion callback remainder differs");
        const auto old_current=owner.pose().current_slot();
        check(!owner.select("unknown",false,1,0,true,frame,error)
              &&owner.pose().current_slot()==old_current,"Unbound event resource changed playback");
        owner.clear();check(!owner.pose().current_ended(),"Clear retained completion");
        std::cout<<"retained animation owner tests passed: exact source real markers/completion, all-slot roots, scratch retention, timestamp gate, MoveGO\n";
    } catch(const std::exception& e) { std::cerr<<e.what()<<'\n';return 1; }
}
