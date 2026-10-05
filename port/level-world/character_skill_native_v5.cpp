#include "character_skill_native_v5.hpp"
#include "character_stance.hpp"
#include "character_target_search_v5.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
namespace dh2::character::skills {namespace {
std::int32_t integer(float f){if(std::isnan(f))return 0;if(f>=2147483648.0f)return INT32_MAX;if(f< -2147483648.0f)return INT32_MIN;return static_cast<std::int32_t>(f);}
float multiply(float a,float b){volatile float result=a*b;return result;}
dh2_script_value number(float f){dh2_script_value v{};v.type=DH2_SCRIPT_NUMBER;v.number=f;return v;}
dh2_script_value boolean(std::uint32_t b){dh2_script_value v{};v.type=DH2_SCRIPT_BOOLEAN;v.boolean=b;return v;}
constexpr std::uint32_t supported[]={0x3b78ec,0x3b7864,0x3b6fa4,0x390690,0x390624,0x390548,0x38fcf0,0x390bd4,0x38e970,0x38e930,0x38fbb8,0x3b8ed8};
struct BackupEntryV5 {target_search::Entry16 entry;std::uintptr_t identity;};
int resolve_backup(void* opaque,const target_search::Entry16* entry,target_search::Object48** object){
 auto& world=*static_cast<SkillNativeWorldV5*>(opaque);
 auto& stored=*reinterpret_cast<const BackupEntryV5*>(entry);
 return world.resolve_object?world.resolve_object(world.context,stored.identity,object):-1;
}
}
CharacterSkillNativeBindingsV5::CharacterSkillNativeBindingsV5(data::FreshInventoryOwnedV4& inventory,
 NativeFsm24& fsm,const SkillManaServicesV5& mana,const SkillNativeWorldV5& world,std::uint32_t capacity):
 inventory_(inventory),fsm_(fsm),mana_{inventory.character(),nullptr,0,{0,0,0,0,0,0,0}},mana_services_(mana),world_(world),heap_(capacity){
 for(auto address:supported)bindings_[address]={this,address};bindings_[0x38eb68]={this,0x38eb68};
}
bool CharacterSkillNativeBindingsV5::coherent()const noexcept {
 return session_&&session_->properties()==inventory_.properties()&&fsm_.state&&
  fsm_.character==inventory_.character()&&world_.character&&world_.character->identity==fsm_.character&&
  !fsm_.reserved&&fsm_.current_present<=1&&mana_.properties==&session_->property_view();
}
int CharacterSkillNativeBindingsV5::attach(CharacterScriptSessionV3& session){
 if(session_&&session_!=&session){error_="Skill native session replacement rejected";return -1;}
 session_=&session;mana_.properties=&session.property_view();
 if(!coherent()){error_="Skill native shared property/FSM/world owner mismatch";return -1;}
 if(!initialized_){auto status=target_search::dh2_target_list_init(&list_,heap_.data(),static_cast<std::uint32_t>(heap_.size()),world_.character,0,&world_.search);
  if(status){error_="Required target list constructor provider failed";return -2;}initialized_=true;}
 return 0;
}
int CharacterSkillNativeBindingsV5::binding(void* opaque,std::uint32_t address,dh2_script_function* function,void** context){
 if(!opaque||!function||!context)return -1;auto& self=*static_cast<CharacterSkillNativeBindingsV5*>(opaque);
 if(!self.coherent())return -1;
 // Top is installed by the dedicated source object route, never type7 output
 // through the ordinary source-values registration factory.
 if(address==0x38eb68)return 0;
 auto it=self.bindings_.find(address);if(it==self.bindings_.end())return 0;
 *function=&invoke;*context=&it->second;return 1;
}
int CharacterSkillNativeBindingsV5::install_object_binding(){
 ScriptSessionView view{};
 if(!coherent()||!session_->view(view)||!world_.objects){error_="Required same-VM object provider unavailable";return -2;}
 // Existing V3 GetTarget usually installed the same input object provider.
 // The runtime setter intentionally rejects any second provider installation.
 if(dh2_script_vm_bind_source_objects(view.vm,"GetTargetListTop",&invoke,&bindings_.at(0x38eb68))){
  if(dh2_script_vm_set_source_objects(view.vm,world_.objects)||
   dh2_script_vm_bind_source_objects(view.vm,"GetTargetListTop",&invoke,&bindings_.at(0x38eb68))){error_="Same-VM target result object binding failed";return -2;}}
 return 0;
}
int CharacterSkillNativeBindingsV5::invoke(void* opaque,const dh2_script_value* args,std::uint32_t count,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* written,char* error,std::size_t size){
 if(!opaque||!written||(!args&&count)||(!out&&capacity))return -1;
 auto& binding=*static_cast<Binding*>(opaque);auto& self=*binding.owner;
 *written=0;try {if(!self.coherent()){self.error_="Skill native shared owner changed";throw 0;}
  const auto status=self.call(binding.address,args,count,out,capacity,written);
  if(!status)return 0;
 }catch(...){if(self.error_.empty())self.error_="Skill native required provider/storage failure";}
 if(error&&size)std::snprintf(error,size,"%s",self.error_.c_str());
 return DH2_SCRIPT_REQUIRED_SERVICE_FAILURE;
}
int CharacterSkillNativeBindingsV5::stance(std::int32_t& out){
 if(!coherent())return -1;StanceFacts16 facts;std::uint32_t player;
 if(session_->source_is_player(player)||session_->constant("COUNT_IPHONE","AnimStances",facts.count))return -2;
 if(player){facts.predicates|=stance_is_player;const auto& equip=inventory_.equipment()[inventory_.current_equipment()];
  const auto* main=equip[1];const auto* off=equip[2];
  const auto* row=main&&main->item?data::item(inventory_.table(),main->item->id):nullptr;
  const auto* offrow=off&&off->item?data::item(inventory_.table(),off->item->id):nullptr;
  if(main&&main->item&&!row)return -2;if(off&&off->item&&!offrow)return -2;
  if(row){facts.predicates|=stance_has_main_hand;if(row->record.words[37]==5)facts.predicates|=stance_has_staff;
   if(row->record.words[37]==4)facts.predicates|=stance_has_bow;}
  if(offrow&&offrow->record.words[22]!=6)facts.predicates|=stance_dual_wielding;
  bool two;if(!inventory_.has_two_hander(false,two,error_))return -2;if(two)facts.predicates|=stance_has_two_hander;
 }
 return dh2_character_anim_stance(&out,&facts)==1?0:-2;
}
int CharacterSkillNativeBindingsV5::call(std::uint32_t address,const dh2_script_value* args,std::uint32_t count,
 dh2_script_value* out,std::uint32_t capacity,std::uint32_t* written){
 error_.clear();auto need=[&](std::uint32_t n){if(capacity<n){error_="Native target result capacity unavailable";return false;}*written=n;return true;};
 switch(address){
 case 0x3b78ec:case 0x3b7864:{
  if(!count||args[0].type!=DH2_SCRIPT_NUMBER)return 0;std::uint32_t result;
  if(dh2_character_skill_mana_v5(&result,&mana_,address==0x3b7864,integer(args[0].number),&mana_services_)){error_="Required source mana policy/debug/property provider failed";return -2;}
  if(!need(1))return -2;out[0]=boolean(result);return 0;
 }
 case 0x3b6fa4:{const auto* slot=inventory_.equipment()[inventory_.current_equipment()][2];
  const auto* row=slot&&slot->item?data::item(inventory_.table(),slot->item->id):nullptr;
  if(slot&&slot->item&&!row){error_="Live shield table row unavailable";return -2;}
  if(!need(1))return -2;out[0]=boolean(row&&row->record.words[22]==6);return 0;}
 case 0x390690:case 0x390624:case 0x390548:
  if(!count||args[0].type!=DH2_SCRIPT_NUMBER)return 0;
  if(address==0x390690)character_filter_=static_cast<std::uint32_t>(integer(args[0].number));
  else if(address==0x390624)object_filter_=static_cast<std::uint32_t>(integer(args[0].number));
  else {const auto sort=integer(args[0].number);list_.sort=sort==1?1:sort==2?2:0;}
  return 0;
 case 0x38e970:if(!need(1))return -2;out[0]=boolean(!list_.count);return 0;
 case 0x38e930:if(!need(1))return -2;out[0]=number(static_cast<float>(list_.count));return 0;
 case 0x38eb68:
  if(!list_.count){if(!need(1))return -2;out[0]={};return 0;}
  if(!need(5))return -2;out[0]={};out[0].type=DH2_SCRIPT_SOURCE_OBJECT;out[0].identity=list_.heap[0].identity;
  out[1]=number(list_.heap[0].distance);{std::uint32_t bits=0x42652ee0;float degrees;std::memcpy(&degrees,&bits,4);out[2]=number(multiply(list_.heap[0].angle,degrees));}
  out[3]=boolean(list_.heap[0].flags&1);out[4]=number(0);return 0;
 case 0x38fbb8:{if(!list_.count)return 0;target_search::Target24 value;return target_search::dh2_target_pop(&list_,&value)?-2:0;}
 case 0x390bd4:{const char* name=count&&args[0].type==DH2_SCRIPT_STRING?args[0].text:"default";
  if(!name){error_="Source null backup name assertion domain";return -2;}
  backups_[name]=std::vector<target_search::Target24>(list_.heap,list_.heap+list_.count);return 0;}
 case 0x3b8ed8:
  if(!count||(args[0].type!=DH2_SCRIPT_IDENTITY&&args[0].type!=DH2_SCRIPT_SOURCE_OBJECT))return 0;
  if(!world_.look_at||world_.look_at(world_.context,fsm_.character,args[0].identity)){error_="Required controller Cmd_LookAt provider unavailable";return -2;}return 0;
 case 0x38fcf0:{
  if(!count||args[0].type!=DH2_SCRIPT_NUMBER||(count>1&&args[1].type!=DH2_SCRIPT_NUMBER))return 0;
  if(count>3){error_="Unrecovered source search optional origin/flags domain";return -2;}
  if(character_filter_!=1||object_filter_!=0){error_="Required non-Enemy/non-AttackableOnly target search domain unavailable";return -2;}
  const auto radius=args[0].number;
  const float origin[3]={world_.character->position[0],world_.character->position[1],world_.character->position[2]};
  const float cone=count>1?multiply(multiply(args[1].number,0.01745329238474369049f),0.5f):3.1415927410125732421875f;
  const target_search::Registry8* registry=world_.registry;
  target_search::Room16 sentinel{},room{};target_search::Entry16 end{};std::vector<BackupEntryV5> entries;
  target_search::Registry8 backup_registry{&sentinel};
  target_search::SnapshotResolve16V5 resolver{&world_,resolve_backup};
  const target_search::SnapshotResolve16V5* snapshot_resolver=nullptr;
  if(count>2&&args[2].type==DH2_SCRIPT_STRING){
   if(!args[2].text||!world_.resolve_object){error_="Required backup identity resolver unavailable";return -2;}
   const auto& backup=backups_[args[2].text];entries.resize(backup.size());
   for(std::size_t i=0;i<backup.size();++i)entries[i]={{i+1<entries.size()?&entries[i+1].entry:&end,nullptr},backup[i].identity};
   end={entries.empty()?&end:&entries[0].entry,nullptr};sentinel={&room,nullptr};room={&sentinel,&end};registry=&backup_registry;snapshot_resolver=&resolver;
  }else if(count>2){error_="Unrecovered source search optional argument domain";return -2;}
  if(!registry||target_search::dh2_target_search_snapshot_v5(&list_,registry,radius,cone,origin,&world_.search,snapshot_resolver)){error_="Required live target search/backup provider failed";return -2;}return 0;
 }
 default:error_="Unsupported source native skill binding";return -2;
 }
}
}
