#include "../retained_pose_playback.hpp"
#include "../asset_catalog.hpp"
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace dh::foundation;
namespace {
void check(bool value,const std::string& error) { if(!value) throw std::runtime_error(error); }
std::vector<Vec3> vertices(const CharacterVisual& visual) {
    std::vector<Vec3> output;
    for(const auto& mesh:visual.meshes()) for(const auto& vertex:mesh.vertices) output.push_back(vertex.position);
    return output;
}
bool equal(const std::vector<Vec3>& a,const std::vector<Vec3>& b) {
    if(a.size()!=b.size()) return false;
    for(std::size_t i=0;i<a.size();++i)
        if(std::abs(a[i].x-b[i].x)+std::abs(a[i].y-b[i].y)+std::abs(a[i].z-b[i].z)>.0001f) return false;
    return true;
}
}
int main(int argc,char**argv) {
    try {
        check(argc==2,"Expected original asset root");
        AssetCatalog assets(argv[1]);CharacterVisual visual,reference;
        CharacterVisualConfig config;config.model_path="models/prince_modular.bdae";
        config.template_clip_path="animations/prince_template_anim.bdae";
        config.clips={{"idle","animations/prince_idle_shield.bdae"},{"walk","animations/prince_walk_1hand.bdae"}};
        config.skin_id_contains="_default_warrior-mesh-skin";config.expected_controller_count=4;
        config.motion_node_id="auto";config.consume_root_motion=true;
        std::string error;
        check(visual.load(assets,config,error) && reference.load(assets,config,error),error);
        RetainedPosePlayback playback(visual);
        check(!playback.advance(.01,error),"Unbound pose advanced");
        check(playback.select("idle",true,1.3f,100,error),error);
        check(playback.current_slot()==1 && playback.samples().size()==1,"First selection sampled zero-weight slot");
        const auto ordinary_clock=visual.animation_elapsed_seconds();
        check(playback.select("walk",false,1.3f,0,error),error);
        check(playback.current_slot()==0 && playback.blend_state().remaining==100,
              "Incoming fade ignored outgoing BlendOut");
        check(playback.advance(.05,error),error);
        check(playback.samples().size()==2 && std::abs(playback.blend_state().weights[0]-.5f)<1e-6f,
              "Wall-time fade or retained sampling differs");
        check(playback.slots()[0].timeline.current_ms==playback.slots()[0].timeline.start_ms,
              "Fresh incoming source timeline initialized before first weighted sample");
        check(playback.slots()[1].timeline.current_ms>playback.slots()[1].timeline.start_ms,
              "Outgoing retained timeline did not advance");
        OriginalPoseBlend expected;SkeletalPose pose;
        check(expected.select(100,error),error);
        auto sample=[&](std::uint32_t index,SkeletalPose& out,std::string& e) {
            const auto& slot=playback.slots()[index];
            return reference.sample_local_pose(slot.clip_id,slot.timeline.current_ms,out,e);
        };
        check(expected.evaluate(0,sample,pose,error) && expected.select(0,error)
              && expected.evaluate(50,sample,pose,error) && reference.apply_local_pose(pose,error),error);
        check(equal(vertices(visual),vertices(reference)),"Published mixed skin differs from source local pose mixture");
        check(playback.set_source_rate(0,error),error);
        const auto stopped0=playback.slots()[0].timeline.current_ms,stopped1=playback.slots()[1].timeline.current_ms;
        check(playback.advance(.05,error),error);
        check(playback.slots()[0].timeline.current_ms==stopped0 && playback.slots()[1].timeline.current_ms==stopped1,
              "SetScale zero did not pause both retained clocks");
        check(playback.blend_state().weights[0]==1 && playback.samples().size()==1,
              "Source rate incorrectly scaled wall fade");
        check(playback.set_source_rate(2,error) && playback.slots()[0].timeline.scale==2
              && playback.slots()[1].timeline.scale==2,"SetScale did not visit zero-weight retained slot");
        check(playback.advance(.5,error) && playback.current_ended(),"One-shot source timeline did not end");
        const auto ended=vertices(visual);check(playback.advance(.2,error),error);
        check(equal(ended,vertices(visual)),"One-shot terminal pose was not held");
        check(visual.animation_elapsed_seconds()==ordinary_clock,"Retained owner altered ordinary visual clock");
        const auto old_slots=playback.slots();const auto old_state=playback.blend_state();
        check(!playback.select("missing",false,1,10,error),"Unknown clip accepted");
        check(equal(ended,vertices(visual)) && playback.slots()[0].generation==old_slots[0].generation
              && playback.blend_state().current==old_state.current,"Failed selection mutated published pose/owner");
        check(!playback.advance(std::numeric_limits<double>::infinity(),error)
              && equal(ended,vertices(visual)),"Invalid interval mutated pose");
        check(playback.select("idle",true,1,0,error),error);
        check(playback.select("walk",true,1,0,error,33),error);
        check(playback.slots()[0].timeline.current_ms==playback.slots()[0].timeline.start_ms+33,
              "Same physical slot same clip loop replay ignored explicit extra");
        const auto walk_start=playback.slots()[0].timeline.start_ms;
        const auto walk_end=playback.slots()[0].timeline.end_ms;
        check(playback.advance(1,error) && !playback.current_ended(),error);
        check(playback.slots()[0].timeline.current_ms>=walk_start && playback.slots()[0].timeline.current_ms<=walk_end,
              "Source loop escaped authored range");
        for(const auto& sample_record:playback.samples())
            check(sample_record.slot==0 && sample_record.clip_id=="walk" && sample_record.generation==2,
                  "Explicit owner record lost slot/clip generation identity");
        playback.clear();check(!playback.current_ended() && !playback.advance(0,error),"Clear retained selected slot");
        std::cout<<"retained pose playback tests passed: original Prince clips, retained clocks, source-rate gates, wall fade, skin publication, hold/loop/replay\n";
    } catch(const std::exception& e) { std::cerr<<e.what()<<'\n'; return 1; }
}
