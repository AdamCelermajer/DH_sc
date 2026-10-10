#include "skill_animation_program.hpp"
#include "../../retained_sequence_playback.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace dh::foundation;
using namespace dh::foundation::skills_animation;
static void check(bool value,const std::string& message){if(!value)throw std::runtime_error(message);}
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);check(bool(f),"Missing "+path);return {std::istreambuf_iterator<char>(f),{}};}
static dh2::data::Bytes view(const std::vector<std::uint8_t>& v){return {v.data(),v.size()};}
int main(int argc,char** argv){try{
 AssetCatalog assets(argc>1?argv[1]:".local-inputs/windows-shared-assets");std::string error;
 auto r=assets.read("original-cache/data/pydata/animations_pyarray.bin"),n=assets.read("original-cache/data/pydata/animations_pyarraynames.bin"),s=assets.read("original-cache/data/pydata/animations_pystructnames.bin");
 auto k=read(".local-inputs/actors/animations_dictionary_pyarraynames.bin"),v=assets.read("original-cache/data/pydata/animations_dictionary_pyarray.bin");
 dh2::data::Dictionary clips;dh2::data::AnimationTables tables;
 check(dh2::data::load_dictionary(view(k),view(v),clips,error),error);check(dh2::data::load_animation_tables(view(r),view(n),view(s),clips,tables,error),error);
 OriginalMeleeBindings bindings;check(bindings.load(assets,"original-melee-bindings.xml",error),error);
 ActorCustomization custom;custom.skin_id_contains="_default_warrior-mesh-skin";custom.expected_controller_count=4;custom.allow_missing_animation_targets=true;
 OriginalCombatVisualPlan idlePlan;check(build_original_combat_visual_plan(assets,bindings,"KnightPlayerBase",custom,"same-actor",idlePlan,error),error);
 const auto* idle=idlePlan.phase("Idle",0,{0});check(idle,"Original explicit idle absent");auto config=idlePlan.config;config.clips={{idle->clipName,idle->resolvedPath}};config.motion_node_id="auto";config.consume_root_motion=true;
 SkillAnimationPrograms bank;check(build_skill_animation_programs(assets,tables,clips,config,{347,413,521},"same-actor",bank,error),error);
 check(bank.plan.config.clips.size()==4&&bank.plan.sequences.size()==3,"Loaded original skill program bank");
 for(const auto& sequence:bank.plan.sequences){check(sequence.phases.size()==1&&sequence.type==0&&sequence.loop==0,"Actual case source policies changed");const auto& phase=sequence.phases[0];check(phase.speed==float(1.3)&&phase.blendOut==100&&phase.moveGO==1,"Actual source step policies");check(bank.steps.at({sequence.state,phase.sourcePath}).fx>=0,"Actual source FX set metadata");}
 auto preserved=bank;check(!build_skill_animation_programs(assets,tables,clips,config,{INT32_MAX},"same-actor",bank,error)&&bank.plan.config.clips.size()==preserved.plan.config.clips.size(),"Invalid source root rejects atomically");
 CharacterVisual visual;check(visual.load(assets,bank.plan.config,error),error);
 auto root=[&visual](const std::string& clip,std::int32_t ms,std::array<float,3>& scratch,std::string& e){Vec3 p{scratch[0],scratch[1],scratch[2]};if(!visual.sample_source_root_translation(clip,ms,p,e))return false;scratch={p.x,p.y,p.z};return true;};
 RetainedSequencePlayback owner(visual,root);const RetainedAnimationOwner* identity=nullptr;unsigned hits=0,closes=0;
 RetainedSequenceServices services;services.frame=[&](std::size_t,const RetainedAnimationFrame& frame,std::string&){for(const auto& event:frame.events)if(event.name=="do_skill"){++hits;check(event.lag_ms>=0,"Original authored marker lag");}check(std::isfinite(frame.authored_motion.x)&&std::isfinite(frame.authored_motion.y),"Finite source root motion");return true;};services.closed=[&](const dh2::timeline::Completion&,std::string&){++closes;return true;};
 for(const auto& sequence:bank.plan.sequences){
  OriginalAttackSelection selected;selected.state=sequence.state;
  check(owner.prepare_preserving(assets,bank.plan,bank.policies,selected,services,"source-skill",error),error);
  if(!identity){identity=owner.animation();RetainedAnimationFrame frame;check(owner.seed_sequence(assets,idle->clipName,idle->resolvedPath,1,100,false,-1,frame,error),error);check(owner.advance_seeded(.15,error),error);}
  check(owner.animation()==identity,"Same retained scene/slot owner survives class skill change");
  const auto before=hits,beforeClose=closes;check(owner.begin(error),error);
  for(unsigned frame=0;frame<240&&!owner.finished();++frame)check(owner.advance(1.0/60,error),error);
  check(owner.finished()&&hits==before+1&&closes==beforeClose+1,"Actual whole skill completion and one original marker");
  check(!owner.advance(.5,error)&&hits==before+1&&closes==beforeClose+1,"Completed skill rejects advancement without redelivering callback");
  std::cout<<"PASS actual retained skill "<<sequence.name<<" marker/completion/sameowner\n";
 }
 std::cout<<"PASS original three skill programs and shared retained body playback\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
