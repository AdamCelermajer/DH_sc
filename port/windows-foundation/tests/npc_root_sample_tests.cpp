#include "original_character.hpp"
#include "original_melee_bindings.hpp"
#include "content_paths.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& error){if(!value)throw std::runtime_error(error);}
std::vector<Vec3> snapshot(const CharacterVisual& visual){std::vector<Vec3> result;for(const auto& mesh:visual.meshes())for(const auto& vertex:mesh.vertices)result.push_back(vertex.position);return result;}
bool same(const std::vector<Vec3>& a,const std::vector<Vec3>& b){if(a.size()!=b.size())return false;for(std::size_t i=0;i<a.size();++i)if(a[i].x!=b[i].x||a[i].y!=b[i].y||a[i].z!=b[i].z)return false;return true;}
int main(int argc,char**argv){try {
    check(argc==2,"workspace root required");const auto root=std::filesystem::path(argv[1]);AssetCatalog metadata(root/".local-inputs/windows-melee-bindings"),assets(root/".local-inputs/windows-population-assets/original-cache");OriginalMeleeBindings bindings;std::string error;check(bindings.load(metadata,"original-melee-bindings.xml",error),error);const auto* actor=bindings.find_actor("Swamp_LizadMan_Type1");check(actor,"original lizard profile missing");
    CharacterVisualConfig config;config.model_path=resolve_content_path(assets,actor->model).lexically_relative(assets.root()).generic_string();config.template_clip_path=resolve_content_path(assets,actor->templateClip).lexically_relative(assets.root()).generic_string();config.allow_missing_animation_targets=true;config.motion_node_id="auto";config.consume_root_motion=true;
    struct Phase{std::string name;const OriginalMeleeStep* source;};std::vector<Phase> phases;
    for(const auto& name:{"Idle","Walk","Spawn","Injured","Died"}) {const auto& sequence=actor->states.at(name).at(0);check(sequence.steps.size()==1,"source diagnostic state shape differs");phases.push_back({name,&sequence.steps[0]});}
    const auto& attack=actor->states.at("Attack").at(0);check(!attack.steps.empty()&&attack.steps[0].redirect==1,"source lizard attack group missing");for(std::size_t i=0;i<attack.steps[0].children.size();++i)phases.push_back({"Attack"+std::to_string(i),&attack.steps[0].children[i]});
    for(const auto& phase:phases)config.clips.emplace_back(phase.name,resolve_content_path(assets,phase.source->uri).lexically_relative(assets.root()).generic_string());
    CharacterVisual visual;check(visual.load(assets,config,error),error);const auto original=snapshot(visual);const auto clock=visual.animation_elapsed_seconds();
    auto reverseConfig=config;std::reverse(reverseConfig.clips.begin(),reverseConfig.clips.end());CharacterVisual reordered;check(reordered.load(assets,reverseConfig,error),error);
    for(const auto& phase:phases) {
        std::int32_t start=0,end=0;check(visual.animation_range(phase.name,start,end,error),error);
        for(const auto time:{start,start+(end-start)/2,end}) {
            Vec3 a{},b{};check(visual.sample_root_translation(phase.name,time,a,error),error);check(reordered.sample_root_translation(phase.name,time,b,error),error);check(a.x==b.x&&a.y==b.y&&a.z==b.z,"source default root depends on initial bank clip order");
            Vec3 exact{7,8,9};check(visual.sample_source_root_translation(phase.name,time,exact,error),error);check(a.x==exact.x&&a.y==exact.y&&a.z==exact.z,"complete pose root differs from original dynamic applicator target on genuine NPC clip");
            SkeletalPose left,right;check(visual.sample_local_pose(phase.name,time,left,error),error);check(reordered.sample_local_pose(phase.name,time,right,error),error);check(left.size()==right.size(),"reordered source pose domain differs");for(std::size_t i=0;i<left.size();++i)check(left[i].translation==right[i].translation&&left[i].quaternion==right[i].quaternion&&left[i].scale==right[i].scale,"source default SRT depends on initial bank clip order");
        }
    }
    for(const auto& phase:phases){check(reordered.restart(phase.name,false,error),error);check(reordered.update(.07,error),error);SkeletalPose independent;std::int32_t start=0,end=0;check(visual.animation_range(phase.name,start,end,error),error);check(visual.sample_local_pose(phase.name,std::min(end,start+70),independent,error),error);CharacterVisual publication;check(publication.load(assets,config,error),error);check(publication.apply_local_pose(independent,error),error);check(same(snapshot(publication),snapshot(reordered)),"idle/walk/spawn/attack direct and independent skin publication differ");}
    for(const auto& phase:phases) {
        std::int32_t start=0,end=0;check(visual.animation_range(phase.name,start,end,error),error);const double duration=(std::int64_t(end)-start)/1000.0;
        Vec3 origin{},endpoint{};check(visual.sample_root_translation(phase.name,start,origin,error),error);check(visual.sample_root_translation(phase.name,end,endpoint,error),error);RootMotionHistory history;Vec3 sum{};double travel=0,peakXY=0;std::uint32_t stamp=100;
        for(unsigned frame=1;frame<=60;++frame){Vec3 delta{};check(visual.step_root_motion(phase.name,duration*frame/60,false,stamp++,history,delta,error),error);sum.x+=delta.x;sum.y+=delta.y;sum.z+=delta.z;travel+=std::hypot(delta.x,delta.y);peakXY=std::max(peakXY,double(std::hypot(history.previous.x-origin.x,history.previous.y-origin.y)));}
        check(std::abs(sum.x-(endpoint.x-origin.x))+std::abs(sum.y-(endpoint.y-origin.y))+std::abs(sum.z-(endpoint.z-origin.z))<.02,"independent root deltas do not telescope to authored endpoint");
        const float scaledX=sum.x*.8999999612569809f,scaledY=sum.y*.8999999612569809f;
        std::cout<<phase.name<<" MoveGO="<<phase.source->moveGO<<" rate="<<phase.source->speed<<" range="<<start<<".."<<end<<" origin="<<origin.x<<","<<origin.y<<","<<origin.z<<" raw="<<sum.x<<","<<sum.y<<","<<sum.z<<" hostXY="<<scaledX<<","<<scaledY<<" travel="<<travel<<" peakXY="<<peakXY<<"\n";
        if(phase.name=="Spawn")check(phase.source->moveGO==1,"source spawn jump displacement policy changed");
        if(phase.name=="Walk") {RootMotionHistory looped;Vec3 loopDelta;check(visual.step_root_motion(phase.name,duration*3,true,999,looped,loopDelta,error),error);check(std::abs(loopDelta.y-3*(endpoint.y-origin.y))<.02,"independent multi-loop displacement lost");RootMotionHistory slot;Vec3 first{},duplicate{},next{};check(visual.step_root_motion(phase.name,duration*.25,false,4,slot,first,error),error);check(visual.step_root_motion(phase.name,duration*.5,false,4,slot,duplicate,error),error);check(duplicate.x==0&&duplicate.y==0&&duplicate.z==0,"same timestamp source delta was not zero");check(visual.step_root_motion(phase.name,duration*.75,false,5,slot,next,error),error);Vec3 half{},threeQuarter{};check(visual.sample_root_translation(phase.name,start+int((end-start)*.5),half,error),error);check(visual.sample_root_translation(phase.name,start+int((end-start)*.75),threeQuarter,error),error);check(std::abs(next.y-(threeQuarter.y-half.y))<.001,"same timestamp did not refresh previous root coordinate");auto preserved=slot;Vec3 unchanged{7,8,9};check(!visual.step_root_motion(phase.name,.01,false,6,slot,unchanged,error),"rewound source history accepted");check(slot.source_seconds==preserved.source_seconds&&unchanged.y==8,"failed root sample mutated history/output");}
    }
    check(same(original,snapshot(visual))&&visual.animation_elapsed_seconds()==clock,"independent root sampling changed live visual/clock");
    CharacterVisual jump;check(jump.load(assets,config,error),error);SkeletalPose beginning,middle;check(jump.sample_local_pose("Spawn",0,beginning,error),error);check(jump.sample_local_pose("Spawn",600,middle,error),error);check(jump.apply_local_pose(beginning,error),error);const auto jumpStart=snapshot(jump);check(jump.apply_local_pose(middle,error),error);check(!same(jumpStart,snapshot(jump)),"original spawn jump pose did not animate");Vec3 jumpMin{},jumpMax{};check(jump.indexed_bounds(jumpMin,jumpMax,error),error);std::cout<<"Spawn600ms indexedZ="<<jumpMin.z<<".."<<jumpMax.z<<" (skeletal pose motion is separate from locomotion root)\n";
    SkeletalPose pose;check(visual.sample_local_pose("Spawn",300,pose,error),error);Vec3 fromPose{},direct{};check(visual.root_translation_from_pose(pose,fromPose,error),error);check(visual.sample_root_translation("Spawn",300,direct,error),error);check(fromPose.x==direct.x&&fromPose.y==direct.y&&fromPose.z==direct.z,"retained slot pose root differs from direct source sampler");
    std::cout<<"PASS genuine NPC root slot sampling, source default-library bank-order independence, state skin continuity, spawn/attack metadata, host scaling, loop accounting, same-timestamp rule, live pose isolation\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}

