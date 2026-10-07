#include "character_skill_native_v6.hpp"
#include <cstdio>
namespace dh2::character::skills {
CharacterSkillNativeBindingsV6::CharacterSkillNativeBindingsV6(CharacterSkillNativeReadOnlyBindingsV6& previous,
 const data::FreshInventoryOwnedV4& inventory,NativeFsm24& fsm,data::SkillTables::Borrow tables,
 const SkillCombatWorldV6& world,DotCombatContext32& context,data::CombatRandom& random,
 const SkillAttackNativeServicesV6& debug,const SkillApplyServicesV6& application):
 previous_(previous),inventory_(inventory),fsm_(fsm),tables_(std::move(tables)),world_(world),
 context_(context),random_(random),debug_(debug),application_(application){
 if(tables_){for(const auto& row:tables_.skills())rows_.push_back({std::int32_t(row.scalar.words[6]),row.scalar.words[7]});}
}
bool CharacterSkillNativeBindingsV6::coherent()const noexcept {
 return session_&&session_->properties()==inventory_.properties()&&fsm_.state&&fsm_.character==inventory_.character()&&
  !fsm_.reserved&&tables_&&application_.debug==&debug_&&world_.handle&&world_.actor&&world_.classify.invoke;
}
int CharacterSkillNativeBindingsV6::attach(CharacterScriptSessionV3& session){
 if(session_&&session_!=&session){error_="V6 retained Session replacement rejected";return -1;}
 session_=&session;if(!coherent()){error_="V6 same property/inventory/FSM/world owner mismatch";return -1;}
 const auto status=previous_.attach(session);if(status)error_=previous_.error();return status;
}
int CharacterSkillNativeBindingsV6::binding(void* opaque,std::uint32_t address,dh2_script_function* function,void** context){
 if(!opaque||!function||!context)return -1;auto& self=*static_cast<CharacterSkillNativeBindingsV6*>(opaque);
 if(!self.coherent())return -1;
 if(address!=0x3b9fbc)return CharacterSkillNativeReadOnlyBindingsV6::binding(&self.previous_,address,function,context);
 *function=invoke;*context=&self;return 1;
}
int CharacterSkillNativeBindingsV6::invoke(void* opaque,const dh2_script_value* args,std::uint32_t count,
 dh2_script_value* values,std::uint32_t capacity,std::uint32_t* written,char* error,std::size_t size){
 if(!opaque||!written||(count&&!args)||(capacity&&!values))return -1;
 auto& self=*static_cast<CharacterSkillNativeBindingsV6*>(opaque);*written=0;
 auto fail=[&](){if(error&&size)std::snprintf(error,size,"%s",self.error_.c_str());return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;};
 if(!self.coherent()||self.busy_){self.error_="V6 changed owner or reentrant native combat entry";return fail();}
 self.busy_=true;struct Guard{bool& busy;~Guard(){busy=false;}}guard{self.busy_};
 struct Run {
  CharacterSkillNativeBindingsV6& owner;target_providers::Handle16* shared=nullptr;
  target_providers::Registry24* registry=nullptr;target_providers::Handle16 local{};
  bool actors(std::uintptr_t target,SkillAttackActorV6*& a,SkillAttackActorV6*& b,SkillApplyActorV6*& apply_a,SkillApplyActorV6*& apply_b){
   return owner.world_.actor(owner.world_.context,owner.inventory_.character(),&a,&apply_a)==0&&
    owner.world_.actor(owner.world_.context,target,&b,&apply_b)==0&&a&&b&&apply_a&&apply_b&&
    a->identity==owner.inventory_.character()&&b->identity==target&&apply_a->identity==a->identity&&apply_b->identity==b->identity&&
    a->properties==&owner.session_->property_view()&&apply_a->properties==a->properties&&apply_b->properties==b->properties;
  }
  static int service(void* opaque,const SkillCombatRequestV6* q,SkillCombatResponseV6* out,data::CombatResult* result){
   auto& run=*static_cast<Run*>(opaque);auto& owner=run.owner;
   const auto list=owner.session_->property_view().resolved[28];
   switch(q->service){
   case skill_combat_list_v6:
    if(list<0||std::size_t(list)>=owner.tables_.lists().size())return -1;
    out->word=std::uint32_t(owner.tables_.lists()[list].size());return 0;
   case skill_combat_handle_v6:
    if(owner.world_.handle(owner.world_.context,q->target,&run.shared,&run.registry)||!run.shared||!run.registry)return -1;
    out->identity=reinterpret_cast<std::uintptr_t>(run.shared);return 0;
   case skill_combat_character_v6:
    if(q->target!=reinterpret_cast<std::uintptr_t>(run.shared))return -1;
    return target_providers::dh2_target_handle_character(&out->identity,&run.local,run.shared,run.registry,&owner.world_.classify)?-1:0;
   case skill_combat_row_v6:{
    if(list<0||std::size_t(list)>=owner.tables_.lists().size()||q->index>=owner.tables_.lists()[list].size())return -1;
    const auto row=owner.tables_.lists()[list][q->index];if(row<0||std::size_t(row)>=owner.rows_.size())return -1;
    out->row=&owner.rows_[row];return 0;}
   case skill_combat_main_hand_v6:case skill_combat_off_hand_v6:
    out->word=owner.inventory_.equipment()[owner.inventory_.current_equipment()][q->service==skill_combat_main_hand_v6?1:2]!=nullptr;return 0;
   case skill_combat_calculate_v6:case skill_combat_apply_v6:{
    SkillAttackActorV6 *a=nullptr,*b=nullptr;SkillApplyActorV6 *apply_a=nullptr,*apply_b=nullptr;
    if(!result||!run.actors(q->target,a,b,apply_a,apply_b))return -1;
    if(q->service==skill_combat_calculate_v6)return dh2_character_skill_attack_calculate_v6(result,&owner.context_,&owner.random_,a,b,&owner.inventory_,q->mask,q->element,&owner.debug_);
    SkillApplyOutputV6 applied{};return dh2_character_skill_apply_result_v6(&applied,result,apply_a,apply_b,&owner.application_);}
   default:
    return owner.world_.other&&owner.world_.other->invoke?owner.world_.other->invoke(owner.world_.other->context,q,out,result):-1;
   }
  }
 }run{self};
 try {
  SkillCombatServicesV6 providers{&run,Run::service};SkillCombatOutputV6 result{};
  if(dh2_character_skill_combat_roll_v6(&result,self.inventory_.character(),args,count,&providers)){
   self.error_="Required V6 source combat provider failed at phase "+std::to_string(result.phase);return fail();}
  const auto needed=result.count+result.boolean_count;if(capacity<needed){self.error_="V6 combat result capacity unavailable after source effects";return fail();}
  for(unsigned i=0;i<result.count;++i){values[i]={};values[i].type=DH2_SCRIPT_NUMBER;values[i].number=float(result.amount[i]);}
  if(result.boolean_count){values[result.count]={};values[result.count].type=DH2_SCRIPT_BOOLEAN;values[result.count].boolean=0;}
  *written=needed;self.error_.clear();return 0;
 }catch(...){self.error_="V6 native combat storage/provider exception";return fail();}
}
}
