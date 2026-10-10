#include "original_melee_bindings.hpp"
#include "actor_profiles.hpp"
#include "animation_markers.hpp"
#include "content_paths.hpp"
#include "../engine-animation/animation.hpp"
#include <cmath>
#include <filesystem>
#include <iostream>
#include <stdexcept>
using namespace dh::foundation;
void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
int main(int argc,char**argv){try{
 check(argc==2,"Supply repository root");AssetCatalog assets(std::filesystem::path(argv[1])/".local-inputs/windows-shared-assets");
 OriginalMeleeBindings bindings;std::string error;check(bindings.load(assets,"original-melee-bindings.xml",error),error);
 for(const auto&actor:bindings.actors())check(actor.second.states.count("Run")==1,"An explicit source Run state is missing");
 const auto*run=bindings.sequence("KnightPlayerBase","Run",0);check(run&&run->id==271&&run->loop==-1&&run->type==0,"Knight original Run sequence differs");
 check(run->steps.size()==1,"Knight Run phase ordering differs");const auto&step=run->steps[0];
 check(step.animationId==1114&&step.redirect==0&&step.moveGO==1&&step.blendOut==0&&std::abs(step.speed-1.2999999523162842)<1e-12,"Knight source speed/motion/phase metadata differs");
 check(step.uri=="data/3D/characters/prince/animations/prince_walk_1hand.bdae","Filename was used to misclassify source Run");
 const auto*walk=bindings.sequence("KnightPlayerBase","Walk",0);check(walk&&walk->steps[0].uri!=step.uri,"Walk was aliased to Run");
 const auto*npc=bindings.find_actor("Swamp_LizadMan_Type1");check(npc&&npc->states.at("Run").empty(),"Absent original NPC Run was invented");
 const auto*metadata=bindings.find_clip(step.uri);check(metadata,"Original Run native clip metadata missing");
 const auto*actor=bindings.find_actor("KnightPlayerBase");auto model=read_content(assets,actor->model),clip=read_content(assets,step.uri);
 dh2::resources::BresView view{};check(dh2_bres_open(&view,model.data(),model.size())==dh2::resources::BresError::ok,"Original model rejected");
 dh2::scene::Scene scene;check(dh2::scene::load(view,scene,error),error);dh2::animation::Player player;
 check(player.load(clip.data(),clip.size(),scene,error,dh2::animation::MissingTargets::ignore),error);
 check(player.start==metadata->startMs&&player.end==metadata->endMs,"Exported Run range differs from native source");
 AnimationMarkers markers;if(player.events.view().count)check(markers.load(clip.data(),clip.size(),player.start,player.end,error),error);
 check(markers.markers().size()==metadata->markers.size(),"Exported event occurrence count differs");
 for(std::size_t i=0;i<metadata->markers.size();++i){const auto&m=markers.markers()[i];const auto&expected=metadata->markers[i];check(m.name==expected.name&&m.time_ms==expected.timeMs&&m.authored_time_ms==expected.authoredTimeMs,"Original Run event timing/name differs");}
 std::cout<<"PASS exact source Run state/speed/motion, distinct Walk, authored NPC absence, staged native clip/event metadata\n";return 0;
}catch(const std::exception&e){std::cerr<<e.what()<<'\n';return 1;}}
