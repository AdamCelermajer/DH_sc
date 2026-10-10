#include "source_campaign_ai_queue_v105.hpp"
#include "renderer_character_campaign_v62.hpp"
#include "model_renderer.hpp"
#include <canonical_character_candidate_v60.hpp>
#include <character_world_ai_queue_v1.hpp>
#include <application_services_owner_v5.hpp>
#include <cstring>
namespace model_renderer {namespace {
struct QueueScopeV105 {std::shared_ptr<void> world;};
bool queue_query(void* raw,std::uintptr_t ai,dh2::character::WorldAIQueueQueryV1 query,std::int32_t& value,std::string& e){
 auto& context=*static_cast<QueueScopeV105*>(raw);SourceCampaignCandidateBorrowV55 scope;
 if(!borrow_source_campaign_candidate_v55(scope,e)||scope.actual_world!=context.world||!scope.objects)return false;
 SourceCampaignCharacterBorrowV62 receiver;
 for(auto id:scope.objects->characters()){
  SourceCampaignCharacterBorrowV62 current;if(!borrow_source_campaign_character_v62(context.world,id,current,e))return false;
  if(current.character->actor&&current.character->actor->ai_events.ai==ai){receiver=std::move(current);break;}
 }
 if(!receiver.character){e="Required SAME constructor-registered CharAI queue receiver";return false;}
 auto& record=*receiver.character;auto& actor=*record.actor;using Q=dh2::character::WorldAIQueueQueryV1;
 if(query==Q::Forced||query==Q::GlobalBlocked||query==Q::Locked){dh2::character::ControllerCommandState32 observed;
  if(!record.services.controller||!record.services.controller(observed,record,e)||!actor.controller)return false;
  auto control=actor.controller->command_state(observed.global_blocked);if(!control||control->owner!=actor.object->identity){e="Required actual source queue controller";return false;}
  value=query==Q::Forced?control->forced:query==Q::Locked?control->locked:control->global_blocked;return true;
 }
 if(query==Q::Dead){if(!record.life){e="Required actual queue Character.IsDead";return false;}value=record.life->dead!=0;return true;}
 if(query==Q::Player){bool player{};if(!record.is_player(player,e))return false;value=player;return true;}
 if(query==Q::Zoned){bool zonable{};if(!source_campaign_character_is_zonable_v104(context.world,actor.object->identity,zonable,e))return false;value=zonable;return true;}
 if(query==Q::Follower||query==Q::Faerie){const auto* rows=record.design.ai();std::int32_t id{};
  if(!rows||!record.properties||dh2_character_target_ai_id(&id,record.properties->resolved.data(),static_cast<std::uint32_t>(rows->rows.size()))){e="Required actual queue GetCharAIId and CharacterProperties";return false;}
  const auto* row=dh2::data::ai_props(*rows,id);if(!row){e="Required actual queue GetCharType row";return false;}value=row->type==(query==Q::Follower?2:3);return true;
 }
 dh2::world::GameObjectInitializationFieldsV62 fields;if(!actor.inherited_initialization_fields_v62(receiver.character,fields,e))return false;
 if(query==Q::RemotelyUpdated){auto id=fields.integer(0x110);auto remote=fields.byte(0x118);if(!id||(*id==-1&&!remote)){e="Required actual queue IsRemotelyUpdated110/118";return false;}value=*id!=-1||*remote;return true;}
 const auto offset=query==Q::Byte80?0x80u:query==Q::Byte2ee?0x2eeu:0x2f0u;
 const auto cell=fields.byte(offset);if(!cell){e="Required actual queue source byte";return false;}value=*cell;return true;
}
}
bool source_campaign_ai_inc_queue_v105(const std::shared_ptr<void>& world,std::string& e){
 SourceCampaignCandidateBorrowV55 actual;if(!borrow_source_campaign_candidate_v55(actual,e)||actual.actual_world!=world||!actual.application)return false;
 const auto dt=actual.application->source_loading_v55().dt8c;std::int32_t signed_dt;static_assert(sizeof(dt)==sizeof(signed_dt));std::memcpy(&signed_dt,&dt,4);
 QueueScopeV105 context{world};const dh2::character::WorldAIQueueServicesV1 services{&context,queue_query};auto queue=dh2::character::character_ai_queue_v105();
 if(!queue->advance(signed_dt,services)){e=queue->error();return false;}return true;
}
bool source_campaign_ai_is_my_turn_v105(const std::shared_ptr<void>& world,std::uintptr_t ai,bool& value,std::string& e){
 QueueScopeV105 context{world};const dh2::character::WorldAIQueueServicesV1 services{&context,queue_query};auto queue=dh2::character::character_ai_queue_v105();
 if(!queue->is_my_turn(ai,services,value)){e=queue->error();return false;}return true;
}
}
