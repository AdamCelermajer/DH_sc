#include "source_campaign_character_interaction_v114.hpp"
#include "model_renderer.hpp"
#include "source_campaign_character_fsm_v101.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "source_campaign_runtime_v61.hpp"
#include "source_campaign_events_v75.hpp"
#include "source_campaign_container_targets_v104.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <application_services_owner_v5.hpp>
#include <application_player_manager_bootstrap_v59.hpp>
#include <game_event_runtime_v75.hpp>
namespace model_renderer {namespace {
using Record=dh2::world::CanonicalCharacterCandidateRecordV60;
bool scope(const std::shared_ptr<void>& world,SourceCampaignCandidateBorrowV55& candidate,std::shared_ptr<SourceWorldBorrowV61>& source,std::string& e){
 return borrow_source_campaign_candidate_v55(candidate,e)&&candidate.actual_world==world&&borrow_source_campaign_condition_world_v70(candidate,source,e);
}
bool ai_row(Record& r,const dh2::data::AiProps*& out,std::string& e){const auto* tables=r.design.ai();std::int32_t id{};
 if(!tables||!r.properties||dh2_character_target_ai_id(&id,r.properties->resolved.data(),tables->rows.size())||(out=dh2::data::ai_props(*tables,id))==nullptr){e="Required actual Character interaction GetCharAI row";return false;}return true;}
bool ui(SourceWorldBorrowV61& world,const SourceCharacterInteractionMenuRequestV114& q,std::string& e){
 if(!world.character_interaction_ui_owner_v114||!world.character_interaction_ui_v114){e="Required SAME actual RenderFX Character interaction callback";return false;}return world.character_interaction_ui_v114(q,e);
}
}
bool bind_source_campaign_character_interaction_ui_v114(const std::shared_ptr<void>& world,std::shared_ptr<void> owner,SourceCharacterInteractionMenuCallbackV114 callback,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> source;
 if(!owner||!callback||!scope(world,candidate,source,e))return false;
 if(source->character_interaction_ui_owner_v114||source->character_interaction_ui_v114){e="Repeated actual Character interaction UI enrollment";return false;}
 source->character_interaction_ui_owner_v114=std::move(owner);source->character_interaction_ui_v114=std::move(callback);e.clear();return true;
}
bool source_campaign_object_as_character_v114(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t& out,std::string& e){
 out=0;if(!id){e.clear();return true;}SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> source;if(!scope(world,candidate,source,e)||!candidate.objects)return false;
 const dh2::world::CanonicalObjectBorrowV1* object{};std::int32_t key{};bool next=candidate.objects->source_ordered_begin_v38(key,object);
 while(next&&(!object||object->identity!=id))next=candidate.objects->source_ordered_next_v38(key,key,object);
 if(!next||!object||!object->lease||!object->as_character){e="Required SAME source Handle/selected IsCharacter receiver";return false;}
 auto captured=*object;return captured.as_character(captured.context,out,e);
}
bool source_campaign_noncharacter_interact_v114(const std::shared_ptr<void>& world,std::uintptr_t target,std::uintptr_t interactor,std::string& e){
 bool handled{};if(!source_campaign_item_container_interact_v114(world,target,interactor,handled,e))return false;
 if(handled)return true;e="Required actual selected nonCharacter virtual98 Interact body";return false;
}
bool source_campaign_character_interact_v114(const std::shared_ptr<void>& world,std::uintptr_t id,std::uintptr_t other,std::string& e){
 SourceCampaignCandidateBorrowV55 candidate;std::shared_ptr<SourceWorldBorrowV61> source;SourceCampaignCharacterBorrowV62 loan;
 if(!scope(world,candidate,source,e)||!borrow_source_campaign_character_v62(world,id,loan,e)||!loan.character->actor||!loan.character->actor->machine)return false;
 auto r=loan.character;std::int32_t state{};if(dh2_character_native_fsm_get_integer(&state,&r->actor->machine->native_fsm(),0)!=1)return false;
 if(state!=13){
  //3a4fa4 snapshots CurrentLevel BEFORE constants/payload creation. The
  //original NULL dereference/assertion path is a named native failure.
  SourceCampaignLevelEventsBorrowV88 level;if(!borrow_source_campaign_level_events_v88(world,level,e)||!level){if(e.empty())e="Required original nonNULL Character.Interact CurrentLevel";return false;}
  const std::int32_t* oid{};const std::int16_t* props{};std::uintptr_t* ignored{};r->actor->kill_metadata_borrow_v23(oid,props,ignored);
  if(!oid||!props){e="Required actual Character.Interact OID64/CharProps13c8";return false;}
  const auto captured_oid=*oid;const auto captured_props=*props;
  const auto* constants=r->design.design();std::int32_t type{};
  if(!constants||!constants->lookup||constants->lookup(constants->context,0,"v2QuestObjectiveType","TalkToNPC",&type)){e="Required actual v2QuestObjectiveType.TalkToNPC";return false;}
  struct Payload {std::int32_t type4;std::uintptr_t character8;std::int32_t oid_c;std::uint8_t network10{},from11{};std::int32_t quantity14{-1},props18;} payload{type,other,captured_oid,0,0,-1,captured_props};
  dh2::loader::GameEventQuestBorrowV75 fields;fields.receiver=r;fields.character8_cell=&payload.character8;fields.id18_cell=&payload.props18;
  fields.pending_network10=&payload.network10;fields.from_network11=&payload.from11;fields.quantity14=&payload.quantity14;
  dh2::loader::ScopedGameQuestEventV75 event(reinterpret_cast<std::uintptr_t>(&payload),&payload.type4,std::move(fields));
  if(!raise_source_campaign_level_event_v88(level,event.borrow(),e))return false; //literal immediate RaiseAsync, stack stays live
 }
 if(!source_campaign_character_set_interact_v114(world,id,3,true,other,false,e))return false;
 auto talk=r->actor->source_bool_field(0x2fa);if(!talk){e="Required SAME Character TalkToNPC2fa";return false;}if(*talk)return true;
 if(!source_campaign_character_raise_event_v114(world,id,5,other,e))return false;
 const dh2::data::AiProps* ai{};if(!ai_row(*r,ai,e))return false;
 if(ai->type==7){
  const auto table=r->properties->resolved[9];if(table==-1||!other)return true;
  std::uintptr_t player{};if(!source_campaign_object_as_character_v114(world,other,player,e))return false;if(!player)return true;
  if(!source_campaign_character_merchant_stock_v114(world,id,table,e))return false;
  auto pm=candidate.application->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* info{};
  if(!pm||!pm->manager()||!pm->manager()->get_by_character(player,false,info,e)||!info)return false;
  if(!ui(*source,{SourceCharacterInteractionMenuV114::open_merchant,id,info->friendly678,0,nullptr},e))return false;
  const std::int32_t* oid{};const std::int16_t* props{};std::uintptr_t* tracked{};r->actor->kill_metadata_borrow_v23(oid,props,tracked);
  if(!oid){e="Required actual Merchant OID64";return false;}
  return ui(*source,{SourceCharacterInteractionMenuV114::merchant_information,id,-1,*oid,r->actor->source_name().c_str()},e);
 }
 //Original IsCleaner is recovered separately from Merchant; only a real
 //Character Handle conversion admits the actual Cleaner UI numeric argument.
 if(ai->type==8&&other){std::uintptr_t player{};if(!source_campaign_object_as_character_v114(world,other,player,e))return false;if(!player)return true;
  auto pm=candidate.application->source_player_manager_v59();dh2::player::PlayerInfoFieldsV1* info{};
  if(!pm||!pm->manager()||!pm->manager()->get_by_character(player,false,info,e)||!info)return false;
  return ui(*source,{SourceCharacterInteractionMenuV114::open_cleaner,id,info->friendly678,0,nullptr},e);
 }
 e.clear();return true;
}
}
