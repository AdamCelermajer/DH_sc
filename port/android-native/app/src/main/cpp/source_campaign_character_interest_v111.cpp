#include "source_campaign_character_interest_v111.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_object_update_actor_v104.hpp"
#include "source_campaign_items_v88.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <character_object_interest_v106.hpp>
#include <character_world_runtime_v1.hpp>
#include <application_services_owner_v5.hpp>
#include <algorithm>
namespace model_renderer {
bool source_campaign_character_interest_v111(const std::shared_ptr<void>& world,std::uintptr_t id,std::string& e){
 SourceCampaignCandidateBorrowV55 scope;SourceCampaignCharacterBorrowV62 actual;
 if(!borrow_source_campaign_candidate_v55(scope,e)||scope.actual_world!=world||!borrow_source_campaign_character_v62(world,id,actual,e))return false;
 auto r=actual.character;auto a=r?r->actor:nullptr;auto targets=r?r->services.world_targets:nullptr;
 dh2::world::GameObjectInitializationFieldsV62 fields;
 if(!a||!targets||!a->source_frame_fields_v106()||!a->inherited_initialization_fields_v62(a,fields,e))return false;
 dh2::character::CharacterInterestFieldsV106 f{id,&a->source_ooi14a4,&a->animation_ai.owner_byte14a8,
  &a->source_frame_fields_v106()->ooi_delay14aa,fields.byte(0x14ac),fields.byte(0x14ad)};
 const auto query=targets->targets().search_services();
 auto invoke=[&](std::uint32_t op,std::uintptr_t subject,std::uintptr_t other,std::uintptr_t& out){
  const dh2::target_search::Request24 q{op,0,subject,other};dh2::target_search::Response16 result{};
  if(query.invoke(query.context,&q,&result)){e=targets->targets().error();return false;}out=result.word;return true;};
 dh2::character::CharacterInterestServicesV106 services;services.owner=r;
 services.interactive=[&](auto target,auto owner,bool& out,std::string&){std::uintptr_t value{};if(!invoke(dh2::target_search::is_interactive,target,owner,value))return false;out=value!=0;return true;};
 services.interaction_type=[&](auto target,auto owner,std::int32_t& out,std::string&){std::uintptr_t value{};if(!invoke(dh2::target_search::interaction_type,target,owner,value))return false;out=static_cast<std::int32_t>(value);return true;};
 services.disabled=[&](auto target,const std::uint8_t*& out,std::shared_ptr<void>& pin,std::string& error){
  dh2::world::ObjectUpdateActorV102 object;if(!borrow_source_campaign_object_update_actor_v104(scope,target,object,error))return false;
  out=object.byte?object.byte(0x81):nullptr;pin=object.object.lease;
  if(!out||!pin){error="Required actual OOI ObjectBase81 receiver";return false;}return true;};
 services.item_owner3bc=[&](auto target,std::uintptr_t& out,std::string& error){std::shared_ptr<void> pin;const std::uintptr_t* owner{};
  if(!borrow_source_campaign_item_owner_v107(world,target,pin,owner,error)||!owner)return false;out=*owner;return true;};
 services.search=[&](auto owner,std::vector<dh2::character::CharacterInterestCandidateV106>& out,std::string& error){
  using namespace dh2::target_search;
  // Preserve actual list80 room order, each actual occupants list order and
  // duplicate entries. The source heap determines the final selection order.
  const auto& source=scope.objects->source_room_objects80_v105();
  std::size_t total{};for(auto list:source){if(!list){error="NULL source room occupants list80";return false;}total+=list->size();if(total>65536){error="OOI source list exceeds bounded target admission";return false;}}
  std::vector<Room16> rooms(source.size());std::vector<Entry16> ends(source.size()),entries(total);
  std::vector<dh2::character::skills::WorldTargetActorBorrowV1> pins; pins.reserve(total+1);
  auto observe=[&](auto target,Object48*& object){dh2::character::skills::WorldTargetActorBorrowV1 loan;
   if(targets->actor(target,&loan)||!loan.search||!loan.position||!loan.target_node){error=targets->error();return false;}
   std::copy_n(loan.position,3,loan.search->position);loan.search->has_target_position=0;
   if(*loan.target_node){if(!loan.target_enabled)return false;if(*loan.target_enabled){if(!loan.cached_target_position)return false;std::copy_n(loan.cached_target_position,3,loan.search->target_position);loan.search->has_target_position=1;}}
   if(loan.heading_angle)loan.search->rotation=*loan.heading_angle;object=loan.search;pins.push_back(std::move(loan));return true;};
  Object48* own{};if(!observe(owner,own))return false;
  Room16 end{};end.next=rooms.empty()?&end:&rooms.front();std::size_t ri{},offset{};
  for(auto list:source){auto& room=rooms[ri];auto& sentinel=ends[ri];room.next=ri+1<rooms.size()?&rooms[ri+1]:&end;room.objects=&sentinel;
   sentinel.next=list->empty()?&sentinel:&entries[offset];std::size_t n{};
   for(auto target:*list){Object48* object{};if(!observe(target,object))return false;entries[offset]={++n<list->size()?&entries[offset+1]:&sentinel,object};++offset;}++ri;}
  const Registry8 registry{&end};std::vector<Target24> heap(std::max<std::size_t>(total,1));List40 list{};
  if(dh2_target_list_init(&list,heap.data(),heap.size(),own,1,&query))return false;
  //3abcec..3abd08 obtains CharacterDesign/OOI_Distance and converts the
  //signed integer to float. Per-object interaction radius is a later filter.
  std::int32_t distance{};const auto design=r->design.design();
  if(!design||!design->lookup||design->lookup(design->context,0,"CharacterDesign","OOI_Distance",&distance)){error="Required actual CharacterDesign.OOI_Distance";return false;}
  if(dh2_target_search_policy_v108(&list,&registry,static_cast<float>(distance),6.283185482025146484375f,89,1,&query)){error=targets->targets().error();return false;}
  out.clear();out.reserve(list.count);while(list.count){Target24 target;if(dh2_target_pop(&list,&target))return false;out.push_back({target.identity,target.flags});}return true;
 };
 return dh2::character::source_character_update_interest_v106(f,scope.application->source_loading_v55().dt8c,services,e);
}
}
