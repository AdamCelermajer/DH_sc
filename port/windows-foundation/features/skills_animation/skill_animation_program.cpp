#include "skill_animation_program.hpp"
#include "../../content_paths.hpp"
#include <set>
#include <stdexcept>
namespace dh::foundation::skills_animation {
std::string skill_sequence_state(std::int32_t id){return "source-skill-sequence-"+std::to_string(id);}
bool build_skill_animation_programs(const AssetCatalog& assets,const dh2::data::AnimationTables& tables,
 const dh2::data::Dictionary& clips,const CharacterVisualConfig& visual,const std::vector<std::int32_t>& ids,
 const std::string& role,SkillAnimationPrograms& output,std::string& error){try{
 if(role.empty()||visual.model_path.empty()||ids.empty())throw std::runtime_error("Skill program requires explicit actor visual, role and sequence roots");
 SkillAnimationPrograms next;next.plan.config=visual;next.plan.roleId=role;
 std::set<std::string> aliases;for(const auto& clip:visual.clips)aliases.insert(clip.first);
 std::set<std::int32_t> roots;
 auto sequence=[&](std::int32_t id)->const dh2::data::AnimationSequence&{
  if(id<0||std::size_t(id)>=tables.sequences.size())throw std::runtime_error("Skill AnimTable reference outside table");
  const auto& s=tables.sequences[id];next.policies.emplace(id,OriginalSequencePolicy{id,s.type,s.loop,tables.sequence_names[id]});return s;
 };
 for(auto id:ids){
  if(!roots.insert(id).second)continue;const auto& root=sequence(id);
  OriginalCombatSequencePlan program;program.state=skill_sequence_state(id);program.id=id;program.name=tables.sequence_names[id];program.loop=root.loop;program.type=root.type;
  std::set<std::int32_t> stack;
  std::function<void(std::int32_t,std::vector<std::size_t>,std::vector<std::int64_t>,std::vector<OriginalCombatRedirect>)> append;
  append=[&](std::int32_t sid,std::vector<std::size_t> prefix,std::vector<std::int64_t> indices,std::vector<OriginalCombatRedirect> ancestors){
   if(prefix.size()>=3||!stack.insert(sid).second)throw std::runtime_error("Skill redirect exceeds original stack or recurses");
   const auto& source=sequence(sid);
   if(source.steps.empty())throw std::runtime_error("Skill source sequence has no authored steps");
   for(std::size_t i=0;i<source.steps.size();++i){
    const auto& step=source.steps[i];auto path=prefix;path.push_back(i);auto source_indices=indices;source_indices.push_back(i);
    next.steps.emplace(std::make_pair(program.state,path),step);
    if(step.redir==1){auto parent=ancestors;parent.push_back({std::int64_t(i),step.anim,step.redir,step.speed,step.blend_out,step.move_go,{}});append(step.anim,path,source_indices,parent);continue;}
    if(step.redir!=0||step.anim<0)throw std::runtime_error("Skill step has symbolic or unsupported clip reference");
    const auto* uri=dh2::data::animation_clip(step,clips);if(!uri)throw std::runtime_error("Skill source clip dictionary reference invalid");
    OriginalCombatPhase phase;phase.sourcePath=path;phase.sourceIndices=source_indices;phase.ancestors=ancestors;phase.animationId=step.anim;phase.redirect=step.redir;
    phase.blendOut=step.blend_out;phase.moveGO=step.move_go;phase.speed=step.speed;phase.sourceUri=*uri;
    phase.resolvedPath=resolve_content_path(assets,normalize_content_uri(*uri),visual.model_path).lexically_relative(assets.root()).generic_string();
    phase.clipName=role+"/"+program.state+"/phase-"+std::to_string(program.phases.size());
    if(!aliases.insert(phase.clipName).second)throw std::runtime_error("Skill animation alias collision");
    next.plan.config.clips.emplace_back(phase.clipName,phase.resolvedPath);next.plan.clipRates.emplace(phase.clipName,phase.speed);
    if(next.plan.config.clips.size()>4096)throw std::runtime_error("Skill bank exceeds original adapter clip bound");
    program.phases.push_back(std::move(phase));
   }
   stack.erase(sid);
  };
  append(id,{},{},{});next.plan.stateNames.push_back(program.state);next.plan.sequences.push_back(std::move(program));
 }
 output=std::move(next);error.clear();return true;
}catch(const std::exception& e){error=e.what();return false;}}
}
