#include "source_skill_animation.hpp"
#include "../../../level-world/character_skill_callbacks_v3.hpp"
#include "../../animation_markers.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
using namespace dh2::foundation::skills_animation;
using namespace dh2::character::skills;
static void require(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
static std::vector<std::uint8_t> read(const std::string& path){std::ifstream f(path,std::ios::binary);require(bool(f),"missing source table");return {std::istreambuf_iterator<char>(f),{}};}
struct Host {
 dh2::character::State state;
 bool allowed=true,using_skill=false,fail_transition=false;
 unsigned checks=0,uses=0,pres=0,stops=0,transitions=0,prefixes=0;
 std::uint32_t lastMoving=0;
 static int ai(void* context,SkillAIContextV3*,const SkillAIRequest32V3* q,SkillAIResponse32V3* r){
  auto& h=*static_cast<Host*>(context);
  switch(q->operation){
   case skill_ai_using_v3:r->word=h.using_skill;return 0;
   case skill_ai_casting_v3:r->word=0;return 0;
   case skill_ai_player_v3:r->word=0;return 0;
   case skill_ai_callback_v3:
    if(q->value==skill_check_usable_v3){++h.checks;r->word=h.allowed;return 0;}
    if(q->value==skill_use_v3){++h.uses;r->word=1;return 0;}
    if(q->value==skill_check_active_v3){r->word=0;return 0;}
    if(q->value==skill_pre_v3){++h.pres;r->word=1;return 0;}
    if(q->value==skill_post_v3){r->word=1;return 0;}
    return -1;
   case skill_ai_stop_loop_v3:++h.stops;return q->value==1?0:-1;
   default:return -1;
  }
 }
 static int native_state(void* context,SkillStateV4* state,const SkillStateRequest32V4* q,SkillStateResponse16V4* r){
  auto& h=*static_cast<Host*>(context);
  switch(q->operation){
   case skill_state_constant_v4:r->word=0x200000;return 0;
   case skill_state_stance_v4:r->word=3;return 0;
   case skill_state_state_event_v4:
    require(q->value==50005&&q->index==0,"native state event envelope");
    ++h.transitions;h.lastMoving=state->moving;
    if(h.fail_transition)return -1;
    h.state.current=6;h.using_skill=true;return 0;
   case skill_state_current_v4:r->word=h.state.current;return 0;
   case skill_state_step_index_v4:r->word=2;return 0;
   case skill_state_step_count_v4:r->word=4;return 0;
   case skill_state_debug_load_v4:case skill_state_debug_get_v4:case skill_state_debug_destroy_v4:return 0;
   case skill_state_debug_construct_v4:r->identity=1;return 0;
   case skill_state_named_prefix_v4:++h.prefixes;return -1; // Missing actual FX must not silently succeed.
   default:return -1;
  }
 }
};
int main(int argc,char** argv){try{
 const std::string dir=argc>1?argv[1]:".local-inputs/windows-shared-assets/original-cache/data/pydata";
 auto records=read(dir+"/skills_pyarray.bin"),names=read(dir+"/skills_pyarraynames.bin"),schema=read(dir+"/skills_pystructnames.bin");
 dh2::data::SkillTables tables;std::string error;
 require(tables.load({records.data(),records.size()},{names.data(),names.size()},{schema.data(),schema.size()},error),error.c_str());auto t=tables.borrow();
 std::vector<std::int32_t> cases;
 for(std::size_t i=0;i<t.skills().size();++i){const auto& s=t.skills()[i].script;
  if(s.find("prince_warrior_bashdown")!=std::string::npos||s.find("prince_mage_coldray")!=std::string::npos||s.find("prince_rogue_jump_kick")!=std::string::npos){
   cases.push_back(std::int32_t(i));const auto* w=t.skills()[i].scalar.words;
   std::cout<<"REAL "<<i<<" "<<t.skill_names()[i]<<" anim="<<w[1]<<" moving="<<w[2]<<" type="<<w[18]<<" script="<<s<<"\n";
  }
 }
 require(cases.size()==3,"three actual class scripts expected");
 unsigned memberships=0;
 for(const auto& name:t.list_names()){
  std::vector<Declaration> list;require(declarations(t,name,list,error),"source class list decode");
  for(std::size_t position=0;position<list.size();++position)for(auto id:cases)if(list[position].row==id){++memberships;std::cout<<"MEMBER "<<name<<" position="<<position<<" "<<list[position].name<<"\n";}
 }
 require(memberships>=3,"actual class list membership");
 require(t.list_index("Knight")>=0&&t.list_index("Mage")>=0&&t.list_index("Rogue")>=0,"exact source class list names");
 auto ar=read(dir+"/animations_pyarray.bin"),an=read(dir+"/animations_pyarraynames.bin"),af=read(dir+"/animations_pystructnames.bin");
 auto dk=read(".local-inputs/actors/animations_dictionary_pyarraynames.bin"),dv=read(dir+"/animations_dictionary_pyarray.bin");
 dh2::data::Dictionary clips;dh2::data::AnimationTables animations;
 require(dh2::data::load_dictionary({dk.data(),dk.size()},{dv.data(),dv.size()},clips,error),error.c_str());
 require(dh2::data::load_animation_tables({ar.data(),ar.size()},{an.data(),an.size()},{af.data(),af.size()},clips,animations,error),error.c_str());
 std::vector<Declaration> untouched(1);untouched[0].row=777;
 require(!declarations(t,"missing-source-list",untouched,error)&&untouched[0].row==777,"catalog failure atomic");
 for(auto id:cases){
  Host h;h.state.animation_override=123;h.state.current=3;
  Instance32 instance{77,t.skills()[id].script.c_str(),0,0,0,0};const Instance32* instances[]={&instance};
  State40 slots{77,{instances,1,0},{}};SkillAIOwnerV3 owner{77,0,0};SkillAIStateV3 fields{};
  SkillAIContextV3 ai{&owner,&slots,&fields,7,0};SkillStateV4 state{&h.state,77,0,0,0,0,0,0,{}};
  SourceSkillAnimation adapter(t,ai,state,[id](unsigned slot,std::int32_t& row){if(slot!=0)return false;row=id;return true;},{&h,Host::ai},{&h,Host::native_state});
  unsigned answer=99;h.allowed=false;
  require(adapter.command(skill_ai_begin_v3,0,answer)==0&&answer==0,"Check denial");
  require(h.transitions==0&&h.state.animation_override==123&&fields.current==-1,"denial preserves state/animation");
  h.allowed=true;require(adapter.command(skill_ai_begin_v3,0,answer)==0&&answer==1,"admitted source begin");
  const auto* w=t.skills()[id].scalar.words;
  require(std::uint32_t(h.state.animation_override)==w[1]+3&&h.lastMoving==std::uint8_t(w[2])&&fields.current==0,"actual row stance and moving byte");
  // The host fixture's stance3 above intentionally verifies arithmetic. Use
  // the actual unshifted skill row to inspect authored class sequence data.
  h.state.animation_override=std::int32_t(w[1]);dh2::data::AnimationRandom rng{1,0};dh2::data::AnimationStart start;
  require(adapter.animation_start(animations,rng,start,error),error.c_str());
  const auto* path=dh2::data::animation_clip(start.step,clips);require(path,"actual source clip lookup");
  std::cout<<"START "<<t.skill_names()[id]<<" sequence="<<animations.sequence_names[w[1]]<<" layers="<<start.layers.size()<<" clip="<<*path<<" fx="<<start.step.fx<<" speed="<<start.step.speed<<" move="<<start.step.move_go<<" blend="<<start.step.blend_out<<"\n";
  auto bres=read("port/level-world/reference/character-visual-v6/cache/"+*path);
  dh2::resources::BresView bv{};require(dh2_bres_open(&bv,bres.data(),bres.size())==dh2::resources::BresError::ok,"source skill BRES");
  auto word=[](const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);};
  const auto begin=std::int32_t(word(bres.data()+bv.root_offset+28)),end=std::int32_t(word(bres.data()+bv.root_offset+32));
  dh::foundation::AnimationMarkers markers;require(markers.load(bres.data(),bres.size(),begin,end,error),error.c_str());
  unsigned authored_skill=0;
  for(const auto& marker:markers.markers()){
   std::cout<<"MARKER "<<t.skill_names()[id]<<" "<<marker.name<<" authored="<<marker.authored_time_ms<<" dispatch="<<marker.time_ms<<"\n";
   if(marker.name=="do_skill"){
    require(adapter.authored_event(marker.name.c_str())==0,"real authored source skill event");++authored_skill;
   }
  }
  require(authored_skill>0&&h.uses==authored_skill,"actual source clip contains executable skill marker");h.uses=0;
  require(adapter.authored_event("do_skill")==0&&h.uses==1,"authored marker uses same AI instance");
  require(adapter.authored_event("do_spell")==0&&h.uses==1,"spell name not skill event");
  h.state.current=3;require(adapter.authored_event("do_skill")==0&&h.uses==1,"marker outside Skill ignored");
  h.state.current=7;require(adapter.authored_event("do_spell")==-2&&h.uses==1,"Cast requires real other-state provider");
  require(adapter.authored_event("fx_unavailable")==-2&&h.prefixes==1,"missing authored FX rejects");
  owner.character=88;require(adapter.command(skill_ai_usable_v3,0,answer)==-1,"foreign actor rejected");owner.character=77;
  h.using_skill=false;h.fail_transition=true;
  require(adapter.command(skill_ai_begin_v3,0,answer)==-2&&fields.current==0,"provider failure retains original prefix");
 }
 // Actual non-Type0 rows exercise native toggle/held-skill behavior, avoiding
 // an adapter that works only for the three selected demonstration classes.
 for(unsigned type:{1u,2u}){
  std::int32_t id=-1;for(std::size_t i=0;i<t.skills().size();++i)if(t.skills()[i].scalar.words[18]==type){id=std::int32_t(i);break;}
  if(id<0){std::cout<<"TYPE "<<type<<" no original Skill rows; native branch not asserted as authored content\n";continue;}Host h;h.using_skill=true;h.state.current=6;
  Instance32 instance{77,t.skills()[id].script.c_str(),0,0,0,0};const Instance32* instances[]={&instance};State40 slots{77,{instances,1,0},{}};
  SkillAIOwnerV3 owner{77,0,0};SkillAIStateV3 fields{};fields.current=0;SkillAIContextV3 ai{&owner,&slots,&fields,7,0};SkillStateV4 state{&h.state,77,0,0,0,0,0,0,{}};
  SourceSkillAnimation adapter(t,ai,state,[id](unsigned,std::int32_t& row){row=id;return true;},{&h,Host::ai},{&h,Host::native_state});unsigned answer=0;
  if(type==1){require(adapter.command(skill_ai_begin_v3,0,answer)==0&&answer==1&&h.pres==1&&h.transitions==0,"source active toggle uses Pre without animation restart");}
  else {require(adapter.command(skill_ai_end_v3,0,answer)==0&&fields.last==1&&h.stops==0,"source held-skill end requests last iteration");fields.continued=1;require(adapter.command(skill_ai_end_v3,0,answer)==0&&h.stops==1,"source continued end requests actual animation StopLoop");}
  std::cout<<"TYPE "<<type<<" row="<<id<<" name="<<t.skill_names()[id]<<"\n";
  h.using_skill=false;ai.script_step=6;auto checks=h.checks;
  require(adapter.command(skill_ai_usable_v3,0,answer)==0&&answer==0&&h.checks==checks,"source signed script step gates Check");
 }
 std::cout<<"PASS source skill animation: real three classes, admission, source selection, marker dispatch, required service failure\n";return 0;
}catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<"\n";return 1;}}
